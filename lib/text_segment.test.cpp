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

TESTCASE(TextSegmentShapeSwedish)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"Yxmördaren Julia Blomqvist på fäktning i Schweiz"};
	auto shape_result = segment.text(text).language(fruit::LanguageTag{"sv-se"}).shape(foobar);

	EXPECT_EQ(&shape_result.font(), &face);

	// Subtract two from std::size(text) because we have ä and å, which occupy two code units in
	// UTF-8
	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text) - 3);

	auto image = render(shape_result);
	auto expected = fruit::io_utils::load("testdata/Yxmördaren Julia Blomqvist på fäktning i Schweiz.dat");
	EXPECT_EQ(std::ranges::equal(std::as_bytes(make_span(image)), expected), true);
}

TESTCASE(TextSegmentShapeBrittish)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"The five boxing wizards jump quickly"};
	auto shape_result = segment.language(fruit::LanguageTag{"en-gb"}).text(text).shape(foobar);

	// Subtract one for "fi ligature"
	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text) - 1);

	auto image = render(shape_result);
	auto expected = fruit::io_utils::load("testdata/The five boxing wizards jump quickly.dat");
	EXPECT_EQ(std::ranges::equal(std::as_bytes(make_span(image)), expected), true);
}

TESTCASE(TextSegmentShapeGerman)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"Die heiße Zypernsonne quälte Max und Victoria ja böse auf dem Weg bis zur Küste"};
	auto shape_result = segment.text(text).language(fruit::LanguageTag{"de-de"}).shape(foobar);

	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text) - 4);

	auto image = render(shape_result);
	auto expected = fruit::io_utils::load("testdata/Die heiße Zypernsonne quälte Max und Victoria ja böse auf dem Weg bis zur Küste.dat");
	EXPECT_EQ(std::ranges::equal(std::as_bytes(make_span(image)), expected), true);
}

TESTCASE(TextSegmentShapeFrance)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"Dès Noël, où un zéphyr haï me vêt de glaçons würmiens, je dîne d’exquis rôtis de bœuf au kir, à l’aÿ d’âge mûr, &cætera"};
	auto shape_result = segment.text(text).language(fruit::LanguageTag{"fr-fr"}).shape(foobar);

	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text) - 22);

	auto image = render(shape_result);
	auto expected = fruit::io_utils::load("testdata/Dès Noël, où un zéphyr haï me vêt de glaçons würmiens, je dîne d’exquis rôtis de bœuf au kir, à l’aÿ d’âge mûr, &cætera.dat");
	EXPECT_EQ(std::ranges::equal(std::as_bytes(make_span(image)), expected), true);
}

TESTCASE(TextSegmentShapeNorwegian)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"Taxisjåføren quizet bedre om calypso, watt og klær på hjemveien"};
	auto shape_result = segment.text(text).language(fruit::LanguageTag{"no"}).shape(foobar);

	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text) - 4);

	auto image = render(shape_result);
	auto expected = fruit::io_utils::load("testdata/Taxisjåføren quizet bedre om calypso, watt og klær på hjemveien.dat");
	EXPECT_EQ(std::ranges::equal(std::as_bytes(make_span(image)), expected), true);
}

TESTCASE(TextSegmentShapeIcelandic)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"Kæmi ný öxi hér, ykist þjólfum nú bæði víl og ádrepa"};
	auto shape_result = segment.text(text).language(fruit::LanguageTag{"is-is"}).shape(foobar);

	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text) - 11);

	auto image = render(shape_result);
	auto expected = fruit::io_utils::load("testdata/Kæmi ný öxi hér, ykist þjólfum nú bæði víl og ádrepa.dat");
	EXPECT_EQ(std::ranges::equal(std::as_bytes(make_span(image)), expected), true);
}

TESTCASE(TextSegmentShapeCheck)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"Vodní žíňky běží kolem lesní tůně a kadeřemi svými čeří stříbrosvit měsíce"};
	auto shape_result = segment.text(text).language(fruit::LanguageTag{"cs-cz"}).shape(foobar);

	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text) - 19);

	auto image = render(shape_result);
	auto expected = fruit::io_utils::load("testdata/Vodní žíňky běží kolem lesní tůně a kadeřemi svými čeří stříbrosvit měsíce.dat");
	EXPECT_EQ(std::ranges::equal(std::as_bytes(make_span(image)), expected), true);
}

TESTCASE(TextSegmentShapeRightToLeft)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"Integer sit amet tortor quis ex ornare mollis"};
	auto shape_result = segment.direction(fruit::TextDirection::RightToLeft).text(text).shape(foobar);

	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text));

	auto image = render(shape_result);
	auto expected = fruit::io_utils::load("testdata/rtl Integer sit amet tortor quis ex ornare mollis.dat");
	EXPECT_EQ(std::ranges::equal(std::as_bytes(make_span(image)), expected), true);
}

#if 1
TESTCASE(TextSegmentShapeTopToBottom)
{
	fruit::FreetypeFontfaceLoader loader;
	fruit::FreetypeFontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextShaper foobar{face};

	fruit::TextSegment segment;
	std::u8string_view const text{u8"Integer sit amet tortor quis ex ornare mollis"};
	auto shape_result = segment.direction(fruit::TextDirection::TopToBottom).text(text).shape(foobar);

	EXPECT_EQ(std::size(shape_result.glyph_info()), std::size(text));

	auto image = render(shape_result);
	fruit::io_utils::store(std::as_bytes(make_span(image)),
						"testdata/ttb Integer sit amet tortor quis ex ornare mollis.dat");
}
#endif