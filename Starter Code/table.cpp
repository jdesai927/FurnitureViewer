#include "table.h"

Table::Table() : Furniture(3) {
	height = new float(0.28f);
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));
	primitives->push_back(new int(1));
	primitives->push_back(new int(0));
	glm::mat4 leg = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.5f, 0.1f));
	localTransforms->push_back(new glm::mat4(leg * glm::translate(glm::mat4(), glm::vec3(-2.0f, 0.0f, -2.0f))));
	localTransforms->push_back(new glm::mat4(leg * glm::translate(glm::mat4(), glm::vec3(-2.0f, 0.0f, 2.0f))));
	localTransforms->push_back(new glm::mat4(leg * glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, -2.0f))));
	localTransforms->push_back(new glm::mat4(leg * glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 2.0f))));
	localTransforms->push_back(new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.3f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(1.0f, 0.1f, 1.0f))));
}

Table::~Table() {
	delete height;
	primitives->clear();
	delete primitives;
	localTransforms->clear();
	delete localTransforms;
}