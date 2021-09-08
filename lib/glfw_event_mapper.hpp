//@	{"dependencies_extra":[{"ref":"glfw3", "origin":"pkg-config"}]}

#ifndef FRUIT_GLFW_EVENT_MAPPER_HPP
#define FRUIT_GLFW_EVENT_MAPPER_HPP

#include "./event_dispatcher.hpp"
#include "./location_event.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace fruit
{
	class GlfwEventMapper
	{
	public:
		template<class Callback>
		explicit GlfwEventMapper(GLFWwindow& window,
			std::reference_wrapper<Callback> cb,
			DeviceId device):
			m_eh{cb},
			m_dev_id{device},
			m_window{window},
			m_old_user_ptr{glfwGetWindowUserPointer(&window)}
		{
			glfwSetWindowUserPointer(&window, this);

			glfwSetCursorPosCallback(&window, [](GLFWwindow* src, double x, double y){

			});

			glfwSetMouseButtonCallback(&window, [](GLFWwindow* src, int button, int action, int) {
				auto const self = static_cast<GlfwEventMapper*>(glfwGetWindowUserPointer(src));
				if(action == GLFW_PRESS)
				{ self->m_mouse_button_state |= (1 << button); }
				else
				{ self->m_mouse_button_state &= ~( 1 << button); }
				double x{};
				double y{};
				glfwGetCursorPos(src, &x, &y);
				self->m_eh.send(m_dev_id,
								LocationEvent{
									Point{static_cast<float>(x), static_cast<float>(y), 0},
									button,
									action == GLFW_PRESS?ButtonState::pushing:Button::releasing});
			});
		}

		~GlfwEventMapper()
		{
			glfwSetWindowUserPointer(m_window, m_old_user_ptr);
		}

	private:
		EventDispatcher<LocationEvent> m_eh;
		DeviceId m_dev_id;
		uint64_t m_mouse_button_state;
		GLFWwindow* m_window;
		void* m_old_user_ptr;
	};
}

#endif