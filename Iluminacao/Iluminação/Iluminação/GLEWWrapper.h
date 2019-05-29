#include <GL/glew.h>

class GLEWWrapper {
public:

	void initialize() {
		initializeGLEW();
	}

	void clear() {
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

private:
	void initializeGLEW() {
		if (glewInit() != GLEW_OK) {
			std::cout << "GLEW init failed! \n";
		}
	}
};
