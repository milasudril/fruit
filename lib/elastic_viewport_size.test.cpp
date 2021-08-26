//@	{"target":{"name":"elastic_viewport_size.test"}}

#include "./elastic_viewport_size.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(ElasticViewportSizeSizeII)
{
	fruit::ElasticViewportSize size;
	size.set_width(8);
	size.set_height(5);

	auto result = size.viewportSize(fruit::ViewportSize{5, 8});
	EXPECT_EQ(result, (fruit::ViewportSize{8, 5}));
}

TESTCASE(ElasticViewportSizeSizeIF)
{
	fruit::ElasticViewportSize size;
	size.set_width(8);
	size.set_height(0.5f);

	auto result = size.viewportSize(fruit::ViewportSize{5, 8});
	EXPECT_EQ(result, (fruit::ViewportSize{8, 4}));
}

TESTCASE(ElasticViewportSizeSizeFI)
{
	fruit::ElasticViewportSize size;
	size.set_width(0.5f);
	size.set_height(5);

	auto result = size.viewportSize(fruit::ViewportSize{5, 8});
	EXPECT_EQ(result, (fruit::ViewportSize{3, 5}));
}

TESTCASE(ElasticViewportSizeSizeFF)
{
	fruit::ElasticViewportSize size;
	size.set_width(0.5f);
	size.set_height(0.25f);

	auto result = size.viewportSize(fruit::ViewportSize{5, 8});
	EXPECT_EQ(result, (fruit::ViewportSize{3, 2}));
}
