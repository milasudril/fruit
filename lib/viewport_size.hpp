#ifndef FRUIT_VIEWPORTSIZE_HPP
#define FRUIT_VIEWPORTSIZE_HPP

#include <string>

namespace fruit
{
	struct ViewportSize
	{
		int width;
		int height;

		constexpr bool operator==(ViewportSize const& other) const = default;
		constexpr bool operator!=(ViewportSize const& other) const = default;
	};

	inline std::string to_string(ViewportSize const& viewport)
	{
		std::string ret{"("};
		ret += std::to_string(viewport.width);
		ret += " x ";
		ret += std::to_string(viewport.height);
		ret += ")";
		return ret;
	}
}

#endif