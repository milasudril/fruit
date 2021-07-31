#ifndef FROZE_EVENT_DISPATCHER_HPP
#define FROZE_EVENT_DISPATCHER_HPP

namespace fruit
{
	class EventDispatcher
	{
	public:
		template<class DisplayCallback>
		void render(DisplayCallback&& cb) const
		{
			render(&cb, [](void* self, GuiPixel const* srcbuff, int width, int height) {
				(*static_cast<DisplayCallback*>(self))(srcbuff, width, height);
			});
		}

		void set_canvas_size(int width, int height);

		void send_typing_event(DeviceId sender, Scancode scancode, ButtonState state, CharCodepoint mapped_codepoint);
		void send_location_event(DeviceId sender, Point<float> loc, int button, ButtonState state);
		void send_displace_event(DeviceId sender, Vector<float> offset);
		void send_midi_event(DeviceId sender, MidiEvent event);
		void send_frame_start_event(DeviceId sender, uint64_t framecounter, Worldclock t);

		void bind(Widget& widget, DeviceId device);
		void unbind(Widget& widget, DeviceId device);
		void unbind(Widget& widget);

	private:
		void render(void*, void (*cb)(GuiPixel const*, int, int)) const;

		std::map<DeviceId, std::vector<std::reference_wrapper<Widget>>> m_sensitive_widgets;
	};
}

#endif