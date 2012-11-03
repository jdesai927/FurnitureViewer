#pragma once
#include <vector>

#include "../glm/gtc/matrix_transform.hpp"
#include "primitive.h"

class Furniture : public Primitive {
public:
	float* height;
	std::vector<int*>* primitives;
	std::vector<glm::mat4*>* localTransforms;
	Furniture(int);
	~Furniture();
};