//@	{"target":{"name":"text_composing_stick_test"}}

#include "./text_composing_stick.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(TextComposingStickCreateObj)
{
	fruit::FontFace face;
	fruit::TextComposingStick obj{
		fruit::TextSegment{}.text(u8"Hello, World"),
		16,
		face
	};

	EXPECT_EQ(obj.size, 16);
	EXPECT_EQ(obj.text.valid(), true);
	EXPECT_EQ(obj.font_face.get().valid(), false);
}