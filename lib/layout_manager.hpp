#ifndef FRUITL_LAYOUTMANAGER_HPP
#define FRUITL_LAYOUTMANAGER_HPP

#include "./event_handler.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"
#include "./elastic_viewport_size.hpp"

namespace fruit
{
	struct LayoutBox
	{
		template<class T, class ... Sizes>
		explicit LayoutBox(T obj, Sizes&&...  s): event_handler{obj}, size{std::forward<Sizes>(s)...}
		{}

		EventHandler<SizeRequestEvent, GeometryUpdateEvent> event_handler;
		ElasticViewportSize size;
	};

	ViewportSize requested_size(LayoutBox const& box, ViewportSize domain_size)
	{ return box.size.viewportSize(domain_size); }
}

#endif