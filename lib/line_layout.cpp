//@	 {"target":{"name":"line_layout.o"}}

#include "./line_layout.hpp"
#include "./utils.hpp"

fruit::SizeRequestResult fruit::LineLayout::handle(SizeRequestEvent const&) const
{
	auto min_size = std::accumulate(std::begin(m_content), std::end(m_content),
									ViewportSize{0, 0},
									[dir = m_direction](auto a, auto item) {
			auto const res = item.event_handler.handle(SizeRequestEvent{});
			auto const size = res.min_size;
			return dir == Direction::LeftToRight ?
				ViewportSize{a.width + size.width, std::max(a.height, size.height)}
				:ViewportSize{std::max(a.width, size.width), a.height + size.height};
		});
	return SizeRequestResult{min_size, min_size};
}

void fruit::LineLayout::handle(GeometryUpdateEvent const& event)
{
	normalize_sum(std::span{std::data(m_content), std::size(m_content)}, [](auto&& item) -> float& {
		return item.size;
	});

	std::ranges::for_each(m_content, [origin = event.location, dir = m_direction](auto item) mutable {
		auto const res = item.event_handler.handle(SizeRequestEvent{});
		auto const size = res.min_size;
		item.event_handler.handle(GeometryUpdateEvent{size, origin});
		origin += dir == Direction::LeftToRight? Vector{size.width, 0, 0} : Vector{0, size.height, 0};
	});
}
