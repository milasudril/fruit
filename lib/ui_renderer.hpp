#ifndef FRUIT_UIRENDERER_HPP
#define FRUIT_UIRENDERER_HPP

#include "./update.hpp"
#include "./event_dispatcher.hpp"
#include "./geometry_update_event.hpp"
#include "./image.hpp"

namespace fruit
{
	template<DisplayFunction UiUpdater>
	class UiRenderer
	{
	public:
		explicit UiRenderer(DeviceId dev_id = DeviceId{}):m_dev_id{dev_id}{}

		void set_viewport_size(int width, int height)
		{
			m_framebuffer = Image<Pixel>{width, height};
			m_dispatcher.send(m_dev_id, GeometryUpdateEvent{ViewportSize{width, height}, Origin<int>});
			update();
		}

		void update()
		{
			if(m_framebuffer.data() != nullptr)
			{
				memset(m_framebuffer.data(), 0, size(m_framebuffer));
				m_dispatcher.send(m_dev_id, UpdateEventSw{m_framebuffer});
				m_display(fb);
			}
		}

		void set_display(UiUpdater&& display)
		{
			m_display = std::move(display);
			update();
		}

		UiUpdater const& display() const
		{
			return m_display;
		}

		template<class T>
		void bind(std::reference_wrapper<T> obj)
		{
			m_dispatacher.bind(m_dev_id, EventDispatcher<UpdateEventSw>{obj});
			m_dispatacher.bind(m_dev_id, EventDispatcher<GeometryUpdateEvent>{obj});
		}

	private:
		UiUpdater m_display;
		Image<Pixel> m_framebuffer;
		DeviceId m_dev_id;
		EventDispatcher<UpdateEventSw, GeometryUpdateEvent> m_dispatacher;
	};
}

#endif