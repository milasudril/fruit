#ifndef FRUIT_DISPLAY_CALLBACK_HPP
#define FRUIT_DISPLAY_CALLBACK_HPP

#include "./pixel.hpp"

#include <concepts>
#include <utility>

namespace fruit
{
	template<class T>
	concept DisplayCallback = requires(T x, Pixel const* source, int width, int height)
	{
		{x(source, width, height)} -> std::same_as<void>;
	};

	using DisplayCallbackPtr = void (*)(void* context, Pixel const* source, int width, int height);
}

#endif