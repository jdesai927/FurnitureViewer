#include "stubs.h"

double Test_RaySphereIntersect(glm::vec3 const& P0, glm::vec3 const& V0, glm::mat4 const& T) {
	// TODO fill this in so it calls your own ray-casting function.
	// See the documentation of this function in stubs.h.

	glm::vec3 V00 = glm::normalize(V0);
	glm::vec4 p0 = glm::vec4(P0.x, P0.y, P0.z, 1.0f);
	glm::vec4 v0 = glm::vec4(V00.x, V00.y, V00.z, 0.0f);
	p0 = T * p0;
	v0 = T * v0;

	//double a = 1.0;
	double b = 2 * ( (p0.x * v0.x) + (p0.y * v0.y) + (p0.z * v0.z) );
	double c = (p0.x * p0.x) + (p0.y * p0.y) + (p0.z * p0.z) - 1.0;

	double disc = (b * b) - (4 * c);

	if (disc < 0) {
		return -1;
	}

	double distSqrt = sqrtf(disc);
	double t0 = ((-1.0 * b) - distSqrt)/2.0;
	double t1 = ((-1.0 * b) + distSqrt)/2.0;

	/*
	double q;
	if (b < 0) {
		q = ((-1.0 * b) + distSqrt)/2.0;
	} else {
		q = ((-1.0 * b) - distSqrt)/2.0;
	}

	double t0 = q;
	double t1 = c/q;
	*/
	double temp;
	if (t0 > t1) {
		temp = t1;
		t1 = t0;
		t0 = temp;
	}
	if (t1 < 0) {
		return -1;
	}
	if (t0 < 0) {
		return t1;
	}
	return t0;
}

double Test_RayPolyIntersect(glm::vec3 const& P0, glm::vec3 const& V0, glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::mat4 const& T) {
	// TODO fill this in so it calls your own ray-casting function.
	// See the documentation of this function in stubs.h.

	glm::vec3 V00 = glm::normalize(V0);
	glm::vec4 fv0 = glm::vec4(V00.x, V00.y, V00.z, 0.0f);
	glm::vec4 fp0 = glm::vec4(P0.x, P0.y, P0.z, 1.0f);
	fp0 = T * fp0;
	fv0 = T * fv0;
	glm::vec3 p = glm::vec3(fp0.x, fp0.y, fp0.z);
	glm::vec3 d = glm::vec3(fv0.x, fv0.y, fv0.z);
	glm::vec3 v0 = p1;
	glm::vec3 v1 = p2;
	glm::vec3 v2 = p3;
	glm::vec3 e1 = v1 - v0;
	glm::vec3 e2 = v2 - v0;

	glm::vec3 h = glm::cross(d, e2);
	float a = glm::dot(e1, h);

	if (a > -0.001 && a < 0.001) {
		return -1;
	}

	double f = 1.0/a;
	glm::vec3 s = p - v0;
	float u = f * (glm::dot(s, h));
	if (u < 0.0 || u > 1.0) {
		return -1;
	}

	glm::vec3 q = glm::cross(s, e1);
	float v = f * (glm::dot(d, q));

	if (v < 0.0 || u + v > 1.0) {
		return -1;
	}

	double t = f * glm::dot(e2, q);

	if (t > 0.001) {
		return t;
	}
	return -1;
}

double Test_RayCubeIntersect(glm::vec3 const& P0, glm::vec3 const& V0, glm::mat4 const& T) {
	// TODO fill this in so it calls your own ray-casting function.
	// See the documentation of this function in stubs.h.

	glm::vec3 V00 = glm::normalize(V0);
	glm::vec4 fp0 = glm::vec4(P0.x, P0.y, P0.z, 1.0f);
	glm::vec4 fv0 = glm::vec4(V00.x, V00.y, V00.z, 0.0f);
	fp0 = T * fp0;
	fv0 = T * fv0;
	glm::vec3 p = glm::vec3(fp0.x, fp0.y, fp0.z);
	glm::vec3 d = glm::vec3(fv0.x, fv0.y, fv0.z);

	double tnear = -999999;
	double tfar = 999999;
	double t1;
	double t2;
	double temp;
	for (int i = 0; i < 3; i++) {
		if (d[i] < 0.001 && d[i] > -0.001) {
			if (p[i] > 0.5 || p[i] < -0.5) {
				return -1;
			}
		}
		t1 = (-0.5 - p[i])/d[i];
		t2 = (0.5 - p[i])/d[i];
		if (t1 > t2) {
			temp = t1;
			t1 = t2;
			t2 = temp;
		}
		if (t1 > tnear) {
			tnear = t1;
		}
		if (t2 < tfar) {
			tfar = t2;
		}
		if (tnear > tfar) {
			return -1;
		}
		if (tfar < 0) {
			return -1;
		}
	}

	if (tnear < 0) {
		return tfar;
	}

	return tnear;
}

double CylinderWallIntersect (glm::vec3 p, glm::vec3 v) {
	double a = (v.x* v.x) + (v.z * v.z);
	double b = (2 * p.x * v.x) + (2 * p.z * v.z);
	double c = (p.x * p.x) + (p.z * p.z) - 0.25;

	double disc = (b * b) - (4 * a * c);

	if (disc < 0) {
		return -1;
	}

	if (a < 0.001 && a > -0.001) {
		double t = ((-1.0 * c)/b);
		glm::vec3 intrs = p + (((float) t) * v);
		if (intrs.y < 3.0 && intrs.y > 0.0) {
			return t;
		}
		return -1;
	}

	double t0 = ((-1.0 * b) - sqrtf(disc))/(2 * a);
	double t1 = ((-1.0 * b) + sqrtf(disc))/(2 * a);

	if (t0 > 0.0001) {
		glm::vec3 intrs = p + (((float) t0) * v);
		if (intrs.y < 3.0 && intrs.y > 0.0) {
			return t0;
		}
		return -1;
	}
	if (t1 > 0.0001) {
		glm::vec3 intrs = p + (((float) t1) * v);
		if (intrs.y < 3.0 && intrs.y > 0.0) {
			return t1;
		}
		return -1;
	}
	return -1;
}

double CylinderCapIntersect(glm::vec3 p, glm::vec3 v, bool top) {
	int i = 1;
	if (!top) {
		i = -1;
	}
	glm::vec3 p1(0.0f, 0.5 * i, 0.0f);
	glm::vec3 norm(0.0f, 1.0f, 0.0f);
	double denom = glm::dot(norm, v);
	if (denom > -0.001 && denom < 0.001) {
		return -1;
	}
	double t = glm::dot(norm, (p1 - p))/denom;
	if (t < 0) {
		return -1;
	}
	glm::vec3 intrs = p + (((float) t) * v);
	double pt = intrs.x * intrs.x + intrs.z * intrs.z;
	if (pt < 0.25001 && pt > -0.25001) {
		return t;
	}
	return -1;
}

double Test_RayCylinderIntersect(glm::vec3 const& P0, glm::vec3 const& V0, glm::mat4 const& T) {
	// TODO fill this in so it calls your own ray-casting function.
	// See the documentation of this function in stubs.h.
	glm::vec3 V00 = glm::normalize(V0);
	glm::vec4 fp0 = glm::vec4(P0.x, P0.y, P0.z, 1.0f);
	glm::vec4 fv0 = glm::vec4(V00.x, V00.y, V00.z, 0.0f);
	fp0 = T * fp0;
	fv0 = T * fv0;
	glm::vec3 p(fp0.x, fp0.y, fp0.z);
	glm::vec3 v(fv0.x, fv0.y, fv0.z);
	double tw = CylinderWallIntersect(p, v);
	double tt = CylinderCapIntersect(p, v, true);
	double tb = CylinderCapIntersect(p, v, false);
	double t = tw;
	if (tw == -1) {
		if (tt == -1) {
			if (tb == -1) {
				return -1;
			} else {
				t = tb;
			}
		} else {
			t = tt;
		}
	}
	if (tt != -1) {
		t = glm::min(t, tt);
	}
	if (tb != -1) {
		t = glm::min(t, tb);
	}
	return t;
}