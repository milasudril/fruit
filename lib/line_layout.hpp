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
			m_cell_sizes.push_back(0.0f);
		}

		SizeRequestResult handle(SizeRequestEvent const&) const;

		void handle(GeometryUpdateEvent const& event) const
		{
			std::ranges::for_each(m_content, [origin = event.location, dir = m_direction](auto item) mutable {
				auto const res = item.handle(SizeRequestEvent{});
				auto const size = res.min_size;
				item.handle(GeometryUpdateEvent{size, origin});
				origin += dir == Direction::LeftToRight? Vector{size.width, 0, 0} : Vector{0, size.height, 0};
			});
		}

		void set_direction(Direction dir)
		{
			m_direction = dir;
		}

		void set_cell_sizes(std::vector<float> sizes)
		{
			FRUIT_ASSERT(std::size(sizes) == std::size(m_content));
			normalize_sum(std::span{std::data(sizes), std::size(sizes)});
			m_cell_sizes = std::move(sizes);
		}

		Direction get_direction() const
		{
			return m_direction;
		}

		size_t widget_count() const { return std::size(m_content); }

	private:
		Direction m_direction;
		std::vector<LayoutBox> m_content;
		std::vector<float> m_cell_sizes;
	};
}

#endif