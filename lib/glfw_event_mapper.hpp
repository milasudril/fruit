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

			glfwSetMouseButtonCallback(&window, [](GLFWwindow*, int button, int, int) {
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