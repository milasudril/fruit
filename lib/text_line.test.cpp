//@	{"target":{"name":"text_line_test"}}

#include "./text_line.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(TextLineCreateObj)
{
	fruit::FontFace face;
	fruit::TextLine obj{face};
}