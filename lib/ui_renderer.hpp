#ifndef FRUIT_LIB_UIRENDERER_HPP
#define FRUIT_LIB_UIRENDERER_HPP

#include "./update_event.hpp"
#include "./event_dispatcher.hpp"
#include "./geometry_update_event.hpp"
#include "./image.hpp"

namespace fruit
{
	namespace ui_renderer_detail
	{
		class EmptyEh
		{
		public:
			void handle(DeviceId, FbUpdateEvent const&) const {}
		};

		inline EmptyEh empty_eh;
	}

	class UiRenderer
	{
	public:
		explicit UiRenderer(DeviceId dev_id = DeviceId{-1}):m_dev_id{dev_id}, m_event_handler{std::ref(ui_renderer_detail::empty_eh)}
		{
		}

		void set_viewport_size(int width, int height)
		{
			m_framebuffer = image<Pixel>{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
			m_dispatcher.send(m_dev_id, GeometryUpdateEvent{ViewportSize{width, height}, Origin<int>});
			update();
		}

		void update()
		{
			if(m_framebuffer.data() != nullptr)
			{
				memset(m_framebuffer.data(), 0, area(m_framebuffer));
				m_dispatcher.send(m_dev_id, RedrawEvent{m_framebuffer.pixels()});
				m_event_handler.handle(m_dev_id, FbUpdateEvent{std::as_const(m_framebuffer).pixels()});
			}
		}

		template<class Callback, class Tag>
		UiRenderer& event_handler(std::reference_wrapper<Callback> cb, Tag)
		{
			m_event_handler = EventHandler<FbUpdateEvent>{cb, Tag{}};
			return *this;
		}

		template<class T>
		void bind(std::reference_wrapper<T> obj)
		{
			m_dispatcher.bind(EventHandler<RedrawEvent>{obj}, m_dev_id);
			m_dispatcher.bind(EventHandler<GeometryUpdateEvent>{obj}, m_dev_id);
		}

		void unbind(void const* obj)
		{
			m_dispatcher.unbind(obj, m_dev_id);
		}

	private:
		image<Pixel> m_framebuffer;
		DeviceId m_dev_id;
		EventDispatcher<RedrawEvent, GeometryUpdateEvent> m_dispatcher;
		EventHandler<FbUpdateEvent> m_event_handler;
	};
}

#endif