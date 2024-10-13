#include "Shader.h"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_opengles2.h>
#else
#include <glad/glad.h>
#endif


std::string readShader(const char* filename)
{
	std::ifstream inputFile(filename);
	if (!inputFile.is_open())
		throw std::runtime_error("Impossible to read shader file");

	std::stringstream buffer;
	buffer << inputFile.rdbuf();

	inputFile.close();
	return buffer.str();
}

unsigned int Shader::loadShaders(ShaderInfo* shaderInfo)
{
	if (shaderInfo == nullptr)
	{
		throw std::runtime_error("Shader info is nullptr");
	}

	auto program = glCreateProgram();
	auto* entry = shaderInfo;

	while (entry->type != GL_NONE)
	{
		auto shaderId = glCreateShader(entry->type);
		entry->shaderId = shaderId;

		auto str = readShader(entry->filename);
		const char* strData = str.c_str();

		glShaderSource(shaderId, 1, &strData, nullptr);
		glCompileShader(shaderId);

		GLint hasCompiled;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &hasCompiled);
		if (!hasCompiled)
		{
			GLint infoLogSize;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogSize);
			
			GLchar* logBuffer = new GLchar[infoLogSize + 1];
			glGetShaderInfoLog(shaderId, infoLogSize, &infoLogSize, logBuffer);

			std::cerr << logBuffer << std::endl;

			delete[] logBuffer;

			throw std::runtime_error("Shader does not compile");
		}

		glAttachShader(program, shaderId);
		++entry;
	}

	glLinkProgram(program);
	GLint hasLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &hasLinked);

	if (!hasLinked)
	{
		GLint infoLogSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize);

		GLchar* logBuffer = new GLchar[infoLogSize + 1];
		glGetProgramInfoLog(program, infoLogSize, &infoLogSize, logBuffer);

		std::cerr << logBuffer << std::endl;

		delete[] logBuffer;

		throw std::runtime_error("Program does not linked");
	}

	return program;
}
