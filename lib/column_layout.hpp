#ifndef FRUIT_COLUMN_LAYOUT_HPP
#define FRUIT_COLUMN_LAYOUT_HPP

#include "./layout_box.hpp"

#include <vector>

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
		Point<int> m_location;
		std::vector<LayoutBox> m_content;
	};
}

#endif