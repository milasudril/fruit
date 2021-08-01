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
#include "lib/rectangle.hpp"

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
class Ui : public fruit::EventDispatcher<fruit::UpdateEventSw>
{
public:
	void set_viewport_size(int width, int height)
	{
		m_framebuffer = std::make_unique<fruit::Pixel[]>(width * height);
		m_width = width;
		m_height = height;
		update();
	}

	void update()
	{
		send(fruit::DeviceId{-1}, fruit::UpdateEventSw{m_framebuffer.get(), m_width, m_height});
		m_display(m_framebuffer.get(), m_width, m_height);
	}

	void set_display(UiUpdater&& display)
	{
		m_display = std::move(display);
		update();
	}

private:
	UiUpdater m_display;
	std::unique_ptr<fruit::Pixel[]> m_framebuffer;
	int m_width;
	int m_height;
};

class Texture
{
public:
	Texture():m_handle{0}, m_width{0}, m_height{0} {}

	~Texture()
	{
		if(m_handle != 0)
		{
			glDeleteTextures(1, &m_handle);
		}
	}

	void operator()(fruit::Pixel const* buffer, int width, int height)
	{
		if(m_handle == 0 || width != m_width || height != m_height)
		{ allocate(width, height); }

		glTextureSubImage2D(m_handle, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, buffer);
	}

	void allocate(int width, int height)
	{
		GLuint handle{0};
		glCreateTextures(GL_TEXTURE_2D, 1, &handle);
		if(handle == 0)
		{ throw std::runtime_error{"Failed to create a texture"}; }

		glTextureStorage2D(handle, 1, GL_RGBA16F, width, height);
		if(glGetError() != GL_NO_ERROR)
		{ throw std::runtime_error{"Failed to allocate storage for current texture"}; }

		glDeleteTextures(1, &m_handle);
		m_handle = handle;
		m_width = width;
		m_height = height;
	}

private:
	GLuint m_handle;
	int m_width;
	int m_height;
};

GLuint create_shader_program()
{
	char const* vertex_shader_src = R"shader(#version 460

layout (location = 0) in vec4 input_loc;
out vec4 vertex_loc;
void main()
{
	gl_Position = input_loc;
	vertex_loc = input_loc;
}
)shader";
	auto const vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
	glCompileShader(vertex_shader);

	char const* fragment_shader_src = R"shader(#version 460

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.5, 0.5, 0.5, 1.0);
}
)shader";

	auto const fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
	glCompileShader(fragment_shader);

	auto const program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	return program;
}

int main()
{
	fruit::Rectangle rect;
	rect.width=300;
	rect.height=200;
	rect.color = fruit::red();

	Ui<Texture> ui;

	auto window = createWindow();
	glfwSetWindowUserPointer(window.get(), &ui);

	if(!initOpenGL(window.get()))
	{ return 1; }

	auto shader_prog = create_shader_program();
	glUseProgram(shader_prog);

	ui.bind(fruit::EventHandler<fruit::UpdateEventSw>{std::ref(rect)}, fruit::DeviceId{-1});
	ui.set_viewport_size(800, 500);

	glfwSetFramebufferSizeCallback(window.get(), [](GLFWwindow* src, int w, int h){
		glViewport(0, 0, w, h);

		auto& ui = *reinterpret_cast<Ui<Texture>*>(glfwGetWindowUserPointer(src));
		ui.set_viewport_size(w, h);
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
