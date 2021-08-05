//@	 {
//@	 "targets":
//@	 	[{
//@	 	 "name":"font_mapper.o"
//@	 	,"type":"object"
//@		,"pkgconfig_libs":["fontconfig", "fmt"]
//@	 	}]
//@	}

#include "./font_mapper.hpp"

#include <stdexcept>
#include <fmt/core.h>

namespace
{
	class FontMapperError:public std::exception
	{
	public:
		char const* what() const noexcept override
		{ return "Failed to initialize the default font mapper"; }
	};
}

fruit::FontMapper::FontMapper(): m_handle{FcInitLoadConfigAndFonts()}
{
	if(m_handle == nullptr)
	{ throw FontMapperError{}; }
}

void fruit::FontMapper::reset()
{
	if(m_handle != nullptr) { FcConfigDestroy(m_handle); }
}

namespace
{
	struct FcPatternDeleter
	{
		void operator()(FcPattern* pattern)
		{
			FcPatternDestroy(pattern);
		}
	};

	class FontNameParseError:public std::runtime_error
	{
	public:
		explicit FontNameParseError(char const* font):
			std::runtime_error{fmt::format("Failed to parse font name {}", font)}
		{}
	};

	class FontNameMatchError:public std::runtime_error
	{
	public:
		explicit FontNameMatchError(char const* font):
			std::runtime_error{fmt::format("Failed to match font name {}", font)}
		{}
	};

	class GetStringFailed:public std::runtime_error
	{
	public:
		explicit GetStringFailed(char const* font):
			std::runtime_error{fmt::format("Failed to convert font resource match into a string {}", font)}
		{}
	};
}

std::filesystem::path fruit::FontMapper::get_path_impl(char const* font) const
{
	auto pattern = std::unique_ptr<FcPattern, FcPatternDeleter>{
	   FcNameParse(reinterpret_cast<FcChar8 const*>(font))};
	if(pattern == nullptr)
	{ throw FontNameParseError{font}; }

	FcConfigSubstitute(m_handle, pattern.get(), FcMatchPattern);
	FcDefaultSubstitute(pattern.get());

	FcResult res;
	auto font_resource =
	   std::unique_ptr<FcPattern, FcPatternDeleter>{FcFontMatch(m_handle, pattern.get(), &res)};
	if(font_resource == nullptr)
	{ throw FontNameMatchError{font}; }

	FcChar8* file;
	if(FcPatternGetString(font_resource.get(), FC_FILE, 0, &file) != FcResultMatch)
	{ throw GetStringFailed{font}; }

	return std::filesystem::path{reinterpret_cast<char const*>(file)};
}