# Introduction

Fruit is a dispatch and render-only UI toolkit. This means that fruit does not contain any support
code to connect to windowing libraries, or interacting with display servers. Instead, the application
will read events form any source, and pass it on to fruit. Fruit will notify the appllication when
the user interacts with a UI element, or when the internal framebuffer has been updated. The flow
is illustrated in the picture below.

<img src="dataflow.svg" alt="Fruit data flow">

