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



	template<class Event>
	class SingleEventDispatcher
	{
	public:
		void send(DeviceId sender, Event const& e)
		{
			auto& handlers = m_sensitive_widgets.find(sender);
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

			i->second.erase(widget);
		}

		void unbind(EventHandler<Event> widget)
		{
			std::ranges::for_each(m_sensitive_widgets, [widget](auto& item) {
				item.second.erase(widget);
			});
		}

	private:
		std::map<DeviceId, std::vector<EventHandler<Event>>> m_sensitive_widgets;
	};

	template<class Event, class ... Events>
	class EventDispatcher : SingleEventDispatcher<Event>, public EventDispatcher<Events...>
	{
	};

}

#endif