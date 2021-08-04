//@	 {
//@	 "target":{"name":"basic_font_face.test"}
//@	}

#include "./basic_font_face.hpp"

#include "testfwk/testfwk.hpp"

#include <vector>
#include <array>

namespace
{
	template<int ExpectedHandle>
	class FontLoaderTest
	{
	public:
		using FontFaceHandle = int;
		static constexpr FontFaceHandle NullHandle = 0;

		template<class Container>
		explicit FontLoaderTest(Container const& expected_data):
		   m_expected_data{std::begin(expected_data), std::end(expected_data)}
		{
		}

		FontFaceHandle createFrom(std::byte const* src_buffer, size_t src_buffer_size)
		{
			auto data_loaded = std::vector<std::byte>{src_buffer, src_buffer + src_buffer_size};
			REQUIRE_EQ(m_expected_data, data_loaded);
			return ExpectedHandle;
		}

		static void free(FontFaceHandle h)
		{
			REQUIRE_EQ(h, ExpectedHandle);
		}

	private:
		std::vector<std::byte> m_expected_data;
		int m_expected_handle;
	};
}

TESTCASE(BasecFontFaceCreate)
{
	std::vector<std::byte> buffer{
		static_cast<std::byte>(1), static_cast<std::byte>(2), static_cast<std::byte>(3)};
	FontLoaderTest<1234> loader{buffer};

	fruit::BasicFontFace<FontLoaderTest<1234>&> foo{loader, buffer.data(), buffer.size()};
	EXPECT_EQ(foo.valid(), true);

	auto bar = std::move(foo);
	EXPECT_EQ(bar.valid(), true);
	EXPECT_EQ(foo.valid(), false);
}