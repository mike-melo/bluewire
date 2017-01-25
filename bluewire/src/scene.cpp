#include "scene.hpp"

void scene::lookat(vec3 eye, vec3 center, vec3 up) {

}

void scene::add_model(const model& model) {
	this->mModels.push_back(model);
}

void scene::prepare() {
	vector<GLfloat> vertices;
	vector<GLfloat> indices;

	for each(model m in this->mModels) {
		vertices.insert(vertices.end(), m.vertices.begin(), m.vertices.end());
		indices.insert(indices.end(), m.faces.begin(), m.faces.end());
	}

	glGenBuffers(1, &this->mVerticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->mVerticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &this->mIndicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void scene::render() {

}