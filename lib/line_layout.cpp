//@	 {"target":{"name":"line_layout.o"}}

#include "./line_layout.hpp"
#include "./utils.hpp"

namespace
{
	inline auto resulting_size(int value, int)
	{
		return value;
	}

	inline auto resulting_size(float value, int total_size)
	{
		return static_cast<int>(value*total_size + 0.5f);
	}

	inline auto resulting_size(std::variant<int, float> value, int total_size)
	{
		return std::visit([total_size](auto val){return resulting_size(val, total_size);}, value);
	}

	inline auto make_viewport_size(std::variant<int, float> width,
								   std::variant<int, float> height,
								   fruit::ViewportSize total_size)
	{
		return fruit::ViewportSize{resulting_size(width, total_size.width), resulting_size(height, total_size.height)};
	}
}

fruit::SizeRequestResult fruit::LineLayout::handle(SizeRequestEvent const& event) const
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
	auto const my_size = make_viewport_size(m_min_width, m_min_height, event.domain_size);
	min_size.width = std::max(my_size.width, min_size.width);
	min_size.height = std::max(my_size.height, min_size.height);
//	printf("LineLayout min_size %d %d\n", min_size.width, min_size.height);
	return SizeRequestResult{min_size, min_size};
}

namespace
{
	inline auto extent(fruit::ViewportSize viewport, fruit::LineLayout::Direction dir)
	{
		return dir == fruit::LineLayout::Direction::LeftToRight? viewport.width: viewport.height;
	}

	inline auto fit_viewport(fruit::ViewportSize viewport, int size, fruit::LineLayout::Direction dir)
	{
		return dir == fruit::LineLayout::Direction::TopToBottom?
			fruit::ViewportSize{viewport.width, size}: fruit::ViewportSize{size, viewport.height};
	}

	inline auto make_offset_vector(fruit::ViewportSize size, fruit::LineLayout::Direction dir)
	{
		return dir == fruit::LineLayout::Direction::TopToBottom? fruit::Vector{0, size.height, 0} : fruit::Vector{size.width, 0, 0};
	}
}

void fruit::LineLayout::handle(GeometryUpdateEvent const& event)
{
	auto content = m_content;
	normalize_sum(std::span{std::data(content), std::size(content)}, [](auto&& item) -> float& {
		return item.size;
	});

	auto remaining_boxes = std::size(content);
	std::vector<ViewportSize> sizes(std::size(content));
	std::vector<bool> completed(std::size(content));
	auto size = extent(event.size, m_direction);
	while(remaining_boxes != 0)
	{
		auto const initial_size = size;
		for(size_t k = 0 ; k != std::size(content); ++k)
		{
			if(!completed[k])
			{
				auto const& item = content[k];
				auto const size_req_result = item.event_handler.handle(SizeRequestEvent{event.size});
				auto const desired_size = static_cast<int>(item.size * initial_size + 0.5f);
				auto const too_small = desired_size < extent(size_req_result.min_size, m_direction);
				auto const computed_size = std::max(desired_size, extent(size_req_result.min_size, m_direction));
				size -= too_small? size_req_result.min_size.height : 0;
				if(too_small)
				{
					sizes[k] = fit_viewport(size_req_result.min_size, computed_size, m_direction);
					completed[k] = true;
					content[k].size = 0.0f;
					--remaining_boxes;
				}
			}
		}

		normalize_sum(std::span{std::data(content), std::size(content)}, [](auto&& item) -> float& {
			return item.size;
		});

		if(size == initial_size)
		{
			for(size_t k = 0; k != std::size(content); ++k)
			{
				if(!completed[k])
				{
					auto const& item = content[k];
					auto const size_req_result = item.event_handler.handle(SizeRequestEvent{});
					auto const computed_size = static_cast<int>(item.size * initial_size + 0.5f);
					sizes[k] = fit_viewport(size_req_result.min_size, computed_size, m_direction);
					completed[k] = true;
				}
			}
			break;
		}
	}

	auto origin = event.location;
	for(size_t k = 0; k != std::size(content); ++k)
	{
		content[k].event_handler.handle(GeometryUpdateEvent{sizes[k], origin});
		origin += make_offset_vector(sizes[k], m_direction);
	}
}