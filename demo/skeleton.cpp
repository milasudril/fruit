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
#include "lib/line_layout.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <cstring>

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
class Ui : public fruit::EventDispatcher<fruit::UpdateEventSw, fruit::GeometryUpdateEvent>
{
public:
	void set_viewport_size(int width, int height)
	{
		m_framebuffer = std::make_unique<fruit::Pixel[]>(width * height);
		m_width = width;
		m_height = height;
		send(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{width, height}, fruit::Origin<int>});
		update();
	}

	void update()
	{
		memset(m_framebuffer.get(), 0, m_width*m_height);
		send(fruit::DeviceId{-1}, fruit::UpdateEventSw{m_framebuffer.get(), m_width, m_height});
		m_display(m_framebuffer.get(), m_width, m_height);
	}

	void set_display(UiUpdater&& display)
	{
		m_display = std::move(display);
		update();
	}

	UiUpdater const& display() const
	{
		return m_display;
	}

private:
	UiUpdater m_display;
	std::unique_ptr<fruit::Pixel[]> m_framebuffer;
	int m_width;
	int m_height;
};

char output[16*2048*1024];

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
		{
			allocate(width, height);
			glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glTextureSubImage2D(m_handle, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, buffer);
	}

	void allocate(int width, int height)
	{
		GLuint handle{0};
		glCreateTextures(GL_TEXTURE_2D, 1, &handle);
		if(handle == 0)
		{ throw std::runtime_error{"Failed to create a texture"}; }

		glTextureStorage2D(handle, 1, GL_RGBA32F, width, height);
		if(glGetError() != GL_NO_ERROR)
		{ throw std::runtime_error{"Failed to allocate storage for current texture"}; }

		int format{};
		int type{};
		glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA32F, GL_TEXTURE_IMAGE_FORMAT, 1, &format);
		glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA32F, GL_TEXTURE_IMAGE_TYPE, 1, &type);

		printf("%d %d\n", format, type);

		glDeleteTextures(1, &m_handle);
		m_handle = handle;
		m_width = width;
		m_height = height;
	}

	void bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_handle);
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
layout (location = 1) in vec2 input_uv;
out vec4 vertex_loc;
out vec2 uv;
void main()
{
	gl_Position = input_loc;
	vertex_loc = input_loc;
	uv = input_uv;
}
)shader";
	auto const vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
	glCompileShader(vertex_shader);

	char const* fragment_shader_src = R"shader(#version 460

out vec4 FragColor;
in vec2 uv;

uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, uv);
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

constexpr std::array<fruit::Point<float>, 6> texture_rect{
	fruit::Point{-1.0f, -1.0f, 0.0f},
	fruit::Point{1.0f, -1.0f, 0.0f},
	fruit::Point{1.0f, 1.0f, 0.0f},

	fruit::Point{1.0f, 1.0f, 0.0f},
	fruit::Point{-1.0f, 1.0f, 0.0f},
	fruit::Point{-1.0f, -1.0f, 0.0f}
};

constexpr std::array<std::pair<float, float>, 6> texture_uvs{
	std::pair<float, float>{0.0f, 1.0f},
	std::pair<float, float>{1.0f, 1.0f},
	std::pair<float, float>{1.0f, 0.0f},

	std::pair<float, float>{1.0f, 0.0f},
	std::pair<float, float>{0.0f, 0.0f},
	std::pair<float, float>{0.0f, 1.0f}
};

int main()
{
	fruit::Rectangle rect_a;
	rect_a.width=300;
	rect_a.height=200;
	rect_a.color = fruit::red();

	fruit::Rectangle rect_b;
	rect_b.width=300;
	rect_b.height=200;
	rect_b.color = fruit::green();
#if 1
	fruit::Rectangle rect_c;
	rect_c.width=300;
	rect_c.height=200;
	rect_c.color = fruit::blue();

	fruit::Rectangle rect_d;
	rect_d.width=300;
	rect_d.height=300;
	rect_d.color = fruit::yellow();
#endif

	fruit::LineLayout row_a;
	row_a.push_back(fruit::LayoutBox{std::ref(rect_a), 0.6f});
	row_a.push_back(fruit::LayoutBox{std::ref(rect_b), 0.4f});
	row_a.set_width(1.0f);

	fruit::LineLayout row_b;
	row_b.push_back(fruit::LayoutBox{std::ref(rect_c)});
	row_b.push_back(fruit::LayoutBox{std::ref(rect_d)});

	fruit::LineLayout column{fruit::LineLayout::Direction::TopToBottom};
	column.push_back(fruit::LayoutBox{std::ref(row_a)});
	column.push_back(fruit::LayoutBox{std::ref(row_b)});
	column.set_width(0.75f);

	fruit::LineLayout column_outer{fruit::LineLayout::Direction::TopToBottom};
	column_outer.push_back(fruit::LayoutBox{std::ref(column)});
	column_outer.set_width(0.75f);

	fruit::LineLayout column_outer_outer{fruit::LineLayout::Direction::TopToBottom};
	column_outer_outer.push_back(fruit::LayoutBox{std::ref(column_outer)});
	column_outer_outer.set_width(1.0f);

	Ui<Texture> ui;

	auto window = createWindow();
	glfwSetWindowUserPointer(window.get(), &ui);

	if(!initOpenGL(window.get()))
	{ return 1; }

	auto shader_prog = create_shader_program();
	glUseProgram(shader_prog);

	GLuint vbo{};
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo,
						 std::size(texture_rect)*sizeof(fruit::Point<float>),
						 std::data(texture_rect),
						 GL_MAP_READ_BIT);

	GLuint uvs{};
	glCreateBuffers(1, &uvs);
	glNamedBufferStorage(uvs,
						 std::size(texture_uvs)*sizeof(std::pair<float,float>),
						 std::data(texture_uvs),
						 GL_MAP_READ_BIT);

	GLuint va{};
	glCreateVertexArrays(1, &va);

	ui.bind(fruit::EventHandler<fruit::UpdateEventSw>{std::ref(rect_a)}, fruit::DeviceId{-1});
	ui.bind(fruit::EventHandler<fruit::UpdateEventSw>{std::ref(rect_b)}, fruit::DeviceId{-1});
	ui.bind(fruit::EventHandler<fruit::UpdateEventSw>{std::ref(rect_c)}, fruit::DeviceId{-1});
	ui.bind(fruit::EventHandler<fruit::UpdateEventSw>{std::ref(rect_d)}, fruit::DeviceId{-1});
	ui.bind(fruit::EventHandler<fruit::GeometryUpdateEvent>{std::ref(column_outer)}, fruit::DeviceId{-1});
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

		glBindVertexArray(va);
		ui.display().bind();
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fruit::Point<float>), nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, uvs);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(std::pair<float, float>), nullptr);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window.get());
	}

	window.reset();
	glfwTerminate();

	return 0;
}
