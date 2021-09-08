//@	{
//@		"target":
//@		{
//@			"name":"skeleton.o",
//@			"dependencies": [
//				{"ref":"glfw3", "origin":"pkg-config"},
//@				{"ref":"glew", "origin":"pkg-config"},
//@				{"ref":"GL", "origin":"system", "rel":"external"}
//@			]
//@		}
//@	}

#include "lib/glfw_event_mapper.hpp"
#include "lib/update.hpp"
#include "lib/event_dispatcher.hpp"
#include "lib/geometry_update_event.hpp"
#include "lib/content_box.hpp"
#include "lib/font_store.hpp"
#include "lib/line_layout.hpp"
#include "lib/location_event.hpp"

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
class Ui : public fruit::EventDispatcher<fruit::UpdateEventSw,
	fruit::GeometryUpdateEvent,
	fruit::LocationEvent>
{
public:
	void set_viewport_size(int width, int height)
	{
		printf("set viewport size: %d %d\n", width, height);
		m_framebuffer = std::make_unique<fruit::Pixel[]>(width * height);
		m_width = width;
		m_height = height;
		send(fruit::DeviceId{-1}, fruit::GeometryUpdateEvent{fruit::ViewportSize{width, height}, fruit::Origin<int>});
		update();
	}

	void update()
	{
		if(m_framebuffer.get() != nullptr)
		{
			auto fb = fruit::ImageView{m_framebuffer.get(), m_width, m_height};
			memset(m_framebuffer.get(), 0, size(fb));
			send(fruit::DeviceId{-1}, fruit::UpdateEventSw{fb});
			m_display(fb);
		}
	}

	void dispatch(fruit::LocationEvent const& loc)
	{
		send(fruit::DeviceId{-1}, loc);
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

	void operator()(fruit::ImageView<fruit::Pixel const> buffer)
	{
		auto width = buffer.width();
		auto height = buffer.height();
		if(m_handle == 0 || width != m_width || height != m_height)
		{
			allocate(width, height);
			glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		glTextureSubImage2D(m_handle, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, buffer.data());
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
#if 0
		int format{};
		int type{};
		glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA32F, GL_TEXTURE_IMAGE_FORMAT, 1, &format);
		glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA32F, GL_TEXTURE_IMAGE_TYPE, 1, &type);
		printf("%d %d\n", format, type);
#endif

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

struct MyEventHandler
{
	void handle(fruit::LocationEvent const& e, std::integral_constant<int, 0>)
	{
		if(e.btn_state_chg_mask)
		{ printf("Button 1  %.8e %.8e %016lx\n", e.loc.x(), e.loc.y(), *e.btn_state_chg_mask); }
	}

	void handle(fruit::LocationEvent const& e, std::integral_constant<int, 1>)
	{
		if(e.btn_state_chg_mask)
		{ printf("Button 2  %.8e %.8e %016lx\n", e.loc.x(), e.loc.y(), *e.btn_state_chg_mask); }
	}
};

int main()
{
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

	fruit::FontMapper font_mapper;
	fruit::FontStore fonts;
	MyEventHandler eh;

	fruit::ContentBox button_1;
	button_1.border_width_top(16)
		.border_width_right(8)
		.border_width_bottom(4)
		.border_width_left(2)
		.padding_left(4)
		.padding_top(4)
		.padding_right(4)
		.padding_bottom(4)
		.border_color(fruit::Pixel{0.0f, 0.2f, 0.0f, 0.8f})
		.content(fruit::TextLine{*fonts.load_and_replace("Andika", font_mapper).font}
			.text(u8"Button 1")
			.char_height(32))
			.event_handler(std::ref(eh), std::integral_constant<int, 0>{});

	fruit::ContentBox button_2;
	button_2.border_width_top(4)
		.border_width_right(4)
		.border_width_bottom(4)
		.border_width_left(4)
		.padding_left(4)
		.padding_top(4)
		.padding_right(4)
		.padding_bottom(4)
		.border_color(fruit::Pixel{0.2f, 0.0f, 0.4f, 0.8f})
		.content(fruit::TextLine{*fonts.load_and_replace("Andika", font_mapper).font}
			.text(u8"Button 2")
			.char_height(32))
			.event_handler(std::ref(eh), std::integral_constant<int, 1>{});

	fruit::LineLayout line;
	line.push_back(fruit::LayoutBox{std::ref(button_1), 0, 0});
	line.push_back(fruit::LayoutBox{std::ref(button_2), 0, 0});
	ui.bind(fruit::EventHandler<fruit::GeometryUpdateEvent>{std::ref(line)}, fruit::DeviceId{-1});
	ui.bind(fruit::EventHandler<fruit::UpdateEventSw>{std::ref(button_1)}, fruit::DeviceId{-1});
	ui.bind(fruit::EventHandler<fruit::UpdateEventSw>{std::ref(button_2)}, fruit::DeviceId{-1});
	ui.bind(fruit::EventHandler<fruit::LocationEvent>{std::ref(button_1)}, fruit::DeviceId{-1});
	ui.bind(fruit::EventHandler<fruit::LocationEvent>{std::ref(button_2)}, fruit::DeviceId{-1});
	ui.set_viewport_size(800, 500);

	glfwSetFramebufferSizeCallback(window.get(), [](GLFWwindow* src, int w, int h){
		glViewport(0, 0, w, h);
		auto& ui = *reinterpret_cast<Ui<Texture>*>(glfwGetWindowUserPointer(src));
		ui.set_viewport_size(w, h);
	});

	glfwSetCursorPosCallback(window.get(), [](GLFWwindow* src, double x, double y){
		auto& ui = *reinterpret_cast<Ui<Texture>*>(glfwGetWindowUserPointer(src));
		ui.dispatch(fruit::make_location_event(*src, x, y));
	});

	glfwSetMouseButtonCallback(window.get(), [](GLFWwindow* src, int button, int action, int) {
		auto& ui = *reinterpret_cast<Ui<Texture>*>(glfwGetWindowUserPointer(src));
		ui.dispatch(fruit::make_location_event(*src, button, action));
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
