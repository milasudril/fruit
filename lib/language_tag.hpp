#ifndef FRUIT_LANGUAGETAG_HPP
#define FRUIT_LANGUAGETAG_HPP

#include <string>

namespace fruit
{
	class LanguageTag
	{
	public:
		explicit LanguageTag(std::string&& data):m_data{std::move(data)}{}

		char const* c_str() const
		{ return m_data.c_str(); }

	private:
		std::string m_data;
	};
}

#endif