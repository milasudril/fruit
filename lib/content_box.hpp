#ifndef FRUIT_CONTENTBOX_HPP
#define FRUIT_CONTENTBOX_HPP

#include "./text_composing_stick.hpp"

namespace fruit
{
	struct ContentBox
	{
		int padding_left;
		int padding_right;
		int padding_top;
		int padding_bottom;

		int margin_left;
		int margin_right;
		int margin_top;
		int margin_bottom;

		Pixel bg_color;
		int border_width;
		Pixel border_color;

		TextComposingStick content;
		Pixel text_color;
	};
}

#endif