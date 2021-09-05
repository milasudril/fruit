//@	{"target":{"name":"content_box.o"}}

#include "./content_box.hpp"
#include "./fill_ops.hpp"

namespace
{
	template<class T>
	decltype(auto) handle(const fruit::SizeRequestEvent& event, T const& item)
	{
		return item.handle(event).min_size;
	}

	decltype(auto) handle(const fruit::SizeRequestEvent&, std::monostate)
	{
		return fruit::SizeRequestResult{}.min_size;
	}
}

fruit::SizeRequestResult fruit::ContentBox::handle(SizeRequestEvent const& event) const
{
	auto size = m_padding_near
		+ m_padding_far
		+ m_border_width_near
		+ m_border_width_far;

	auto res = std::visit([&event](auto const& item){
		return ::handle(event, item);
	}, m_content);
	size += Vector{res.width, res.height, 0};

	res = ViewportSize{size.x(), size.y()};

	return SizeRequestResult{res, res};
}

void fruit::ContentBox::handle(UpdateEventSw const& event) const
{
	auto const size_vec = Vector{m_size.width, m_size.width, 0};
	{
		// render background
		auto const rect_begin = m_location;
		auto const rect_end = m_location + size_vec;
		fill_ops::source_over(event.buffer, rect_begin, rect_end, m_bg_color);
	}
#if 0

	m_content.compose(buffer, m_text_color);

	{
		// render top border
		auto const rect_begin = m_location;
		auto const rect_end = m_location + Vector{m_width, m_border_width_near.y(), 0};
		source_over(buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// render bottom border
		auto const rect_begin = m_location + Vector{0, -m_border_width_near.y(), 0};
		auto const rect_end = m_location + size_vec;
		source_over(buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// render left border
		auto const rect_begin = m_location
		auto const rect_end = m_location + Vector{m_border_width_near.x(), m_height, 0};
		source_over(buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// render right border
		auto const rect_begin = m_location  + Vector{-m_border_width_near.x(), 0, 0};
		auto const rect_end = m_location + size_vec;
		source_over(buffer, rect_begin, rect_end, m_border_color);
	}
#endif
}
