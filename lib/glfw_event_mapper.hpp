//@	{"dependencies_extra":[{"ref":"glfw3", "origin":"pkg-config"}]}

#ifndef FRUIT_GLFW_EVENT_MAPPER_HPP
#define FRUIT_GLFW_EVENT_MAPPER_HPP

#include "./event_dispatcher.hpp"
#include "./location_event.hpp"
#include "./ball_event.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace fruit
{
	inline LocationEvent convert(LocationEvent::MouseButtonTag, GLFWwindow& src, int button, int action)
	{
		double x{};
		double y{};
		glfwGetCursorPos(&src, &x, &y);

		return LocationEvent{
			Point{static_cast<float>(x), static_cast<float>(y), 0.0f},
			action == GLFW_RELEASE? ButtonPressure{ButtonPressure::action_release}:
				ButtonPressure{ButtonPressure::action_press},
			button
		};
	}

	inline LocationEvent convert(LocationEvent::MouseMoveTag, GLFWwindow&, double x, double y)
	{
		return LocationEvent{
			Point{static_cast<float>(x), static_cast<float>(y), 0.0f},
			ButtonPressure{0.0f},
			0};
	}

	inline BallEvent convert(BallEvent::ScrollTag, GLFWwindow&, double dx, double dy)
	{

		return BallEvent{Vector{static_cast<float>(dx), static_cast<float>(dy), 0.0f}};
	}
}

#endif