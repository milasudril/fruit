//@	{"target":{"name":"text_line_test"}}

#include "./text_line.hpp"

#include "./io_utils.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(TextLineSizeReqEmptyHorz)
{
	fruit::FontfaceLoader loader;
	fruit::FontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextLine obj{face};
	obj.char_height(20);
	auto size = obj.handle(fruit::SizeRequestEvent{});
	EXPECT_EQ(size.min_size, (fruit::ViewportSize{0, 20}));
}

TESTCASE(TextLineSizeReqEmptyVertical)
{
	fruit::FontfaceLoader loader;
	fruit::FontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextLine obj{face};
	obj.char_height(20)
		.direction(fruit::TextDirection::TopToBottom);
	auto size = obj.handle(fruit::SizeRequestEvent{});
	EXPECT_EQ(size.min_size, (fruit::ViewportSize{20, 0}));
}

TESTCASE(TextLineSizeReqHorz)
{
	fruit::FontfaceLoader loader;
	fruit::FontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextLine obj{face};
	obj.char_height(20)
		.text(u8"Hello, World");
	auto const size_a = obj.handle(fruit::SizeRequestEvent{});
	EXPECT_EQ(size_a.min_size, (fruit::ViewportSize{120, 23}));

	auto const size_b = obj.handle(fruit::SizeRequestEvent{});
	EXPECT_EQ(size_a.min_size, size_b.min_size);
}

TESTCASE(TextLineSizeReqUpdatedContent)
{
	fruit::FontfaceLoader loader;
	fruit::FontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextLine obj{face};
	obj.char_height(20).text(u8"Hello, World");
	auto const size_a = obj.handle(fruit::SizeRequestEvent{});

	obj.char_height(30).text(u8"Foobar");
	auto const size_b = obj.handle(fruit::SizeRequestEvent{});
	EXPECT_NE(size_a.min_size, size_b.min_size);
}

TESTCASE(TextLineSizeReqUpdatedSizeKeepContent)
{
	fruit::FontfaceLoader loader;
	fruit::FontFace face{loader, fruit::io_utils::load("testdata/DejaVuSans.ttf")};
	fruit::TextLine obj{face};
	obj.char_height(20).text(u8"Hello, World");
	auto const size_a = obj.handle(fruit::SizeRequestEvent{});

	obj.char_height(30);
	auto const size_b = obj.handle(fruit::SizeRequestEvent{});
	EXPECT_NE(size_a.min_size, size_b.min_size);
}