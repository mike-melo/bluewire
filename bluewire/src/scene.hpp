#pragma once

#include <GL/glew.h>
#include <vmath/vmath.h>

#include "gl.hpp"
#include "model.hpp"

#include <string>

using namespace std;
using namespace vmath;

class scene {
public:
	scene(const gl& gl);

	void lookat(vec3 eye, vec3 center, vec3 up);
	void add_model(const model& model, mat4 world_pos);
	void render();

private:
	gl mGl;

	vec3 eye;
	vec3 center;
	vec3 up;
};
