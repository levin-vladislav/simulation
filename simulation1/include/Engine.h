#pragma once
#include "PhysicsEngine.h"
#include "GraphicsEngine.h"
class Engine
{
private:
	GraphicsEngine graphicsEngine;
	PhysicsEngine physicsEngine;
public:
	void init();
	void mainloop();
};