#include "chair.h"

Chair::Chair(int m) : Furniture(0.55, 4, m) {
	primitives->push_back(new int(0));
	primitives->push_back(new int(0));
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));

	glm::mat4 scalpart = glm::scale(glm::mat4(), glm::vec3(0.35f, 0.1f, 0.35f));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.25f, 0.0f)) * scalpart));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.45f, 0.15f)) * glm::rotate(glm::mat4(), 90.0f, glm::vec3(1, 0, 0)) * scalpart));
	glm::mat4 leg = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.3f, 0.1f));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(-0.1f, 0.1f, -0.1f)) * leg));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(-0.1f, 0.1f, 0.1f)) * leg));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.1f, 0.1f, -0.1f)) * leg));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f)) * leg));
}

Chair::~Chair() {
	primitives->clear();
	delete primitives;
	localTransforms->clear();
	delete localTransforms;
}