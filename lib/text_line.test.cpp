//@	{"target":{"name":"text_line_test"}}

#include "./text_line.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(TextLineCreateObj)
{
	fruit::FontFace face;
	fruit::TextLine obj{
		fruit::TextSegment{}.text(u8"Hello, World"),
		16,
		face
	};

	EXPECT_EQ(obj.size, 16);
	EXPECT_EQ(obj.text.valid(), true);
	EXPECT_EQ(obj.font_face.get().valid(), false);
}