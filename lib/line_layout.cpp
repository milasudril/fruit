//@	 {"target":{"name":"line_layout.o"}}

#include "./line_layout.hpp"

fruit::SizeRequestResult fruit::LineLayout::handle(SizeRequestEvent const&) const
{
	auto min_size = std::accumulate(std::begin(m_content), std::end(m_content),
									ViewportSize{0, 0},
									[dir = m_direction](auto a, auto item) {
			auto const res = item.handle(SizeRequestEvent{});
			auto const size = res.min_size;
			return dir == Direction::LeftToRight ?
				ViewportSize{a.width + size.width, std::max(a.height, size.height)}
				:ViewportSize{std::max(a.width, size.width), a.height + size.height};
		});
	return SizeRequestResult{min_size, min_size};
}
