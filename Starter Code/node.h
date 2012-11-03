#include "primitive.h"
#include "furniture.h"
#include <vector>

class Node {
public:
	Node* parent;
	glm::vec4* axis;
	std::vector<Node*>* children;
	Primitive* shape;
	Furniture* furniture;
	glm::mat4* translate;
	glm::mat4* rotate;
	glm::mat4* scale;
	int* xInd;
	int* zInd;
	void traverseNodes(int, glm::mat4);
	void applyTransform(glm::mat4);
	Node(Primitive*, Furniture*, Node*, int, int);
	~Node();
};