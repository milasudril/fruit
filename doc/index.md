# Introduction

Fruit is a dispatch and render-only UI toolkit. This means that fruit does not contain any support
code to connect to windowing libraries, or interacting with display servers. Instead, the application
will read events form any source, and pass it on to fruit. Fruit will notify the appllication when
the user interacts with a UI element, or when the internal framebuffer has been updated. The flow
is illustrated in the picture below.

<img src="dataflow.svg" alt="Fruit data flow">

An example of an input library is GLFW, but you can also use ports from JACK as event source, or a
combination of multiple libraries. For drawing, you can use any library that accepts raster graphics
such as Cairo, OpenGL, or Vulkan. Since fruit does not have its own event loop, it is also possible
to embed a fruit-based component into a GTK or Qt-based application.
