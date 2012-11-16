#include "cabinet.h"

Cabinet::Cabinet(int m) : Furniture(0.3f, 6, m) {
	primitives->push_back(new int(0));
	primitives->push_back(new int(2));
	primitives->push_back(new int(2));
	primitives->push_back(new int(2));
	glm::mat4 scaleknob = glm::scale(glm::mat4(), glm::vec3(0.07f, 0.07f, 0.07f));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.15f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(0.5, 0.5, 0.5))));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.25f, 0.15f)) * scaleknob));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.15f, 0.15f)) * scaleknob));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.05f, 0.15f)) * scaleknob));
}

Cabinet::~Cabinet() {
	primitives->clear();
	delete primitives;
	localTransforms->clear();
	delete localTransforms;
}