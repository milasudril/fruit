//@	{
//@	"dependencies_extra":[{"ref":"./io_utils.o", "rel":"implementation"}]
//@	}

#include <filesystem>
#include <vector>
#include <cstddef>
#include <limits>
#include <span>

namespace fruit::io_utils
{
	void store(std::span<std::byte const> data, std::filesystem::path const& path);

	using WriteCallback = size_t (*)(void*, std::span<std::byte const>);

	void store(std::span<std::byte const> data, void* sink, WriteCallback write_callback);

	template<class OutputStream>
	requires requires(OutputStream s)
	{ { write(s, std::declval<std::span<std::byte const>>())} -> std::same_as<size_t>; }
	void store(std::span<std::byte const> buffer, OutputStream&& sink)
	{
		store(buffer, &sink, [](void* handle, std::span<std::byte> buffer){
			return write(*static_cast<OutputStream*>(handle), buffer);
		});
	}

	template<class OutputStream>
	requires requires(OutputStream s)
	{ { s(std::declval<std::span<std::byte const>>()) } -> std::same_as<size_t>; }
	void store(std::span<std::byte const> buffer, OutputStream&& sink)
	{
		store(buffer, &sink, [](void* handle, std::span<std::byte const> buffer){
			return (*static_cast<OutputStream*>(handle))(buffer);
		});
	}


	std::vector<std::byte> load(std::filesystem::path const& path,
								size_t max_num_bytes = std::numeric_limits<size_t>::max());

	using ReadCallback = size_t (*)(void*, std::span<std::byte>);

	std::vector<std::byte> load(void* source, ReadCallback read_callback, size_t max_num_bytes);

	template<class InputStream>
	requires requires(InputStream s)
	{{read(s, std::declval<std::span<std::byte>>())} -> std::same_as<size_t>;}
	std::vector<std::byte> load(InputStream&& source,
	                            size_t max_num_bytes = std::numeric_limits<size_t>::max())
	{
		return load(&source, [](void* handle, std::span<std::byte> buffer){
			return read(*static_cast<InputStream*>(handle), buffer);
		}, max_num_bytes);
	}

	template<class InputStream>
	requires requires(InputStream s)
	{ { s(std::declval<std::span<std::byte>>()) } -> std::same_as<size_t>; }
	std::vector<std::byte> load(InputStream&& source,
	                            size_t max_num_bytes = std::numeric_limits<size_t>::max())
	{
		return load(&source, [](void* handle, std::span<std::byte> buffer){
			return (*static_cast<InputStream*>(handle))(buffer);
		}, max_num_bytes);
	}

}