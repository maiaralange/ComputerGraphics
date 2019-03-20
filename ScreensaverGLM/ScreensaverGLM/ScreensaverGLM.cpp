using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <GLM/vec2.hpp>

#include "Shader.h"
#include "GLFWWrapper.h"
#include "GLEWWrapper.h"
#include "VBO.h"
#include "VAO.h"

#define VERTEX_SHADER_PATH "./triangle.vs"
#define FRAGMENT_SHADER_PATH "./triangle.frag"

glm::vec2 normalRight = { -1.0, 0.0 };
glm::vec2 normalLeft = { 1.0, 0.0 };
glm::vec2 normalUp = { 0.0, -1.0 };
glm::vec2 normalDown = { 0.0, 1.0 };

vector<float> positions = {
	0.0, 0.5, 0.0,
	0.5, -0.5, 0.0,
	-0.5, -0.5, 0.0
};

vector<float> colors = {
	1, 0, 0,
	0, 1, 0,
	0, 0, 1
};

float matrix[] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

glm::vec2 calculateReflection(glm::vec2 normal, glm::vec2 rin) {
	glm::vec2 integer = { 2, 2 };
	return integer * normal * (normal * rin) - rin;
}

int main() {
	GLFWWrapper glfwWrapper;
	GLEWWrapper glewWrapper;
	glfwWrapper.initialize();
	glewWrapper.initialize();

	VBO vboPositions, vboColors;
	VAO vao;
	vboPositions.bind(positions);
	vboColors.bind(colors);
	vao.bindPositions(vboPositions);
	vao.bindColors(vboColors);

	Shader triangleShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	triangleShader.useProgram();

	int matrixLocation = glGetUniformLocation(triangleShader.program, "matrix");
	float xSpeed = 0.9f, ySpeed = 1.0f;
	float xLastPosition = 0.0f, yLastPosition = 0.0f;

	while (!glfwWindowShouldClose(glfwWrapper.window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static double previousSeconds = glfwGetTime();
		double currentSeconds = glfwGetTime();
		double elapsedSeconds = currentSeconds - previousSeconds;
		previousSeconds = currentSeconds;

		glm::vec2 rin = { -xSpeed, -ySpeed };
		glm::vec2 rout, normal;

		if (fabs(xLastPosition) + 0.5f > 1.0f) {
			normal = xLastPosition > 0 ? normalRight : normalLeft;
			rout = calculateReflection(normal, rin);
			xSpeed = rout[0];
		}
		if (fabs(yLastPosition) + 0.5f >= 1.0f) {
			normal = yLastPosition > 0 ? normalUp : normalDown;
			rout = calculateReflection(normal, rin);
			ySpeed = rout[1];
		}

		matrix[12] = elapsedSeconds * xSpeed + xLastPosition;
		xLastPosition = matrix[12];
		matrix[13] = elapsedSeconds * ySpeed + yLastPosition;
		yLastPosition = matrix[13];

		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
		glBindVertexArray(vao.vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(glfwWrapper.window);
	}
}
