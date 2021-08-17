//@	{"target":{"name":"line_layout.test"}}

#include "./line_layout.hpp"

#include "testfwk/testfwk.hpp"

namespace
{
	struct Object
	{
		fruit::ViewportSize size;
		fruit::Point<int> location;

		fruit::SizeRequestResult handle(fruit::SizeRequestEvent const&) const
		{
			return fruit::SizeRequestResult{size, size};
		}

		void handle(fruit::GeometryUpdateEvent const& event)
		{
			size = event.size;
			location = event.location;
		}
	};
}

TESTCASE(LineLayoutSizeRequest)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::TopToBottom};

	Object a{fruit::ViewportSize{2, 3}, fruit::Origin<int>};
	Object b{fruit::ViewportSize{3, 2}, fruit::Origin<int>};
	Object c{fruit::ViewportSize{1, 4}, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a)});
	layout.push_back(fruit::LayoutBox{std::ref(b)});
	layout.push_back(fruit::LayoutBox{std::ref(c)});

	auto size = layout.handle(fruit::SizeRequestEvent{}).min_size;
	EXPECT_EQ(size.width, 3);
	EXPECT_EQ(size.height, 9);
}

TESTCASE(LineLayoutGeometryUpdate)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::TopToBottom};

	fruit::ViewportSize size_a{2, 3};
	fruit::ViewportSize size_b{3, 2};
	fruit::ViewportSize size_c{1, 4};

	Object a{size_a, fruit::Origin<int>};
	Object b{size_b, fruit::Origin<int>};
	Object c{size_c, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a)});
	layout.push_back(fruit::LayoutBox{std::ref(b)});
	layout.push_back(fruit::LayoutBox{std::ref(c)});

	layout.handle(fruit::GeometryUpdateEvent{fruit::ViewportSize{3, 2}, fruit::Point{3, 4, 5}});
	EXPECT_EQ(a.location.x(), 3);
	EXPECT_EQ(b.location.x(), 3);
	EXPECT_EQ(c.location.x(), 3);

	EXPECT_EQ(a.location.y(), 4);
	EXPECT_EQ(b.location.y(), 7);
	EXPECT_EQ(c.location.y(), 9);

	EXPECT_EQ(a.size, size_a);
	EXPECT_EQ(b.size, size_b);
	EXPECT_EQ(c.size, size_c);
}

TESTCASE(LineLayoutGeometryUpdateNoConstraints)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::TopToBottom};

	fruit::ViewportSize size_a{2, 0};
	fruit::ViewportSize size_b{3, 0};
	fruit::ViewportSize size_c{1, 0};

	Object a{size_a, fruit::Origin<int>};
	Object b{size_b, fruit::Origin<int>};
	Object c{size_c, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a), 1.0f});
	layout.push_back(fruit::LayoutBox{std::ref(b), 1.0f});
	layout.push_back(fruit::LayoutBox{std::ref(c), 1.0f});

	layout.handle(fruit::GeometryUpdateEvent{fruit::ViewportSize{3, 3}, fruit::Point{3, 4, 5}});
	EXPECT_EQ(a.location.x(), 3);
	EXPECT_EQ(b.location.x(), 3);
	EXPECT_EQ(c.location.x(), 3);

	EXPECT_EQ(a.location.y(), 4);
	EXPECT_EQ(b.location.y(), 5);
	EXPECT_EQ(c.location.y(), 6);

	EXPECT_EQ(a.size, (fruit::ViewportSize{size_a.width, 1}));
	EXPECT_EQ(b.size, (fruit::ViewportSize{size_b.width, 1}));
	EXPECT_EQ(c.size, (fruit::ViewportSize{size_c.width, 1}));
}

TESTCASE(LineLayoutGeometryUpdatePartialConstraints)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::TopToBottom};

	fruit::ViewportSize size_a{2, 0};
	fruit::ViewportSize size_b{3, 2};
	fruit::ViewportSize size_c{1, 0};

	Object a{size_a, fruit::Origin<int>};
	Object b{size_b, fruit::Origin<int>};
	Object c{size_c, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a), 1.0f});
	layout.push_back(fruit::LayoutBox{std::ref(b), 1.0f});
	layout.push_back(fruit::LayoutBox{std::ref(c), 1.0f});

	layout.handle(fruit::GeometryUpdateEvent{fruit::ViewportSize{3, 4}, fruit::Point{3, 4, 5}});
	EXPECT_EQ(a.location.x(), 3);
	EXPECT_EQ(b.location.x(), 3);
	EXPECT_EQ(c.location.x(), 3);

	EXPECT_EQ(a.location.y(), 4);
	EXPECT_EQ(b.location.y(), 5);
	EXPECT_EQ(c.location.y(), 7);

	EXPECT_EQ(a.size, (fruit::ViewportSize{size_a.width, 1}));
	EXPECT_EQ(b.size, (fruit::ViewportSize{size_b.width, 2}));
	EXPECT_EQ(c.size, (fruit::ViewportSize{size_c.width, 1}));
}