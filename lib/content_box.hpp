//@	{"dependencies_extra":[{"ref":"./content_box.o", "rel":"implementation"}]}

#ifndef FRUIT_CONTENTBOX_HPP
#define FRUIT_CONTENTBOX_HPP

#include "./text_line.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"
#include "./update_event.hpp"

namespace fruit
{
	class ContentBox
	{
	public:
		SizeRequestResult handle(SizeRequestEvent const& event) const
		{
			auto size = m_padding_near
				+ m_padding_far
				+ m_margin_near
				+ m_margin_far
				+ m_border_width_near
				+ m_border_width_far;

			auto res = m_content.handle(event).min_size;
			size += Vector{res.width, res.height, 0};

			res = ViewportSize{size.x(), size.y()};

			return SizeRequestResult{res, res};
		}

		void handle(GeometryUpdateEvent const& event);

		void handle(UpdateEventSw const& event) const;

	private:
		Point<int> m_loc;
		Vector<int> m_padding_near;
		Vector<int> m_padding_far;
		Vector<int> m_margin_near;
		Vector<int> m_margin_far;
		Vector<int> m_border_width_near;
		Vector<int> m_border_width_far;

		TextLine m_content;
		Pixel m_text_color;

		int m_current_width;
		int m_current_height;

		Pixel m_bg_color;
		Pixel m_border_color;
	};
}

#endif