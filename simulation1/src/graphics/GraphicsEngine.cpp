
#include "GraphicsEngine.h"

void GraphicsEngine::init()
{
	// Cathcing exceptions
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return;
	}

	// Initializing main window
	window = glfwCreateWindow(800, 600, "Simulation", NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}


	// Creating GL context
	glfwMakeContextCurrent(window);
	glewInit();

	// Initializing multi-windows
	initWindows();

	std::cout << "GraphicsEngine initialized.\n";
}

void GraphicsEngine::initWindows()
{
	Window redWin, blueWin;

	redWin.init(50, 50, 0.1f, 0.1f);
	blueWin.init(20, 30, 0.5f, 0.5f);

	redWin.SetColor(1, 0, 0);
	blueWin.SetColor(0, 0, 1);

	windows["Red Window"] = redWin;
	windows["Blue Window"] = blueWin;
}

bool GraphicsEngine::update()
{
	// Binding main FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Updates screen
	glClear(GL_COLOR_BUFFER_BIT);

	// rendering multi-windows
	for (const std::pair<std::string, Window>& pair : windows)
	{
		windows[pair.first].update();
	}

	// Checking events
	glfwSwapBuffers(window);
	glfwPollEvents();

	if (glfwWindowShouldClose(window))
	{
		destroy();
		return false;
	}
	
	return true;

}

void GraphicsEngine::destroy()
{
	// destroying multi-windows
	for (const std::pair<std::string, Window>& pair : windows)
	{
		windows[pair.first].destroy();

	}

	// destroying context
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "GraphicsEngine destroyed!\n";
}