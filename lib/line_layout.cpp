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

void fruit::LineLayout::handle(GeometryUpdateEvent const& event)
{
	auto content = m_content;
	normalize_sum(std::span{std::data(content), std::size(content)}, m_direction);
	auto num_remaining_boxes = std::size(content);
	auto const my_size = requested_size(*this, event.size);
	auto current_size = my_size;
	std::vector<ViewportSize> sizes(std::size(content));
	std::vector<bool> completed(std::size(content));
	while(num_remaining_boxes != 0)
	{
		auto const initial_size = current_size;
		for(size_t k = 0; k != std::size(content); ++k)
		{
			if(!completed[k])
			{
				auto const& item = content[k];
				auto const min_size = item.compute_min_size();
				auto const item_req_size = requested_size(item, current_size);
				auto const sel_size = max(min_size, item_req_size);
				if(sel_size == min_size)
				{
					completed[k] = true;
					sizes[k] = sel_size;
					content[k].size = ElasticViewportSize{sel_size};
					if(m_direction == Direction::LeftToRight)
					{
						current_size.width -= sel_size.width;
					}
					else
					{
						current_size.height -= sel_size.height;
					}
					--num_remaining_boxes;
				}
			}
		}

		normalize_sum(std::span{std::data(content), std::size(content)}, m_direction);

		if(current_size == initial_size)
		{
			break;
		}
	}

	for(size_t k = 0; k != std::size(content); ++k)
	{
		if(!completed[k])
		{
			sizes[k] = requested_size(content[k], current_size);
		}
	}

	auto origin = event.location;
	for(size_t k = 0; k != std::size(content); ++k)
	{
		m_content[k].event_handler.handle(GeometryUpdateEvent{sizes[k], origin});
		origin += (m_direction == Direction::LeftToRight) ?
			Vector{sizes[k].width, 0, 0} : Vector{0, sizes[k].height, 0};
	}
}