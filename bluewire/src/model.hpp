#pragma once

#include <GL/glew.h>
#include "obj_file.hpp"

#include <string>

using namespace std;

class model {
public:
	model(const string& id, mat4 worldPos, const string& file_name);
	
	vector<GLfloat> vertices();
	vector<GLushort> faces();
	mat4 worldPos();

private:
	void load();

	mat4 mWorldPos;

	string mId;
	string mFileName;

	vector<GLfloat> mVertices;
	vector<GLushort> mFaces;
};
