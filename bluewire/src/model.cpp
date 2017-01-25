#include "model.hpp"

#include <GL/glew.h>
#include <vmath\vmath.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;
using namespace vmath;

model::model(const string& id, mat4 world_pos, const string& file_name) {
	this->mId = id;
	this->mWorldPos = world_pos;
	this->mFileName = file_name;
}

vector<GLfloat> model::vertices() {
	return this->mVertices;
}
vector<GLushort> model::faces() {
	return this->mFaces;
}

mat4 model::worldPos() {
	return this->mWorldPos;
}

void model::load() {
	obj_file objFile;

	objFile.load(this->mFileName);
	this->vertices = objFile.vertices();
	this->faces = objFile.faces();
}
