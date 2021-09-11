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
		void handle(fruit::DeviceId sender, int x)
		{
			sender_int = sender;
			value_int = x;
		}

		void handle(fruit::DeviceId sender, EventA x)
		{
			sender_a = sender;
			event_a = x;
		}

		int handle(fruit::DeviceId sender, EventB x)
		{
			sender_b = sender;
			event_b = x;
			return 134;
		}

		fruit::DeviceId sender_int{-1};
		int value_int;
		fruit::DeviceId sender_a{-1};
		EventA event_a;
		fruit::DeviceId sender_b{-1};
		EventB event_b;
	};
}

TESTCASE(EventHandlerCreateAndCall)
{
	TestReceiver obj;
	fruit::EventHandler<int, EventA, EventB> eh{std::ref(obj)};

	eh.handle(fruit::DeviceId{2}, 1);
	eh.handle(fruit::DeviceId{4}, EventA{2});
	auto val = eh.handle(fruit::DeviceId{6}, EventB{3});
	EXPECT_EQ(val, 134);
	EXPECT_EQ(obj.sender_int, (fruit::DeviceId{2}));
	EXPECT_EQ(obj.value_int, 1);
	EXPECT_EQ(obj.sender_a, (fruit::DeviceId{4}));
	EXPECT_EQ(obj.event_a.data, 2);
	EXPECT_EQ(obj.sender_b, (fruit::DeviceId{6}));
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