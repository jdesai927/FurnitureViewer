#pragma once

#include "primitive.h"

class Cylinder : public Primitive {
public:
	short int* ibotop;
	short int* ibobottom;
	int* numInds;

	void generateVBO();
	void generateIBO();
	Cylinder();
	~Cylinder();
};