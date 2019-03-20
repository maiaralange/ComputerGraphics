#include <GLFW/glfw3.h>

#define WINDOW_TITLE "Hello Triangle!"

class GLFWWrapper {
public:
	GLFWwindow *window;

	void initialize() {
		initializeGLFW();
		setWindowHints();
		createWindow();
	}

private:
	void setWindowHints() {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	}

	void createWindow() {
		window = glfwCreateWindow(512, 512, WINDOW_TITLE, NULL, NULL);
		glfwMakeContextCurrent(window);
	}

	void initializeGLFW() {
		if (!glfwInit()) {
			std::cout << "GLFW init failed! \n";
		}
	}
};
