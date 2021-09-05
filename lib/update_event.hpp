#ifndef FRUIT_UPDATE_EVENT_HPP
#define FRUIT_UPDATE_EVENT_HPP

#include "./pixel.hpp"
#include "./image_view.hpp"

#include <cstdint>
#include <string>

namespace fruit
{
	struct UpdateEventSw
	{
		ImageView<Pixel> buffer;
	};
}

#endif