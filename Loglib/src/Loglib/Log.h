#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define LOG_API
#else
	#ifdef LOG_BUILD_DLL
	#define LOG_API __declspec(dllexport)
	#else
	#define LOG_API __declspec(dllimport)
	#endif
#endif

#pragma once
#include <iostream>
#include <string>

class LOG_API Logger
{
public:
	void message(const std::string& msg);
};