#ifndef FRUIT_TEXTELEMENT_HPP
#define FRUIT_TEXTELEMENT_HPP

#include "./text_segment.hpp"
#include "./font_face.hpp"

#include <functional>

namespace fruit
{
	struct TextElement
	{
		TextSegment text;
		int size;
		std::reference_wrapper<FontFace> font_face;
	};
}

#endif