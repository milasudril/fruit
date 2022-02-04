//@	{"target":{"name":"text_box.o"}}

#include "./text_box.hpp"

void fruit::TextBox::handle_no_result(SizeRequestEvent const&) const
{
	do_render();
	if(std::size(m_render_result.first) == 0)
	{
		auto const min_size = is_horizontal(m_text.direction())?
			ViewportSize{0, m_char_height} : ViewportSize{m_char_height, 0};
		m_size = SizeRequestResult{min_size, min_size};
		return;
	}

	auto const& img = m_render_result.second;
	auto const w = std::ranges::max_element(img, [](auto const& a, auto const& b) {
		return a.width() < b.width();
	}) -> width();

	auto const h = std::ranges::max_element(img, [](auto const& a, auto const& b) {
		return a.height() < b.height();
	}) -> height();

	auto const min_size = ViewportSize{static_cast<int>(w), static_cast<int>(h)};
	m_size = SizeRequestResult{min_size, min_size};
}

void fruit::TextBox::do_render() const
{
	auto shape_res = m_text.shape(TextShaper{m_font, m_char_height});
	if(std::ranges::all_of(shape_res, [](auto const& item) { return item.glyph_count() == 0; }))
	{
		m_render_result = {};
		return;
	}

	std::vector<TextAlphaMask> res;
	res.reserve(std::size(shape_res));
	std::ranges::transform(shape_res, std::back_inserter(res), render);
	m_render_result = std::pair{std::move(shape_res), std::move(res)};
}

void fruit::TextBox::compose(image_span<Pixel> target_buffer, Point<int> origin, Pixel color) const
{
	if(std::size(m_render_result.first) == 0)
	{
		do_render();
		if(std::size(m_render_result.first) == 0)
		{ return; }
	}

	std::ranges::for_each(m_render_result.second, [target_buffer, origin, color](auto const& item) mutable {
		auto const& img = item;
		auto const dx = Vector{static_cast<int>(img.width()), 0, 0};
		auto const dy = Vector{0, static_cast<int>(img.height()), 0};
		auto const end = origin + dx + dy;
		for(int y = origin.y(); y < std::min(end.y(), static_cast<int>(target_buffer.height())); ++y)
		{
			for(int x = origin.x(); x < std::min(end.x(), static_cast<int>(target_buffer.width())); ++x)
			{
				auto const factor = static_cast<float>(img(x - origin.x(), y - origin.y()))/255.0f;
				target_buffer(x, y).value() = factor*color.value() + target_buffer(x, y).value()*(1.0f - factor);
			}
		}
		origin += dy;
	});
}