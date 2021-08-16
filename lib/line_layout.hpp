//@	{"dependencies_extra":[{"ref":"./line_layout.o", "rel":"implementation"}]}

#ifndef FRUIT_COLUMNLAYOUT_HPP
#define FRUIT_COLUMNLAYOUT_HPP

#include "./layout_manager.hpp"
#include "./utils.hpp"
#include "./error_message.hpp"

#include <vector>
#include <algorithm>
#include <numeric>

namespace fruit
{
	class LineLayout
	{
	public:
		enum class Direction:int{LeftToRight, UpToDown};

		explicit LineLayout(Direction dir = Direction::LeftToRight):m_direction{dir}{}

		void push_back(LayoutBox box)
		{
			m_content.push_back(box);
		}

		SizeRequestResult handle(SizeRequestEvent const&) const;

		void handle(GeometryUpdateEvent const& event) const;

		void set_direction(Direction dir)
		{
			m_direction = dir;
		}

		Direction get_direction() const
		{
			return m_direction;
		}

		size_t widget_count() const { return std::size(m_content); }

	private:
		Direction m_direction;
		std::vector<LayoutBox> m_content;
	};
}

#endif