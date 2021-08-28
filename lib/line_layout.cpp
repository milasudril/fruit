//@	 {"target":{"name":"line_layout.o"}}

#include "./line_layout.hpp"
#include "./utils.hpp"

fruit::ViewportSize fruit::LineLayout::compute_min_size() const
{
	ViewportSize s{0, 0};
	std::ranges::for_each(m_content, [&s, direction = m_direction](auto const& item) {
		auto const size_member = item.compute_min_size();
		if(direction == Direction::LeftToRight)
		{
			s.width += size_member.width;
			s.height = std::max(s.height, s.height);
		}
		else
		{
			s.width = std::max(size_member.width, s.width);
			s.height += s.height;
		}
	});
	return max(s, requested_size(*this, ViewportSize{0, 0}));
}


#if 0
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

	inline auto resulting_size(fruit::LineLayout::Minimize, int total_size)
	{
		return total_size;
	}

	inline auto resulting_size(std::variant<fruit::LineLayout::Minimize, int, float> value, int total_size)
	{
		return std::visit([total_size](auto val){return resulting_size(val, total_size);}, value);
	}

	inline auto make_viewport_size(std::variant<fruit::LineLayout::Minimize, int, float> width,
								   std::variant<fruit::LineLayout::Minimize, int, float> height,
								   fruit::ViewportSize total_size)
	{
		return fruit::ViewportSize{resulting_size(width, total_size.width), resulting_size(height, total_size.height)};
	}

	inline auto extent(fruit::ViewportSize viewport, fruit::LineLayout::Direction dir)
	{
		return dir == fruit::LineLayout::Direction::LeftToRight? viewport.width: viewport.height;
	}
}

fruit::ViewportSize fruit::LineLayout::compute_min_size(ViewportSize domain_size) const
{
	domain_size.width = resulting_size(m_min_width, domain_size.width);
	domain_size.height = resulting_size(m_min_height, domain_size.height);
	printf("Size request %p\n", this);
	auto content = m_content;
	normalize_sum(std::span{std::data(content), std::size(content)}, [](auto&& item) -> float& {
		return item.size;
	});

	auto min_size = std::accumulate(std::begin(content), std::end(content),
									ViewportSize{0, 0},
									[dir = m_direction,
									domain_extent = extent(domain_size, m_direction)
									](auto a, auto item) {
			auto const res = item.event_handler.handle(SizeRequestEvent{});
			// TODO: Need to consider item.size
			auto const size = res.min_size;
			auto const desired_size = static_cast<int>(domain_extent*item.size + 0.5f);
			auto const computed_size = std::max(extent(size, dir), desired_size);

			printf("%d ", computed_size);

			return dir == Direction::LeftToRight ?
				ViewportSize{a.width + computed_size, std::max(a.height, size.height)}
				:ViewportSize{std::max(a.width, size.width), a.height + computed_size};
		});
	puts("---------");

	auto const my_size = make_viewport_size(m_min_width, m_min_height, domain_size);
	min_size.width = std::max(my_size.width, min_size.width);
	min_size.height = std::max(my_size.height, min_size.height);
//	printf("LineLayout min_size %d %d\n", min_size.width, min_size.height);
	return min_size;
}

namespace
{

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
	auto const my_size = handle(SizeRequestEvent{event.size}).min_size;
//	auto const my_size = make_viewport_size(m_min_width, m_min_height, event.size);
	printf("My size: %s %p\n", to_string(my_size).c_str(), this);
	auto size = extent(my_size, m_direction);
	while(remaining_boxes != 0)
	{
		auto const initial_size = size;
		for(size_t k = 0 ; k != std::size(content); ++k)
		{
			if(!completed[k])
			{
				auto const& item = content[k];
				auto const size_req_result = item.event_handler.handle(SizeRequestEvent{my_size});
				auto const desired_size = static_cast<int>(item.size * initial_size + 0.5f);
				auto const min_size = extent(size_req_result.min_size, m_direction);
				auto const too_small = desired_size < min_size;
				auto const computed_size = std::max(desired_size, min_size);
				size -= too_small? min_size : 0;
				if(too_small)
				{
					sizes[k] = fit_viewport(size_req_result.min_size, computed_size, m_direction);
					printf("sizes %s\n", to_string(sizes[k]).c_str());
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
			break;
		}
	}

	for(size_t k = 0; k != std::size(content); ++k)
	{
		if(!completed[k])
		{
			auto const& item = content[k];
			auto const size_req_result = item.event_handler.handle(SizeRequestEvent{my_size});
			auto const computed_size = static_cast<int>(item.size * size + 0.5f);
			sizes[k] = fit_viewport(size_req_result.min_size, computed_size, m_direction);
			completed[k] = true;
		}
	}

	auto origin = event.location;
	for(size_t k = 0; k != std::size(content); ++k)
	{
		content[k].event_handler.handle(GeometryUpdateEvent{sizes[k], origin});
		origin += make_offset_vector(sizes[k], m_direction);
	}

//	puts("=====================");
}
#endif