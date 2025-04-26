#include <iostream>
#include "Window.h"

void Window::init(int w, int h, float x, float y)
{
    width = w; height = h;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLuint shader = createShader("shaders/window.vx", "shaders/window.fg");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    window_object = Drawable();
    const std::vector<Vertex> vertices = getRectangleVertices(x, y, width / 100, height / 100);
    window_object.init(vertices, shader, texture);
    window_object.updateUniform("texture_object", texture);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::beginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::endRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::SetColor(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

void Window::Render()
{
    beginRender();
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    endRender();
}

void Window::RenderToScreen()
{
    window_object.update();
    window_object.draw();
}

void Window::destroy()
{
    window_object.destroy();
}