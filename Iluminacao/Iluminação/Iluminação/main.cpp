using namespace std;

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "GLFWWrapper.h"
#include "GLEWWrapper.h"
#include "VBO.h"
#include "VAO.h"
#include "Material.h"
#include "Face.h"
#include "Group.h"
#include "Mesh.h"
#include "Inserter.h"
#include "OBJReader.h"

#define VERTEX_SHADER "Shaders/model_loading.vs"
#define FRAGMENT_SHADER "Shaders/model_loading.fs"
#define OBJ_BANANA "banana/banana.obj"
#define OBJ_TABLE "table/table.obj"
#define OBJ_STOOL "stool/stool.obj"

void onResize(GLFWwindow* window, int width, int height);
void onScroll(GLFWwindow* window, double xpos, double ypos);
void onZoom(GLFWwindow* window, double xoffset, double yoffset);
void onKeyPress();
void onPress(GLFWwindow* window, int key, int scanCode, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 7.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float translation = -1.0f;

GLFWWrapper glfwWrapper;
GLEWWrapper glewWrapper;

vector<Mesh*>* objects = new vector<Mesh*>();
int selectedObject = -1;

void createStool(ModelData &modelData, Mesh *firstStool) {
	Mesh* stool = new Mesh(modelData);
	stool->copy(firstStool);
	objects->push_back(stool);
}

int main() {
	glfwWrapper.initialize()
		.setFramebufferSizeCallback(onResize)
		.setCursorPositionCallback(onScroll)
		.setScrollCallback(onZoom)
		.setKeyCallback(onPress);
	glewWrapper.initialize();

	Shader ourShader(VERTEX_SHADER, FRAGMENT_SHADER);
	OBJReader reader;
	
	ModelData modelData = { 0.0f, 2.0f, new glm::vec3(0.0f, 0.0f, -10.0f) };
	Mesh* table = reader.read(OBJ_TABLE, ourShader, modelData);
	objects->push_back(table);

	modelData = { 0.0f, 0.5f, new glm::vec3(2.0f, 0.0f, -7.4f) };
	Mesh* firstStool = reader.read(OBJ_STOOL, ourShader, modelData);
	objects->push_back(firstStool);
	modelData = { 0.0f, 0.5f, new glm::vec3(0.1f, 0.0f, -5.9f) };
	createStool(modelData, firstStool);
	modelData = { 0.0f, 0.5f, new glm::vec3(-3.5f, 0.0f, -9.7f) };
	createStool(modelData, firstStool);
	modelData = { 0.0f, 0.5f, new glm::vec3(-1.8f, 0.0f, -7.4f) };
	createStool(modelData, firstStool);
	modelData = { 0.0f, 0.5f, new glm::vec3(3.5f, 0.0f, -9.7f) };
	createStool(modelData, firstStool);
	modelData = { 0.0f, 0.5f, new glm::vec3(2.0f, 0.0f, -12.0f) };
	createStool(modelData, firstStool);
	modelData = { 0.0f, 0.5f, new glm::vec3(-1.8f, 0.0f, -12.0f) };
	createStool(modelData, firstStool);
	modelData = { 0.0f, 0.5f, new glm::vec3(0.1f, 0.0f, -14.0f) };
	createStool(modelData, firstStool);

	modelData = { 0.0f, 0.33f, new glm::vec3(0.16f, 2.83f, -9.68f) };
	Mesh* banana = reader.read(OBJ_BANANA, ourShader, modelData);
	objects->push_back(banana);

	for (vector<Mesh*>::iterator object = objects->begin(); object != objects->end(); ++object) {
		(*object)->setup(ourShader);
	}

	glUniform1f(glGetUniformLocation(ourShader.program, "ambientIntensity"), 0.8f);
	glUniform1f(glGetUniformLocation(ourShader.program, "lightIntensity"), 0.8f);

	while (!glfwWrapper.windowShouldClose()) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		onKeyPress();
		glewWrapper.clear();
		ourShader.useProgram();

		glUniform3f(glGetUniformLocation(ourShader.program, "camera"), camera.Position.x, camera.Position.y, camera.Position.z);

		for (int i = 0; i < objects->size(); i++) {
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.getViewMatrix();
			glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "projection"), 1, GL_FALSE, &projection[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "view"), 1, GL_FALSE, &view[0][0]);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, (*objects->at(i)->modelData.translate));
			model = glm::rotate(model, glm::radians(objects->at(i)->modelData.rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(objects->at(i)->modelData.scale, objects->at(i)->modelData.scale, objects->at(i)->modelData.scale));
			glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "model"), 1, GL_FALSE, &model[0][0]);
			glUniform1f(glGetUniformLocation(ourShader.program, "colorPercentage"), selectedObject == i ? 0.2f : 0.0f);

			objects->at(i)->draw(ourShader);
		}

		glfwWrapper.swapBuffers();
		glfwPollEvents();
	}

	return 0;
}

void onKeyPress() {
	if (glfwWrapper.onKeyPress(ESCAPE))
		glfwWrapper.terminate();

	if (glfwWrapper.onKeyPress(W))
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwWrapper.onKeyPress(S))
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwWrapper.onKeyPress(A))
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwWrapper.onKeyPress(D))
		camera.processKeyboard(RIGHT, deltaTime);

	if (glfwWrapper.onKeyPress(ZERO))
		selectedObject = 0;
	if (glfwWrapper.onKeyPress(ONE))
		selectedObject = 1;
	if (glfwWrapper.onKeyPress(TWO))
		selectedObject = 2;
	if (glfwWrapper.onKeyPress(THREE))
		selectedObject = 3;
	if (glfwWrapper.onKeyPress(FOUR))
		selectedObject = 4;
	if (glfwWrapper.onKeyPress(FIVE))
		selectedObject = 5;
	if (glfwWrapper.onKeyPress(SIX))
		selectedObject = 6;
	if (glfwWrapper.onKeyPress(SEVEN))
		selectedObject = 7;
	if (glfwWrapper.onKeyPress(EIGHT))
		selectedObject = 8;
	if (glfwWrapper.onKeyPress(NINE))
		selectedObject = 9;

	if (selectedObject != -1) {
		if (glfwWrapper.onKeyPress(R))
			objects->at(selectedObject)->modelData.rotation += 0.05f;
		if (glfwWrapper.onKeyPress(T))
			objects->at(selectedObject)->modelData.rotation -= 0.05f;
		if (glfwWrapper.onKeyPress(B))
			objects->at(selectedObject)->modelData.translate->x += 0.01f;
		if (glfwWrapper.onKeyPress(V))
			objects->at(selectedObject)->modelData.translate->x -= 0.01f;
		if (glfwWrapper.onKeyPress(Y))
			objects->at(selectedObject)->modelData.translate->y += 0.01f;
		if (glfwWrapper.onKeyPress(U))
			objects->at(selectedObject)->modelData.translate->y -= 0.01f;
		if (glfwWrapper.onKeyPress(H))
			objects->at(selectedObject)->modelData.translate->z += 0.01f;
		if (glfwWrapper.onKeyPress(J))
			objects->at(selectedObject)->modelData.translate->z -= 0.01f;
	}
}

void onPress(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	if (selectedObject != -1) {
		if (key == F && action == GLFW_PRESS) {
			auto scale = objects->at(selectedObject)->modelData.scale * 0.1f;
			objects->at(selectedObject)->modelData.scale += scale;
		}
		else if (key == G && action == GLFW_PRESS) {
			if (objects->at(selectedObject)->modelData.scale > 0) {
				auto scale = objects->at(selectedObject)->modelData.scale * 0.1f;
				objects->at(selectedObject)->modelData.scale -= scale;
			}
		}
	}
}

void onResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void onScroll(GLFWwindow* window, double xPosition, double yPosition) {
	if (firstMouse) {
		lastX = xPosition;
		lastY = yPosition;
		firstMouse = false;
	}

	float xOffset = xPosition - lastX;
	float yOffset = lastY - yPosition; 

	lastX = xPosition;
	lastY = yPosition;

	camera.processMouseScroll(xOffset, yOffset);
}

void onZoom(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseZoom(yoffset);
}