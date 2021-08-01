//@	{"target":{"name":"update.test"}}

#include "./update.hpp"

#include "testfwk/testfwk.hpp"

#include <memory>

namespace
{
	fruit::DeviceId id_sent{0};
	fruit::UpdateEventSw event_sent{};

	struct UiUpdater
	{
		void send(fruit::DeviceId id, fruit::UpdateEventSw const& event) const
		{
			id_sent = id;
			event_sent = event;
		}
	};
}

TESTCASE(updateUi)
{
	UiUpdater updater;
	auto buffer = std::make_unique<fruit::Pixel[]>(6);
	fruit::UpdateEventSw event{};
	event.buffer = buffer.get();
	event.width = 3;
	event.height = 2;
	update(updater, fruit::DeviceId{1}, event, [&event](fruit::Pixel const* buffer, int w, int h){
		EXPECT_EQ(buffer, event.buffer);
		EXPECT_EQ(w, event.width);
		EXPECT_EQ(h, event.height);
	});

	EXPECT_EQ(id_sent, fruit::DeviceId{1});
	EXPECT_EQ(event_sent, event);
}