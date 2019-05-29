#include <GLFW/glfw3.h>

#define WINDOW_TITLE "CG - Grau A"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

enum Key {
	ESCAPE = GLFW_KEY_ESCAPE,
	A = GLFW_KEY_A,
	B = GLFW_KEY_B,
	C = GLFW_KEY_C,
	D = GLFW_KEY_D,
	E = GLFW_KEY_E,
	F = GLFW_KEY_F,
	G = GLFW_KEY_G,
	H = GLFW_KEY_H,
	I = GLFW_KEY_I,
	J = GLFW_KEY_J,
	K = GLFW_KEY_K,
	L = GLFW_KEY_L,
	M = GLFW_KEY_M,
	N = GLFW_KEY_N,
	O = GLFW_KEY_O,
	P = GLFW_KEY_P,
	Q = GLFW_KEY_Q,
	R = GLFW_KEY_R,
	S = GLFW_KEY_S,
	T = GLFW_KEY_T,
	U = GLFW_KEY_U,
	V = GLFW_KEY_V,
	W = GLFW_KEY_W,
	X = GLFW_KEY_X,
	Y = GLFW_KEY_Y,
	Z = GLFW_KEY_Z,
	ZERO = GLFW_KEY_0,
	ONE = GLFW_KEY_1,
	TWO = GLFW_KEY_2,
	THREE = GLFW_KEY_3,
	FOUR = GLFW_KEY_4,
	FIVE = GLFW_KEY_5,
	SIX = GLFW_KEY_6,
	SEVEN = GLFW_KEY_7,
	EIGHT = GLFW_KEY_8,
	NINE = GLFW_KEY_9
};

class GLFWWrapper {
public:
	GLFWWrapper initialize() {
		initializeGLFW();
		setWindowHints();
		createWindow();
		setEnableVariables();
		return *this;
	}

	GLFWWrapper setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
		glfwSetFramebufferSizeCallback(window, callback);
		return *this;
	}

	GLFWWrapper setCursorPositionCallback(GLFWcursorposfun callback) {
		glfwSetCursorPosCallback(window, callback);
		return *this;
	}

	GLFWWrapper setScrollCallback(GLFWscrollfun callback) {
		glfwSetScrollCallback(window, callback);
		return *this;
	}

	GLFWWrapper setKeyCallback(GLFWkeyfun callback) {
		glfwSetKeyCallback(window, callback);
		return *this;
	}

	int windowShouldClose() {
		return glfwWindowShouldClose(window);
	}

	bool onKeyPress(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	void swapBuffers() {
		glfwSwapBuffers(window);
	}

	void terminate() {
		glfwSetWindowShouldClose(window, true);
		glfwTerminate();
	}

private:
	GLFWwindow *window;

	void setWindowHints() {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	}

	void createWindow() {
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
		glfwMakeContextCurrent(window);
	}

	void initializeGLFW() {
		if (!glfwInit()) {
			std::cout << "GLFW init failed! \n";
		}
	}

	void setEnableVariables() {
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
};
