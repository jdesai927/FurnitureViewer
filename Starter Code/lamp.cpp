#include "lamp.h"

Lamp::Lamp(int m) : Furniture(0.17f, 5, m) {
	primitives->push_back(new int(2));
	//primitives->push_back(new int(1));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.1f, 0.0f)) * glm::rotate(glm::mat4(), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f))));
	//localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(0.1, 0.3, 0.1))));
}

Lamp::~Lamp() {
	primitives->clear();
	delete primitives;
	localTransforms->clear();
	delete localTransforms;
}