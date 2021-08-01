//@	{
//@		"target":
//@		{
//@			"name":"skeleton.o",
//@			"dependencies": [
//@				{"ref":"glfw3", "origin":"pkg-config"},
//@				{"ref":"glew", "origin":"pkg-config"},
//@				{"ref":"GL", "origin":"system", "rel":"external"}
//@			]
//@		}
//@	}

#include "lib/update.hpp"
#include "lib/event_dispatcher.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

struct GlfwWindowDeleter
{
	void operator()(GLFWwindow* handle)
	{
		glfwDestroyWindow(handle);
	}
};

using WindowHandle = std::unique_ptr<GLFWwindow, GlfwWindowDeleter>;

WindowHandle createWindow()
{
	static bool initialized = false;
	if(!initialized)
	{
		if(!glfwInit())
		{
			return nullptr;
		}
		initialized = true;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	return WindowHandle{glfwCreateWindow(800, 500, "Fruit skeleton", nullptr, nullptr)};
}

bool initOpenGL(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);

	if(glewInit() != GLEW_OK)
	{
		printf("GLEW init failed\n");
		return false;
	}

	auto gl_version = glGetString(GL_VERSION);
	if(gl_version == nullptr)
	{
		printf("Failed to initialize OpenGL\n");
		return false;
	}
	printf("OpenGL version: %s\n", reinterpret_cast<char const*>(gl_version));
	return true;
}

template<fruit::DisplayFunction UiUpdater>
class Ui
{
public:
	void set_canvas_size(int width, int height)
	{
		m_framebuffer = std::make_unique<fruit::Pixel[]>(width * height);
		m_width = width;
		m_height = height;
		update();
	}

	void update()
	{
		m_event_dispatcher.send(fruit::DeviceId{-1}, fruit::UpdateEventSw{m_framebuffer.get(), m_width, m_height});
		m_display(m_framebuffer.get(), m_width, m_height);
	}

private:
	fruit::EventDispatcher<fruit::UpdateEventSw> m_event_dispatcher;
	UiUpdater m_display;
	std::unique_ptr<fruit::Pixel[]> m_framebuffer;
	int m_width;
	int m_height;
};

class GlTextureTransfer
{
public:
	void operator()(fruit::Pixel const*, int, int)
	{
	}

private:

};

int main()
{
	Ui<GlTextureTransfer> ui;

	auto window = createWindow();
	glfwSetWindowUserPointer(window.get(), &ui);

	if(!initOpenGL(window.get()))
	{ return 1; }


	glfwSetFramebufferSizeCallback(window.get(), [](GLFWwindow* src, int w, int h){
		glViewport(0, 0, w, h);
		auto& ui = *reinterpret_cast<Ui<GlTextureTransfer>*>(src);
		ui.set_canvas_size(w, h);
	});

	while(!glfwWindowShouldClose(window.get()))
	{
		glfwPollEvents();
		ui.update();
		glfwSwapBuffers(window.get());
	}

	window.reset();
	glfwTerminate();

	return 0;
}
