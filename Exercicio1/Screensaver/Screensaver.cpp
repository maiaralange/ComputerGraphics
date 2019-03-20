using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Shader.h"
#include "GLFWWrapper.h"
#include "GLEWWrapper.h"
#include "VBO.h"
#include "VAO.h"

#define VERTEX_SHADER_PATH "./triangle.vs"
#define FRAGMENT_SHADER_PATH "./triangle.frag"

vector<float> normalRight = { -1.0, 0.0 };
vector<float> normalLeft = { 1.0, 0.0 };
vector<float> normalUp = { 0.0, -1.0 };
vector<float> normalDown = { 0.0, 1.0 };

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

float dotProduct(vector<float> normal, vector<float> rin) {
	float result = 0.0f;
	for (int i = 0; i < 2; i++) {
		result += normal[i] * rin[i];
	}
	return result;
}

vector<float> multiplyVectorByNumber(vector<float> vector, float number) {
	return { vector[0] * number, vector[1] * number };
}

vector<float> subtractVectors(vector<float> firstVector, vector<float> secondVector) {
	return { firstVector[0] - secondVector[0], firstVector[1] - secondVector[1] };
}

vector<float> calculateReflection(vector<float> normal, vector<float> rin) {
	float dotProductRin = dotProduct(normal, rin);
	vector<float> doubleNormal = multiplyVectorByNumber(normal, 2);
	vector<float> multiplication = multiplyVectorByNumber(doubleNormal, dotProductRin);
	return subtractVectors(multiplication, rin);
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

		vector<float> rin = { -xSpeed, -ySpeed };
		vector<float> rout, normal;

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
