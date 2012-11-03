#include "chair.h"

Chair::Chair() : Furniture(4) {
	height = new float(0.5f);
	primitives->push_back(new int(0));
	primitives->push_back(new int(0));
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));

	glm::mat4 scalpart = glm::scale(glm::mat4(), glm::vec3(0.5f, 0.2f, 0.5f));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0, 0.3, 0)) * scalpart));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0, 0.4, -0.1)) * glm::rotate(glm::mat4(), 90.0f, glm::vec3(1, 0, 0)) * scalpart));
	glm::mat4 leg = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.5f, 0.1f));
	localTransforms->push_back(new glm::mat4(leg * glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, -1.0f))));
	localTransforms->push_back(new glm::mat4(leg * glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, 1.0f))));
	localTransforms->push_back(new glm::mat4(leg * glm::translate(glm::mat4(), glm::vec3(1.0f, 0.0f, -1.0f))));
	localTransforms->push_back(new glm::mat4(leg * glm::translate(glm::mat4(), glm::vec3(1.0f, 0.0f, 1.0f))));
}

Chair::~Chair() {
	delete height;
	primitives->clear();
	delete primitives;
	localTransforms->clear();
	delete localTransforms;
}