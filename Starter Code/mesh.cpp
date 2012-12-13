#include "mesh.h"
#include <iostream>
#include "../glm/gtc/matrix_transform.hpp"

Mesh::Mesh(int m, float length, int numVerts, std::vector<glm::vec3> verts) : Primitive(length, 99) {
	//Extrusion
	mtl = new int(m);
	numVbo = new int(24 * numVerts);
	bool convex = isConvex(numVerts, verts);
	if (convex) {
		*numVbo += ((numVerts - 2) * 24);
	}
	glm::vec3 minvert = verts.at(0);
	float minx = 999999.9f;
	float maxx = -999999.9f;
	float miny = 999999.9f;
	float maxy = -999999.9f;
	float minz = 999999.9f;
	float maxz = -999999.9f;
	for (int i = 1; i < numVerts; i++) {
		if (verts.at(i).y < minvert.y) {
			minvert = verts.at(i);
		}
		if (verts.at(i).x < minx) {
			minx = verts.at(i).x;
		}
		if (verts.at(i).y < miny) {
			miny = verts.at(i).y;
		}
		if (verts.at(i).z < minz) {
			minz = verts.at(i).z;
		}
		if (verts.at(i).x > maxx) {
			maxx = verts.at(i).x;
		}
		if (verts.at(i).y > maxy) {
			maxy = verts.at(i).y;
		}
		if (verts.at(i).z > maxz) {
			maxz = verts.at(i).z;
		}
	}
	mybound = new glm::mat4(glm::scale(glm::mat4(), glm::vec3(maxx - minx, maxy - miny, maxz - minz)));
	float morey = 0.0f;//0.35f;
	if (minvert.y < 0) {
		morey = -1 * minvert.z;
	}
	numCbo = new int((3 * *numVbo)/4);
	numIbo = new int(*numVbo/4);
	vbo = new float[*numVbo];
	ibo = new short int[*numIbo];
	cbo = new float[*numCbo];
	nbo = new float[*numVbo];
	for (int c = 0; c < *numCbo; c+=3) {
		cbo[c] = 0.0f;
		cbo[c + 1] = 1.0f;
		cbo[c + 2] = 0.0f;
	}
	for (int in = 0; in < *numIbo; in++) {
		ibo[in] = in;
	}
	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
	glm::vec3 norm;
	int k = 0;
	for (int i = 0; i < numVerts; i++) {
		v1 = verts.at(i);
		if (i + 1 == numVerts) {
			v2 = verts.at(0);
		} else {
			v2 = verts.at(i+1);
		}
		x1 = v1.x;
		x2 = v2.x;
		y1 = v1.y;
		y2 = v2.y;
		z1 = v1.z;
		z2 = v2.z;
		v3.x = x1;
		v3.y = y1 + length + morey;
		v3.z = z1;
		norm = glm::normalize(glm::cross(v3 - v1, v2 - v1));
		for (int j = k; j < k + 24; j+=4) {
			nbo[j] = norm.x;
			nbo[j + 1] = norm.y;
			nbo[j + 2] = norm.z;
			nbo[j + 3] = 0.0f;
		}
		vbo[k] = x1;
		vbo[k + 1] = y1 + morey;
		vbo[k + 2] = z1;
		vbo[k + 3] = 1.0f;
		vbo[k + 4] = x1;
		vbo[k + 5] = y1 + length  + morey;
		vbo[k + 6] = z1;
		vbo[k + 7] = 1.0f;
		vbo[k + 8] = x2;
		vbo[k + 9] = y2  + morey;
		vbo[k + 10] = z2;
		vbo[k + 11] = 1.0f;
		vbo[k + 12] = x2;
		vbo[k + 13] = y2 + morey;
		vbo[k + 14] = z2;
		vbo[k + 15] = 1.0f;
		vbo[k + 16] = x2;
		vbo[k + 17] = y2 + length  + morey;
		vbo[k + 18] = z2;
		vbo[k + 19] = 1.0f;
		vbo[k + 20] = x1;
		vbo[k + 21] = y1 + length + morey;
		vbo[k + 22] = z1;
		vbo[k + 23] = 1.0f;
		k += 24;
	}
	if (convex) {
		v1 = verts.at(0);
		x1 = v1.x;
		y1 = v1.y;
		z1 = v1.z;
		for (int i = 1; i < numVerts - 1; i++) {
			for (int j = k; j < k + 12; j+=4) {
				nbo[j] = 0.0f;
				nbo[j + 1] = -1.0f;
				nbo[j + 2] = 0.0f;
				nbo[j + 3] = 0.0f;
			}
			v2 = verts.at(i);
			v3 = verts.at(i + 1);
			x2 = v2.x;
			y2 = v2.y;
			z2 = v2.z;
			vbo[k] = x1;
			vbo[k + 1] = y1 + morey;
			vbo[k + 2] = z1;
			vbo[k + 3] = 1.0f;
			vbo[k + 4] = x2;
			vbo[k + 5] = y2 + morey;
			vbo[k + 6] = z2;
			vbo[k + 7] = 1.0f;
			vbo[k + 8] = v3.x;
			vbo[k + 9] = v3.y + morey;
			vbo[k + 10] = v3.z;
			vbo[k + 11] = 1.0f;	
			k += 12;
		}
		for (int i = 1; i < numVerts - 1; i++) {
			v2 = verts.at(i);
			v3 = verts.at(i + 1);
			x2 = v2.x;
			y2 = v2.y;
			z2 = v2.z;
			for (int j = k; j < k + 12; j+=4) {
				nbo[j] = 0.0f;
				nbo[j + 1] = 1.0f;
				nbo[j + 2] = 0.0f;
				nbo[j + 3] = 0.0f;
			}
			vbo[k] = x1;
			vbo[k + 1] = y1 + length + morey;
			vbo[k + 2] = z1;
			vbo[k + 3] = 1.0f;
			vbo[k + 4] = x2;
			vbo[k + 5] = y2 + length + morey;
			vbo[k + 6] = z2;
			vbo[k + 7] = 1.0f;
			vbo[k + 8] = v3.x;
			vbo[k + 9] = v3.y + length + morey;
			vbo[k + 10] = v3.z;
			vbo[k + 11] = 1.0f;
			k += 12;
		}
	}
}

bool Mesh::isConvex(int numVerts, std::vector<glm::vec3> verts) {
	float mag = 0;
	bool positive;
	for (int i = 0; i < numVerts - 1; i++) {
		mag = glm::cross(verts.at(i), verts.at(i + 1)).z;
		if (mag > 0) {
			positive = true;
			break;
		} else if  (mag < 0) {
			positive = false;
			break;
		}
	}
	for (int i = 0; i < numVerts - 1; i++) {
		mag = glm::cross(verts.at(i), verts.at(i + 1)).y;
		if ((mag > 0) != positive) {
			return false;
		}
	}
	return true;
}

Mesh::Mesh(int m, int numSlices, int numVerts, std::vector<glm::vec4> verts) : Primitive(0, 100) {
	//Surfrev
	mtl = new int(m);
	if (numSlices < 3) {
		std::cerr << "Invalid number of slices" << std::endl;
		exit(1);
	}
	glm::vec4 maxvert = verts.at(0);
	glm::vec4 minvert = verts.at(0);
	float minx, maxx, miny, maxy, minz, maxz;
	for (int i = 1; i < numVerts; i++) {
		if (verts.at(i).y > maxvert.y) {
			maxvert = verts.at(i);
		}
		if (verts.at(i).y < minvert.y) {
			minvert = verts.at(i);
		}
		if (verts.at(i).x < minx) {
			minx = verts.at(i).x;
		}
		if (verts.at(i).y < miny) {
			miny = verts.at(i).y;
		}
		if (verts.at(i).z < minz) {
			minz = verts.at(i).z;
		}
		if (verts.at(i).x > maxx) {
			maxx = verts.at(i).x;
		}
		if (verts.at(i).y > maxy) {
			maxy = verts.at(i).y;
		}
		if (verts.at(i).z > maxz) {
			maxz = verts.at(i).z;
		}
	}
	mybound = new glm::mat4(glm::translate(glm::mat4(), glm::vec3(0.0f, (maxy - miny) * 0.5f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3((maxx - minx) * 2.0f, maxy - miny, (maxx - minx) * 2.0f)));
	*height = (maxvert.y - minvert.y);
	float morey = 0.0f;
	if (minvert.y < 0) {
		morey += (-1 * minvert.y);
	}
	float incr = 360.0f/numSlices;
	bool topcap = false;
	//bool bottomcap = false;
	int add = 0;
	if (verts.at(numVerts - 1).x != 0) {
		topcap = true;
		//add += ((numSlices - 2) * 36);
	}
	if (verts.at(0).x != 0) {
		topcap = true;
		//bottomcap = true;
		//add += ((numSlices - 2) * 36);
	}
	if (topcap) {
		numVerts++;
		verts.push_back(verts.at(0));
	}
	numVbo = new int(((numVerts - 1) * (numSlices + 1) * 24) + add);
	numCbo = new int((3 * *numVbo)/4);
	numIbo = new int(*numVbo/4);
	vbo = new float[*numVbo];
	ibo = new short int[*numIbo];
	cbo = new float[*numCbo];
	nbo = new float[*numVbo];
	for (int c = 0; c < *numCbo; c+=3) {
		cbo[c] = 0.7f;
		cbo[c + 1] = 0.0f;
		cbo[c + 2] = 0.7f;
	}
	for (int in = 0; in < *numIbo; in++) {
		ibo[in] = in;
	}

	glm::vec4 vec1;
	glm::vec4 vec2;
	glm::vec4 vec3;
	glm::vec4 vec4;
	glm::vec3 vec31;
	glm::vec3 vec32;
	glm::vec3 vec33;
	glm::mat4 rot;
	glm::vec3 norm;
	glm::mat4 rot2 = glm::rotate(glm::mat4(), incr, glm::vec3(0, 1, 0));
	int k = 0;
	int kx;
	for (float theta = -180.0f; theta <= 180.0f; theta += incr) {
		rot = glm::rotate(glm::mat4(), theta, glm::vec3(0, 1, 0));
		for (int i = 0; i < numVerts - 1; i++) {
			vec1 = rot * verts.at(i);
			vec2 = rot2 * vec1;
			vec3 = rot * verts.at(i + 1);
			vec4 = rot2 * vec3;
			vec31 = glm::vec3(vec1.x, vec1.y, vec1.z);
			vec32 = glm::vec3(vec2.x, vec2.y, vec2.z);
			vec33 = glm::vec3(vec3.x, vec3.y, vec3.z);
			norm = glm::normalize(glm::cross(vec31 - vec32, vec32 - vec33));
			for (int j = k; j < k + 24; j+=4) {
				nbo[j] = norm.x;
				nbo[j + 1] = norm.y;
				nbo[j + 2] = norm.z;
				nbo[j + 3] = 0.0f;
			}
			vbo[k] = vec1.x;
			vbo[k + 1] = vec1.y + morey;
			vbo[k + 2] = vec1.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec2.x;
			vbo[k + 1] = vec2.y + morey;
			vbo[k + 2] = vec2.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec3.x;
			vbo[k + 1] = vec3.y + morey;
			vbo[k + 2] = vec3.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec4.x;
			vbo[k + 1] = vec4.y + morey;
			vbo[k + 2] = vec4.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec2.x;
			vbo[k + 1] = vec2.y + morey;
			vbo[k + 2] = vec2.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec3.x;
			vbo[k + 1] = vec3.y + morey;
			vbo[k + 2] = vec3.z;
			vbo[k + 3] = 1.0f;
			k += 4;
		}
	}
	/*
	if (topcap) {
		vec1 = verts.at(numVerts - 1);
		for (float theta = -180.0f; theta <= 180.0f; theta += incr) {
			rot = glm::rotate(glm::mat4(), theta, glm::vec3(0, 1, 0));
			vec2 = rot * vec1;
			vec3 = rot2 * vec2;
			vec31 = glm::vec3(vec1.x, vec1.y, vec1.z);
			vec32 = glm::vec3(vec2.x, vec2.y, vec2.z);
			vec33 = glm::vec3(vec3.x, vec3.y, vec3.z);
			norm = glm::normalize(glm::cross(vec31 - vec32, vec32 - vec33));
			for (int n = k; n < k + 12; n+=4) {
				nbo[n] = norm.x;
				nbo[n + 1] = norm.y;
				nbo[n + 2] = norm.z;
				nbo[n + 3] = 0.0f;
			}
			vbo[k] = vec1.x;
			vbo[k + 1] = vec1.y + morey;
			vbo[k + 2] = vec1.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec2.x;
			vbo[k + 1] = vec2.y + morey;
			vbo[k + 2] = vec2.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec3.x;
			vbo[k + 1] = vec3.y + morey;
			vbo[k + 2] = vec3.z;
			vbo[k + 3] = 1.0f;
			k += 4;
		}
	}
	if (bottomcap) {
		vec1 = verts.at(0);
		for (float theta = -180.0f; theta <= 180.0f; theta += incr) {
			rot = glm::rotate(glm::mat4(), theta, glm::vec3(0, 1, 0));
			vec2 = rot * vec1;
			vec3 = rot2 * vec2;
			vec31 = glm::vec3(vec1.x, vec1.y, vec1.z);
			vec32 = glm::vec3(vec2.x, vec2.y, vec2.z);
			vec33 = glm::vec3(vec3.x, vec3.y, vec3.z);
			norm = glm::normalize(glm::cross(vec31 - vec32, vec32 - vec33));
			for (int n = k; n < k + 12; n+=4) {
				nbo[n] = norm.x;
				nbo[n + 1] = norm.y;
				nbo[n + 2] = norm.z;
				nbo[n + 3] = 0.0f;
			}
			vbo[k] = vec1.x;
			vbo[k + 1] = vec1.y + morey;
			vbo[k + 2] = vec1.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec2.x;
			vbo[k + 1] = vec2.y + morey;
			vbo[k + 2] = vec2.z;
			vbo[k + 3] = 1.0f;
			k += 4;
			vbo[k] = vec3.x;
			vbo[k + 1] = vec3.y + morey;
			vbo[k + 2] = vec3.z;
			vbo[k + 3] = 1.0f;
			k += 4;
		}
	}
	*/
}