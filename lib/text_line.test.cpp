//@	{"target":{"name":"text_line_test"}}

#include "./text_line.hpp"

#include "./io_utils.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(TextLineCreateObj)
{
	fruit::FontfaceLoader loader;
	fruit::FontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextLine obj{face};

	(void)obj.handle(fruit::SizeRequestEvent{});
}