#ifndef FRUIT_EVENTHANDLER_HPP
#define FRUIT_EVENTHANDLER_HPP

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
}

#endif