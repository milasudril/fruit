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

	void compose(fruit::ImageView<fruit::Pixel>, std::monostate, fruit::Point<int>, fruit::Pixel)
	{
	}

	template<class T>
	void compose(fruit::ImageView<fruit::Pixel> buffer, T const& item, fruit::Point<int> origin, fruit::Pixel color)
	{
		item.compose(buffer, origin, color);
	}
}

fruit::SizeRequestResult fruit::ContentBox::handle(DeviceId, SizeRequestEvent const& event) const
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

void fruit::ContentBox::handle(DeviceId, UpdateEventSw const& event) const
{
	auto const size_vec = Vector{m_size.width, m_size.height, 0};

	{
		// Background
		auto const rect_begin = m_location;
		auto const rect_end = m_location + size_vec;
		fill_ops::source_over(event.buffer, rect_begin, rect_end, m_bg_color);
	}

	std::visit([color = m_text_color,
			    buffer = event.buffer,
				origin = m_location + m_padding_near + m_border_width_near
   			](auto const& item) {
		compose(buffer, item, origin, color);
	}, m_content);

	{
		// Border top
		auto const rect_begin = m_location;
		auto const rect_end = m_location + Vector{size_vec.x(), m_border_width_near.y(), 0};
		fill_ops::source_over(event.buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// Border bottom
		auto const rect_begin = m_location + Vector{0, size_vec.y() - m_border_width_far.y(), 0};
		auto const rect_end = m_location + size_vec;
		fill_ops::source_over(event.buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// Border left
		auto const rect_begin = m_location + Vector{0, m_border_width_near.y(), 0};
		auto const rect_end = m_location + Vector{m_border_width_near.x(), size_vec.y() - m_border_width_far.y(), 0};
		fill_ops::source_over(event.buffer, rect_begin, rect_end, m_border_color);
	}

	{
		// Border right
		auto const rect_begin = m_location + Vector{size_vec.x() - m_border_width_far.x(), m_border_width_near.y(), 0};
		auto const rect_end = m_location + Vector{size_vec.x(), size_vec.y() - m_border_width_far.y(), 0};
		fill_ops::source_over(event.buffer, rect_begin, rect_end, m_border_color);
	}
}
