#ifndef FRUIT_GEOMETRYUPDATEEVENT_HPP
#define FRUIT_GEOMETRYUPDATEEVENT_HPP

#include "./viewport_size.hpp"
#include "./point.hpp"

namespace fruit
{
	struct GeometryUpdateEvent
	{
		ViewportSize size;
		Point<int> location;
	};
}

#endif