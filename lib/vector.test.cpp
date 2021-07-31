//@	{"target":{"name":"vector.test"}}

#include "./vector.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(Vector)
{
	fruit::Vector v{3, 4, 5};
	EXPECT_EQ(v.x(), 3);
	EXPECT_EQ(v.y(), 4);
	EXPECT_EQ(v.z(), 5);
	fruit::Vector x{1, 0, 0};
	fruit::Vector y{0, 1, 0};
	fruit::Vector z{0, 0, 1};

	EXPECT_EQ(dot(v, x), v.x());
	EXPECT_EQ(dot(v, y), v.y());
	EXPECT_EQ(dot(v, z), v.z());
	EXPECT_EQ(dot(v, v), 9+16+25);
	EXPECT_EQ(length_squared(v), 9+16+25);

	auto v2 = 2*v;
	EXPECT_EQ(v2.x(), 2*v.x());
	EXPECT_EQ(v2.y(), 2*v.y());
	EXPECT_EQ(v2.z(), 2*v.z());
	EXPECT_EQ(dot(v, v2), 2*dot(v,v));

	EXPECT_EQ(length(v2), 2*length(v));
	EXPECT_GT(length(v), 7.0);
	EXPECT_LT(length(v), 8.0);
	auto l = length(v);
	static_assert(std::is_same_v<decltype(l), double>);

	auto v_norm = normalized(v);
	EXPECT_LE(length(v_norm), std::nextafter(1.0, 2.0));
	EXPECT_GE(length(v_norm), std::nextafter(1.0, 0.0));

	auto v3 = v2 + v;
	EXPECT_EQ(v3.x(), v2.x() + v.x());
	EXPECT_EQ(v3.y(), v2.y() + v.y());
	EXPECT_EQ(v3.z(), v2.z() + v.z());

	auto v4 = v3 - v;
	EXPECT_EQ(v4.x(), v2.x());
	EXPECT_EQ(v4.y(), v2.y());
	EXPECT_EQ(v4.z(), v2.z());

}

TESTCASE(VectorFloat)
{
	fruit::Vector v{3.0f, 4.0f, 5.0f};

	auto v_norm = normalized(v);
	EXPECT_LE(length(v_norm), std::nextafter(1.0f, 2.0f));
	EXPECT_GE(length(v_norm), std::nextafter(1.0f, 0.0f));
}