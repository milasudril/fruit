#ifndef FRUIT_LIB_TEXTPROPERTIES_HPP
#define FRUIT_LIB_TEXTPROPERTIES_HPP

#include "./text_direction.hpp"
#include "./writing_system.hpp"
#include "./language_tag.hpp"

namespace fruit
{
	struct TextProperties
	{
		TextDirection direction{TextDirection::LeftToRight};
		LanguageTag language{LanguageTag{"en-us"}};
		WritingSystem script{WritingSystem::Latin};
	};
}

#endif