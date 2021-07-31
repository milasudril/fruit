//@	{"target":{"name":"point.test"}}

#include "./point.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(Point)
{
	fruit::Point const P1{1, 2, 3};
	fruit::Point const P2{4, 6, 8};
	EXPECT_EQ(P1.x(), 1);
	EXPECT_EQ(P1.y(), 2);
	EXPECT_EQ(P1.z(), 3);
	EXPECT_EQ(P2.x(), 4);
	EXPECT_EQ(P2.y(), 6);
	EXPECT_EQ(P2.z(), 8);

	auto const v = P2 - P1;
	static_assert(std::is_same_v<decltype(v), fruit::Vector<int> const>);
	EXPECT_EQ(v.x(), 3);
	EXPECT_EQ(v.y(), 4);
	EXPECT_EQ(v.z(), 5);

	EXPECT_EQ(distance(P1, P2), distance(P2, P1));
	auto const d = distance(P1, P2);
	EXPECT_GT(d, 7.0);
	EXPECT_LT(d, 8.0);

	auto const P3 = P1 + fruit::Vector{4, 6, 8};
	static_assert(std::is_same_v<decltype(P3), fruit::Point<int> const>);
	EXPECT_EQ(P3.x(), P1.x() + 4);
	EXPECT_EQ(P3.y(), P1.y() + 6);
	EXPECT_EQ(P3.z(), P1.z() + 8);

	auto const P4 = P3 - fruit::Vector{4, 6, 8};
	static_assert(std::is_same_v<decltype(P4), fruit::Point<int> const>);
	EXPECT_EQ(P4.x(), P1.x());
	EXPECT_EQ(P4.y(), P1.y());
	EXPECT_EQ(P4.z(), P1.z());
}