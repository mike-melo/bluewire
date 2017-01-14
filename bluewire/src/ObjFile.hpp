#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

using namespace std;

class ObjFile {
public:
	void load(const string& file_name);
	
private:
	vector<vector<GLfloat>> vertices;
};
