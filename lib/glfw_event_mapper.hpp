//@	{"dependencies_extra":[{"ref":"glfw3", "origin":"pkg-config"}]}

#ifndef FRUIT_GLFW_EVENT_MAPPER_HPP
#define FRUIT_GLFW_EVENT_MAPPER_HPP

#include "./event_dispatcher.hpp"
#include "./location_event.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace fruit
{
	inline LocationEvent make_location_event(GLFWwindow& src, int button, int action)
	{
		auto change_mask = static_cast<uint64_t>(1) << static_cast<uint64_t>(button);

		if(action == GLFW_RELEASE)
		{ change_mask = ~change_mask; }

		double x{};
		double y{};
		glfwGetCursorPos(&src, &x, &y);

		return LocationEvent{Point{static_cast<float>(x), static_cast<float>(y), 0.0f}, change_mask};
	}

	inline LocationEvent make_location_event(GLFWwindow&, double x, double y)
	{
		return LocationEvent{Point{static_cast<float>(x), static_cast<float>(y), 0.0f}, std::optional<uint64_t>{}};
	}
}

#endif