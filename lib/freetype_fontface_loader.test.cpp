//@	{"target":{"name":"freetype_fontface_loader.test"}}

#include "./freetype_fontface_loader.hpp"

#include "./io_utils.hpp"

#include "testfwk/testfwk.hpp"

#include <algorithm>

TESTCASE(FreetypeFontfaceLoaderInit)
{
	fruit::FreetypeFontfaceLoader loader;
}

TESTCASE(FreetypeFontfaceLoaderLoadFont)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace font{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};

	auto A = std::as_bytes(make_span(font.char_height(16).render('A', fruit::TextDirection::LeftToRight).image));
	auto A_ref = fruit::io_utils::load("testdata/A_rendered.dat");
	EXPECT_EQ(std::ranges::equal(A, A_ref), true);
}