class VAO {
public:
	GLuint vao = 0;

	VAO() {
		glGenVertexArrays(1, &vao);
	}

	void bind(int start, int size, int offset) {
		glVertexAttribPointer(start, size, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
		glEnableVertexAttribArray(start);
	}
};