//@	{"target":{"name":"event_dispatcher.test"}}

#include "./event_dispatcher.hpp"

#include "testfwk/testfwk.hpp"

namespace
{
	struct TestEventHandler
	{
		int recv_val_a{0};
		long recv_val_b{0};

		void handle(int sent_val)
		{
			recv_val_a = sent_val;
		}

		void handle(long sent_val)
		{
			recv_val_b = sent_val;
		}
	};

	struct TestEventAccumulate
	{
		int recv_val{0};

		void handle(int sent_val)
		{
			recv_val += sent_val;
		}
	};

}

TESTCASE(EventDispatcherSingle)
{
	fruit::EventDispatcher<int> event_dispatcher;
	TestEventHandler eh_1;
	TestEventHandler eh_2;
	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh_1)}, fruit::DeviceId{0});
	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh_2)}, fruit::DeviceId{1});
	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh_1)}, fruit::DeviceId{2});

	REQUIRE_NE(eh_1.recv_val_a, 1);
	REQUIRE_NE(eh_2.recv_val_a, 2);

	event_dispatcher.send(fruit::DeviceId{0}, 1);
	event_dispatcher.send(fruit::DeviceId{1}, 2);

	EXPECT_EQ(eh_1.recv_val_a, 1);
	EXPECT_EQ(eh_2.recv_val_a, 2);

	event_dispatcher.send(fruit::DeviceId{2}, 3);
	EXPECT_EQ(eh_1.recv_val_a, 3);

	event_dispatcher.unbind(fruit::EventHandler<int>(std::ref(eh_2)), fruit::DeviceId{1});
	event_dispatcher.send(fruit::DeviceId{1}, 2);
	EXPECT_EQ(eh_2.recv_val_a, 2);

	event_dispatcher.unbind(fruit::EventHandler<int>(std::ref(eh_1)));
	event_dispatcher.send(fruit::DeviceId{1}, 4);
	EXPECT_EQ(eh_1.recv_val_a, 3);
	event_dispatcher.send(fruit::DeviceId{2}, 4);
	EXPECT_EQ(eh_1.recv_val_a, 3);
}

TESTCASE(EventDispatcherSingleAddSameTwice)
{
	fruit::EventDispatcher<int> event_dispatcher;
	TestEventAccumulate eh;
	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh)}, fruit::DeviceId{0});
	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh)}, fruit::DeviceId{0});

	REQUIRE_NE(eh.recv_val, 1);
	event_dispatcher.send(fruit::DeviceId{0}, 1);
	EXPECT_EQ(eh.recv_val, 1);
}

TESTCASE(EventDispatcherMulti)
{
	fruit::EventDispatcher<int, long> event_dispatcher;

	TestEventHandler eh;
	REQUIRE_NE(eh.recv_val_a, 1);
	REQUIRE_NE(eh.recv_val_b, 2);

	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh)}, fruit::DeviceId{0});
	event_dispatcher.bind(fruit::EventHandler<long>{std::ref(eh)}, fruit::DeviceId{0});
	event_dispatcher.send(fruit::DeviceId{0}, 1);
	event_dispatcher.send(fruit::DeviceId{0}, 2l);

	EXPECT_EQ(eh.recv_val_a, 1);
	EXPECT_EQ(eh.recv_val_b, 2l);

	event_dispatcher.unbind(fruit::EventHandler<int>{std::ref(eh)});
	event_dispatcher.send(fruit::DeviceId{0}, 3);
	EXPECT_EQ(eh.recv_val_a, 1);

	event_dispatcher.send(fruit::DeviceId{0}, 3l);
	EXPECT_EQ(eh.recv_val_b, 3l);

	event_dispatcher.unbind(&eh);
	event_dispatcher.send(fruit::DeviceId{0}, 4l);
	EXPECT_EQ(eh.recv_val_b, 3l);
}