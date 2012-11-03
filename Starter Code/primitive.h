#pragma once

#include "../glm/glm.hpp"

class Primitive {

public:
	float* vbo;
	short int* ibo;
	float* cbo;
	float* nbo;
	int* numVbo;
	int* numIbo;
	int* numCbo;
	//virtual float* generateVBO() = 0;
	//virtual float* generateIBO() = 0;
	//virtual float* generateCBO() = 0;
	int* kind;
	Primitive(int);
	~Primitive();

};