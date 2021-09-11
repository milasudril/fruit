#ifndef FRUIT_LIB_UIMANAGER_HPP
#define FRUIT_LIB_UIMANAGER_HPP

#include "./ui_renderer.hpp"
#include "./event_dispatcher.hpp"

namespace fruit
{
	template<DisplayFunction UiUpdater, class ... DispatchedEvents>
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
			printf("%d %d\n", width, height);
			m_renderer.set_viewport_size(width, height);
		}

		decltype(auto) display()
		{
			return m_renderer.display();
		}

		void update()
		{
			m_renderer.update();
		}

	private:
		UiRenderer<UiUpdater> m_renderer;
	};
}

#endif