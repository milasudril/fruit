#ifndef FRUIT_LIB_UIMANAGER_HPP
#define FRUIT_LIB_UIMANAGER_HPP

#include "./ui_renderer.hpp"
#include "./event_dispatcher.hpp"

namespace fruit
{
	template<class ... DispatchedEvents>
	class UiManager: public EventDispatcher<DispatchedEvents...>
	{
	public:
		template<class T>
		void bind_to_renderer(std::reference_wrapper<T> obj)
		{
			m_renderer.bind(obj);
		}

		void set_viewport_size(int width, int height)
		{
			m_renderer.set_viewport_size(width, height);
		}

		template<class Callback, class Tag>
		void set_display_handler(std::reference_wrapper<Callback> cb, Tag)
		{
			m_renderer.event_handler(cb, Tag{});
		}

		void update()
		{
			m_renderer.update();
		}

	private:
		UiRenderer m_renderer;
	};
}

#endif