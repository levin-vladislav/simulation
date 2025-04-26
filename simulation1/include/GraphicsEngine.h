#pragma once
#include <iostream>
#include <unordered_map>
#include "Window.h"


class GraphicsEngine
{
private:
	GLFWwindow* window;
	std::unordered_map<std::string, Window> windows;

public:
	void init();
	bool update();
	void destroy();
};