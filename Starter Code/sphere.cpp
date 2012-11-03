#include "sphere.h"
#include <math.h>
#include <vector>

#define M_PI 3.1415926

Sphere::Sphere() : Primitive(2) {
	float num = 2 * 19 * 9;
	numIbo = new int(num);
	numCbo = new int(num * 3);
	numVbo = new int(num * 4);
	createSphere();
}

Sphere::~Sphere() {
	delete [] nbo;
	delete [] vbo;
	delete [] ibo;
	delete [] cbo;
	delete numVbo;
	delete numCbo;
	delete numIbo;
}

void Sphere::createSphere() {
	cbo = new float[*numCbo];
	vbo = new float[*numVbo];
	nbo = new float[*numVbo];
	ibo = new short int[*numIbo];
	glm::vec3 cen(0, 0, 0);
	glm::vec3 thisPoint;
	float phi;
	float phir;
	float phir20;
	float theta;
	float thetar;
	float x;
	float y;
	float z;
	float c80;
	float c = M_PI/180.0f;
	int i = 0;
	int j = 0;
	int k = 0;
	for (int k = 0; k < *numIbo; k++) {
		ibo[k] = k;
	}
	for(phi=-80.0; phi<=80.0; phi+=20.0)
	{
		phir=c*phi;
		phir20=c*(phi+20);
		for (theta=-180.0; theta<=180.0;theta+=20.0) {
			thetar=c*theta;
			x=sin(thetar)*cos(phir);
			y=cos(thetar)*cos(phir);
			z=sin(phir);
			thisPoint = glm::vec3(0.4 * x, 0.4 * y, 0.4 * z);
			vbo[i] = thisPoint.x;
			vbo[i + 1] = thisPoint.y;
			vbo[i + 2] = thisPoint.z;
			vbo[i + 3] = 1.0f;
			thisPoint = glm::normalize(thisPoint - cen);
			nbo[i] = thisPoint.x;
			nbo[i + 1] = thisPoint.y;
			nbo[i + 2] = thisPoint.z;
			nbo[i + 3] = 0.0f;
			cbo[j] = 0.2f;
			cbo[j + 1] = 0.7f;
			cbo[j + 2] = 0.3f;
			j += 3;
			i += 4;
			x=sin(thetar)*cos(phir20);
			y=cos(thetar)*cos(phir20);
			z=sin(phir20);
			thisPoint = glm::vec3(0.4 * x, 0.4 * y, 0.4 * z);
			vbo[i] = thisPoint.x;
			vbo[i + 1] = thisPoint.y;
			vbo[i + 2] = thisPoint.z;
			vbo[i + 3] = 1.0f;
			thisPoint = glm::normalize(thisPoint - cen);
			nbo[i] = thisPoint.x;
			nbo[i + 1] = thisPoint.y;
			nbo[i + 2] = thisPoint.z;
			nbo[i + 3] = 0.0f;
			i += 4;
			cbo[j] = 0.2f;
			cbo[j + 1] = 0.7f;
			cbo[j + 2] = 0.3f;
			j += 3;
		}
	}

	/*
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0, 0.0 , 1.0);
		c=M_PI/180.0;
		c80=c*80.0;
		z=sin(c80);
		for(theta=-180.0; theta<=180.0;theta+=20.0) {
			thetar=c*theta;
			x=sin(thetar)*cos(c80);
			y=cos(thetar)*cos(c80);
			glVertex3f(x, y, z);
		}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0, 0.0, -1.0);
		z=-sin(c80);
		for(theta=-180.0;theta<=180.0;theta+=20.0) {
			thetar=c*theta;
			x=sin(thetar)*cos(c80);
			y=cos(thetar)*cos(c80);
			glVertex3f(x, y, z);
		}
	glEnd();
	*/
}