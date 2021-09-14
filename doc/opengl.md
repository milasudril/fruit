# Interacting with OpenGL

Fruit renders all components on an Image with a format compatible with GL_RGBA GL_FLOAT. Thus, you
can use a texture with this format to present output from fruit. It is likely that you want to call
UiManager::set_viewport_size from the same place as you would call glViewport. That is, when the
size of the target framebuffer should change. When the content of the texture needs to change, fruit
will send a FbUpdateEvent to the EventHandler associated with the UiManager. Depending on the vireport
size, you may wish to avoid triggering texture updates since it can be quite CPU heavy to transfer 
new content to the texture.

