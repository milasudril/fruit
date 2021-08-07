//@	{"target":{"name":"text_segment.test"}}

#include "./text_segment.hpp"

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