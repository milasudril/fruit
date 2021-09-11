//@	{"dependencies_extra":[{"ref":"./layout_box.o", "rel":"implementation"}]}

#ifndef FRUIT_LIB_LAYOUTBOX_HPP
#define FRUIT_LIB_LAYOUTBOX_HPP

#include "./event_handler.hpp"
#include "./size_request_event.hpp"
#include "./geometry_update_event.hpp"
#include "./elastic_viewport_size.hpp"
#include "./update.hpp"

#include <span>

namespace fruit
{
	/**
	* \brief Determines the layout direction
	*
	* The direction determines the location scheme of the LineLayout
	*
	*/
	enum class LayoutDirection:int
	{
		LeftToRight,  /**< Use left-to-right or horizontal location scheme*/
		TopToBottom   /**< Use top-to-bottom or vertical location scheme*/
	};

	struct LayoutBox
	{
		template<class T, class ... Sizes>
		explicit LayoutBox(T obj, Sizes&&...  s): event_handler{obj}, size{std::forward<Sizes>(s)...}
		{}

		EventHandler<SizeRequestEvent, GeometryUpdateEvent, UpdateEventSw> event_handler;
		ElasticViewportSize size;

		ViewportSize compute_min_size(DeviceId sender) const
		{
			return max(size.viewportSize(ViewportSize{0, 0}),
				event_handler.handle(sender, SizeRequestEvent{ViewportSize{0, 0}}).min_size);
		}
	};

	inline ViewportSize requested_size(LayoutBox const& box, ViewportSize domain_size)
	{ return box.size.viewportSize(domain_size); }

	void normalize_sum(std::span<LayoutBox> sizes, LayoutDirection direction);
}

#endif