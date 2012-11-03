#include "prism.h"

void setVert(float* ret, int* ind, glm::vec3 p) {
	ret[*ind] = p.x;
	ret[*ind + 1] = p.y;
	ret[*ind + 2] = p.z;
	ret[*ind + 3] = 1.0f;
	*ind += 4;
}

//faces sequence
/*      6_ _ __7
       /|     /|
     1/_|_ _2/ |
      | |5 _ |4|
	  | /    | /
	 0|/_ _ 3|/ 
*/

float* Prism::generateVBO(float eX, float eY, float eZ, glm::vec4 c) {
	float* ret = new float[96];
	int* ind = new int(0);
	float x1 = c.x;
	float y1 = c.y;
	float z1 = c.z;
	float x2 = x1 + eX;
	float y2 = y1 + eY;
	float z2 = z1 - eZ;
	glm::vec3 v0(x1, y1, z1);
	glm::vec3 v1(x1, y2, z1);
	glm::vec3 v2(x2, y2, z1);
	glm::vec3 v3(x2, y1, z1);
	glm::vec3 v4(x2, y1, z2);
	glm::vec3 v5(x1, y1, z2);
	glm::vec3 v6(x1, y2, z2);
	glm::vec3 v7(x2, y2, z2);
	/*
	makeFace(ret, ind, v0, v1, v2, v3);
	makeFace(ret, ind, v6, v1, v0, v5);
	makeFace(ret, ind, v2, v7, v4, v3);
	makeFace(ret, ind, v6, v7, v1, v2);
	makeFace(ret, ind, v3, v0, v5, v4);
	makeFace(ret, ind, v7, v6, v5, v4);
	*/
	
	setVert(ret, ind, v3); //front
	setVert(ret, ind, v2);
	setVert(ret, ind, v1);
	setVert(ret, ind, v0);

	setVert(ret, ind, v5); //left
	setVert(ret, ind, v0);
	setVert(ret, ind, v1);
	setVert(ret, ind, v6);

	setVert(ret, ind, v3); //right
	setVert(ret, ind, v4);
	setVert(ret, ind, v7);
	setVert(ret, ind, v2);

	setVert(ret, ind, v2); //top
	setVert(ret, ind, v1);
	setVert(ret, ind, v6);
	setVert(ret, ind, v7);

	setVert(ret, ind, v4); //bottom
	setVert(ret, ind, v5);
	setVert(ret, ind, v0);
	setVert(ret, ind, v3);

	setVert(ret, ind, v4); //back
	setVert(ret, ind, v5);
	setVert(ret, ind, v6);
	setVert(ret, ind, v7);
	
	delete ind;
	return ret;
}

void setInd(short int* ret, short int v0, short int v1, short int v2, short int v3, int* ind) {
	ret[  *ind  ] = v0;
	ret[*ind + 1] = v1;
	ret[*ind + 2] = v2;
	ret[*ind + 3] = v0;
	ret[*ind + 4] = v2;
	ret[*ind + 5] = v3;
	*ind += 6;
}

short int* Prism::generateIBO() {
	short int* ret = new short int[36];
	int* ind = new int(0);
	int i = 0;
	while (*ind < 36) {
		setInd(ret, i, i + 1, i + 2, i + 3, ind);
		i += 4;
	}
	delete ind;
	return ret;
}

float* Prism::generateCBO(int colInt) {
	float* ret = new float[72];
	for (int i = 0; i < 72; i++) {
		if ((i + colInt) % 3 == 0) {
			ret[i] = 0.8f;
		} else {
			ret[i] = 0.2f;
		}
	}
	return ret;
}

float* Prism::generateNBO() {
	float* norms = new float[96];
	/*
	glm::vec3 vec1;
	glm::vec3 vec2;
	glm::vec3 vec3;
	for (int i = 0; i < 96; i += 4) {
		if (i % 16 == 0) {
			vec1.x = vbo[i]; vec1.y = vbo[i+1]; vec1.z = vbo[i+2];
			vec2.x = vbo[i+4]; vec2.y = vbo[i+5]; vec2.z = vbo[i+6];
			vec3.x = vbo[i+8]; vec2.y = vbo[i+9]; vec2.z = vbo[i+10];
			vec1 = glm::cross(vec1 - vec2, vec2 - vec3);
		}
		norms[i]     = vec1.x;//0.0f;
		norms[i + 1] = vec1.y;//0.0f;
		norms[i + 2] = vec1.z;//-1.0f;
		norms[i + 3] = 0.0f;
	}
	*/
	
	for (int i = 0; i < 16; i += 4) {
		norms[i]     = 0.0f;//0.0f;
		norms[i + 1] = 0.0f;//0.0f;
		norms[i + 2] = 1.0f;//-1.0f;
		norms[i + 3] = 0.0f;
	}
	for (int i = 16; i < 32; i += 4) {
		norms[i]     = 1.0f; //right
		norms[i + 1] = 0.0f;
		norms[i + 2] = 0.0f;
		norms[i + 3] = 0.0f;
	}
	for (int i = 32; i < 48; i += 4) {
		norms[i]     = -1.0f; //left
		norms[i + 1] = 0.0f;
		norms[i + 2] = 0.0f;
		norms[i + 3] = 0.0f;
	}
	for (int i = 48; i < 64; i += 4) {
		norms[i]     = 0.0f; //top
		norms[i + 1] = 1.0f;
		norms[i + 2] = 0.0f;
		norms[i + 3] = 0.0f;
	}
	for (int i = 64; i < 80; i += 4) {
		norms[i]     = 0.0f; //bottom
		norms[i + 1] = -1.0f;
		norms[i + 2] = 0.0f;
		norms[i + 3] = 0.0f;
	}
	for (int i = 80; i < 96; i += 4) {
		norms[i]     = 0.0f; //back
		norms[i + 1] = 0.0f;
		norms[i + 2] = -1.0f;
		norms[i + 3] = 0.0f;
	}
	
	return norms;
}

Prism::Prism(int colInt, float edgeLengthX, float edgeLengthY, float edgeLengthZ, glm::vec4 c) : Primitive(0){
	vbo = generateVBO(edgeLengthX, edgeLengthY, edgeLengthZ, c);
	ibo = generateIBO();
	cbo = generateCBO(colInt);
	nbo = generateNBO();
	numVbo = new int(96);
	numCbo = new int(72);
	numIbo = new int(36);
}

Prism::~Prism() {
	delete [] nbo;
	delete [] vbo;
	delete [] ibo;
	delete [] cbo;
	delete numVbo;
	delete numCbo;
	delete numIbo;
}