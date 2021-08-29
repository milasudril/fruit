#ifndef FRUIT_TEXTCOMPOSINGSTIC_HPP
#define FRUIT_TEXTCOMPOSINGSTIC_HPP

#include "./text_segment.hpp"
#include "./font_face.hpp"

#include <functional>

namespace fruit
{
	struct TextComposingStick
	{
		TextSegment text;
		int size;
		std::reference_wrapper<FontFace> font_face;
	};
}

#endif