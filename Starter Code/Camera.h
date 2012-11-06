// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "Define.h"

// LOOKAT: DO not try to write "using namespace glm" anywhere in a qt application,
// because it will cause the definition of uint ambigious

class Camera
{
private:
	glm::vec3 center;
	glm::vec3 up;
	float fovy;
	int height;
	int width;
	float zNear;
	float zFar;

public:
	// constructor and destructor
	Camera(void);
	~Camera(void);
	
	// get & set properties
	// for example:
	// vec3 getEyePos(void);
	// void setEyePos(vec3 pos);
	// get camera matrices:
	glm::vec3 eye;
	glm::mat4 getCameraMatrix();

	// resize
	void resizeWindow(int w, int h);

	// LOOKAT: Add your interactive camera methods here:
};

#endif

