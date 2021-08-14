//@	{
//@	"target":{"name":"font_store.test"}
//@	}

#include "./font_store.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(FontStoreLoadAndGet)
{
	::fruit::FontStore store;
	auto res1 = store.load_and_replace("testdata/DejaVuSans.ttf");
	EXPECT_EQ(res1.name, "DejaVu Sans/Book");
	EXPECT_EQ(res1.font->valid(), true);

	::fruit::FontMapper mapper;
	auto res2 = store.load_and_replace("DejaVu Serif", mapper);
	EXPECT_EQ(res2.name, "DejaVu Serif");
	EXPECT_EQ(res2.font->valid(), true);

	auto res1_found = store.find("DejaVu Sans/Book");
	EXPECT_EQ(res1_found.font, res1.font);

	auto res2_found = store.find("DejaVu Serif");
	EXPECT_EQ(res2_found.font, res2.font);

	auto res_not_found = store.find("Bajs");
	EXPECT_EQ(res_not_found.font, nullptr)

	auto res3 = store.get_or_load("Andika", mapper);
	EXPECT_EQ(res3.font->family(), std::string_view{"Andika"});

	REQUIRE_NE(store.find("DejaVu Serif").font, nullptr);
	auto res4 = store.get_or_load("DejaVu Serif", mapper);
	EXPECT_EQ(res4.font, res2.font);
}