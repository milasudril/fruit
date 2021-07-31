# Fruit

-- A FRozenUI Toolkit

## Feature list

* Client-side implementation main loop implementation
* Support for application defined event types
* Support for application defined event sources
* Type-safe event routing

## Why Fruit

Fruit is meant to be a general purpose UI toolkit. General here means that the toolkit is not only
adapted for common tasks like GTK and QT, but it should also be usable without any display server.
Also, it should be possible to plug Fruit into any already existing event loop, and compose the
output of Fruit with something else.

To achieve these goals, Fruit will always render widgets to an internal memory surface. Also, Fruit
has no built-in event loop. This is why it is *frozen*. To activate any event, the application is
supposed to manually dispatch events.

## Event routing

Events are routed from an application defined device id, that could correspond to a keyboard, a
mouse, or any other input device, physical or virtual. Any device can act as a source for any kind
of event. Thus, it is possible to emulate any device, with any other device.  The receivers of
events are `EventHandler`. For an event handler to work as an event handler, all it needs is an
overload for `handle`, that accepts the event type of interest.

The library defines a few commonly used event types. In addition the predefined event types, the
application may define any custom event type. As with event types, an application can define its own
event handlers.