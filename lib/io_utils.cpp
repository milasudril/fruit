//@	{
//@	"target":{"name":"io_utils.o"}
//@	}

#include "./io_utils.hpp"

#include <cstdio>
#include <memory>
#include <algorithm>

namespace
{
	struct FileDeleter
	{
		void operator()(FILE* f)
		{
			if(f != nullptr)
			{
				fclose(f);
			}
		}
	};
}

std::vector<std::byte> fruit::io_utils::load(void* source, ReadCallback read_callback, size_t max_num_bytes)
{
	std::array<std::byte, 65536> buffer;
	std::vector<std::byte> ret;
	ret.reserve(std::min(max_num_bytes, static_cast<size_t>(65536)));
	while(true)
	{
		std::span const read_buffer{std::begin(buffer), std::min(std::size(buffer), std::size(ret) - max_num_bytes)};
		auto const n_read = read_callback(source, read_buffer);
		if(n_read == 0)
		{
			return ret;
		}
		ret.insert(std::end(ret), std::begin(buffer),  std::begin(buffer) + n_read);
		if(std::size(ret) == max_num_bytes)
		{ return ret; }
	}
}

std::vector<std::byte> fruit::io_utils::load(std::filesystem::path const& path, size_t max_num_bytes)
{
	std::unique_ptr<FILE, FileDeleter> file{fopen(path.c_str(), "rb")};
	if(file.get() == nullptr)
	{
		throw std::runtime_error{"Failed to load file"};
	}

	return load([f = file.get()](std::span<std::byte> buffer){
		return fread(std::data(buffer), 1, std::size(buffer), f);
	}, max_num_bytes);
}

void fruit::io_utils::store(std::span<std::byte const> data, std::filesystem::path const& path)
{
	std::unique_ptr<FILE, FileDeleter> file{fopen(path.c_str(), "wb")};
	if(file.get() == nullptr)
	{
		throw std::runtime_error{"Failed to store file"};
	}
	std::ranges::for_each(data, [f = file.get()](auto item) {
		putc(static_cast<int>(item), f);
	});
}