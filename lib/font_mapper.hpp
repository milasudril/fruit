//@	{
//@	"dependencies_extra":[{"ref":"./font_mapper.o","rel":"implementation"}]
//@	}

#ifndef FRUIT_FONTMAPPER_HPP
#define FRUIT_FONTMAPPER_HPP

#include "./error_message.hpp"

#include <fontconfig/fontconfig.h>

#include <filesystem>
#include <memory>

namespace fruit
{
	namespace font_mapper_detail
	{
		struct Deleter
		{
			void operator()(FcConfig* handle)
			{
				if(handle != nullptr)
				{ FcConfigDestroy(handle); }
			}
		};
	}

	class FontMapper
	{
	public:
		FontMapper();

		std::filesystem::path get_path(char const* font) const
		{
			FRUIT_ASSERT(valid());
			return get_path_impl(font);
		}

		bool valid() const
		{
			return m_handle != nullptr;
		}

	private:
		std::unique_ptr<FcConfig, font_mapper_detail::Deleter> m_handle;

		std::filesystem::path get_path_impl(char const* font) const;
	};
}

#endif