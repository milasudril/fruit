//@	{"target":{"name":"content_box.o"}}

#include "./content_box.hpp"

fruit::SizeRequestResult fruit::ContentBox::handle(SizeRequestEvent const& event) const
{
	auto size = m_padding_near
		+ m_padding_far
		+ m_border_width_near
		+ m_border_width_far;

	auto res = m_content.handle(event).min_size;
	size += Vector{res.width, res.height, 0};

	res = ViewportSize{size.x(), size.y()};

	return SizeRequestResult{res, res};
}

void fruit::ContentBox::handle(UpdateEventSw const& event) const
{
	auto const size_vec = Vector{m_size.width, m_size.width, 0};
	auto const buffer = event.buffer;
	{
		// render background
		auto const rect_begin = m_location;
		auto const rect_end = m_location + size_vec;
		fill_rect(buffer, rect_begin, rect_end, m_bg_color);
	}

	m_content.compose(buffer, m_text_color);

	{
		// render top border
		auto const rect_begin = m_location;
		auto const rect_end = m_location + Vector{m_width, m_border_width_near.y(), 0};
		fill_rect(buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// render bottom border
		auto const rect_begin = m_location + Vector{0, -m_border_width_near.y(), 0};
		auto const rect_end = m_location + size_vec;
		fill_rect(buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// render left border
		auto const rect_begin = m_location
		auto const rect_end = m_location + Vector{m_border_width_near.x(), m_height, 0};
		fill_rect(buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// render right border
		auto const rect_begin = m_location  + Vector{-m_border_width_near.x(), 0, 0};
		auto const rect_end = m_location + size_vec;
		fill_rect(buffer, rect_begin, rect_end, m_border_color);
	}

}
