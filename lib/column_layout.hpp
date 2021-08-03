#ifndef FRUIT_COLUMN_LAYOUT_HPP
#define FRUIT_COLUMN_LAYOUT_HPP

#include "./event_handler.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"

#include <vector>
#include <algorithm>

namespace fruit
{
	using LayoutBox = EventHandler<SizeRequestEvent, GeometryUpdateEvent>;

	class ColumnLayout
	{
	public:
		void push_back(LayoutBox box)
		{
			m_content.push_back(box);
		}

		void handle(GeometryUpdateEvent const& event)
		{
			std::ranges::for_each(m_content, [origin = event.location](auto item) mutable {
				auto const size = item.handle(SizeRequestEvent{});
				item.handle(GeometryUpdateEvent{size, origin});
				origin += Vector{0, size.height, 0};
			});
		}

	private:
		std::vector<LayoutBox> m_content;
	};
}

#endif