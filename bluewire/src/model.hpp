#pragma once

#include <GL/glew.h>
#include "obj_file.hpp"

#include <string>

using namespace std;

class model {
public:
	model(const string& id, const string& file_name);
	
private:
	void load();

	string mId;
	string mFileName;

	vector<GLfloat> mVertices;
	vector<GLushort> mFaces;
};
