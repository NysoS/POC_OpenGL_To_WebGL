#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "Shader.h"	

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

		ShaderInfo shader[] = {
			{GL_VERTEX_SHADER, "triangle.vert"},
			{GL_FRAGMENT_SHADER, "triangle.frag"},
			{GL_NONE, nullptr}
		};

		m_program = Shader::loadShaders(shader);
		glUseProgram(m_program);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), reinterpret_cast<char*>(nullptr) + sizeof(vertex_type::position));
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
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

	Vertexf p0{ {-0.8, -0.8}, {1., 0., 0.} };
	Vertexf p1{ {0, 0.8}, { 0., 1., 0.} };
	Vertexf p2{ {0.8, -0.8}, {0., 0., 1.} };

	Triangle triangle(p0, p1, p2);

	while (!glfwWindowShouldClose(window))
	{
		//process inputs

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		triangle.render();
		glFlush();

		//swap buffer at the end
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
};
