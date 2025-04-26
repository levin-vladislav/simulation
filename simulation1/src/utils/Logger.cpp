#include <iostream>
#include "Logger.h"

void Log(std::string& text)
{
	std::cout << text << '\n';
}

void Error(std::string& cause, std::string& text)
{
	std::cerr << cause << ": " << text << '\n';
}