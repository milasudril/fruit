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

namespace fruit
{
	class MessageHandler
	{
	public:
	private:
		void* m_handle;
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

		void bind(MessageHandler& widget, DeviceId device);
		void unbind(MessageHandler& widget, DeviceId device);
		void unbind(MessageHandler& widget);

	private:
		void render(DeviceId sender, void*, DisplayCallbackPtr) const;

		std::map<DeviceId, std::vector<MessageHandler>> m_sensitive_widgets;
	};
}

#endif