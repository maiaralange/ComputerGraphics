class Inserter {
public:
	virtual void insert(Mesh *mesh, Group *group, stringstream &line) = 0;
};

class NullObjectInserter : public Inserter {
public:
	void insert(Mesh *mesh, Group *group, stringstream &line) {}
};

class VertexInserter : public Inserter {
public:
	void insert(Mesh *mesh, Group *group, stringstream &sline) {
		float x, y, z;
		sline >> x >> y >> z;
		glm::vec3* vertex = new glm::vec3(x, y, z);
		mesh->vertices->push_back(vertex);
	}
};

class GroupInserter : public Inserter {
public:
	void insert(Mesh *mesh, Group *group, stringstream &sline) {
		Group *newGroup = new Group();
		group = newGroup;
		string token;
		sline >> token;
		group->name = token;
		group->material->name = token;
		mesh->groups->push_back(group);
	}
};

class NormalInserter : public Inserter {
public:
	void insert(Mesh *mesh, Group *group, stringstream &sline) {
		float x, y, z;
		sline >> x >> y >> z;
		glm::vec3 *vertex = new glm::vec3(x, y, z);
		mesh->normals->push_back(vertex);
	}
};

class MaterialFileInserter : public Inserter {
public:
	void insert(Mesh *mesh, Group *group, stringstream &sline) {
		sline >> mesh->materialFile;
	}
};

class TextureInserter : public Inserter {
public:
	void insert(Mesh *mesh, Group *group, stringstream &sline) {
		float x, y;
		sline >> x >> y;
		glm::vec2* vertex = new glm::vec2(x, (1.0f - y));
		mesh->textures->push_back(vertex);
	}
};

class FaceInserter : public Inserter {
public:
	void insert(Mesh *mesh, Group *group, stringstream &sline) {
		Face* face = new Face();
		string first, second, third, fourth;
		sline >> first;
		insertVerticesIntoFace(face, first);
		sline >> second;
		insertVerticesIntoFace(face, second);
		sline >> third;
		insertVerticesIntoFace(face, third);
		mesh->groups->back()->faces->push_back(face);
		sline >> fourth;
		if (shouldCreateAnotherFace(fourth)) {
			stringstream newLine(first + " " + third + " " + fourth);
			insert(mesh, group, newLine);
		}
	}

private:
	bool shouldCreateAnotherFace(string sline) {
		return sline != "";
	}

	void insertVerticesIntoFace(Face *face, string token) {
		string current;
		stringstream stoken(token);
		getline(stoken, current, '/');
		face->vertices.push_back(std::stoi(current) - 1);
		getline(stoken, current, '/');
		face->textures.push_back(std::stoi(current) - 1);
		getline(stoken, current, '/');
		face->normals.push_back(std::stoi(current) - 1);
	}
};