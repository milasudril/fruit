#ifndef FRUIT_COLUMN_LAYOUT_HPP
#define FRUIT_COLUMN_LAYOUT_HPP

#include "./event_dispatcher.hpp"

namespace fruit
{
	class ColumnLayout
	{
	public:
		void push_back(LayoutBox box)
		{
			m_content.push_back(box);
		}

		void handle(ViewportSizeEvent const& event)
		{
			auto loc = m_location;

		}

	private:

	};
}

#endif