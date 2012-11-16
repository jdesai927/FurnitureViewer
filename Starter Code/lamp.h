#include "sphere.h"
#include "prism.h"
#include "cylinder.h"
#include "furniture.h"

class Lamp : public Furniture {
public:
	Lamp(int m);
	~Lamp();
};