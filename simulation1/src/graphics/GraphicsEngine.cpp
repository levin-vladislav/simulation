
#include "GraphicsEngine.h"

void GraphicsEngine::init()
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return;
	}

	window = glfwCreateWindow(800, 600, "Simulation", NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}

	Window redWin, blueWin;

	redWin.init(50, 50, 0.1f, 0.1f);
	blueWin.init(20, 30, 0.5f, 0.5f);

	redWin.SetColor(1, 0, 0);
	blueWin.SetColor(0, 0, 1);

	windows["Red Window"] = redWin;
	windows["Blue Window"] = blueWin;


	glfwMakeContextCurrent(window);
	glewInit();

	glEnable(GL_TEXTURE_2D);
	std::cout << "GraphicsEngine initialized.\n";
}

bool GraphicsEngine::update()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// rendering
	for (const std::pair<std::string, Window>& pair : windows)
	{
		windows[pair.first].Render();
		windows[pair.first].RenderToScreen();

	}

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
	for (const std::pair<std::string, Window>& pair : windows)
	{
		windows[pair.first].destroy();

	}
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "GraphicsEngine destroyed!\n";
}