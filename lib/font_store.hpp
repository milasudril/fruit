//@	{
//@	"dependencies_extra":[{"ref":"./font_store.o","rel":"implementation"}]
//@	}

#ifndef FRUIT_FONTSTORE_HPP
#define FRUIT_FONTSTORE_HPP

#include "./io_utils.hpp"
#include "./font_face.hpp"
#include "./font_mapper.hpp"

#include <map>

namespace fruit
{
	struct FontResource
	{
		std::string_view name;
		FontFace const* font;
	};

	class FontStore
	{
	public:
		FontResource load(std::vector<std::byte>&& buffer);

		template<class Source>
		requires requires(Source s)
		{
			{io_utils::load(s)} -> std::same_as<std::vector<std::byte>>;
		}
		FontResource load(Source&& src)
		{
			return load(io_utils::load(std::forward<Source>(src)));
		}

		FontResource load(FontMapper const& font_mapper, char const* name)
		{
			return load(font_mapper.get_path(name));
		}

		FontResource find(std::string_view item) const
		{
			auto i = m_fonts.find(item);
			if(i == std::end(m_fonts))
			{
				return FontResource{item, nullptr};
			}

			return FontResource{i->first, &i->second};
		}

	private:
		FontfaceLoader m_loader;
		std::map<std::string, FontFace, std::less<>> m_fonts;
	};
}

#endif