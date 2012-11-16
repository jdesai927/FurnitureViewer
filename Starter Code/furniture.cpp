#include "furniture.h"

Furniture::Furniture(float h, int k, int m) : Primitive(h, k) {
	primitives = new std::vector<int*>();
	worldTransforms = new std::vector<glm::mat4*>();
	localTransforms = new std::vector<glm::mat4*>();
	inverses = new std::vector<glm::mat4*>();
	mtl = new int(m);
}

void Furniture::computeInverses(glm::mat4 T) {
	inverses->clear();
	for (int i = 0; i < localTransforms->size(); i++) {
		glm::mat4* trans = new glm::mat4(T * *localTransforms->at(i));
		inverses->push_back(new glm::mat4(glm::inverse(*trans)));
		worldTransforms->push_back(trans);
	}
}

Furniture::~Furniture() {
	primitives->clear();
	delete primitives;
	localTransforms->clear();
	delete localTransforms;
	inverses->clear();
	delete inverses;
	delete mtl;
}