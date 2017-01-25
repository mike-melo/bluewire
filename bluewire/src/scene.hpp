#pragma once

#include <GL/glew.h>
#include <vmath/vmath.h>

#include "model.hpp"

#include <string>

using namespace std;
using namespace vmath;

class scene {
public:
	void lookat(vec3 eye, vec3 center, vec3 up);
	void add_model(const model& model);
	void prepare();
	void render();

private:

	vector<model> mModels;

	vec3 mEye;
	vec3 mCenter;
	vec3 mUp;

	GLuint mIndicesBuffer;
	GLuint mVerticesBuffer;
	GLuint mShaderProgram;
};
