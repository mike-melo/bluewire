#include "obj_file.hpp"

#include <GL/glew.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

vector<GLfloat> obj_file::vertices() {
	return this->mVertices;
}

vector<GLint> obj_file::faces() {
	return this->mFaces;
}

void obj_file::load(const string& pFileName) {
	string line;
	ifstream fileStream(pFileName);
	while (getline(fileStream, line)) {
		bool lineHasVertices = line.compare(0, string("v ").length(), string("v ")) == 0;
		bool lineHasFaces = line.compare(0, string("f ").length(), string("f ")) == 0;
		
		if (lineHasVertices) {
			istringstream stringStream(line);
			GLfloat x, y, z;
			stringStream >> string("v") >> x >> y >> z;
			mVertices.push_back(x);
			mVertices.push_back(y);
			mVertices.push_back(z);
		}

		if (lineHasFaces) {
			istringstream stringStream(line);
			GLint v1, v2, v3;
			stringStream >> string("f") >> v1 >> v2 >> v3;
			mFaces.push_back(v1 - 1);
			mFaces.push_back(v2 - 1);
			mFaces.push_back(v3 - 1);
		}
	}	
}