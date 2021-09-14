//@	{
//@		"target":
//@		{
//@			"name":"basic_example_layout_mgr.o"
//@		}
//@	}

#include "lib/font_store.hpp"
#include "lib/line_layout.hpp"
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
	using ExampleUi = fruit::UiManager<fruit::LocationEvent>;
	ExampleUi ui;
	ExampleEventHandler event_handler;

	ui.set_display_handler(std::ref(event_handler), ControlIdTag<ControlId::display>{});

	fruit::FontMapper font_mapper;
	fruit::FontStore fonts;
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
		
	fruit::LineLayout line_layout;
	line_layout.push_back(fruit::LayoutBox{std::ref(hello_button), 0, 0});
	ui.bind_to_renderer(std::ref(line_layout));

	ui.bind(fruit::EventHandler<fruit::LocationEvent>{std::ref(hello_button)});

	ui.set_viewport_size(800, 500);

	return 0;
}
