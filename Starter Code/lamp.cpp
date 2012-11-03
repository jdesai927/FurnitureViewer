#include "lamp.h"

Lamp::Lamp() : Furniture(5) {
	height = new float(0.17f);
	primitives->push_back(new int(2));
	primitives->push_back(new int(1));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.15f, 0.0f)) * glm::rotate(glm::mat4(), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(0.2, 0.2, 0.2))));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.05f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(0.1, 0.1, 0.1))));
}

Lamp::~Lamp() {
	delete height;
	primitives->clear();
	delete primitives;
	localTransforms->clear();
	delete localTransforms;
}