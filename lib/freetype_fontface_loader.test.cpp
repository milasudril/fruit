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
	fruit::FontMapper mapper;
	auto buffer = fruit::io_utils::load(mapper.get_path("DejaVu Sans"));
	auto font = loader.createFrom(buffer);

	fruit::FreetypeFontfaceLoader::size(font, 16);
	auto A = std::as_bytes(make_span(fruit::FreetypeFontfaceLoader::bitmap(font, 'A')));
	auto A_ref = fruit::io_utils::load("testdata/A_rendered.dat");
	EXPECT_EQ(std::ranges::equal(A, A_ref), true);

	fruit::FreetypeFontfaceLoader::free(font);
}