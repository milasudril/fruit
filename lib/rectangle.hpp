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
		ViewportSize handle(SizeRequestEvent const&) const
		{
			return ViewportSize{width, height};
		}

		void handle(GeometryUpdateEvent const& event)
		{
			width = event.size.width;
			height = event.size.height;
			loc = event.location;
		}

		Point<int> loc = Origin<int>;
		int width = 0;
		int height = 0;
		Pixel color = black();

		void handle(UpdateEventSw const& event) const
		{
			auto buffer = event.buffer;
			auto w_out = event.width;
			auto h_out = event.height;

			auto rect_begin = loc - Vector{width, height, 0}/2;
			auto rect_end = loc + Vector{width, height, 0}/2;

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