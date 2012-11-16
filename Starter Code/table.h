#include "sphere.h"
#include "cylinder.h"
#include "prism.h"
#include "furniture.h"

class Table : public Furniture {
public:
	Table(int m);
	~Table();
};