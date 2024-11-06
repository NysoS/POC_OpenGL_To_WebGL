#include "Log.h"

void Logger::message(const std::string& msg)
{
	std::cout << "Log : " << msg.c_str() << std::endl;
}
