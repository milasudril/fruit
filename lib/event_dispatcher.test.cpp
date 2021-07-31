//@	{"target":{"name":"event_dispatcher.test"}}

#include "./event_dispatcher.hpp"

#include "testfwk/testfwk.hpp"

namespace
{
	struct TestEventHandlerA
	{
		int recv_val{0};

		void handle(int sent_val)
		{
			recv_val = sent_val;
		}
	};

}

TESTCASE(EventDispatcherSingle)
{
	fruit::EventDispatcher<int> event_dispatcher;
	TestEventHandlerA eh_1;
	TestEventHandlerA eh_2;
	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh_1)}, fruit::DeviceId{0});
	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh_2)}, fruit::DeviceId{1});
	event_dispatcher.bind(fruit::EventHandler<int>{std::ref(eh_1)}, fruit::DeviceId{2});

	EXPECT_NE(eh_1.recv_val, 1);
	EXPECT_NE(eh_2.recv_val, 2);

	event_dispatcher.send(fruit::DeviceId{0}, 1);
	event_dispatcher.send(fruit::DeviceId{1}, 2);

	EXPECT_EQ(eh_1.recv_val, 1);
	EXPECT_EQ(eh_2.recv_val, 2);

	event_dispatcher.send(fruit::DeviceId{2}, 3);
	EXPECT_EQ(eh_1.recv_val, 3);

	event_dispatcher.unbind(fruit::EventHandler<int>(std::ref(eh_2)), fruit::DeviceId{1});
	event_dispatcher.send(fruit::DeviceId{1}, 2);
	EXPECT_EQ(eh_2.recv_val, 2);
}