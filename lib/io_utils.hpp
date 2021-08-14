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
	std::vector<std::byte> load(std::filesystem::path const& path,
								size_t max_num_bytes = std::numeric_limits<size_t>::max());

	void store(std::span<std::byte const> data, std::filesystem::path const& path);

	using ReadCallback = size_t (*)(void*, std::span<std::byte>);

	std::vector<std::byte> load(void* source, ReadCallback read_callback, size_t max_num_bytes);

	template<class InputStream>
	requires requires(InputStream s)
	{
		{read(s, std::declval<std::span<std::byte>>())} -> std::same_as<size_t>;
	}
	std::vector<std::byte> load(InputStream&& source,
	                            size_t max_num_bytes = std::numeric_limits<size_t>::max())
	{
		return load(&source, [](void* handle, std::span<std::byte> buffer){
			return read(*static_cast<InputStream*>(handle), buffer);
		}, max_num_bytes);
	}

	template<class InputStream>
	requires requires(InputStream s)
	{
		{s(std::declval<std::span<std::byte>>())} -> std::same_as<size_t>;
	}
	std::vector<std::byte> load(InputStream&& source,
	                            size_t max_num_bytes = std::numeric_limits<size_t>::max())
	{
		return load(&source, [](void* handle, std::span<std::byte> buffer){
			return (*static_cast<InputStream*>(handle))(buffer);
		}, max_num_bytes);
	}

}