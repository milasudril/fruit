#ifndef FRUIT_EVENT_DISPATCHER_HPP
#define FRUIT_EVENT_DISPATCHER_HPP

#include "./display_callback.hpp"
#include "./device_id.hpp"
#include "./typing_event.hpp"
#include "./location_event.hpp"
#include "./ball_event.hpp"
#include "./midi_event.hpp"
#include "./world_clock.hpp"

namespace fruit
{
	class EventDispatcher
	{
	public:
		template<DisplayCallback T>
		void render(T&& cb) const
		{
			render(&cb, [](void* self, Pixel const* srcbuff, int width, int height) {
				(*static_cast<DisplayCallback*>(self))(srcbuff, width, height);
			});
		}

		void set_canvas_size(int width, int height);

		void send(DeviceId sender, TypingEvent const& event);
		void send(DeviceId sender, LocationEvent const& event);
		void send(DeviceId sender, BallEvent const& event);
		void send(DeviceId sender, MidiEvent const& event);
		void send_frame_start_event(DeviceId sender, uint64_t framecounter, WorldClock t);

		void bind(Widget& widget, DeviceId device);
		void unbind(Widget& widget, DeviceId device);
		void unbind(Widget& widget);

	private:
		void render(void*, DisplayCallbackPtr) const;

		std::map<DeviceId, std::vector<std::reference_wrapper<Widget>>> m_sensitive_widgets;
	};
}

#endif