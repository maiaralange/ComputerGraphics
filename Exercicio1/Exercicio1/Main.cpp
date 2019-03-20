using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "GLFWWrapper.h"
#include "GLEWWrapper.h"

#define VERTEX_SHADER_PATH "./core.vs"
#define FRAGMENT_SHADER_PATH "./core.frag"
#define BORDER_FRAGMENT_SHADER_PATH "./border.frag"

const float positions[] = {
	0.0, 0.5, 0.0,
	0.25, -0.5, 0.0,
	-0.75, -0.5, 0.0,

	0.0, 0.5, 0.0,
	0.75, 0.0, 0.0,
	0.25, -0.5, 0.0
};

const float colors[] = {
	1, 0, 0,
	0, 1, 0,
	0, 0, 1,

	1, 0, 0,
	0, 0, 1,
	0, 1, 0
};

void bindVAO(GLuint &vao, const GLuint &vboPositions, const GLuint &vboColors) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vboColors);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void bindColorsVBO(GLuint &vboColors) {
	glGenBuffers(1, &vboColors);
	glBindBuffer(GL_ARRAY_BUFFER, vboColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void bindPositionsVBO(GLuint &vboPositions) {
	glGenBuffers(1, &vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
}

int main() {
	GLFWWrapper glfwWrapper;
	glfwWrapper.initialize();

	GLEWWrapper glewWrapper;
	glewWrapper.initialize();

	GLuint vboPositions = 0, vboColors = 0, vao = 0;
	bindPositionsVBO(vboPositions);
	bindColorsVBO(vboColors);
	bindVAO(vao, vboPositions, vboColors);

	Shader triangleShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	Shader borderShader(VERTEX_SHADER_PATH, BORDER_FRAGMENT_SHADER_PATH);
	
	while (!glfwWindowShouldClose(glfwWrapper.window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		triangleShader.useProgram();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		borderShader.useProgram();
		glDrawArrays(GL_LINE_LOOP, 0, 6);
		
		glfwSwapBuffers(glfwWrapper.window);
	}
}
