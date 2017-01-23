#include "model.hpp"

#include <GL/glew.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

model::model(const string& id, const string& file_name) {
	this->mId = id;
	this->mFileName = file_name;
}

void model::load() {

}
