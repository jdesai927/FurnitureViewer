#include "cylinder.h"

#define M_PI 3.1415926

Cylinder::Cylinder() : Primitive(0, 1) {
	numIbo = new int(22);
	numInds = new int(11);
	numVbo = new int(132);
	numCbo = new int(66);
	generateVBO();
	generateIBO();
}

void Cylinder::generateIBO() {
	ibo = new short int[*numIbo];
	ibotop = new short int[*numInds];
	ibobottom = new short int[*numInds];
	int i;
	for (i = 0; i < *numIbo; i++) {
		ibo[i] = i;
	}
	for (int k = 0; k < *numInds; k++) {
		ibotop[k] = i;
		i++;
	}
	for (int j = 0; j < *numInds; j++) {
		ibobottom[j] = i;
		i++;
	}
}

void Cylinder::generateVBO() {
	float x = 0.0f;
	float y = -0.5f;
	float z = 0.0f;
	vbo = new float[*numVbo];
	nbo = new float[*numVbo];
	cbo = new float[*numCbo];
	float sides = 10.0f;
	float height = 1.0f;
	const float theta = 2. * M_PI / sides;
	float c = cos(theta);
	float s = sin(theta);

	float x2 = 0.5f, z2 = 0;
	int a = 0;
	int b = 0;
	int m = 88;
	int col = 0;
	for(int i=0; i<=sides; i++) {
		const float nf = 1./sqrt(x2*x2+z2*z2),
			xn = x2*nf, zn = z2*nf;
		glm::vec3 myvec(xn, 0.0f, zn);
		myvec = glm::normalize(myvec);
		nbo[a] = myvec.x;
		nbo[a + 1] = 0.0f;
		nbo[a + 2] = myvec.z;
		nbo[a + 3] = 0.0f;
		a += 4;
		cbo[col] = 0.0f;
		cbo[col + 1] = 0.5f;
		cbo[col + 2] = 0.8f;
		col += 3;
		vbo[b] = x + x2;
		vbo[b + 1] = y * 0.2;
		vbo[b + 2] = z + z2;
		vbo[b + 3] = 1.0f;
		vbo[m] = x + x2;
		vbo[m + 1] = y + height;
		vbo[m + 2] = z + z2;
		vbo[m + 3] = 1.0f;
		nbo[m] = 0.0f;
		nbo[m + 1] = 1.0f;
		nbo[m + 2] = 0.0f;
		nbo[m + 3] = 0.0f;
		m += 4;
		b += 4;
		myvec = glm::vec3(xn, 0.0f, zn);
		myvec = glm::normalize(myvec);
		nbo[a] = myvec.x;
		nbo[a + 1] = 0.0f;
		nbo[a + 2] = myvec.z;
		nbo[a + 3] = 0.0f;
		a += 4;
		cbo[col] = 0.0f;
		cbo[col + 1] = 0.5f;
		cbo[col + 2] = 0.8f;
		col += 3;
		vbo[b] = x + x2;
		vbo[b + 1] = y + height;
		vbo[b + 2] = z + z2;
		vbo[b + 3] = 1.0f;
		b += 4;
		const float x3 = x2;
		x2 = c * x2 - s * z2;
		z2 = s * x3 + c * z2;
	}
}

Cylinder::~Cylinder() {
	delete [] nbo;
	delete [] vbo;
	delete [] ibo;
	delete [] cbo;
	delete numVbo;
	delete numCbo;
	delete numIbo;
}