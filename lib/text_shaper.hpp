//@	{
//@	"dependencies":[{"ref":"harfbuzz", "origin":"pkg-config"}]
//@	}

#ifndef FRUIT_TEXTSHAPER_HPP
#define FRUIT_TEXTSHAPER_HPP

#include "./font_face.hpp"
#include "./point.hpp"

#include <hb.h>
#include <hb-ft.h>

#include <memory>
#include <stdexcept>

namespace fruit
{
	namespace text_shaper_detail
	{
		struct Deleter
		{
			void operator()(hb_font_t* font)
			{
				if(font != nullptr)
				{
					hb_font_destroy(font);
				}
			}
		};

		class InitError:public std::exception
		{
		public:
			char const* what() const noexcept override
			{ return "Failed to create a text shaper from the selected font"; }
		};
	}

	class TextShaper
	{
	public:
		explicit TextShaper(std::reference_wrapper<FontFace const> face, int char_height = 16):
		m_face{face},
		m_handle{hb_ft_font_create(face.get().native_handle(), nullptr)},
		m_char_height{char_height}
		{
			if(m_handle == nullptr)
			{ throw text_shaper_detail::InitError{}; }
		}

		bool valid() const
		{
			return m_handle != nullptr;
		}

		FontFace const& font() const
		{
			return m_face.get();
		}

		int char_height() const
		{
			return m_char_height;
		}

		hb_font_t* native_handle() const
		{
			return m_handle.get();
		}

	private:
		std::reference_wrapper<FontFace const> m_face;
		std::unique_ptr<hb_font_t, text_shaper_detail::Deleter> m_handle;
		int m_char_height;
	};
}


#endif
