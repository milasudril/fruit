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

TESTCASE(TextSegmentSetDirection)
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
