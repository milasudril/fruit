#ifndef FRUIT_LIB_UPDATE_HPP
#define FRUIT_LIB_UPDATE_HPP

#include "./update_event.hpp"
#include "./device_id.hpp"

#include <concepts>
#include <utility>

namespace fruit
{
	template<class T>
	concept DisplayFunction = requires(T x, ImageView<Pixel const> source)
	{
		{x(source)} -> std::same_as<void>;
	};

	template<class UiUpdater, DisplayFunction Display>
	requires requires(UiUpdater const& updater) {
		{updater.send(std::declval<DeviceId>(), std::declval<UpdateEventSw>())} -> std::same_as<void>;
	}
	void update(UiUpdater const& updater, DeviceId id, UpdateEventSw const& event, Display&& display)
	{
		updater.send(id, event);
		display(event.buffer);
	}
}

#endif