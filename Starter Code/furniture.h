#pragma once
#include <vector>

#include "../glm/gtc/matrix_transform.hpp"
#include "primitive.h"

class Furniture : public Primitive {
public:
	std::vector<int*>* primitives;
	std::vector<glm::mat4*>* worldTransforms;
	std::vector<glm::mat4*>* localTransforms;
	std::vector<glm::mat4*>* inverses;
	void computeInverses(glm::mat4);
	int* mtl;
	Furniture(float,int,int);
	~Furniture();
};