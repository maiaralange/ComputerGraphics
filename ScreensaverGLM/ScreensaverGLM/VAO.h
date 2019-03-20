class VAO {
public:
	GLuint vao = 0;

	VAO() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void bindColors(VBO colors) {
		glBindBuffer(GL_ARRAY_BUFFER, colors.vbo);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	void bindPositions(VBO positions) {
		glBindBuffer(GL_ARRAY_BUFFER, positions.vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
};