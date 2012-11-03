#include "node.h"

Node::Node(Primitive* prim, Furniture* f, Node* par, int xI, int zI) {
	shape = prim;
	furniture = f;
	children = new std::vector<Node*>();
	parent = par;
	xInd = new int(xI);
	zInd = new int(zI);
	rotate = new glm::mat4();
	translate = new glm::mat4();
	scale = new glm::mat4();
}

void Node::traverseNodes(int type, glm::mat4 transform) {
	switch(type) {
	case 0:
		*rotate = transform * *rotate;
		break;
	case 1:
		*scale = transform * *scale;
		break;
	case 2:
		*translate = transform * *translate;
		break;
	}
	Node* n;
	for (int i = 0; i < children->size(); i++) {
		n = children->at(i);
		if (n != NULL) {
			n->applyTransform(transform);
		}
	}
}

void Node::applyTransform(glm::mat4 transform) {
	*translate = transform * *translate;
	Node* n;
	for (int i = 0; i < children->size(); i++) {
		n = children->at(i);
		if (n != NULL) {
			n->applyTransform(transform);
		}
	}
}

Node::~Node() {
	std::vector<Node*>* pn = parent->children;
	for (int i = 0; i < pn->size(); i++) {
		if (pn->at(i) == this) {
			pn->erase(pn->begin() + i);
			break;
		}
	}
	delete xInd;
	delete zInd;
	delete rotate;
	delete translate;
	delete scale;
}