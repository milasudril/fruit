#ifndef FRUIT_COLUMNLAYOUT_HPP
#define FRUIT_COLUMNLAYOUT_HPP

#include "./layout_manager.hpp"

#include <vector>
#include <algorithm>
#include <numeric>

namespace fruit
{
	class ColumnLayout
	{
	public:
		void push_back(LayoutBox box)
		{
			m_content.push_back(box);
		}

		ViewportSize handle(SizeRequestEvent const&) const
		{
			return std::accumulate(std::begin(m_content), std::end(m_content),
					                     ViewportSize{0, 0},
					                     [](auto a, auto item) {
					auto size = item.handle(SizeRequestEvent{});
					return ViewportSize{std::max(a.width, size.width), a.height + size.height};
				});
		}

		void handle(GeometryUpdateEvent const& event) const
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