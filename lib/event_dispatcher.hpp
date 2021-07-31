#ifndef FRUIT_EVENT_SINGLEEVENTDISPATCHER_HPP
#define FRUIT_EVENT_SINGLEEVENTDISPATCHER_HPP

#include "./device_id.hpp"

#include <map>
#include <vector>
#include <algorithm>

namespace fruit
{
	template<class Event>
	class EventHandler
	{
	public:
		template<class Widget>
		explicit EventHandler(std::reference_wrapper<Widget> widget):m_obj{&widget.get()},
		m_func{[](void* self, Event const& event){
			static_cast<Widget*>(self)->handle(event);
		}}
		{}

		void handle(Event const& event)
		{
			m_func(m_obj, event);
		}

		void const* object() const
		{
			return m_obj;
		}

	private:
		void* m_obj;
		void (*m_func)(void* self, Event const& event);
	};

	template<class Event>
	bool operator==(EventHandler<Event> a, EventHandler<Event> b)
	{
		return a.object() == b.object();
	}

	template<class Event>
	bool operator!=(EventHandler<Event> a, EventHandler<Event> b)
	{
		return !(a == b);
	}

	namespace event_dispatcher_detail
	{
		struct CompareDeviceId
		{
			bool operator()(DeviceId a, DeviceId b) const
			{
				return a.value() < b.value();
			}
		};

		template<class T>
		void erase(std::vector<T>& vals, T const& value)
		{
			auto i = std::ranges::find(vals, value);
			if(i == std::end(vals))
			{ return; }

			vals.erase(i);
		}
	}

	template<class Event, class ... Events>
	class EventDispatcher : EventDispatcher<Event>, public EventDispatcher<Events...>
	{
	public:
		void unbind(void const* widget)
		{
			EventDispatcher<Event>::unbind(widget);
			EventDispatcher<Events...>::unbind(widget);
		}
	};

	template<class Event>
	class EventDispatcher<Event>
	{
	public:
		void send(DeviceId sender, Event const& e)
		{
			auto const& handlers = m_sensitive_widgets.find(sender);
			if(handlers == std::end(m_sensitive_widgets))
			{ return; }

			std::ranges::for_each(std::begin(handlers->second), std::end(handlers->second),[&e](auto& item) {
				item.handle(e);
			});
		}

		void bind(EventHandler<Event> widget, DeviceId device)
		{
			auto& v = m_sensitive_widgets[device];
			auto i = std::ranges::find(v, widget);
			if(i == std::end(v))
			{ v.push_back(widget); }
		}

		void unbind(EventHandler<Event> widget, DeviceId device)
		{
			auto i = m_sensitive_widgets.find(device);
			if(i == std::end(m_sensitive_widgets))
			{ return; }

			event_dispatcher_detail::erase(i->second, widget);
		}

		void unbind(EventHandler<Event> widget)
		{
			std::ranges::for_each(m_sensitive_widgets, [widget](auto& item) {
				event_dispatcher_detail::erase(item.second, widget);
			});
		}

	private:
		std::map<DeviceId, std::vector<EventHandler<Event>>, event_dispatcher_detail::CompareDeviceId> m_sensitive_widgets;
	};


}

#endif