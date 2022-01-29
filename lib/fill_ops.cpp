//@	{"target":{"name":"fill_ops.o"}}

#include "./fill_ops.hpp"

void fruit::fill_ops::source_over(image_span<Pixel> target_buffer, Point<int> begin, Point<int> end, Pixel source)
{
	auto const x_min = std::max(0, begin.x());
	auto const y_min = std::max(0, begin.y());
	auto const x_max = std::min(end.x(), static_cast<int>(target_buffer.width()));
	auto const y_max = std::min(end.y(), static_cast<int>(target_buffer.height()));

	for(int y = y_min; y < y_max; ++y)
	{
		for(int x = x_min; x < x_max; ++x)
		{
			target_buffer(x, y).value() = source.value() + target_buffer(x, y).value()*(1.0f - source.alpha());
		}
	}
}