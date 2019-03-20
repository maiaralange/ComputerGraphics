#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader {
public:
	GLuint program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		std::string vertexCode, fragmentCode;
		GLuint vertex, fragment;

		getStreams(vertexPath, fragmentPath, vertexCode, fragmentCode);

		const GLchar* vertexCodeTranslated = vertexCode.c_str();
		const GLchar* fragmentCodeTranslated = fragmentCode.c_str();
		
		createShader(vertex, vertexCodeTranslated, GL_VERTEX_SHADER);
		createShader(fragment, fragmentCodeTranslated, GL_FRAGMENT_SHADER);
		createShaderProgram(vertex, fragment);
		
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	
	void useProgram() {
		glUseProgram(program);
	}

private:
	void createShaderProgram(const GLuint &vertex, const GLuint &fragment) {
		GLint success;
		GLchar infoLog[512];

		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "Linking shader program failed.\n" << infoLog << std::endl;
		}
	}

	void createShader(GLuint &shader, const GLchar* &shaderCode, GLuint type) {
		GLint success;
		GLchar infoLog[512];

		shader = glCreateShader(type);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "Compiling shader failed.\n" << infoLog << std::endl;
		}
	}

	void getStreams(const GLchar* vertexPath, const GLchar* fragmentPath, std::string &vertexCode, std::string &fragmentCode) {
		std::ifstream vertexFile;
		std::ifstream fragmentFile;

		vertexFile.exceptions(std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::badbit);

		try {
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);
			std::stringstream vertexStream, fragmentStream;

			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			vertexCode = vertexStream.str();
			fragmentCode = fragmentStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
	}
};
