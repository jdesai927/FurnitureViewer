// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#include "DisplayClass.h"
#include <math.h>

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
	prism = new Prism(0, 0.6, 0.6, 0.6, glm::vec4(-0.3f, -0.3f, 0.3f, 1.0f));
	sphere = new Sphere();
	cylinder = new Cylinder();
	lightPos = new glm::vec3(0.0f, 3.0f, 5.0f);
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
		drawPrimitive(b, n->shape, *n->translate * *n->rotate * *n->scale);
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