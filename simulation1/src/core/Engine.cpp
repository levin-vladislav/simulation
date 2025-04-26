#include <iostream>
#include "Engine.h"

void Engine::init() {
	std::cout << "Initialized.\n";
	graphicsEngine = GraphicsEngine();
	graphicsEngine.init();
	physicsEngine = PhysicsEngine();
	physicsEngine.init();
}

void Engine::mainloop() {
	std::cout << "Mainloop started.\n";
	while (graphicsEngine.update()){}
}