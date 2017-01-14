#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

using namespace std;

class obj_file {
public:
	void load(const string& file_name);
	vector<vector<GLfloat>> vertices();

private:
	vector<vector<GLfloat>> mVertices;
};
