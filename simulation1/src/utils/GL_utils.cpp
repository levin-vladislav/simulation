#include "GL_utils.h"
#include "Logger.h"

VertexFormat vertex_format = {
	{
	{0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, coord)},
	{1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex_coord)},
	},
	sizeof(Vertex)
};

void setupVertexAttributes(const VertexFormat& format) {
	for (const auto& attr : format.attributes) {
		glEnableVertexAttribArray(attr.index);
		glVertexAttribPointer(
			attr.index,
			attr.size,
			attr.type,
			attr.normalized,
			format.stride,
			(const void*)attr.offset
		);
	}
}

void Drawable::init(const std::vector<Vertex> vertices, GLuint shader, GLuint texture)
{
	// Saving values
	_vertices = vertices;
	_shader = shader;
	_texture = texture;

	// Creating and binding VAO and VBO of object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

	// Setup of vertices
	setupVertexAttributes(vertex_format);

	// Logout of vertices
	for (Vertex vertex : vertices)
	{
		std::cout << vertex.coord.x << ' ' << vertex.coord.y << std::endl;
		std::cout << vertex.tex_coord.x << ' ' << vertex.tex_coord.y << std::endl;
	}

	// Enabling some functions
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}

void Drawable::draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, _vertices.size());
	glBindVertexArray(0);
}

void Drawable::bindTexture()
{
	glUseProgram(_shader);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	updateUniform("texture_object", _texture);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Drawable::updateUniform(std::string name, UniformValue value)
{
	GLint location = glGetUniformLocation(_shader, name.c_str());
	if (location == -1)
	{
		std::cerr << "Location of \"" << name << "\" was not found.\n";
	}

	std::visit([&](auto&& v) {
		using T = std::decay_t<decltype(v)>;
		if constexpr (std::is_same_v<T, float>)
			glUniform1f(location, v);
		else if constexpr (std::is_same_v<T, int>)
			glUniform1i(location, v);
		else if constexpr (std::is_same_v<T, glm::vec2>)
			glUniform2fv(location, 1, &v[0]);
		else if constexpr (std::is_same_v<T, glm::vec3>)
			glUniform3fv(location, 1, &v[0]);
		else if constexpr (std::is_same_v<T, glm::vec4>)
			glUniform4fv(location, 1, &v[0]);
		else if constexpr (std::is_same_v<T, glm::mat4>)
			glUniformMatrix4fv(location, 1, GL_FALSE, &v[0][0]);
		else if constexpr (std::is_same_v<T, GLuint>)
			glUniform1i(location, v);
		else
		{
			std::cerr << "Unexpected type was given in uniform.";
		}
		}, value);


}

void Drawable::updateUniforms(std::unordered_map<std::string, UniformValue> onetime_uniforms)
{
	for (const auto& [name, value] : onetime_uniforms)
	{
		updateUniform(name, value);
	}
}


void Drawable::update()
{
}

void Drawable::destroy()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

static std::string loadShaderSource(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open shader file: " << path << "\n";
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

static GLuint compileShader(GLenum type, const std::string& source) {
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compile error:\n" << infoLog << "\n";
	}

	return shader;
}

GLuint createShader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexCode = loadShaderSource(vertexPath);
	std::string fragmentCode = loadShaderSource(fragmentPath);

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "Program link error:\n" << infoLog << "\n";
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}


std::vector<Vertex> getRectangleVertices(float x, float y, float width, float height)
{

	return {
		Vertex {glm::vec2(x - width / 2, y - height / 2), glm::vec2(0, 0)},
		Vertex {glm::vec2(x + width / 2, y - height / 2), glm::vec2(0, 1)},
		Vertex {glm::vec2(x + width / 2, y + height / 2), glm::vec2(1, 0)},
		Vertex {glm::vec2(x - width / 2, y + height / 2), glm::vec2(1, 1)}
	};
}
