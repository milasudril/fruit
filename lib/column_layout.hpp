#ifndef FRUIT_COLUMN_LAYOUT_HPP
#define FRUIT_COLUMN_LAYOUT_HPP

#include "./event_handler.hpp"

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

		void handle(ViewportSizeEvent const& event)
		{
		}

	private:
		std::vector<LayoutBox> m_content;
	};
}

#endif