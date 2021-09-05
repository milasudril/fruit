//@	{"dependencies_extra":[{"ref":"./content_box.o", "rel":"implementation"}]}

#ifndef FRUIT_CONTENTBOX_HPP
#define FRUIT_CONTENTBOX_HPP

#include "./text_line.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"
#include "./update_event.hpp"

#include <variant>

namespace fruit
{
	class ContentBox
	{
	public:
		ContentBox():
			m_padding_near{0, 0, 0},
			m_padding_far{0, 0, 0},
			m_border_width_near{0, 0, 0},
			m_border_width_far{0, 0, 0},
			m_text_color{0.0f, 0.0f, 0.0f, 1.0f},
			m_size{0, 0},
			m_location{Origin<int>},
			m_bg_color{1.0, 1.0, 1.0, 1.0},
			m_border_color{0, 0, 0, 0}
		{}

		SizeRequestResult handle(SizeRequestEvent const& event) const;

		void handle(GeometryUpdateEvent const& event)
		{
			m_size = event.size;
			m_location = event.location;
		}

		void handle(UpdateEventSw const& event) const;

	private:
		Vector<int> m_padding_near;
		Vector<int> m_padding_far;
		Vector<int> m_border_width_near;
		Vector<int> m_border_width_far;

		std::variant<std::monostate, TextLine> m_content;
		Pixel m_text_color;

		ViewportSize m_size;
		Point<int> m_location;

		Pixel m_bg_color;
		Pixel m_border_color;
	};
}

#endif