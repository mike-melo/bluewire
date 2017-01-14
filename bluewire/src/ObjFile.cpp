#include "ObjFile.hpp"

#include <GL/glew.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

void ObjFile::load(const string& file_name) {
	string line;
	ifstream file_stream(file_name);
	while (getline(file_stream, line)) {
		bool line_has_vertices = line.compare(0, string("v ").length(), string("v ")) == 0;
		
		if (line_has_vertices) {
			istringstream string_stream(line);
			GLfloat x, y, z;
			string_stream >> string("v") >> x >> y >> z;
			vector<GLfloat> vertex{ x, y, z };
			vertices.push_back(vertex);
		}
	}	
}