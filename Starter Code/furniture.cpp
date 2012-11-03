#include "furniture.h"

Furniture::Furniture(int k) : Primitive(k) {
	primitives = new std::vector<int*>();
	localTransforms = new std::vector<glm::mat4*>();
}

Furniture::~Furniture() {
	primitives->clear();
	delete primitives;
	localTransforms->clear();
	delete localTransforms;
}