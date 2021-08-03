#ifndef FRUIT_EVENTHANDLER_HPP
#define FRUIT_EVENTHANDLER_HPP

#include <type_traits>
#include <string>

namespace fruit
{
	namespace event_handler_detail
	{
		template<class T, class = void>
		struct ResultType
		{
			using type = void;
		};

		template<class T>
		struct ResultType<T, std::void_t<typename T::result_type>>
		{
			using type = typename T::result_type;
		};
	}

	template<class Event, class ... Events>
	class EventHandler : EventHandler<Events...>
	{
		using Base = EventHandler<Events...>;
	public:
		using Base::object;
		using Base::handle;

		template<class Widget>
		explicit EventHandler(std::reference_wrapper<Widget> widget):Base{widget},
		m_func{[](void* self, Event const& event){
			return static_cast<Widget*>(self)->handle(event);
		}}
		{}

		decltype(auto) handle(Event const& event) const
		{
			return m_func(object(), event);
		}

		bool operator==(EventHandler const& other) const
		{
			return static_cast<Base const&>(*this) == static_cast<Base const&>(other);
		}

		bool operator!=(EventHandler const& other) const
		{
			return !(*this == other);
		}

	private:
		using result_type = typename event_handler_detail::ResultType<Event>::type;

		result_type (*m_func)(void* self, Event const& event);
	};

	template<class Event, class ... Events>
	std::string to_string(EventHandler<Event, Events...> const& obj)
	{
		return std::to_string(reinterpret_cast<intptr_t>(obj.object()));
	}

	template<class Event>
	class EventHandler<Event>
	{
	public:
		template<class Widget>
		explicit EventHandler(std::reference_wrapper<Widget> widget):m_obj{&widget.get()},
		m_func{[](void* self, Event const& event){
			return static_cast<Widget*>(self)->handle(event);
		}}
		{}

		using result_type = typename event_handler_detail::ResultType<Event>::type;

		decltype(auto) handle(Event const& event) const
		{
			return m_func(m_obj, event);
		}

		void* object() const
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
}

#endif