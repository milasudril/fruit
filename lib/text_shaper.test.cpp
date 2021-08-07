//@	{
//@	"target":{"name":"text_shaper.test"}
//@	}

#include "./text_shaper.hpp"

#include "./font_mapper.hpp"
#include "./io_utils.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(TextShaperCreateAndMove)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load(fruit::FontMapper{}.get_path("DejaVu Sans"))};
	fruit::TextShaper foobar{face};

	EXPECT_EQ(foobar.valid(), true)
	auto other = std::move(foobar);
	EXPECT_EQ(foobar.valid(), false);
	EXPECT_EQ(other.valid(), true);
}

TESTCASE(TextShaperRasterize)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load(fruit::FontMapper{}.get_path("DejaVu Sans"))};
	fruit::TextShaper foobar{face};
}