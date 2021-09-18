//@	{"dependencies_extra":[{"ref":"glfw3", "origin":"pkg-config"}]}

#ifndef FRUIT_LIB_GLFWEVENTMAPPER_HPP
#define FRUIT_LIB_GLFWEVENTMAPPER_HPP

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
	
	template<class UserData>
	class GlfwCallbackContext
	{
	public: 
		explicit GlfwCallbackContext(GLFWwindow& window, UserData& user_data):
			m_window{window}
		{
			m_old_user_pointer = glfwGetWindowUserPointer(&m_window.get());
			glfwSetWindowUserPointer(&m_window.get(), &user_data);
		}
		
		~GlfwCallbackContext()
		{
			if(m_old_user_pointer != nullptr)
			{ glfwSetWindowUserPointer(&m_window.get(), m_old_user_pointer); }
		}
		
		GlfwCallbackContext(GlfwCallbackContext&& other):
			m_window{other.m_window},
			m_old_user_pointer{other.get()}
		{
			other.m_old_user_pointer = nullptr;
		}
		
		GlfwCallbackContext& operator=(GlfwCallbackContext&& other)
		{
			m_window = other.m_window;
			m_old_user_pointer = other.m_old_user_pointer;
			other.m_old_user_pointer = nullptr;
			return *this;
		}
		
		UserData& get() const
		{
			return *static_cast<UserData*>(glfwGetWindowUserPointer(&m_window.get()));
		}

	private:
		std::reference_wrapper<GLFWwindow> m_window;
		void* m_old_user_pointer;
	};

	template<class UserData>
	explicit GlfwCallbackContext(GLFWwindow&, UserData&) -> GlfwCallbackContext<UserData>;	
#if 0
	// Setup GLFW callbacks
	glfwSetFramebufferSizeCallback(window.get(), [](GLFWwindow* src, int w, int h){
		glViewport(0, 0, w, h);
		auto& ui = *reinterpret_cast<MyUi*>(glfwGetWindowUserPointer(src));
		ui.
		set_viewport_size(w, h);
	});

	glfwSetCursorPosCallback(window.get(), [](GLFWwindow* src, double x, double y){
		auto& ui = *reinterpret_cast<MyUi*>(glfwGetWindowUserPointer(src));
		ui.send(fruit::DeviceId{-1}, fruit::convert(fruit::LocationEvent::MouseMoveTag{}, *src, x, y));
	});

	glfwSetMouseButtonCallback(window.get(), [](GLFWwindow* src, int button, int action, int) {
		auto& ui = *reinterpret_cast<MyUi*>(glfwGetWindowUserPointer(src));
		ui.send(fruit::DeviceId{-1}, fruit::convert(fruit::LocationEvent::MouseButtonTag{}, *src, button, action));
	});

	glfwSetScrollCallback(window.get(), [](GLFWwindow* src, double dx, double dy) {
		auto& ui = *reinterpret_cast<MyUi*>(glfwGetWindowUserPointer(src));
		ui.send(fruit::DeviceId{-1}, fruit::convert(fruit::BallEvent::ScrollTag{}, *src, dx, dy));
	});
#endif
}

#endif
