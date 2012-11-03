#pragma once

#include "primitive.h"
#include <vector>

class Prism : public Primitive {
public:
	float* generateVBO(float, float, float, glm::vec4);
	short int* generateIBO();
	float* generateCBO(int);
	float* generateNBO();
	Prism(int, float, float, float, glm::vec4);
	~Prism();
};