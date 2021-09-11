#ifndef FRUIT_GEOMETRYUPDATEEVENT_HPP
#define FRUIT_GEOMETRYUPDATEEVENT_HPP

#include "./viewport_size.hpp"
#include "./point.hpp"

namespace fruit
{
	/**
	 * \brief Describes a notification that an object has been moved or changed size
	 *
	 * \ingroup Events
	 */
	struct GeometryUpdateEvent
	{
		ViewportSize size;
		Point<int> location;
	};
}

#endif