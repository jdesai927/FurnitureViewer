#pragma once

#include "cylinder.h"
#include "prism.h"
#include "sphere.h"
#include "furniture.h"

class Chair : public Furniture {
public:
	Chair(int m);
	~Chair();
};