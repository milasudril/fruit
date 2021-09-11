#ifndef FRUIT_LIB_UPDATEEVENT_HPP
#define FRUIT_LIB_UPDATEEVENT_HPP

#include "./pixel.hpp"
#include "./image_view.hpp"

#include <cstdint>
#include <string>

namespace fruit
{
	/**
	 * \brief Describes a notification that an object should redraw itself
	 *
	 * \ingroup Events
	 */
	struct RedrawEvent
	{
		ImageView<Pixel> buffer;
	};

	/**
	 * \brief Describes a notification that a framebuffer has been updated
	 *
	 * \ingroup Events
	 */
	struct FbUpdateEvent
	{
		ImageView<Pixel const> buffer;
	};
}

#endif