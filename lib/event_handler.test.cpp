//@	{"target":{"name": "event_handler.test"}}

#include "./event_handler.hpp"

#include "testfwk/testfwk.hpp"

namespace
{
	struct EventA
	{
		int data;
	};

	struct EventB
	{
		using result_type = int;
		int data;
	};

	struct TestReceiver
	{
		void handle(int x)
		{
			value_int = x;
		}

		void handle(EventA x)
		{
			event_a = x;
		}

		int handle(EventB x)
		{
			event_b = x;
			return 134;
		}

		int value_int;
		EventA event_a;
		EventB event_b;
	};
}

TESTCASE(EventHandlerCreateAndCall)
{
	TestReceiver obj;
	fruit::EventHandler<int, EventA, EventB> eh{std::ref(obj)};

	eh.handle(1);
	eh.handle(EventA{2});
	auto val = eh.handle(EventB{3});
	EXPECT_EQ(val, 134);
	EXPECT_EQ(obj.value_int, 1);
	EXPECT_EQ(obj.event_a.data, 2);
	EXPECT_EQ(obj.event_b.data, 3);
}


TESTCASE(EventHandlerEquality)
{
	TestReceiver obj_1;
	TestReceiver obj_2;

	fruit::EventHandler<int, EventA, EventB> eh_a{std::ref(obj_1)};
	fruit::EventHandler<int, EventA, EventB> eh_b{std::ref(obj_2)};
	fruit::EventHandler<int, EventA, EventB> eh_c{std::ref(obj_1)};

	EXPECT_EQ(eh_a, eh_c);
	EXPECT_EQ(eh_c, eh_a);
	EXPECT_NE(eh_a, eh_b);
	EXPECT_NE(eh_b, eh_c);
}