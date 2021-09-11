#ifndef FRUIT_LOCATIONEVENT_HPP
#define FRUIT_LOCATIONEVENT_HPP

#include "./point.hpp"

namespace fruit
{
	class ButtonPressure
	{
	public:
		static constexpr float action_press = 1.0f;
		static constexpr float action_release = -1.0f;

		constexpr explicit ButtonPressure(float value):m_value{value}{}

		constexpr auto operator<=>(ButtonPressure const&) const = default;

		constexpr float value() const { return m_value; }

	private:
		float m_value;
	};

	constexpr bool state_chg_pressed(ButtonPressure obj)
	{
		return obj.value() > 0.0f;
	}

	constexpr bool state_chg_released(ButtonPressure obj)
	{
		return obj.value() < 0.0f;
	}


	struct LocationEvent
	{
		struct MouseMoveTag{};
		struct MouseButtonTag{};

		Point<float> loc;
 		ButtonPressure button_pressure;
		int active_button;
	};
}

#endif