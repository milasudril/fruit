//@	 {"target":{"name":"line_layout.o"}}

#include "./line_layout.hpp"
#include "./utils.hpp"

#include <set>
#include <iterator>

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

struct SizeResult
{
	int size;
};

void fruit::LineLayout::handle(GeometryUpdateEvent const& event)
{
	auto content = m_content;
	normalize_sum(std::span{std::data(content), std::size(content)}, [](auto&& item) -> float& {
		return item.size;
	});

	if(m_direction == Direction::UpToDown)
	{
		auto remaining_boxes = std::size(content);
		std::vector<ViewportSize> sizes(std::size(content));
		std::vector<bool> completed(std::size(content));
		auto size = event.size.height;
		while(remaining_boxes != 0)
		{
			auto const initial_size = size;
			for(size_t k = 0 ; k != std::size(content); ++k)
			{
				if(!completed[k])
				{
					auto const& item = content[k];
					auto const size_req_result = item.event_handler.handle(SizeRequestEvent{});
					auto const desired_size = static_cast<int>(item.size * initial_size + 0.5f);
					auto const too_small = desired_size < size_req_result.min_size.height;
					auto const computed_size = std::max(desired_size, size_req_result.min_size.height);
					size -= too_small? size_req_result.min_size.height : 0;
					if(too_small)
					{
						sizes[k] = ViewportSize{size_req_result.min_size.width, computed_size};
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
						auto const desired_size = static_cast<int>(item.size * initial_size + 0.5f);
						sizes[k] = ViewportSize{size_req_result.min_size.width, desired_size};
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
			origin += Vector{0, sizes[k].height, 0};
		}

#if 0
		std::ranges::for_each(m_content, [initial_size = size, &size](auto& item){
			if(!item.size_was_converted)
			{
				auto const size_req_result = item.event_handler.handle(SizeRequestEvent{});
				auto const desired_size = static_cast<int>(item.size * initial_size);
				item.size_was_converted = desired_size < size_req_result.min_size.width;
				item.size_computed = std::max(desired_size, size_req_result.min_size.width);
				size -= item.size_was_converted ? size_req_result.min_size.width : 0;
			}
		});
#endif
	}


#if 0
	std::ranges::for_each(m_content, [origin = event.location,
						  input_size = m_direction == Direction::LeftToRight?
							event.size.width:event.size.height,
						  dir = m_direction](auto const& item) mutable {
		auto const res = item.event_handler.handle(SizeRequestEvent{});
		auto const size = std::max(dir == Direction::LeftToRight? res.min_size.width : res.min_size.height,
								   static_cast<int>(item.size * input_size));
		item.event_handler.handle(GeometryUpdateEvent{size, origin});
		origin += dir == Direction::LeftToRight? Vector{size, 0, 0} : Vector{0, size, 0};
	});
#endif
}
