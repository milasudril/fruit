//@	{"target":{"name":"layout_box.o"}}

#include "./layout_box.hpp"
#include <numeric>

namespace
{
	float relative_size(int){ return 0.0f; }

	float relative_size(float value) { return value; }

	void scale_value(int&, float){}

	void scale_value(float& x, float factor){ x *= factor; }

	float sum_ltr(float a, fruit::LayoutBox const& item)
	{
		return a + std::visit([](auto value){ return relative_size(value.first);}, item.size.value());
	}

	float sum_ttb(float a, fruit::LayoutBox const& item)
	{
		return a + std::visit([](auto value){ return relative_size(value.second);}, item.size.value());
	};
}

void fruit::normalize_sum(std::span<LayoutBox> sizes, LayoutDirection direction)
{
	auto const sum = std::accumulate(std::begin(sizes), std::end(sizes), 0.0f,
									(direction == LayoutDirection::LeftToRight) ? sum_ltr : sum_ttb );

	auto const scale_ltr = [factor = 1.0f/sum](auto& item) {
		std::visit([factor](auto& value){ scale_value(value.first, factor);}, item.size.value());
	};

	auto const scale_ttb = [factor = 1.0f/sum](auto& item) {
		std::visit([factor](auto& value){ scale_value(value.second, factor);}, item.size.value());
	};

	if(direction == LayoutDirection::LeftToRight)
	{
		std::ranges::for_each(sizes, scale_ltr);
	}
	else
	{
		std::ranges::for_each(sizes, scale_ttb);
	}
}