#include "primitive.h"

Primitive::Primitive(int k) {
	kind = new int(k);
}

Primitive::~Primitive() {
	delete kind;
}