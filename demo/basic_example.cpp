//@	{
//@		"target":
//@		{
//@			"name":"basic_example.o"
//@		}
//@	}

#include "lib/font_store.hpp"
#include "lib/ui_manager.hpp"
#include "lib/content_box.hpp"

enum class ControlId:int{display, button};

template<ControlId id>
using ControlIdTag = std::integral_constant<ControlId, id>;

struct ExampleEventHandler
{
	void handle(fruit::DeviceId, fruit::LocationEvent const&, ControlIdTag<ControlId::button>)
	{
		// button was clicked
	}

	void handle(fruit::DeviceId, fruit::FbUpdateEvent const&, ControlIdTag<ControlId::display>)
	{
		// UI was redrawn
	}

	void handle(fruit::DeviceId, fruit::BoundaryCrossedEvent const&, ControlIdTag<ControlId::button>)
	{
		// Cursor moved into button
	}
};

int main()
{
	// First, create a UiManager that will manage an internal framebuffer, and route different kinds
	// of events. We want to route LocationEvents. Thus, we add LocationEvent to the list of supported
	// events. FbUpdateEvents are supported implicitly, and should not be added manually.
	//
	using ExampleUi = fruit::UiManager<fruit::LocationEvent>;
	ExampleUi ui;

	// To act on events, we need to have one or more event handlers. In this example, we will use
	// the same event handler for both LocationEvents and click and FbUpdateEvents. Use an
	// instance of ExampleEventHandler to receive all events.
	//
	ExampleEventHandler event_handler;

	// Events that end up in the event handler may be associated with a compile-time "control id"
	// This way, it is possible to know which control that triggered the event. The control id is
	// bound by using a type. For this purpose we can use std::integral_constant with appropriate
	// template arguments.
	//
	ui.set_display_handler(std::ref(event_handler), ControlIdTag<ControlId::display>{});

	// Initiate font managment. For simplicity, we use the FontMapper and FontStore to handle fonts.
	// It is also possible to skip FontStore if you already have a class for font lookups. If you
	// want to load a TTF directly, you can skip FontMapper.
	//
	fruit::FontMapper font_mapper;
	fruit::FontStore fonts;

	// Create a "button" and set some properties
	//
	fruit::ContentBox hello_button;
	hello_button.border_width_top(4)
		.border_width_right(4)
		.border_width_bottom(4)
		.border_width_left(4)
		.padding_left(4)
		.padding_top(4)
		.padding_right(4)
		.padding_bottom(4)
		.border_color(fruit::Pixel{1.0f, 0.5f, 0.0f, 1.0f})
		.content(fruit::TextLine{*fonts.load_and_replace("Andika", font_mapper).font}
			.text(u8"Hello, World")
			.char_height(32))
		.event_handler(std::ref(event_handler), ControlIdTag<ControlId::button>{});

	// Binding `hello_button` to renderer makes fruit render the button
	//
	ui.bind_to_renderer(std::ref(hello_button));

	// We also want to catch LocationEvents
	//
	ui.bind(fruit::EventHandler<fruit::LocationEvent>{std::ref(hello_button)});

	// Make sure that ui has a valid framebuffer. You should set the initial size to match the
	// initial size of the target buffer.
	//
	ui.set_viewport_size(800, 500);

	// Configure your "input library" so events form it are being routed to `ui`

	// Run the event loop
	ui.update();

	return 0;
}
