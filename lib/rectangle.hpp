#ifndef FRUIT_RECTANGLE_HPP
#define FRUTI_RECTANLGE_HPP

#include "./update_event.hpp"
#include "./point.hpp"
#include "./pixel.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"

namespace fruit
{
	struct Rectangle
	{
		SizeRequestResult handle(SizeRequestEvent const&) const
		{
			return SizeRequestResult{ViewportSize{width, height}, ViewportSize{width, height}};
		}

		void handle(GeometryUpdateEvent const& event)
		{
			current_width = event.size.width;
			current_height = event.size.height;
			loc = event.location;
		}

		Point<int> loc = Origin<int>;
		int width = 0;
		int height = 0;
		Pixel color = black();
		int current_width = 0;
		int current_height = 0;

		void handle(UpdateEventSw const& event) const
		{
			auto const buffer = event.buffer;
			auto const w_out = event.width;
			auto const h_out = event.height;

			auto const rect_begin = loc;
			auto const rect_end = loc + Vector{current_width, current_height, 0};

			for(int k = std::max(0, rect_begin.y()); k < std::min(h_out, rect_end.y()); ++k)
			{
				for(int l = std::max(0, rect_begin.x()); l < std::min(w_out, rect_end.x()); ++l)
				{
					buffer[k*w_out + l] = color;
				}
			}
		}
	};
}

#endif