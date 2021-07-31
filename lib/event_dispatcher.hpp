#ifndef FRUIT_EVENT_DISPATCHER_HPP
#define FRUIT_EVENT_DISPATCHER_HPP

#include "./display_callback.hpp"
#include "./device_id.hpp"
#include "./typing_event.hpp"
#include "./location_event.hpp"
#include "./ball_event.hpp"
#include "./midi_event.hpp"
#include "./world_clock.hpp"
#include "./frame_start_event.hpp"

#include <map>
#include <vector>
#include <functional>

namespace fruit
{
	namespace event_dispatcher_detail
	{
		template<class T>
		void handle(void* reciever, T const& event)
		{
			static_cast<T*>(reciever)->handle(event);
		}
	}

	class EventHandler
	{
		struct EventHandlerVtable
		{
			void (*frame_start_event)(void*, FrameStartEvent const&);
			void (*location_event)(void*, LocationEvent const&);
			void (*ball_event)(void*, BallEvent const&);
			void (*midi_event)(void*, MidiEvent const&);
			void (*typing_event)(void*, TypingEvent const&);
		};

	public:
		template<class Widget>
		explicit EventHandler(std::reference_wrapper<Widget> widget):m_handle{&widget.get()}
		{
			static EventHandlerVtable vt{
				event_dispatcher_detail::handle<Widget, FrameStartEvent>,
				event_dispatcher_detail::handle<Widget, LocationEvent>,
				event_dispatcher_detail::handle<Widget, BallEvent>,
				event_dispatcher_detail::handle<Widget, MidiEvent>,
				event_dispatcher_detail::handle<Widget, TypingEvent>
			};

			m_vt = &vt;
		}

		void handle(FrameStartEvent const& event)
		{
			m_vt.get().frame_start_event(m_handle, event);
		}

		void handle(LocationEvent const& event)
		{
			m_vt.get().location_event(m_handle, event);
		}

		void handle(BallEvent const& event)
		{
			m_vt.get().ball_event(m_handle, event);
		}

		void handle(MidiEvent const& event)
		{
			m_vt.get().midi_event(m_handle, event);
		}

		void handle(TypingEvent const& event)
		{
			m_vt.get().typing_event(m_handle, event);
		}

	private:
		void* m_handle;
		void (*m_render)(void* handle, Pixel* sinkbuff, int width, int height);
		std::reference_wrapper<EventHandlerVtable> m_vt;
	};

	class EventDispatcher
	{
	public:
		template<DisplayCallback T>
		void render(DeviceId sender, T&& cb) const
		{
			render(sender, &cb, [](void* self, Pixel const* srcbuff, int width, int height) {
				(*static_cast<T*>(self))(srcbuff, width, height);
			});
		}

		void set_canvas_size(int width, int height);

		void send(DeviceId sender, TypingEvent const& event);
		void send(DeviceId sender, LocationEvent const& event);
		void send(DeviceId sender, BallEvent const& event);
		void send(DeviceId sender, MidiEvent const& event);
		void send(DeviceId sender, FrameStartEvent const& event);

		void bind(EventHandler& widget, DeviceId device);
		void unbind(EventHandler& widget, DeviceId device);
		void unbind(EventHandler& widget);

	private:
		void render(DeviceId sender, void*, DisplayCallbackPtr) const;

		std::map<DeviceId, std::vector<EventHandler>> m_sensitive_widgets;
	};
}

#endif