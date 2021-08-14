//@	{"target":{"name":"utils.test"}}

#include "./utils.hpp"

#include "testfwk/testfwk.hpp"

#include <array>

TESTCASE(NormalizeSum)
{
	std::array<float, 3> values{1.0f, 3.0f, 4.0f};

	::fruit::normalize_sum(std::span{std::data(values), 3});

	EXPECT_EQ(values, (std::array<float, 3>{1.0f/8.0f, 3.0f/8.0f, 4.0f/8.0f}));
}