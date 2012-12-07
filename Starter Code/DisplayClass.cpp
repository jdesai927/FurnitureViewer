// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#include "DisplayClass.h"
#include <math.h>
#include "stubs.h"

#define M_PI 3.1415926

// private methods
#pragma region private methods
// glsl utilities
// from swiftless.com
char* DisplayClass::textFileRead(const char* fileName) {
	char* text;

	if (fileName != NULL) {
		FILE *file = fopen(fileName, "rt");

		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';	//cap off the string with a terminal symbol, fixed by Cory
			}
			fclose(file);
		}
	}
	return text;
}

void DisplayClass::printLinkInfoLog(int prog) 
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetProgramInfoLog(prog,infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog:" << std::endl << infoLog << std::endl;
		delete [] infoLog;
	}
}

void DisplayClass::printShaderInfoLog(int shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog:" << std::endl << infoLog << std::endl;
		delete [] infoLog;
	}

	// should additionally check for OpenGL errors here
}
#pragma endregion

// constructors and destructors
#pragma region constructors and destructors
DisplayClass::DisplayClass(void)
{
	prism = new Prism(0, 1.0f, 1.0f, 1.0f, glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	sphere = new Sphere();
	cylinder = new Cylinder();
	lightPos = new glm::vec3(10.0f, 6.0f, 0.0f);
	lightCol = new glm::vec3(1.0f, 1.0f, 1.0f);
	ambientCol = new glm::vec3(1.0f, 1.0f, 1.0f);
	this->camera = new Camera();
	rotation = 0.0f;

	//Call GLEW only _after_ you get the window
	//I had to tell the author of your textbook that ;-)  -Cory
	glewInit();

	//Create the VBOs and IBO we'll be using to render images in OpenGL
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &ibo);

	// assign locations
	positionLocation = 0;
	colorLocation = 1;
	normalLocation = 2;

	//Everybody does this
	glClearColor(0.6, 0.6, 0.6, 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);

	//here is stuff for setting up our shaders
	const char* fragFile = "diffuseFrag.frag";
	const char* vertFile = "diffuseVert.vert";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();

	//load up the source, compile and link the shader program
	const char* vertSource = textFileRead(vertFile);
	const char* fragSource = textFileRead(fragFile);
	glShaderSource(vertexShader, 1, &vertSource, 0);
	glShaderSource(fragmentShader, 1, &fragSource, 0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//For your convenience, i decided to throw in some compiler/linker output helper functions
	//from CIS 565
	GLint compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(vertexShader);
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(fragmentShader);
	} 

	//set the attribute locations for our shaders
	glBindAttribLocation(shaderProgram, positionLocation, "vs_position");
	glBindAttribLocation(shaderProgram, normalLocation, "vs_normal");
	glBindAttribLocation(shaderProgram, colorLocation, "vs_color");

	//finish shader setup
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for linking success
	GLint linked;
	glGetProgramiv(shaderProgram,GL_LINK_STATUS, &linked);
	if (!linked) 
	{
		printLinkInfoLog(shaderProgram);
	}

	//Get the uniform locations for our shaders, unfortunately they can not be set by us, we have
	//to ask OpenGL for them
	u_modelMatrixLocation = glGetUniformLocation(shaderProgram, "u_modelMatrix");
	u_projMatrixLocation = glGetUniformLocation(shaderProgram, "u_projMatrix");
	u_lightPosLocation = glGetUniformLocation(shaderProgram, "u_lightPos");
	u_lightColLocation = glGetUniformLocation(shaderProgram, "u_lightCol");
	u_cameraPosLocation = glGetUniformLocation(shaderProgram, "u_cameraPos");
	u_ambientColLocation = glGetUniformLocation(shaderProgram, "u_ambientCol");

	//Always remember that it doesn't do much good if you don't have OpenGL actually use the shaders
	glUseProgram(shaderProgram);

	camera->resizeWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

DisplayClass::~DisplayClass(void)
{
	delete this->camera;
	delete this->prism;

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cbo);
	glDeleteBuffers(1, &nbo);
	glDeleteBuffers(1, &ibo);

	//Tear down the shader program in reverse of building it
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}
#pragma endregion

void DisplayClass::resizeWindow(int w, int h)
{
	// everytime when the window is resized, the camera matrix will be changed
	camera->resizeWindow(w, h);
	// LOOKAT: go into getCameraMatrix to see what camera matrix is. (projection * view)
	glm::mat4 cameraMatrix = camera->getCameraMatrix();
	//cameraMatrix = glm::rotate(cameraMatrix, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &cameraMatrix[0][0]);
}

void drawSphere () {

}

void DisplayClass::redraw()
{ 
	//Draw the two components of our scene separately, for your scenegraphs it will help
	//your sanity to do separate draws for each type of primitive geometry, otherwise your VBOs will
	//get very, very complicated fast

	// create an identity matrix for the model matrix
	// LOOKAT: model matrix is the transformation matrix for objects THEMSELVES
	glm::mat4 modelMatrix = glm::mat4(1.0);

	if (graph != NULL && graph->rootNode != NULL) {
		drawNode(graph->rootNode);
	}

	// reset modelMatrix and rotate the blue box clockwise then translate it.
	// LOOKAT: see the order of translation and rotation?
	//modelMatrix = glm::mat4(1.0);
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.0f, 2.0f, -0.1f));
	//modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(0, 0, 1));
	//createBlueSquare(modelMatrix);
}

void DisplayClass::drawNode(Node* n) {
	bool b = false;
	if (selectedNode == n) {
		b = true;
	}
	if (n->shape != NULL) {
		glm::mat4 id;
		if (*n->shape->kind > 98) {
			id = id;// * glm::scale(glm::mat4(), glm::vec3(0.1, 0.1, 0.1));
		}
		drawPrimitive(b, n->shape, *n->translate * *n->rotate * *n->scale * id);
	}
	if (n->furniture != NULL) {
		drawFurniture(b, n->furniture, *n->translate * *n->rotate * *n->scale);
	}
	for (int i = 0; i < n->children->size(); i++) {
		drawNode(n->children->at(i));
	}
}

void DisplayClass::drawFurniture(bool b, Furniture* f, glm::mat4 modelMatrix) {
	for (int i = 0; i < f->primitives->size(); i++) {
		switch (*f->primitives->at(i)) {
		case 0:
			drawPrimitive(b, prism, modelMatrix * *f->localTransforms->at(i));
			break;
		case 1:
			drawPrimitive(b, cylinder, modelMatrix * *f->localTransforms->at(i));
			break;
		case 2:
			drawPrimitive(b, sphere, modelMatrix * *f->localTransforms->at(i));
			break;
		}
	}
}

void DisplayClass::drawPrimitive(bool b, Primitive* p, glm::mat4 modelMatrix) {
	//now we put the data into the Vertex Buffer Object for the graphics system to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, *p->numVbo * sizeof(float), p->vbo, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
	//while the program run
	//again with colors

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
	float* mycbo;
	if (b) {
		mycbo = new float[*p->numCbo];
		for (int i = 0; i < *p->numCbo; i++) {
			mycbo[i] = 1.0f;
		}
	} else {
		mycbo = p->cbo;
	}

	glBufferData(GL_ARRAY_BUFFER, *p->numCbo * sizeof(float), mycbo, GL_STREAM_DRAW);	//the color is going to change every frame
	//as it bounces between squares

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, *p->numVbo * sizeof(float), p->nbo, GL_STATIC_DRAW); //the square normals don't need to change, ever,

	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);

	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//now use color data, remember we're not using 4 at a time anymore
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, *p->numIbo * sizeof(unsigned short), p->ibo, GL_STATIC_DRAW);

	//set the modelview uniform
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	//set light position uniform
	glUniform3f(u_lightPosLocation, lightPos->x, lightPos->y, lightPos->z);

	//set light color uniform
	glUniform3f(u_lightColLocation, lightCol->x, lightCol->y, lightCol->z);

	//set camera position uniform
	glUniform3f(u_cameraPosLocation, camera->eye.x, camera->eye.y, camera->eye.z);

	glUniform3f(u_ambientColLocation, ambientCol->x, ambientCol->y, ambientCol->z);

	//draw the elements
	if (*p->kind == 2 || *p->kind == 1) {
		glDrawElements(GL_QUAD_STRIP, *p->numIbo, GL_UNSIGNED_SHORT, 0);
	} else if (*p->kind == 1) {
		glDrawElements(GL_TRIANGLE_STRIP, *p->numIbo, GL_UNSIGNED_SHORT, 0);
		Cylinder* c = static_cast<Cylinder*>(p);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, *c->numInds * sizeof(unsigned short), c->ibotop, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLE_FAN, *c->numInds, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, *c->numInds * sizeof(unsigned short), c->ibobottom, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLE_FAN, *c->numInds, GL_UNSIGNED_SHORT, 0);
	} else {
		glDrawElements(GL_TRIANGLES, *p->numIbo, GL_UNSIGNED_SHORT, 0);
	}
	//shut off the information since we're done drawingi
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}

void DisplayClass::createGreenPrism(glm::mat4 modelMatrix) {

}

glm::vec3 DisplayClass::mapPoint(float px, float py, glm::vec3 M, glm::vec3 H, glm::vec3 V) {
	float sx = px/(*resoX - 1);
	float sy = py/(*resoY - 1);
	return (M + ((2 * sx) - 1) * H + ((2 * sy) - 1) * V);
}

Node* DisplayClass::recIntersection(glm::vec3 P, glm::vec3 V, Node* n) {
	*n->t = -1.0;
	Node* c = NULL;
	double res;
	double cT;
	//Node* ch = NULL;
	if (n->furniture != NULL) {
		for (int i = 0; i < n->furniture->primitives->size(); i++) {
			switch(*n->furniture->primitives->at(i)) {
			case 0:
				res = Test_RayCubeIntersect(P, V, *n->furniture->inverses->at(i));
				if (res != -1) {
					if (c == NULL || res < *c->t) {
						c = n;
						*c->t = res;
						glm::vec4 p4(P.x, P.y, P.z, 1.0f);
						glm::vec4 v4(V.x, V.y, V.z, 0.0f);
						p4 = *n->furniture->inverses->at(i) * p4;
						v4 = *n->furniture->inverses->at(i) * v4;
						glm::vec4 inters = p4 + ((float)res) * v4;
						glm::vec4 myNorm;
						bool forward = inters.z > 0.4999 && inters.z < 0.5001;
						bool backward = inters.z < -0.4999 && inters.z > -0.5001;
						bool right = inters.x > 0.4999 && inters.x < 0.5001;
						bool left = inters.x < -0.4999 && inters.x > -0.5001;
						bool up = inters.y > 0.4999 && inters.y < 0.5001;
						bool down = inters.y < -0.4999 && inters.y > -0.5001;
						if (forward) {
							myNorm.x = 0.0f;
							myNorm.y = 0.0f;
							myNorm.z = 1.0f;
						} else if (backward) {
							myNorm.x = 0.0f;
							myNorm.y = 0.0f;
							myNorm.z = -1.0f;
						} else if (right) {
							myNorm.x = 1.0f;
							myNorm.y = 0.0f;
							myNorm.z = 0.0f;
						} else if (left) {
							myNorm.x = -1.0f;
							myNorm.y = 0.0f;
							myNorm.z = 0.0f;
						} else if (up) {
							myNorm.x = 0.0f;
							myNorm.y = 1.0f;
							myNorm.z = 0.0f;
						} else if (down) {
							myNorm.x = 0.0f;
							myNorm.y = -1.0f;
							myNorm.z = 0.0f;
						}
						myNorm[3] = 0.0f;
						myNorm = glm::normalize(glm::transpose(*n->furniture->inverses->at(i)) * myNorm);
						*c->currentWorldTransform = *n->furniture->worldTransforms->at(i);
						c->normal->x = myNorm.x;
						c->normal->y = myNorm.y;
						c->normal->z = myNorm.z;
					}
				}
				break;
			case 1:
				res = Test_RayCylinderIntersect(P, V, *n->furniture->inverses->at(i));
				if (res != -1) {
					if (c == NULL || res < *c->t) {
						c = n;
						*c->t = res;
						glm::vec4 p4(P.x, P.y, P.z, 1.0f);
						glm::vec4 v4(V.x, V.y, V.z, 0.0f);
						p4 = *n->furniture->inverses->at(i) * p4;
						v4 = *n->furniture->inverses->at(i) * v4;
						glm::vec4 inters = p4 + ((float)res) * v4;
						glm::vec4 myNorm(2.0f * inters.x, 0.0f, 2.0f * inters.z, 0.0f);
						myNorm = glm::normalize(glm::transpose(*n->furniture->inverses->at(i)) * myNorm);
						*c->currentWorldTransform = *n->furniture->worldTransforms->at(i);
						c->normal->x = myNorm.x;
						c->normal->y = myNorm.y;
						c->normal->z = myNorm.z;
					}
				}
				break;
			case 2:
				res = Test_RaySphereIntersect(P, V, *n->furniture->inverses->at(i));
				if (res != -1) {
					if (c == NULL || res < *c->t) {
						c = n;
						*c->t = res;
						glm::vec4 p4(P.x, P.y, P.z, 1.0f);
						glm::vec4 v4(V.x, V.y, V.z, 0.0f);
						p4 = *n->furniture->inverses->at(i) * p4;
						v4 = *n->furniture->inverses->at(i) * v4;
						glm::vec4 inters = p4 + ((float)res) * v4;
						glm::vec4 myNorm(inters.x * 2.0f, inters.y * 2.0f, inters.z * 2.0f, 0.0f);
						myNorm = glm::normalize(glm::transpose(*n->furniture->inverses->at(i)) * myNorm);
						*c->currentWorldTransform = *n->furniture->worldTransforms->at(i);
						c->normal->x = myNorm.x;
						c->normal->y = myNorm.y;
						c->normal->z = myNorm.z;
					}
				} /*else {
				  glm::vec3 x;
				  }*/
				break;
			}
		}
		//do intersection tests, get intersection with minimum t-value
	} else if (n->shape != NULL) {
		if (*n->shape->kind < 97 || Test_RayCubeIntersect(P, V, *n->boundTrans) != -1) {
			glm::vec3 p1;
			glm::vec3 p2;
			glm::vec3 p3;
			glm::vec4 norm(0.0f, 0.0f, 0.0f, 0.0f);
			double minT = -1;
			for (int q = 0; q < *n->shape->numVbo; q+=12) {
				p1.x = n->shape->vbo[q];
				p1.y = n->shape->vbo[q + 1];
				p1.z = n->shape->vbo[q + 2];
				p2.x = n->shape->vbo[q + 4];
				p2.y = n->shape->vbo[q + 5];
				p2.z = n->shape->vbo[q + 6];
				p3.x = n->shape->vbo[q + 8];
				p3.y = n->shape->vbo[q + 9];
				p3.z = n->shape->vbo[q + 10];
				res = Test_RayPolyIntersect(P, V, p1, p2, p3, *n->inv);
				if (res != -1 && (minT > res || minT == -1)) {
					minT = res;
					norm.x = n->shape->nbo[q];
					norm.y = n->shape->nbo[q + 1];
					norm.z = n->shape->nbo[q + 2];
				}
			}
			if (minT != -1) {
				if (c == NULL || *c->t == -1 || res < *c->t) {
					c = n;
					*c->t = minT;
					norm = glm::normalize(glm::transpose(*n->inv) * norm);
					*c->currentWorldTransform = *n->worldTransform;
					c->normal->x = norm.x;
					c->normal->y = norm.y;
					c->normal->z = norm.z;
				}
			}
		}
		//loop through triangles and find intersection with minimum t-value
	}
	Node* ch = NULL;
	for (int i = 0; i < n->children->size(); i++) {
		ch = recIntersection(P, V, n->children->at(i));
		if (ch != NULL) {
			if (c == NULL || *c->t == -1 || *ch->t < *c->t) {
				c = ch;
			}
		}
	}
	return c;
}

Node* DisplayClass::getIntersectionObject(glm::vec3 P, glm::vec3 V) {
	return recIntersection(P, V, graph->rootNode);
}

void DisplayClass::traceRay(glm::vec3* color, int depth, glm::vec3 P, glm::vec3 V, float currentRI, float oldRI, glm::vec3 lcol) {
	if (depth > 5) {
		color->x = rayAmbientCol->x;
		color->y = rayAmbientCol->y;
		color->z = rayAmbientCol->z;
		return;
	}
	Node* n = getIntersectionObject(P, V);
	if (n == NULL) {
		color->x = rayAmbientCol->x;
		color->y = rayAmbientCol->y;
		color->z = rayAmbientCol->z;
		return;
	}

	//find intersection
	glm::vec3 intersection = P + (((float) *n->t) * V);

	//get material
	float* mt;
	switch(*n->mtl) {
	case 1:
		mt = mtl1;
		break;
	case 2:
		mt = mtl2;
		break;
	case 3:
		mt = mtl3;
		break;
	case 4:
		mt = mtlf;
		break;
	}

	glm::vec3 N = glm::normalize(*n->normal); //compute normal at intersection point
	glm::vec3 Rd = glm::normalize(glm::reflect(V, N)); //compute reflected ray direction
	float RI = mt[6];
	glm::vec3 Rfn = N;

	//if reflective, make recursive call
	glm::vec3 spec(0.0f, 0.0f, 0.0f);
	if (mt[4] > 0.0f) {
		glm::vec3* reflectedColor = new glm::vec3();
		traceRay(reflectedColor, depth + 1, intersection + 0.001f * Rd, Rd, currentRI, oldRI, lcol);//glm::vec3(lcol.x * mt[0], lcol.y * mt[1], lcol.z * mt[2]));
		spec = *reflectedColor;
		delete reflectedColor;
	}

	glm::vec3 refr(0.0f, 0.0f, 0.0f);
	glm::vec3 Rf;
	glm::vec3 tcol;
	float insFl = -1.0f;
	if (mt[5] > 0.0f) {
		insFl = glm::dot(N, V);
		tcol = glm::vec3(lcol.x * mt[0], lcol.y * mt[1], lcol.z * mt[2]);
		if (insFl > -0.0001f) {
			Rfn = -1.0f * N;
			//tcol = lcol;
			//RI = 1.0f;
		}
		if (currentRI == 1.0f) {
			oldRI = RI;
		}
		float eta = currentRI/oldRI;
		Rf = glm::normalize(glm::refract(V, Rfn, eta));
		glm::vec3* refractedColor = new glm::vec3();
		traceRay(refractedColor, depth + 1, intersection + 0.001f * Rf, Rf, oldRI, currentRI, lcol);
		refr = *refractedColor;
		delete refractedColor;
	}

	//ambient color
	color->x = rayAmbientCol->x * mt[0];
	color->y = rayAmbientCol->y * mt[1];
	color->z = rayAmbientCol->z * mt[2];

	*color = *color * 0.2f;

	*color = glm::clamp(*color + refr, 0.0f, 1.0f);
	
	if (mt[5] > 0.0f) {
		*color = ((1.0f - mt[4]) * *color) + (mt[4] * spec);
		/*float cosT = glm::dot(Rf, V); //2.0f * glm::dot(-1.0f * Rfn, V);
		float rcoef = glm::clamp(pow(1.0f - cosT, 5), 0.0f, 1.0f);
		*color = glm::clamp(((1.0f - rcoef) * *color) + (rcoef * tcol), 0.0f, 1.0f);*/
		return;
	}
	
	//light ray
	glm::vec3 L;
	glm::vec3 R;
	
	for (int lc = 0; lc < 3; lc++) {
		L = glm::normalize(*rayLightPos->at(lc) - intersection);
		R = glm::reflect(-1.0f * L, N);

		//cast ray for shadow
		glm::vec3 blockob;
		Node* blocker = getIntersectionObject(*rayLightPos->at(lc), -1.0f * L);
		if (blocker != NULL) {
			blockob = *rayLightPos->at(lc) - (((float) *blocker->t) * L);
		}
		float di = glm::length(blockob - intersection);
		//if not shadowed, add more color
		if (blocker == NULL || (di < 0.00001f && di > -0.00001f)) {

			//diffuse color
			glm::vec3 diffuseColor = glm::clamp(glm::vec3(mt[0] * rayLightCol->at(lc)->x, mt[1] * rayLightCol->at(lc)->y, mt[2] * rayLightCol->at(lc)->z), 0.0f, 1.0f);

			//specular term
			float specTerm = glm::clamp(pow(glm::dot(V, R), mt[3]), 0.0f, 1.0f);

			//diffuse term
			float diffuseTerm = glm::clamp(glm::dot(N, L), 0.0f, 1.0f);

			//final color weighted calculation
			//blinn-phong
			*color = *color + (diffuseTerm * diffuseColor * 0.6f) + (glm::clamp(spec + specTerm * *rayLightCol->at(lc), 0.0f, 1.0f) * 0.2f);

		}
	}
	//reflectivity-weighted
	*color = ((1.0f - mt[4]) * *color) + (mt[4] * spec);

}

void DisplayClass::doRayTrace() {
	graph->rootNode->computeAllInverses();
	unsigned int width = static_cast<int>(*resoX);
	unsigned int height = static_cast<int>(*resoY);
	glm::vec3 A = glm::cross(rayCamera->center, rayCamera->up);
	glm::vec3 B = glm::cross(A, rayCamera->center);
	glm::vec3 M = rayCamera->center + rayCamera->eye;
	glm::vec3 V = (B * glm::length(rayCamera->center) * tan(glm::radians(rayCamera->fovy)))/ glm::length(B);
	//float fovx = atan(length(V) * (*WriteBMP::resoX/ *WriteBMP::resoY)/length(M));
	glm::vec3 H = (*resoX/ *resoY) * V;
	H.x = H.y;
	H.y = 0.0f;
	H.z = 0.0f;
	BMP output;
	output.SetSize(width, height);
	output.SetBitDepth(24);
	glm::vec3 P;
	glm::vec3 D;
	glm::vec3* E = new glm::vec3(rayCamera->eye.x, rayCamera->eye.y, rayCamera->eye.z);
	glm::vec3* color = new glm::vec3();
	std::cout << "Beginning raytrace" << std::endl;
	for(unsigned int x = 0; x < width; x++) {
		for(unsigned int y = 0; y < height; y++) {
			color = new glm::vec3(0.0f, 0.0f, 0.0f);
			E->x = rayCamera->eye.x;
			E->y = rayCamera->eye.y;
			E->z = rayCamera->eye.z;
			P = DisplayClass::mapPoint(x, height - y - 1, M, H, V);
			D = glm::normalize(P - *E);///glm::length(P - *E);
			traceRay(color, 0, *E, D, 1.0f, 1.0f, *rayLightCol->at(0));
			output(x, y)->Red = 255 * color->x;
			output(x, y)->Green = 255 * color->y;
			output(x, y)->Blue = 255 * color->z;
			delete color;
			color = 0;
		}

		std::cout << "finished vertical line: " << x << std::endl;
	}
	std::cout << "Finished raytrace!" << std::endl;
	output.WriteToFile(rayOutputFile->c_str());
}

void DisplayClass::createRedSquare(glm::mat4 modelMatrix)
{
	//these four points define where the quad would be BEFORE transformations
	//this is referred to as object-space and it's best to center geometry at the origin for easier transformations
	float* vertices = new float[16];
	vertices[0] = -4.0f; vertices[1] = 4.0f; vertices[2] = -2.0f; vertices[3] = 1.0f;
	vertices[4] = -4.0f; vertices[5] = -4.0f; vertices[6] = -2.0f; vertices[7] = 1.0f;
	vertices[8] = 4.0f; vertices[9] = -4.0f; vertices[10] = -2.0f; vertices[11] = 1.0f;
	vertices[12] = 4.0f; vertices[13] = 4.0f; vertices[14] = -2.0f; vertices[15] = 1.0f;

	//now we put the data into the Vertex Buffer Object for the graphics system to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
	//while the program runs

	//once the data is loaded, we can delete the float arrays, the data is safely stored with OpenGL
	//vertices is an array that we created under this scope and stored in the HEAP, so release it if we don't want to use it anymore. 
	delete [] vertices;

	//again with colors
	float* colors = new float[12];

	colors[0] = 1; colors[1] = 0; colors[2] = 0;
	colors[3] = 1; colors[4] = 0; colors[5] = 0;
	colors[6] = 1; colors[7] = 0; colors[8] = 0;
	colors[9] = 1; colors[10] = 0; colors[11] = 0;

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STREAM_DRAW);	//the color is going to change every frame
	//as it bounces between squares
	delete [] colors;

	//once more, this time with normals
	float* normals = new float[16];

	normals[0] = 0; normals[1] = 0; normals[2] = 1; normals[3] = 0;
	normals[4] = 0; normals[5] = 0; normals[6] = 1; normals[7] = 0;
	normals[8] = 0; normals[9] = 0; normals[10] = 1; normals[11] = 0;
	normals[12] = 0; normals[13] = 0; normals[14] = 1; normals[15] = 0;

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,
	//while the program runs
	delete [] normals;

	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);

	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//now use color data, remember we're not using 4 at a time anymore
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//the last thing we need to do is setup our indices
	unsigned short* indices = new unsigned short[6];

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 3; indices[4] = 0; indices[5] = 2;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	delete [] indices;

	//set the modelview uniform
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	//draw the elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}

void DisplayClass::createBlueSquare(glm::mat4 modelMatrix)
{
	//the only difference between a red square and a blue square is the color, so we can leave the other VBOs as they are
	float* colors = new float[12];

	colors[0] = 0; colors[1] = 0; colors[2] = 1;
	colors[3] = 0; colors[4] = 0; colors[5] = 1;
	colors[6] = 0; colors[7] = 0; colors[8] = 1;
	colors[9] = 0; colors[10] = 0; colors[11] = 1;

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STREAM_DRAW);

	// we don't need to send vertices and normals to card again, because the blue square share the SAME geometry with the red square.

	delete [] colors;

	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);

	//bind again
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//set the modelview matrix again since it changed
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	//draw again, even the indices from before are good
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}