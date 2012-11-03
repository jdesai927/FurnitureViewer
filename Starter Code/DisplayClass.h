// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#ifndef _DISPLAYCLASS_H_
#define _DISPLAYCLASS_H_

// c++ libraries
#include <fstream>
#include <iostream>
// glew
#include "glew.h"
// QT class (gl.h must be included AFTER glew.h, noticed that gl.h and glu.h is automatically inculded in QGLWidget)
#include <QGLWidget>
// glm
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include "prism.h"
#include "cylinder.h"
#include "table.h"
#include "chair.h"
#include "cabinet.h"
#include "lamp.h"
#include "primitive.h"
#include "scenegraph.h"

// DON'T using namespace glm;
// notice that using glm will cause some ambiguous symbols. For instance, uint is both defined under glm and qt.

class DisplayClass
{
public: // public members
	Camera *camera;
	//Scene graph
	SceneGraph* graph;
	// for rotating the 2 cubes;
	float rotation;

public: // public methods
	DisplayClass(void);
	~DisplayClass(void);
	Prism* prism;
	Prism* floor;
	Sphere* sphere;
	Cylinder* cylinder;
	glm::vec3* lightPos;
	Node* selectedNode;

	void resizeWindow(int w, int h);
	void redraw();
	void drawNode(Node* n);
	void drawPrimitive(bool,Primitive*,glm::mat4);
	void drawFurniture(bool,Furniture*,glm::mat4);
	void createRedSquare(glm::mat4 modelMatrix);
	void createBlueSquare(glm::mat4 modelMatrix);
	void createGreenPrism(glm::mat4 modelMatrix);

private: // private members
	//vertex arrays needed for drawing
	unsigned int vbo;
	unsigned int cbo;
	unsigned int nbo;
	unsigned int ibo;

	//attributes
	unsigned int positionLocation;
	unsigned int colorLocation;
	unsigned int normalLocation;

	//uniforms
	unsigned int u_modelMatrixLocation;
	unsigned int u_projMatrixLocation;
	unsigned int u_lightPosLocation;

	//needed to compile and link and use the shaders
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

private: // private methods
	//helper function to read shader source and put it in a char array
	//thanks to Swiftless
	char* textFileRead(const char*);

	//some other helper functions from CIS 565
	void printLinkInfoLog(int);
	void printShaderInfoLog(int);
};

#endif
