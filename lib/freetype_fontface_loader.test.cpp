//@	{"target":{"name":"freetype_fontface_loader.test"}}

#include "./freetype_fontface_loader.hpp"

#include "./font_mapper.hpp"
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
	fruit::FreetypeFontFace font{loader, fruit::io_utils::load(fruit::FontMapper{}.get_path("DejaVu Sans"))};

	auto A = std::as_bytes(make_span(font.set_size(16).render('A')));
	auto A_ref = fruit::io_utils::load("testdata/A_rendered.dat");
	EXPECT_EQ(std::ranges::equal(A, A_ref), true);
}