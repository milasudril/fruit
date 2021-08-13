//@	{"target":{"name":"font_face.test"}}

#include "./font_face.hpp"

#include "./io_utils.hpp"

#include "testfwk/testfwk.hpp"

#include <algorithm>

TESTCASE(FontfaceLoaderInit)
{
	fruit::FontfaceLoader loader;
}

TESTCASE(FontfaceLoaderLoadFont)
{
	fruit::FontfaceLoader loader;
	fruit::FontFace font{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};

	auto A = std::as_bytes(make_span(font.render('A', fruit::TextDirection::LeftToRight, 16).image));
	auto A_ref = fruit::io_utils::load("testdata/A_rendered.dat");
	EXPECT_EQ(std::ranges::equal(A, A_ref), true);
}