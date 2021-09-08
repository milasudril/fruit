#ifndef FRUIT_LOCATIONEVENT_HPP
#define FRUIT_LOCATIONEVENT_HPP

#include "./point.hpp"

#include <optional>

namespace fruit
{
	struct LocationEvent
	{
		Point<float> loc;
		std::optional<uint64_t> btn_state_chg_mask;
	};
}

#endif