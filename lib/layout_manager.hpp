#ifndef FRUITL_LAYOUTMANAGER_HPP
#define FRUITL_LAYOUTMANAGER_HPP

#include "./event_handler.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"

namespace fruit
{
	using LayoutBox = EventHandler<SizeRequestEvent, GeometryUpdateEvent>;
}

#endif