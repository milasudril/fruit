//@	{"target":{"name":"content_box.o"}}

#include "./content_box.hpp"

fruit::SizeRequestResult fruit::ContentBox::handle(SizeRequestEvent const& event) const
{
	auto size = m_padding_near
		+ m_padding_far
		+ m_margin_near
		+ m_margin_far
		+ m_border_width_near
		+ m_border_width_far;

	auto res = m_content.handle(event).min_size;
	size += Vector{res.width, res.height, 0};

	res = ViewportSize{size.x(), size.y()};

	return SizeRequestResult{res, res};
}