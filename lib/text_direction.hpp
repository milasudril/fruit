//@	 { "dependencies_extra":[{"ref":"harfbuzz", "origin":"pkg-config"}]}

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
}


#endif
