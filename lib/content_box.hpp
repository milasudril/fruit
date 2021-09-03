#ifndef FRUIT_CONTENTBOX_HPP
#define FRUIT_CONTENTBOX_HPP

#include "./text_element.hpp"

namespace fruit
{
	struct ContentBox
	{
		Point<int> locaction;
		int current_width;
		int current_height;

		int padding_left;
		int padding_right;
		int padding_top;
		int padding_bottom;

		int margin_left;
		int margin_right;
		int margin_top;
		int margin_bottom;

		Pixel bg_color;
		Pixel border_color;
		int border_width;

		TextElement content;
		Pixel text_color;

		SizeRequestResult handle(SizeRequestEvent const&) const;

		void handle(GeometryUpdateEvent const& event);

		void handle(UpdateEventSw const& event) const;
	};
}

#endif