#pragma once

#include <GL/glew.h>

#define GLSL(version, shader)  "#version " #version "\n" #shader

const GLchar* vertexShader = GLSL(430,
	layout(location = 0) in vec3 vPosition;
	layout(location = 1) uniform mat4 MVP;

	void main() {
		gl_Position = MVP * vec4(vPosition, 1);
	}
);

const GLchar* fragmentShader = GLSL(430,
	out vec4 fColor;

	void main() {
		fColor = vec4(0.0, 0.0, 1.0, 1.0);
	}
);