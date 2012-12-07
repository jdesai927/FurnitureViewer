#pragma once

#include "primitive.h"
#include <vector>

class Mesh : public Primitive {
public:
	int* mtl;
	glm::mat4* mybound;
	Mesh(int,float, int, std::vector<glm::vec3>);
	Mesh(int,int, int, std::vector<glm::vec4>);
	bool isConvex(int, std::vector<glm::vec3>);
	~Mesh();
};