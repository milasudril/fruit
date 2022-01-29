//@	{"target":{"name":"text_line.o"}}

#include "./text_line.hpp"

fruit::SizeRequestResult fruit::TextLine::handle_no_result(SizeRequestEvent const&) const
{
	do_render();
	if(!m_render_result)
	{
		auto const min_size = is_horizontal(m_text.direction())?
			ViewportSize{0, m_char_height} : ViewportSize{m_char_height, 0};
		return SizeRequestResult{min_size, min_size};
	}

	auto const& img = m_render_result->second;
	auto const min_size = ViewportSize{static_cast<int>(img.width()), static_cast<int>(img.height())};
	return SizeRequestResult{min_size, min_size};
}

void fruit::TextLine::do_render() const
{
	auto shape_res = m_text.shape(TextShaper{m_font, m_char_height});
	if(shape_res.glyph_count() == 0)
	{
		m_render_result = {};
		return;
	}

	auto res = render(shape_res);
	m_render_result = std::optional{std::pair{std::move(shape_res), std::move(res)}};
}

void fruit::TextLine::compose(image_span<Pixel> target_buffer, Point<int> origin, Pixel color) const
{
	if(!m_render_result)
	{
		do_render();
		if(!m_render_result)
		{ return; }
	}

	auto const& img = m_render_result->second;
	auto const end = origin + Vector{static_cast<int>(img.width()), static_cast<int>(img.height()), 0};
	for(int y = origin.y(); y < std::min(end.y(), static_cast<int>(target_buffer.height())); ++y)
	{
		for(int x = origin.x(); x < std::min(end.x(), static_cast<int>(target_buffer.width())); ++x)
		{
			auto const factor = static_cast<float>(img(x - origin.x(), y - origin.y()))/255.0f;
			target_buffer(x, y).value() = factor*color.value() + target_buffer(x, y).value()*(1.0f - factor);
		}
	}
}