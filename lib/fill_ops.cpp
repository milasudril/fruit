//@	{"target":{"name":"fill_ops.o"}}

#include "./fill_ops.hpp"

void fruit::fill_ops::source_over(ImageView<Pixel> target_buffer, Point<int> begin, Point<int> end, Pixel source)
{
	auto const x_min = std::max(0, begin.x());
	auto const y_min = std::max(0, begin.y());
	auto const x_max = std::min(end.x(), target_buffer.width());
	auto const y_max = std::min(end.y(), target_buffer.height());

	for(int y = y_min; y < y_max; ++y)
	{
		for(int x = x_min; x < x_max; ++x)
		{
			target_buffer(x, y) = source + target_buffer(x, y)*(1.0f - source.alpha());
		}
	}
}