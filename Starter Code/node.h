#include "primitive.h"
#include "furniture.h"
#include <vector>

class Node {
public:
	Node* parent;
	glm::vec4* axis;
	glm::vec3* normal;
	std::vector<Node*>* children;
	Primitive* shape;
	Furniture* furniture;
	glm::mat4* currentWorldTransform;
	glm::mat4* translate;
	glm::mat4* rotate;
	glm::mat4* scale;
	glm::mat4* inv;
	double* t;
	int* xInd;
	int* zInd;
	void computeAllInverses();
	void traverseNodes(int, glm::mat4);
	void applyTransform(glm::mat4);
	Node(Primitive*, Furniture*, Node*, int, int);
	~Node();
};