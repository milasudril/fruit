//@	{"dependencies_extra":[{"ref":"./content_box.o", "rel":"implementation"}]}

#ifndef FRUIT_CONTENTBOX_HPP
#define FRUIT_CONTENTBOX_HPP

#include "./text_line.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"
#include "./update_event.hpp"
#include "./location_event.hpp"
#include "./event_handler.hpp"

#include <variant>

namespace fruit
{
	namespace content_box_detail
	{
		class EmptyEh
		{
		public:
			void handle(DeviceId, LocationEvent const&) const {}
		};

		inline EmptyEh empty_eh;
	}

	class ContentBox
	{
	public:
		ContentBox():
			m_event_handler{std::ref(content_box_detail::empty_eh)},
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

		template<class Callback, class Tag>
		ContentBox& event_handler(std::reference_wrapper<Callback> cb, Tag)
		{
			m_event_handler = EventHandler<LocationEvent>{cb, Tag{}};
			return *this;
		}

		SizeRequestResult handle(DeviceId, SizeRequestEvent const& event) const;

		void handle(DeviceId, GeometryUpdateEvent const& event)
		{
			m_size = event.size;
			m_location = event.location;
		}

		void handle(DeviceId sender, UpdateEventSw const& event) const;

		void handle(DeviceId sender, LocationEvent const& event) const
		{
			auto point = event.loc;
			auto const end = m_location + Vector{m_size.width, m_size.height, 0};
			if((point.x() > m_location.x() && point.x() < end.x())
				&& (point.y() > m_location.y() && point.y() < end.y()))
			{
				m_event_handler.handle(sender, event);
			}
		}

		template<class T>
		ContentBox& content(T&& value)
		{
			m_content = std::forward<T>(value);
			return *this;
		}

		ContentBox& padding_left(int value)
		{
			m_padding_near.x() = value;
			return *this;
		}

		ContentBox& padding_right(int value)
		{
			m_padding_far.x() = value;
			return *this;
		}

		ContentBox& padding_top(int value)
		{
			m_padding_near.y() = value;
			return *this;
		}

		ContentBox& padding_bottom(int value)
		{
			m_padding_far.y() = value;
			return *this;
		}

		ContentBox& border_width_left(int value)
		{
			m_border_width_near.x() = value;
			return *this;
		}

		ContentBox& border_width_right(int value)
		{
			m_border_width_far.x() = value;
			return *this;
		}

		ContentBox& border_width_top(int value)
		{
			m_border_width_near.y() = value;
			return *this;
		}

		ContentBox& border_width_bottom(int value)
		{
			m_border_width_far.y() = value;
			return *this;
		}

		ContentBox& border_color(Pixel value)
		{
			m_border_color = value;
			return *this;
		}



	private:
		EventHandler<LocationEvent> m_event_handler;

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