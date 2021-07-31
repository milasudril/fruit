#ifndef FRUIT_DISPLAY_CALLBACK_HPP
#define FRUIT_DISPLAY_CALLBACK_HPP

#include <concepts>

namespace fruit
{
	template<class T>
	concept DisplayCallback = requires(T x)
	{
		{x(Pixel const* source, int width, int height)} -> std::same_as<void>;
	};

	using DisplayCallbackPtr = void (*)(void* context, Pixel const* source, int width, int height);
}

#endif