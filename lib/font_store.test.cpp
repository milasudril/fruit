//@	{
//@	"target":{"name":"font_store.test"}
//@	}

#include "./font_store.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(FontStoreLoadFile)
{
	::fruit::FontStore store;
	auto res = store.load("testdata/DejaVuSans.ttf");
	EXPECT_EQ(res.name, "DejaVu Sans/Book");
	EXPECT_EQ(res.font.get().valid(), true);
}

#if 0
#ifndef FRUIT_FONTSTORE_HPP
#define FRUIT_FONTSTORE_HPP

#include "./io_utils.hpp"
#include "./font_face.hpp"

#include <map>

namespace fruit
{
	struct FontResource
	{
		std::string_view name;
		std::reference_wrapper<FontFace const> font;
	};

	class FontStore
	{
	public:
		FontResource load(std::vector<std::byte>&& buffer);

		template<class Source>
		requires requires(Source s)
		{
			{io_utils::load(s)} -> std::same_as<std::vector<std::byte>>;
		}
		FontResource load(Source&& src)
		{
			return load(io_utils::load(std::forward<Source>(src)));
		}


	private:
		std::map<std::string, FontFace> m_fonts;
		FontfaceLoader m_loader;
	};
}

#endif
#endif