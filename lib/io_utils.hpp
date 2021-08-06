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
}