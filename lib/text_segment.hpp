//@	{
//@		"dependencies_extra":[{"ref":"./text_segment.o", "rel":"implementation"}],
//@		"dependencies": [{"ref":"harfbuzz", "origin":"pkg-config"}]
//@	}

#ifndef FRUIT_LIB_TEXTSEGMENT_HPP
#define FRUIT_LIB_TEXTSEGMENT_HPP

#include "./text_properties.hpp"
#include "./error_message.hpp"
#include "./text_shaper.hpp"
#include "./viewport_size.hpp"
#include "./image.hpp"

#include <hb.h>

#include <string_view>
#include <memory>
#include <stdexcept>

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

	struct GlyphInfo
	{
		GlyphIndex index;
		uint32_t start_offset;
	};

	struct GlyphGeometry
	{
		Vector<int> cursor_increment{0, 0, 0};
		Vector<int> render_offset{0, 0, 0};
	};

	class TextShapeResult
	{
	public:
		TextShapeResult():m_glyph_count{0}, m_font{nullptr}, m_direction{}, m_char_height{0}{}

		bool valid() const { return m_glyph_count != 0; }

		explicit TextShapeResult(uint32_t num_glyphs,
		                         hb_glyph_info_t const* info,
		                         hb_glyph_position_t const* geom,
		                         std::reference_wrapper<FontFace const> font,
		                         TextDirection direction,
		                         int char_height);

		std::span<GlyphInfo const> glyph_info() const
		{
			FRUIT_ASSERT(m_glyph_info.get() != nullptr);
			return std::span{m_glyph_info.get(), m_glyph_count};
		}

		std::span<GlyphGeometry const> glyph_geometry() const
		{
			FRUIT_ASSERT(m_glyph_geometry.get() != nullptr);
			return std::span{m_glyph_geometry.get(), m_glyph_count};
		}

		FontFace const& font() const
		{
			return *m_font;
		}

		int char_height() const
		{
			return m_char_height;
		}

		TextDirection direction() const
		{
			return m_direction;
		}

		size_t glyph_count() const { return m_glyph_count; }

	private:
		size_t m_glyph_count;
		std::unique_ptr<GlyphInfo[]> m_glyph_info;
		std::unique_ptr<GlyphGeometry[]> m_glyph_geometry;
		FontFace const* m_font;
		TextDirection m_direction;
		int m_char_height;
	};

	using TextAlphaMask = pixel_store::image<uint8_t>;

	TextAlphaMask render(TextShapeResult const& res);

	class TextSegment
	{
	public:
		TextSegment& text(std::basic_string_view<char8_t> buffer) &
		{
			m_text = buffer;
			return *this;
		}

		TextSegment&& text(std::basic_string_view<char8_t> buffer) &&
		{
			m_text = buffer;
			return std::move(*this);
		}

		TextSegment& direction(TextDirection val) &
		{
			m_properties.direction = val;
			return *this;
		}

		TextSegment&& direction(TextDirection val) &&
		{
			m_properties.direction = val;
			return std::move(*this);
		}


		TextDirection direction() const
		{
			return m_properties.direction;
		}
		TextSegment& language(LanguageTag const& lang) &
		{
			m_properties.language = lang;
			return *this;
		}

		TextSegment&& language(LanguageTag const& lang) &&
		{
			m_properties.language = lang;
			return std::move(*this);
		}

		LanguageTag const& language() const
		{
			return m_properties.language;
		}

		TextSegment& script(WritingSystem val) &
		{
			m_properties.script = val;
			return *this;
		}

		TextSegment&& script(WritingSystem val) &&
		{
			m_properties.script = val;
			return std::move(*this);
		}

		WritingSystem script() const
		{
			return m_properties.script;
		}

		TextShapeResult shape(TextShaper const& shaper) const &
		{
			FRUIT_ASSERT(shaper.valid());
			return shape_impl(shaper);
		}

	private:
		TextShapeResult shape_impl(TextShaper const& shaper) const;
		TextProperties m_properties;
		std::basic_string<char8_t> m_text;
	};
}


#endif
