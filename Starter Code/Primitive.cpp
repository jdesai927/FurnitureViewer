#include "primitive.h"

Primitive::Primitive(float h, int k) {
	kind = new int(k);
	height = new float(h);
}

Primitive::~Primitive() {
	delete kind;
	delete height;
}