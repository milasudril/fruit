#ifndef FRUIT_LIB_VIEWPORTSIZE_HPP
#define FRUIT_LIB_VIEWPORTSIZE_HPP

#include <string>
#include <algorithm>

namespace fruit
{
	struct ViewportSize
	{
		int width;
		int height;

		constexpr bool operator==(ViewportSize const& other) const = default;
		constexpr bool operator!=(ViewportSize const& other) const = default;
	};

	inline ViewportSize max(ViewportSize a, ViewportSize b)
	{
		return ViewportSize{std::max(a.width, b.width), std::max(a.height, b.height)};
	}

	inline ViewportSize min(ViewportSize a, ViewportSize b)
	{
		return ViewportSize{std::min(a.width, b.width), std::min(a.height, b.height)};
	}


	inline std::string to_string(ViewportSize viewport)
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