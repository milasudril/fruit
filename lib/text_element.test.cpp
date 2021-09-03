//@	{"target":{"name":"text_element_test"}}

#include "./text_element.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(TextElementCreateObj)
{
	fruit::FontFace face;
	fruit::TextElement obj{
		fruit::TextSegment{}.text(u8"Hello, World"),
		16,
		face
	};

	EXPECT_EQ(obj.size, 16);
	EXPECT_EQ(obj.text.valid(), true);
	EXPECT_EQ(obj.font_face.get().valid(), false);
}