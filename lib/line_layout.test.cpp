//@	{"target":{"name":"line_layout.test"}}

#include "./line_layout.hpp"

#include "testfwk/testfwk.hpp"

namespace
{
	struct Object
	{
		fruit::ViewportSize size;
		fruit::Point<int> location;

		fruit::SizeRequestResult handle(fruit::DeviceId, fruit::SizeRequestEvent const&) const
		{
			return fruit::SizeRequestResult{size, size};
		}

		void handle(fruit::DeviceId, fruit::GeometryUpdateEvent const& event)
		{
			size = event.size;
			location = event.location;
		}

		void handle(fruit::DeviceId, fruit::UpdateEventSw const&) const {}
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

	auto size = layout.handle(fruit::DeviceId{-1}, fruit::SizeRequestEvent{}).min_size;
	EXPECT_EQ(size.width, 3);
	EXPECT_EQ(size.height, 9);
}

TESTCASE(LineLayoutSizeRequestHorizontal)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::LeftToRight};

	Object a{fruit::ViewportSize{3, 2}, fruit::Origin<int>};
	Object b{fruit::ViewportSize{2, 3}, fruit::Origin<int>};
	Object c{fruit::ViewportSize{4, 1}, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a)});
	layout.push_back(fruit::LayoutBox{std::ref(b)});
	layout.push_back(fruit::LayoutBox{std::ref(c)});

	auto size = layout.handle(fruit::DeviceId{-1}, fruit::SizeRequestEvent{}).min_size;
	EXPECT_EQ(size.height, 3);
	EXPECT_EQ(size.width, 9);
}

TESTCASE(LineLayoutGeometryUpdateVertical)
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

	layout.handle(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{3, 2}, fruit::Point{3, 4, 5}});
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

TESTCASE(LineLayoutGeometryUpdateVerticalNoConstraints)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::TopToBottom};

	fruit::ViewportSize size_a{2, 0};
	fruit::ViewportSize size_b{3, 0};
	fruit::ViewportSize size_c{1, 0};

	Object a{size_a, fruit::Origin<int>};
	Object b{size_b, fruit::Origin<int>};
	Object c{size_c, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a), 0.0f, 1.0f});
	layout.push_back(fruit::LayoutBox{std::ref(b), 0.0f, 1.0f});
	layout.push_back(fruit::LayoutBox{std::ref(c), 0.0f, 1.0f});
	layout.set_height(1.0f);

	layout.handle(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{3, 3}, fruit::Point{3, 4, 5}});
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

TESTCASE(LineLayoutGeometryUpdateVerticalPartialConstraints)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::TopToBottom};

	fruit::ViewportSize size_a{2, 0};
	fruit::ViewportSize size_b{3, 2};
	fruit::ViewportSize size_c{1, 0};

	Object a{size_a, fruit::Origin<int>};
	Object b{size_b, fruit::Origin<int>};
	Object c{size_c, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a), 0.0f, 1.0f});
	layout.push_back(fruit::LayoutBox{std::ref(b), 0.0f, 1.0f});
	layout.push_back(fruit::LayoutBox{std::ref(c), 0.0f, 1.0f});
	layout.set_height(1.0f);

	layout.handle(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{3, 4}, fruit::Point{3, 4, 5}});
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

TESTCASE(LineLayoutGeometryUpdateHorizontal)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::LeftToRight};

	fruit::ViewportSize size_a{3, 2};
	fruit::ViewportSize size_b{2, 3};
	fruit::ViewportSize size_c{4, 1};

	Object a{size_a, fruit::Origin<int>};
	Object b{size_b, fruit::Origin<int>};
	Object c{size_c, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a)});
	layout.push_back(fruit::LayoutBox{std::ref(b)});
	layout.push_back(fruit::LayoutBox{std::ref(c)});

	layout.handle(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{2, 3}, fruit::Point{4, 3, 5}});
	EXPECT_EQ(a.location.y(), 3);
	EXPECT_EQ(b.location.y(), 3);
	EXPECT_EQ(c.location.y(), 3);

	EXPECT_EQ(a.location.x(), 4);
	EXPECT_EQ(b.location.x(), 7);
	EXPECT_EQ(c.location.x(), 9);

	EXPECT_EQ(a.size, size_a);
	EXPECT_EQ(b.size, size_b);
	EXPECT_EQ(c.size, size_c);
}

TESTCASE(LineLayoutGeometryUpdateNoConstraintsHorizontal)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::LeftToRight};

	fruit::ViewportSize size_a{0, 2};
	fruit::ViewportSize size_b{0, 3};
	fruit::ViewportSize size_c{0, 1};

	Object a{size_a, fruit::Origin<int>};
	Object b{size_b, fruit::Origin<int>};
	Object c{size_c, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a), 1.0f, 0.0f});
	layout.push_back(fruit::LayoutBox{std::ref(b), 1.0f, 0.0f});
	layout.push_back(fruit::LayoutBox{std::ref(c), 1.0f, 0.0f});
	layout.set_width(1.0f);

	layout.handle(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{3, 3}, fruit::Point{4, 3, 5}});
	EXPECT_EQ(a.location.y(), 3);
	EXPECT_EQ(b.location.y(), 3);
	EXPECT_EQ(c.location.y(), 3);

	EXPECT_EQ(a.location.x(), 4);
	EXPECT_EQ(b.location.x(), 5);
	EXPECT_EQ(c.location.x(), 6);

	EXPECT_EQ(a.size, (fruit::ViewportSize{1, size_a.height}));
	EXPECT_EQ(b.size, (fruit::ViewportSize{1, size_b.height}));
	EXPECT_EQ(c.size, (fruit::ViewportSize{1, size_c.height}));
}

TESTCASE(LineLayoutGeometryUpdatePartialConstraintsHorizontal)
{
	fruit::LineLayout layout{fruit::LineLayout::Direction::LeftToRight};

	fruit::ViewportSize size_a{0, 2};
	fruit::ViewportSize size_b{2, 3};
	fruit::ViewportSize size_c{0, 1};

	Object a{size_a, fruit::Origin<int>};
	Object b{size_b, fruit::Origin<int>};
	Object c{size_c, fruit::Origin<int>};

	layout.push_back(fruit::LayoutBox{std::ref(a), 1.0f, 0.0f});
	layout.push_back(fruit::LayoutBox{std::ref(b), 1.0f, 0.0f});
	layout.push_back(fruit::LayoutBox{std::ref(c), 1.0f, 0.0f});
	layout.set_width(1.0f);

	layout.handle(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{4, 3}, fruit::Point{4, 3, 5}});
	EXPECT_EQ(a.location.y(), 3);
	EXPECT_EQ(b.location.y(), 3);
	EXPECT_EQ(c.location.y(), 3);

	EXPECT_EQ(a.location.x(), 4);
	EXPECT_EQ(b.location.x(), 5);
	EXPECT_EQ(c.location.x(), 7);

	EXPECT_EQ(a.size, (fruit::ViewportSize{1, size_a.height}));
	EXPECT_EQ(b.size, (fruit::ViewportSize{2, size_b.height}));
	EXPECT_EQ(c.size, (fruit::ViewportSize{1, size_c.height}));
}

TESTCASE(LineLayoutGeometryUpdateLineLayoutInLineLayout1)
{
	fruit::LineLayout outer{fruit::LineLayout::Direction::LeftToRight};
	fruit::LineLayout inner{fruit::LineLayout::Direction::LeftToRight};

	outer.set_width(0.5f);
	inner.set_width(0.5f);

	fruit::ViewportSize size{0, 3};
	Object a{size, fruit::Origin<int>};

	// Scaling here does not affect the result due to normailzation
	inner.push_back(fruit::LayoutBox{std::ref(a), 0.5f, 0.0f});
	outer.push_back(fruit::LayoutBox{std::ref(inner), 0.5f, 0.0f});

	outer.handle(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{8, 8}, fruit::Origin<int>});
	EXPECT_EQ(a.size, (fruit::ViewportSize{2, 3}));
}