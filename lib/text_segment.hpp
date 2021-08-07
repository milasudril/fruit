//@	 { "dependencies_extra":[{"ref":"harfbuzz", "origin":"pkg-config"}]}

#ifndef FRUIT_TEXTSEGMENT_HPP
#define FRUIT_TEXTSEGMENT_HPP

#include "./text_direction.hpp"
#include "./writing_system.hpp"
#include "./language_tag.hpp"

#include <hb.h>

#include <string_view>
#include <memory>

namespace fruit
{
	namespace text_segment_detail
	{
		struct Deleter
		{
			void operator()(hb_buffer_t* hb)
			{
				if(hb != nullptr)
				{ hb_buffer_destroy(hb); }
			}
		};
	}

	class TextSegment
	{
	public:
		TextSegment()
		{
			auto handle = hb_buffer_create();
			if(handle == nullptr)
			{
				//FIXME
			}
			m_handle.reset(handle);
		}

		bool valid() const
		{
			return m_handle != nullptr;
		}

		hb_buffer_t* get() const
		{
			return m_handle.get();
		}

		TextSegment& text(std::basic_string_view<char8_t> buffer)
		{
			auto const handle = get();
			// https://lists.freedesktop.org/archives/harfbuzz/2016-July/005711.html
			auto const dir = direction();
			auto const s = script();
			// Do not want to convert to string and back
			auto lang = hb_buffer_get_language(handle);

			hb_buffer_reset(handle);
			auto const data = reinterpret_cast<char const*>(std::data(buffer));
			auto const size = std::size(buffer);
			hb_buffer_add_utf8(handle, data, size, 0, size);

			hb_buffer_set_language(handle, lang);
			direction(dir).script(s);
			return *this;
		}

		TextSegment& direction(TextDirection val)
		{
			hb_buffer_set_direction(get(), static_cast<hb_direction_t>(val));
			return *this;
		}

		TextDirection direction() const
		{
			return static_cast<TextDirection>(hb_buffer_get_direction(get()));
		}

		TextSegment& language(LanguageTag const& lang)
		{
			hb_buffer_set_language(get(), hb_language_from_string(lang.c_str(), -1));
			return *this;
		}

		LanguageTag language() const
		{
			return LanguageTag{hb_language_to_string(hb_buffer_get_language(get()))};
		}

		TextSegment& script(WritingSystem val)
		{
			hb_buffer_set_script(get(), static_cast<hb_script_t>(val));
			return *this;
		}

		WritingSystem script() const
		{
			return static_cast<WritingSystem>(hb_buffer_get_script(get()));
		}

	private:
		std::unique_ptr<hb_buffer_t, text_segment_detail::Deleter> m_handle;
	};
}


#endif
