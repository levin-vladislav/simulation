#include <iostream>
#include "Window.h"

void Window::init(int w, int h, float x, float y)
{
    // creating FBO
    width = w; height = h;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // creating shader
    GLuint shader = createShader("shaders/window.vx", "shaders/window.fg");

    // creating texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Binding FBO to texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    
    // Checks if FBO was created successfully
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!\n";

    // Returning to main FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Creating Drawable object with window texture
    window_object = Drawable();
    const std::vector<Vertex> vertices = getRectangleVertices(x, y, 0.1, 0.1);
    window_object.init(vertices, shader, texture);
    window_object.bindTexture();

}

void Window::update()
{
    Render();
    RenderToScreen();
}

void Window::SetColor(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

void Window::destroy()
{
    window_object.destroy();
}


void Window::beginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::endRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 800, 600);
}

void Window::Render()
{
    beginRender();
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    // rendering objects
    endRender();
}

void Window::RenderToScreen()
{
    window_object.update();
    window_object.draw();
}
