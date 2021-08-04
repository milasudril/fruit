#ifndef FRUIT_BASICFONTFACE_HPP
#define FRUIT_BASICFONTFACE_HPP

#include <type_traits>
#include <cstddef>
#include <cstdint>

namespace fruit
{
	template<class FontFaceFactory>
	class BasicFontFace
	{
	public:
		using Factory = std::decay_t<FontFaceFactory>;

		explicit BasicFontFace(FontFaceFactory factory,
		                       std::byte const* src_buffer,
		                       size_t src_buffer_size):
		   m_handle{factory.createFrom(src_buffer, src_buffer_size)}
		{
		}

		BasicFontFace(BasicFontFace&& other): m_handle{other.m_handle}
		{
			other.m_handle = Factory::NullHandle;
		}

		BasicFontFace& operator=(BasicFontFace&& other)
		{
			std::swap(m_handle, other.m_handle);
			if(valid()) { Factory::free(m_handle); }
			other.m_handle = Factory::NullHandle;
			return *this;
		}

		~BasicFontFace()
		{
			if(valid()) { Factory::free(m_handle); }
		}

		bool valid() const
		{
			return m_handle != Factory::NullHandle;
		}

		BasicFontFace& size(int val)
		{
			Factory::size(m_handle, val);
			return *this;
		}

		decltype(auto) bitmap(uint32_t char_index) const
		{
			return Factory::bitmap(m_handle, char_index);
		}

		auto get() const
		{
			return m_handle;
		}

	private:
		typename Factory::FontFaceHandle m_handle;
	};
}

#endif