#ifndef FRUIT_COLUMNLAYOUT_HPP
#define FRUIT_COLUMNLAYOUT_HPP

#include "./layout_manager.hpp"

#include <vector>
#include <algorithm>
#include <numeric>

namespace fruit
{
	class LineLayout
	{
	public:
		enum class Direction{LeftToRight, UpToDown};

		explicit LineLayout(Direction dir = Direction::LeftToRight):m_direction{dir}{}

		void push_back(LayoutBox box)
		{
			m_content.push_back(box);
		}

		ViewportSize handle(SizeRequestEvent const&) const
		{
			return std::accumulate(std::begin(m_content), std::end(m_content),
					                     ViewportSize{0, 0},
					                     [dir = m_direction](auto a, auto item) {
					auto size = item.handle(SizeRequestEvent{});
					return dir == Direction::LeftToRight ?
						ViewportSize{a.width + size.width, std::max(a.height, size.height)}
						:ViewportSize{std::max(a.width, size.width), a.height + size.height};
				});
		}

		void handle(GeometryUpdateEvent const& event) const
		{
			std::ranges::for_each(m_content, [origin = event.location, dir = m_direction](auto item) mutable {
				auto const size = item.handle(SizeRequestEvent{});
				item.handle(GeometryUpdateEvent{size, origin});
				origin += dir == Direction::LeftToRight? Vector{size.width, 0, 0} : Vector{0, size.height, 0};
			});
		}

		void set_direction(Direction dir)
		{
			m_direction = dir;
		}

		Direction get_direction() const
		{
			return m_direction;
		}

	private:
		Direction m_direction;
		std::vector<LayoutBox> m_content;
	};
}

#endif