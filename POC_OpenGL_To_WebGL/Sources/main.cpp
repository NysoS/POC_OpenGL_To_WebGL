#ifndef __main
#define __main

#include "Shader.h"	

#include <iostream>
#include <array>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include "glad.h"
#include <SDL_opengles2.h>
#else
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>

template <typename T>
struct Point2D
{
	Point2D(const T& x_ = 0, const T& y_ = 0)
		: x(x_), y(y_)
	{}

	Point2D(const Point2D& pt)
		: x(pt.x), y(pt.y)
	{}

	T x;
	T y;
};

template <typename T>
struct Color3
{
	Color3(const T& r_ = 0, const T& g_ = 0, const T& b_ = 0)
		: r(r_), g(g_), b(b_)
	{}

	Color3(const Color3& color)
		: r(color.r), g(color.g), b(color.b)
	{}

	T r, g, b;
};

template <typename T>
struct Vertex
{
	Point2D<T> position;
	Color3<T> color;
};

template <typename T>
class Triangle
{
public:
	using vertex_type = Vertex<T>;

	Triangle(const vertex_type& p0, const vertex_type& p1, const vertex_type& p2)
		: m_points{{p0, p1, p2}}, m_vao(0), m_vbo(0)
	{
		load();
	}

	~Triangle()
	{}

	void load() 
	{
		//generate number of vertex array
		glGenVertexArrays(1, &m_vao);
		//bind vao 
		glBindVertexArray(m_vao);

		//generate number of vertex buffer
		glGenBuffers(1, &m_vbo);
		//bind vbo
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//Allow buffer for passing points
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points.data(), GL_STATIC_DRAW);

#ifdef __EMSCRIPTEN__
		ShaderInfo shader[] = {
			{GL_VERTEX_SHADER, "shader/triangle.webgl.vert"},
			{GL_FRAGMENT_SHADER, "shader/triangle.webgl.frag"},
			{GL_NONE, nullptr}
		};
#else
		ShaderInfo shader[] = {
			{GL_VERTEX_SHADER, "shader/triangle.vert"},
			{GL_FRAGMENT_SHADER, "shader/triangle.frag"},
			{GL_NONE, nullptr}
		};
#endif

		m_program = Shader::loadShaders(shader);
		glUseProgram(m_program);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), 0);
		glEnableVertexAttribArray(0);

#ifdef __EMSCRIPTEN__
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), (char*)(nullptr) + sizeof(vertex_type::position));
#else
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), reinterpret_cast<char*>(nullptr) + sizeof(vertex_type::position));
#endif
		glEnableVertexAttribArray(1);
	}

	void render() 
	{
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_points.size());
	}

	void update()
	{
	}

private:
	std::array<vertex_type, 3> m_points;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_program;
};

struct RenderContext
{
	GLFWwindow* window = nullptr;
	Triangle<float> triangle;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mainLoop(void* arg_)
{
	RenderContext* renderCtx = static_cast<RenderContext*>(arg_);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	renderCtx->triangle.render();

	glfwSwapBuffers(renderCtx->window);
	
	glfwPollEvents();
}
//
//void sdlWindow()
//{
////#ifdef __EMSCRIPTEN__
////	EmscriptenWebGLContextAttributes attrs;
////	attrs.antialias = false;
////	attrs.majorVersion = 2;
////	attrs.minorVersion = 0;
////	attrs.alpha = false;
////	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE_webgl_context = emscripten_webgl_create_context(0, &attrs);
////	emscripten_webgl_make_context_current(webgl_context);
////
////	Context ctx(800, 700);
////	glDepthMask(false);
////#else
//	SDL_Init(SDL_INIT_EVERYTHING);
//
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
//	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
//
//	SDL_Window* window = SDL_CreateWindow("OpenGL Poc To Web", 30, 30, 800, 700, SDL_WINDOW_OPENGL);
//	if (!window)
//		throw std::runtime_error("error to create sdl window");
//
//	SDL_GLContext context = SDL_GL_CreateContext(window);
//
//	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
//	{
//		std::cout << "Failed to intialize GLAD" << std::endl;
//		return;
//	}
////#endif
//
//	glViewport(30, 30, 800, 700);
//
//	using Vertexf = Vertex<float>;
//	using Trianglef = Triangle<float>;
//
//	Vertexf p0{ {-0.8f, -0.8f}, {1.f, 0.f, 0.f} };
//	Vertexf p1{ {0.f, 0.8f}, { 0.f, 1.f, 0.f} };
//	Vertexf p2{ {0.8f, -0.8f}, {0.f, 0.f, 1.f} };
//
//	Triangle triangle(p0, p1, p2);
//
//#ifdef __EMSCRIPTEN__
//	emscripten_set_main_loop_args(mainLoop, window, triangle);
//#else
//	bool running = true;
//	while (running)
//	{
//		SDL_Event event;
//		while (SDL_PollEvent(&event))
//		{
//			if (event.type == SDL_QUIT)
//			{
//				running = false;
//			}
//		}
//
//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		triangle.render();
//		glFlush();
//
//		SDL_GL_SwapWindow(window);
//	}
//#endif
//
//	SDL_GL_DeleteContext(context);
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//}

int openglWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 700, "POC_OPENGL_TO_WEBGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 700);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	using Vertexf = Vertex<float>;
	using Trianglef = Triangle<float>;

	Vertexf p0{ {-0.8f, -0.8f}, {1.f, 0.f, 0.f} };
	Vertexf p1{ {0.f, 0.8f}, { 0.f, 1.f, 0.f} };
	Vertexf p2{ {0.8f, -0.8f}, {0.f, 0.f, 1.f} };

	Triangle triangle(p0, p1, p2);

#ifdef __EMSCRIPTEN__
	RenderContext rc{ window, triangle };
	emscripten_set_main_loop_arg(mainLoop, &rc, 60, 1);
#else
	while (!glfwWindowShouldClose(window))
	{
		//process inputs

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//triangle.render();
		//glFlush();

		////swap buffer at the end
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
#endif

	glfwTerminate();

	return 0;
}

int main(int argc, char* argv[])
{
	//sdlWindow();
	return openglWindow();

	//return 0;
};

#endif // !__main