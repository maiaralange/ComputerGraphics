class Group {
public:
	vector<Face*>* faces;
	Material* material;
	string name;
	VAO vao;
	VBO vbo;

	Group() {
		faces = new vector<Face*>();
		material = new Material();
	}

	void setup(vector<GLfloat> &finalVector, Shader shader) {
		glBindVertexArray(vao.vao);

		vbo.bind(finalVector);
		vao.bind(0, 3, 0);
		vao.bind(1, 2, 3);
		vao.bind(2, 3, 5);

		glBindVertexArray(0);
	}
};
