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

std::vector<std::byte> fruit::io_utils::load(std::filesystem::path const& path, size_t max_num_bytes)
{
	std::unique_ptr<FILE, FileDeleter> file{fopen(path.c_str(), "rb")};
	if(file.get() == nullptr)
	{
		throw std::runtime_error{"Failed to load file"};
	}
	std::vector<std::byte> ret;
	ret.reserve(std::min(max_num_bytes, static_cast<size_t>(65536)));
	for(size_t k = 0; k < max_num_bytes; ++k)
	{
		auto input = getc(file.get());
		if(input == EOF)
		{
			return ret;
		}
		ret.push_back(static_cast<std::byte>(input));
	}
	return ret;
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