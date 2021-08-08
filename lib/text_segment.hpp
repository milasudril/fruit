//@	{
//@		"dependencies_extra":[{"ref":"./text_segment.o", "rel":"implementation"}],
//@		"dependencies": [{"ref":"harfbuzz", "origin":"pkg-config"}]
//@	}

#ifndef FRUIT_TEXTSEGMENT_HPP
#define FRUIT_TEXTSEGMENT_HPP

#include "./text_direction.hpp"
#include "./writing_system.hpp"
#include "./language_tag.hpp"
#include "./error_message.hpp"
#include "./text_shaper.hpp"
#include "./viewport_size.hpp"

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
		uint32_t index;
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
		explicit TextShapeResult(uint32_t num_glyphs,
		                         hb_glyph_info_t const* info,
		                         hb_glyph_position_t const* geom,
		                         std::reference_wrapper<FreetypeFontFace const> font);

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

		FreetypeFontFace const& font() const
		{
			return m_font;
		}

	private:
		size_t m_glyph_count;
		std::unique_ptr<GlyphInfo[]> m_glyph_info;
		std::unique_ptr<GlyphGeometry[]> m_glyph_geometry;
		std::reference_wrapper<FreetypeFontFace const> m_font;
	};

	ViewportSize bounding_box(TextShapeResult const& shape_result);

	void render(TextShapeResult const& res, ImageView<uint8_t> buffer);

	class TextSegment
	{
	public:
		TextSegment():m_handle{hb_buffer_create()}
		{
			if(!hb_buffer_allocation_successful(native_handle()))
			{ FRUIT_JAM("Failed to allocate hb_buffer"); }

			direction(TextDirection::LeftToRight).
				language(LanguageTag{"en-us"}).
				script(WritingSystem::Latin);
		}

		bool valid() const
		{
			return m_handle != nullptr;
		}

		hb_buffer_t* native_handle() const
		{
			return m_handle.get();
		}

		TextSegment& text(std::basic_string_view<char8_t> buffer)
		{
			FRUIT_ASSERT(valid());
			text_impl(buffer);
			return *this;
		}

		TextSegment& direction(TextDirection val)
		{
			FRUIT_ASSERT(valid());
			hb_buffer_set_direction(native_handle(), static_cast<hb_direction_t>(val));
			return *this;
		}

		TextDirection direction() const
		{
			FRUIT_ASSERT(valid());
			return static_cast<TextDirection>(hb_buffer_get_direction(native_handle()));
		}

		TextSegment& language(LanguageTag const& lang)
		{
			FRUIT_ASSERT(valid());
			hb_buffer_set_language(native_handle(), hb_language_from_string(lang.c_str(), -1));
			return *this;
		}

		LanguageTag language() const
		{
			FRUIT_ASSERT(valid());
			return LanguageTag{hb_language_to_string(hb_buffer_get_language(native_handle()))};
		}

		TextSegment& script(WritingSystem val)
		{
			hb_buffer_set_script(native_handle(), static_cast<hb_script_t>(val));
			return *this;
		}

		WritingSystem script() const
		{
			FRUIT_ASSERT(valid());
			return static_cast<WritingSystem>(hb_buffer_get_script(native_handle()));
		}

		TextShapeResult shape(TextShaper const& shaper) const
		{
			FRUIT_ASSERT(valid());
			FRUIT_ASSERT(shaper.valid());
			return shape_impl(shaper);
		}

	private:
		void text_impl(std::basic_string_view<char8_t> buffer);
		TextShapeResult shape_impl(TextShaper const& shaper) const;
		std::unique_ptr<hb_buffer_t, text_segment_detail::Deleter> m_handle;
	};
}


#endif
