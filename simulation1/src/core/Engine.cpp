#include <iostream>
#include "Engine.h"
#include <stdlib.h>

void Engine::init() {
	system("cls");
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