#ifndef FRUITL_LAYOUTMANAGER_HPP
#define FRUITL_LAYOUTMANAGER_HPP

#include "./event_handler.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"

namespace fruit
{
	struct LayoutBox
	{
		template<class T>
		explicit LayoutBox(T obj, float s = 0.0f): event_handler{obj}, size{s}
		{}

		EventHandler<SizeRequestEvent, GeometryUpdateEvent> event_handler;
		float size;
	};
}

#endif