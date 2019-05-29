class VBO {
public: 
	GLuint vbo = 0;

	VBO() {
		glGenBuffers(1, &vbo);
	}

	void bind(vector<GLfloat> points) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(), points.data(), GL_STATIC_DRAW);
	}
};