#include "node.h"
#include "mesh.h"

Node::Node(Primitive* prim, Furniture* f, Node* par, int xI, int zI) {
	shape = prim;
	furniture = f;
	normal = new glm::vec3();
	children = new std::vector<Node*>();
	parent = par;
	xInd = new int(xI);
	zInd = new int(zI);
	currentWorldTransform = new glm::mat4();
	boundTrans = new glm::mat4();
	worldTransform = new glm::mat4();
	inv = new glm::mat4();
	rotate = new glm::mat4();
	translate = new glm::mat4();
	mtl = new int(-1);
	t = new double(-1.0);
	scale = new glm::mat4();
}

void Node::computeAllInverses() {
	*worldTransform = *translate * *rotate * *scale;
	if (furniture != NULL) {
		furniture->computeInverses(*worldTransform);
	}
	*inv = glm::inverse(*worldTransform);
	if (shape != NULL && *shape->kind > 97) {
		*boundTrans = glm::inverse(*worldTransform * *static_cast<Mesh*>(shape)->mybound);
	}
	for (int i = 0; i < children->size(); i++) {
		children->at(i)->computeAllInverses();
	}
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