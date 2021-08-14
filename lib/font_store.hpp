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
		FontResource load_and_replace(std::vector<std::byte>&& buffer);

		FontResource load_and_replace(std::string&& resource_name, std::vector<std::byte>&& buffer)
		{
			auto ip = m_fonts.insert_or_assign(std::move(resource_name), fruit::FontFace{m_loader, std::move(buffer)});
			return FontResource{ip.first->first, &ip.first->second};
		}

		template<class Source>
		requires requires(Source s)
		{ {io_utils::load(s)} -> std::same_as<std::vector<std::byte>>; }
		FontResource load_and_replace(Source&& src)
		{
			return load_and_replace(io_utils::load(std::forward<Source>(src)));
		}

		template<class Source>
		requires requires(Source s)
		{ { io_utils::load(s)} -> std::same_as<std::vector<std::byte>>; }
		FontResource load_and_replace(std::string&& resource_name, Source&& src)
		{
			return load_and_replace(std::move(resource_name), io_utils::load(std::forward<Source>(src)));
		}

		FontResource load_and_replace(std::string&& name, FontMapper const& font_mapper)
		{
			auto path = font_mapper.get_path(name.c_str());
			return load_and_replace(std::move(name), std::move(path));
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

		template<class Source>
		FontResource get_or_load(std::string_view resource_name, Source&& source)
		{
			if(auto ret = find(resource_name); ret.font != nullptr)
			{ return ret; }

			return load_and_replace(std::string{resource_name}, std::move(source));
		}

	private:
		FontfaceLoader m_loader;
		std::map<std::string, FontFace, std::less<>> m_fonts;
	};
}

#endif