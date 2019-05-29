class OBJReader {
public:
	Mesh* read(string const &path, Shader &shader, ModelData &modelData) {
		getDirectoryFromPath(path);
		loadOBJ(path, modelData);
		loadTextures(shader);
		return mesh;
	}

private:
	Inserter *vertexInserter = new VertexInserter();
	Inserter *groupInserter = new GroupInserter();
	Inserter *normalInserter = new NormalInserter();
	Inserter *textureInserter = new TextureInserter();
	Inserter *faceInserter = new FaceInserter();
	Inserter *materialFileInserter = new MaterialFileInserter();
	Inserter *nullObjectInserter = new NullObjectInserter();
	Mesh *mesh;
	Group *group;
	string directory;

	map<string, Inserter*> typeToInserterMap = {
		{ "v", vertexInserter },
		{ "vn", normalInserter },
		{ "vt", textureInserter },
		{ "f", faceInserter },
		{ "g", groupInserter },
		{ "mtllib", materialFileInserter },
		{ "usemtl", groupInserter },
		{ "#", nullObjectInserter }
	};

	void getDirectoryFromPath(string const &path) {
		directory = path.substr(0, path.find_last_of('/'));
	}

	void loadOBJ(string const &path, ModelData &modelData) {
		ifstream file(path);
		mesh = new Mesh(modelData);
		while (!file.eof()) {
			stringstream sline;
			string line, current;
			getline(file, line);
			sline << line; sline >> current;
			auto it = typeToInserterMap.find(current);
			if (it != typeToInserterMap.end()) {
				typeToInserterMap[current]->insert(mesh, group, sline);
			}
		}
		file.close();
	}

	void loadTextures(Shader &shader) {
		ifstream file(directory + '/' + mesh->materialFile);
		string materialName;
		float ns = 0;
		unsigned int id;
		glm::vec3 ka, kd, ks;
		while (!file.eof()) {
			string line, current;
			stringstream sline;
			getline(file, line);
			sline << line;
			sline >> current;
			if (current == "newmtl") {
				sline >> materialName;
			}
			else if (current == "map_Kd") {
				string path;
				sline >> path;
				id = loadTexturesFromFile(path, directory, shader);
				insertInformationIntoMaterial(id, ka, kd, ks, ns, materialName);
			}
			else if (current == "Ka") {
				sline >> ka.x >> ka.y >> ka.z;
			}
			else if (current == "Kd") {
				sline >> kd.x >> kd.y >> kd.z;
			}
			else if (current == "Ks") {
				sline >> ks.x >> ks.y >> ks.z;
			}
			else if (current == "Ns") {
				sline >> ns;
			}
		}
		file.close();
	}

	void insertInformationIntoMaterial(unsigned int id, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float ns, string &materialName) {
		for (unsigned int i = 0; i < mesh->groups->size(); i++) {
			if (mesh->groups->at(i)->material->name == materialName) {
				mesh->groups->at(i)->material->id = id;
				mesh->groups->at(i)->material->ka = ka;
				mesh->groups->at(i)->material->kd = kd;
				mesh->groups->at(i)->material->ks = ks;
				mesh->groups->at(i)->material->ns = ns;
			}
		}
	}

	unsigned int loadTexturesFromFile(string &filename, const string &directory, Shader &shader) {
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

		if (data) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
		else {
			std::cout << "Texture failed to load at path: " << filename << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
};