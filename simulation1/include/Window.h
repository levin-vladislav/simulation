#include "GL_utils.h"

struct Window
{
private:
	int width, height;
	float r, g, b;
	void beginRender();
	void endRender();

public:
	GLuint fbo;
	Drawable window_object;
	void init(int w, int h, float x, float y);
	void Render();
	void RenderToScreen();
	void SetColor(float red, float green, float blue);
	void destroy();
};