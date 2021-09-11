#ifndef FRUIT_LIB_LANGUAGETAG_HPP
#define FRUIT_LIB_LANGUAGETAG_HPP

#include <string>

namespace fruit
{
	class LanguageTag
	{
	public:
		explicit LanguageTag(std::string&& data):m_data{std::move(data)}{}

		char const* c_str() const
		{ return m_data.c_str(); }

		std::string const& name() const
		{
			return m_data;
		}

		bool operator==(LanguageTag const&) const = default;
		bool operator!=(LanguageTag const&) const = default;

	private:
		std::string m_data;
	};

	std::string to_string(LanguageTag const&&) = delete;

	inline std::string to_string(LanguageTag const& tag)
	{
		return tag.name();
	}
}

#endif