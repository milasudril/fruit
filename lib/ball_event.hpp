#ifndef FRUIT_LIB_BALLEVENT_HPP
#define FRUIT_LIB_BALLEVENT_HPP

#include "./vector.hpp"

namespace fruit
{
	/**
	 * \brief Describes some kind of rotation event
	 *
	 * A BallEvent describes some kind of rotation event. Typical applications include propagation
	 * of mouse wheel events, where `offset` will contain the amout (and direction) the wheel been
	 * turned.
	 *
	 * \ingroup Events
	 */
	struct BallEvent
	{
		struct ScrollTag{};

		Vector<float> offset;  /**< The amount of rotation around different axes */
	};
}

#endif