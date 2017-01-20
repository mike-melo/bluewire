#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

using namespace std;

class obj_file {
public:
	void load(const string& file_name);
	vector<GLfloat> vertices();
	vector<GLushort> faces();

private:
	vector<GLfloat> mVertices;
	vector<GLushort> mFaces;
};
