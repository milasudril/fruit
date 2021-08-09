//@	 {"dependencies":[{"ref":"harfbuzz", "origin":"pkg-config"}]}

#ifndef FRUIT_TEXTDIRECTION_HPP
#define FRUIT_TEXTDIRECTION_HPP

#include <hb.h>

namespace fruit
{
	enum class TextDirection
	{
		LeftToRight = HB_DIRECTION_LTR,
		RightToLeft = HB_DIRECTION_RTL,
		TopToBottom = HB_DIRECTION_TTB,
		BottomToTop = HB_DIRECTION_BTT
	};

	inline char const* to_string(TextDirection dir)
	{
		switch(dir)
		{
			case TextDirection::LeftToRight: return "Left-to-right";
			case TextDirection::RightToLeft: return "Right-to-left";
			case TextDirection::TopToBottom: return "Top-to-bottom";
			case TextDirection::BottomToTop: return "Bottom-to-top";
		}
		__builtin_unreachable();
	}

	inline bool vertical(TextDirection dir)
	{
		return dir == TextDirection::TopToBottom || dir == TextDirection::BottomToTop;
	}
}


#endif
