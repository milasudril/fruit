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
	fruit::UpdateEventSw event{fruit::ImageView{buffer.get(), 3, 2}};
	update(updater, fruit::DeviceId{1}, event, [&event](fruit::ImageView<fruit::Pixel const> source){
		EXPECT_EQ(source.data(), event.buffer.data());
		EXPECT_EQ(source.width(), event.buffer.width());
		EXPECT_EQ(source.height(), event.buffer.height());
	});

	EXPECT_EQ(id_sent, fruit::DeviceId{1});
	EXPECT_EQ(event_sent.buffer.data(),   event.buffer.data());
	EXPECT_EQ(event_sent.buffer.width(),  event.buffer.width());
	EXPECT_EQ(event_sent.buffer.height(), event.buffer.height());
}