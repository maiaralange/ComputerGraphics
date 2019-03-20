#include <GL/glew.h>

class GLEWWrapper {
public:

	void initialize() {
		initializeGLEW();
	}

private:
	void initializeGLEW() {
		if (glewInit() != GLEW_OK) {
			std::cout << "GLEW init failed! \n";
		}
	}
};
