//@	{
//@	"dependencies_extra":[{"ref":"./font_mapper.o","rel":"implementation"}]
//@	}

#ifndef FRUIT_FONTMAPPER_HPP
#define FRUIT_FONTMAPPER_HPP

#include <fontconfig/fontconfig.h>
#include <filesystem>
#include <cassert>

namespace fruit
{
	class FontMapper
	{
	public:
		FontMapper();

		~FontMapper()
		{
			reset();
		}

		FontMapper(FontMapper&& other)
		{
			m_handle = other.m_handle;
			other.m_handle = nullptr;
		}

		FontMapper& operator=(FontMapper&& other)
		{
			std::swap(m_handle, other.m_handle);
			other.reset();
			other.m_handle = nullptr;
			return *this;
		}

		std::filesystem::path get_path(char const* font) const
		{
			assert(valid());
			return get_path_impl(font);
		}

		bool valid() const
		{
			return m_handle != nullptr;
		}

	private:
		void reset();
		FcConfig* m_handle;

		std::filesystem::path get_path_impl(char const* font) const;
	};
}

#endif