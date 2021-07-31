# Fruit

-- A FRozenUI Toolkit

## Why Fruit

Fruit is meant to be a general purpose UI toolkit. General here means that the toolkit is not only
adapted for common tasks like GTK and QT, but it should also be usable without any display server.
Also, it should be possible to plug Fruit into any already existing event loop, and compose the
output of Fruit with something else.

To achieve these goals, Fruit will always render widgets to an internal memory surface. Also, Fruit
has no built-in event loop. This is why it is *frozen*. To activate any event, the application is
supposed to manually dispatch events.