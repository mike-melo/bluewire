#include "obj_file.hpp"

#include <GL/glew.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

vector<vector<GLfloat>> obj_file::vertices() {
	return this->mVertices;
}

void obj_file::load(const string& pFileName) {
	string line;
	ifstream fileStream(pFileName);
	while (getline(fileStream, line)) {
		bool lineHasVertices = line.compare(0, string("v ").length(), string("v ")) == 0;
		
		if (lineHasVertices) {
			istringstream stringStream(line);
			GLfloat x, y, z;
			stringStream >> string("v") >> x >> y >> z;
			vector<GLfloat> vertex{ x, y, z };
			mVertices.push_back(vertex);
		}
	}	
}