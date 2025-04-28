#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <any>
#include <variant>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <unordered_map>
#include <tuple>
#include <glm/glm.hpp>


static std::string loadShaderSource(const std::string& path);
static GLuint compileShader(GLenum type, const std::string& source);
GLuint createShader(const std::string& vertexPath, const std::string& fragmentPath);

using UniformValue = std::variant<
	float,
	int,
	glm::vec2,
	glm::vec3,
	glm::vec4,
	glm::mat4,
	GLuint
>;

struct VertexAttribute
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	size_t offset;
};

struct VertexFormat
{
	std::vector<VertexAttribute> attributes;
	size_t stride;
};

struct Vertex
{
	glm::vec2 coord;
	glm::vec2 tex_coord;

};


void setupVertexAttributes(const VertexFormat& format);

class Drawable
{
private:
	GLuint vao;
	GLuint vbo;
	GLuint _texture;
	GLuint _shader;

	std::vector<Vertex> _vertices;

public:
	void init(const std::vector<Vertex> vertices, GLuint shader, GLuint texture);
	void draw();
	void bindTexture();
	void updateUniform(std::string name, UniformValue value);
	void updateUniforms(std::unordered_map<std::string, UniformValue> uniforms);

	void update();
	void destroy();
};

std::vector<Vertex> getRectangleVertices(float x, float y, float width, float height);