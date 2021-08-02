#ifndef FRUIT_COLUMN_LAYOUT_HPP
#define FRUIT_COLUMN_LAYOUT_HPP

#include "./event_handler.hpp"

namespace fruit
{
	class ColumnLayout
	{
	public:
		template<class LayoutBox>
		void push_back(LayoutBox& object)
		{
			m_content.push_back(box);
		}

		void handle(ViewportSizeEvent const& event)
		{
		}

	private:
		std::vector<std::pair<EventHandler<SizeRequestEvent>,
			EventHandler<GeometryUpdateEvent>> m_content;
	};
}

#endif