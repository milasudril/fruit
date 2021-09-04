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
	auto const min_size = ViewportSize{img.width(), img.height()};
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

void fruit::TextLine::compose(Image<Pixel>& output_buffer, Point<int> origin, Pixel color) const
{
	if(!m_render_result)
	{
		do_render();
		if(!m_render_result)
		{ return; }
	}

	auto const& img = m_render_result->second;
	auto const end = origin + Vector{img.width(), img.height(), 0};
	for(int y = origin.y(); y < std::min(end.y(), output_buffer.height()); ++y)
	{
		for(int x = origin.x(); x < std::min(end.x(), output_buffer.width()); ++x)
		{
			auto const factor = static_cast<float>(img(x, y)/255.0f);
			output_buffer(x, y) = factor * color + (1.0f - factor) * color;
		}
	}
}