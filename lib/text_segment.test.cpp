//@	{"target":{"name":"text_segment.test"}}

#include "./text_segment.hpp"

#include "./io_utils.hpp"
#include "./font_mapper.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(TextSegmentValidity)
{
	fruit::TextSegment segment;
	EXPECT_EQ(segment.valid(), true);

	auto other = std::move(segment);
	EXPECT_EQ(segment.valid(), false);
	EXPECT_EQ(other.valid(), true);
}

TESTCASE(TextSegmentDirection)
{
	fruit::TextSegment segment;
	EXPECT_EQ(segment.direction(), fruit::TextDirection::LeftToRight);

	segment.direction(fruit::TextDirection::RightToLeft);
	EXPECT_EQ(segment.direction(), fruit::TextDirection::RightToLeft);

	segment.direction(fruit::TextDirection::TopToBottom);
	EXPECT_EQ(segment.direction(), fruit::TextDirection::TopToBottom);

	segment.direction(fruit::TextDirection::BottomToTop);
	EXPECT_EQ(segment.direction(), fruit::TextDirection::BottomToTop);
}

TESTCASE(TextSegmentLanguage)
{
	fruit::TextSegment segment;
	EXPECT_EQ(segment.language(), fruit::LanguageTag{"en-us"});

	segment.language(fruit::LanguageTag{"sv-se"});
	EXPECT_EQ(segment.language(), fruit::LanguageTag{"sv-se"});
}

TESTCASE(TextSegmentScript)
{
	fruit::TextSegment segment;
	EXPECT_EQ(static_cast<int>(segment.script()), static_cast<int>(fruit::WritingSystem::Latin));

	segment.script(fruit::WritingSystem::Arabic);
	EXPECT_EQ(static_cast<int>(segment.script()), static_cast<int>(fruit::WritingSystem::Arabic));
}

TESTCASE(TextSegmentShape)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load(fruit::FontMapper{}.get_path("DejaVu Sans"))};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	auto const text = std::basic_string_view{u8"Hallå, världen!"};
	auto shape_result = segment.text(text).language(fruit::LanguageTag{"sv-se"}).shape(foobar);

	EXPECT_EQ(&shape_result.font(), &face);
	// Subtract two from std::size(text) because we have ä and å, which occupy two code units in
	// UTF-8
	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text) - 2);
	EXPECT_EQ(std::size(shape_result.glyph_geometry()), std::size(text) - 2);

	auto bb = bounding_box(shape_result);
	EXPECT_EQ(bb.width, 117);
	EXPECT_EQ(bb.height, 16);

	auto buffer = std::make_unique<uint8_t[]>(bb.width * bb.height);
	render(shape_result, fruit::ImageView{buffer.get(), bb.width, bb.height});
	fruit::io_utils::store(std::as_bytes(std::span<uint8_t const>{buffer.get(), static_cast<size_t>(bb.width*bb.height)}), "./test.dat");
}