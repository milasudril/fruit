#ifndef FRUIT_EVENT_SINGLEEVENTDISPATCHER_HPP
#define FRUIT_EVENT_SINGLEEVENTDISPATCHER_HPP

#include "./device_id.hpp"

#include <map>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace fruit
{
	namespace event_handler_detail
	{
		template <typename T, typename = void>
		struct ResultType {
			using type = void;
		};
		template <typename T>
		struct ResultType<T,std::void_t<typename T::result_type>> {
			using type = typename T::result_type;
		};
	}

	template<class Event>
	class EventHandler
	{
	public:
		using result_type = typename event_handler_detail::ResultType<Event>::type;

		template<class Widget>
		explicit EventHandler(std::reference_wrapper<Widget> widget):m_obj{&widget.get()},
		m_func{[](void* self, Event const& event){
			return static_cast<Widget*>(self)->handle(event);
		}}
		{}

		decltype(auto) handle(Event const& event) const
		{
			return m_func(m_obj, event);
		}

		void const* object() const
		{
			return m_obj;
		}

	private:
		void* m_obj;
		result_type (*m_func)(void* self, Event const& event);
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

		template<class T>
		void erase(std::vector<EventHandler<T>>& vals, void const* ptr)
		{
			auto i = std::ranges::find_if(vals, [ptr](auto item){
				return item.object() == ptr;
			});
			if(i == std::end(vals))
			{ return; }

			vals.erase(i);
		}
	}

	template<class Event, class ... Events>
	class EventDispatcher : EventDispatcher<Event>, EventDispatcher<Events...>
	{
	public:
		using EventDispatcher<Event>::bind;
		using EventDispatcher<Events...>::bind;
		using EventDispatcher<Event>::send;
		using EventDispatcher<Events...>::send;
		using EventDispatcher<Event>::unbind;
		using EventDispatcher<Events...>::unbind;

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
		void send(DeviceId sender, Event const& e) const
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

		void unbind(void const* widget)
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