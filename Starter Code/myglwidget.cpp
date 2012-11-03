// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#include "MyGLWidget.h"


# pragma region constructors and destructos
MyGLWidget::MyGLWidget(QWidget* parent) : QGLWidget(parent) {
	//This starter code uses a timer to make a square rotate without
	//user intervention.  You can remove this.
	timer = new QTimer(this);
	invertMode = new bool(false);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	timer->start(17);
	this->grabKeyboard();
}

std::istringstream MyGLWidget::getNumStream(std::string reader) {
	//reader = reader.substr(5, reader.size() - 1);
	return static_cast<std::istringstream>(reader);
}

glm::vec3 MyGLWidget::getVec(std::string reader) {
	glm::vec3 ret;
	std::istringstream myStream = getNumStream(reader);
	int i = 0;
	float temp;
	while (myStream >> temp) {
		ret[i] = temp;
		i++;
	}
	return ret;
}

MyGLWidget::~MyGLWidget() {
	this->releaseKeyboard();
	delete invertMode;
	if (displayClass)
		delete displayClass;
	delete timer;
}
# pragma endregion

extern char* filename;

#pragma region event handlers
// initialize opengl components event handler
void MyGLWidget::initializeGL() {
	displayClass = new DisplayClass();
	std::ifstream config;
	config.open(filename);
	std::string reader;
	getline(config, reader);
	glm::vec3 vec = getVec(reader);
	int xSize = vec[0];
	int zSize = vec[1];
	int numItems = vec[2];
	std::vector<std::vector<Node*>> grid;
	for (int fn = 0; fn < xSize; fn++) {
		std::vector<Node*> col;
		for (int fx = 0; fx < zSize; fx++) {
			col.push_back(NULL);
		}
		grid.push_back(col);
	}
	int i = 0;
	glm::vec4 origin(((float) xSize)/-2.0f, 0.0f, ((float) zSize)/-2.0f, 1.0f);
	displayClass->graph = new SceneGraph();
	displayClass->floor = new Prism(1, xSize, 0.1f, -1 * zSize, origin);
	SceneGraph* sg = displayClass->graph;
	sg->rootNode = new Node(displayClass->floor, NULL, NULL, 0, 0);
	sg->rootNode->scale = new glm::mat4();//glm::scale(glm::mat4(), glm::vec3(xSize, 0.1f, zSize)));
	sg->rootNode->rotate = new glm::mat4();
	sg->rootNode->translate = new glm::mat4();
	Furniture* currentPrimitive;
	Node* par;
	glm::mat4 transl;
	glm::mat4 rot;
	glm::mat4 scal;
	Node* thisNode;
	int xi;
	float* yi;
	int zi;
	std::string myName;
	while (i < numItems) {
		getline(config, reader);
		getline(config, reader);
		if (reader == "table") {
			currentPrimitive = new Table();
		} else if (reader == "chair") {
			currentPrimitive = new Chair();
		} else if (reader == "cabinet") {
			currentPrimitive = new Cabinet();
		} else if (reader == "lamp") {
			currentPrimitive = new Lamp();
		} else if (reader == "multitable") {
			currentPrimitive = new Table();
			*currentPrimitive->localTransforms->at(currentPrimitive->localTransforms->size() - 1) *= glm::scale(glm::mat4(), glm::vec3(2.0f, 1.0f, 1.0f));
			/*for (int n = 0; n < currentPrimitive->localTransforms->size(); n++) {
				*currentPrimitive->localTransforms->at(n) = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.0f, 0.0f)) * *currentPrimitive->localTransforms->at(n);
			}*/
		}

		myName = reader;
		//colors
		getline(config, reader);

		//translation
		getline(config, reader);
		vec = getVec(reader);
		xi = vec.x;
		yi = new float(0);
		zi = vec.y;

		//rotation
		getline(config, reader);
		vec = getVec(reader);
		rot = glm::rotate(glm::mat4(), vec.x, glm::vec3(0.0f, 1.0f, 0.0f));

		//scale
		getline(config, reader);
		vec = getVec(reader);
		scal = glm::scale(glm::mat4(), vec);
		
		thisNode = new Node(NULL, currentPrimitive, NULL, xi, zi);
		par = sg->rootNode;
		if (grid.at(xi).at(zi) != NULL) {
			par = grid.at(xi).at(zi);
			*yi = *par->furniture->height;
			/*for (int i = 0; i < par->children->size(); i++) {
				if (*par->children->at(i)->xInd == xi && *par->children->at(i)->zInd == zi) {
					par = par->children->at(i);
					*yi = *par->furniture->height;
					if (par->children->size() > 0) {
						par = setPar(thisNode, par, xi, yi, zi);
						break;
					}
					break;
				}
			}*/
		}
		thisNode->parent = par;
	    grid.at(xi).at(zi) = thisNode;
		if (myName == "multitable") {
			grid.at(xi + 1).at(zi) = thisNode;
		}

		//setPar(par, myPar, xi, yi, zi);
		transl = glm::translate(glm::mat4(), glm::vec3(xi + origin.x, *yi, zi + origin.z));
		if (myName == "multitable") {
			transl = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.0f, 0.0f)) * transl;
		}

		*thisNode->furniture->height *= scal[1][1];
		if (thisNode->parent != displayClass->graph->rootNode) {
			*thisNode->furniture->height += *thisNode->parent->furniture->height;
		}
		thisNode->rotate = new glm::mat4(rot);// * *par->rotate);
		thisNode->translate = new glm::mat4(transl);
		thisNode->scale = new glm::mat4(scal);// * *par->scale);

		//add to parent's list of children
		par->children->push_back(thisNode);
		i++;
	}
	displayClass->selectedNode = displayClass->graph->rootNode;
}

Node* MyGLWidget::setPar(Node* thisNode, Node* par, int xi, float* yi, int zi) {
	if (par->children->size() == 1) {
		*yi += *par->children->at(0)->furniture->height;
		return setPar(thisNode, par->children->at(0), xi, yi, zi);
	}
	return par;
}

// updateGL event handler
void MyGLWidget::paintGL() {
	//Always and only do this at the start of a frame, it wipes the slate clean
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//part of the animation
	displayClass->rotation += 2.5;

	displayClass->redraw();
}

// resize window event handler
//This function is called automatically when the widget is resized, including the first "sizing"
void MyGLWidget::resizeGL(int width, int height) {
    //set the viewport, more boilerplate
	glViewport(0, 0, width, height);

	displayClass->resizeWindow(width, height);
}

void MyGLWidget::keyReleaseEvent(QKeyEvent *e) {
	switch (e->key()) {
	case Qt::Key::Key_Shift:
		*invertMode = false;
	}
}

void MyGLWidget::selectNextNode(bool b, Node* n) {
	Node* p = n->parent;
	if (p == NULL) {
		displayClass->selectedNode = n;
		return;
	}
	for (int i = 0; i < p->children->size(); i++) {
		if (p->children->at(i) == n) {
			if (i + 1 == p->children->size()) {
				if (b) {
					selectNextNode(true, p);
					return;
				}
				for (int j = 0; j < p->children->size(); j++) {
					if (p->children->at(j)->children->size() > 0) {
						displayClass->selectedNode = p->children->at(j)->children->at(0);
						return;
					}
				}
				selectNextNode(true, p);
				return;
			}
			if (b) {
				for (int k = i + 1; k < p->children->size(); k++) {
					if (p->children->at(k)->children->size() > 0) {
						displayClass->selectedNode = p->children->at(k)->children->at(0);
						return;
					}
				}
				selectNextNode(true, p);
				return;
			}
			displayClass->selectedNode = p->children->at(i + 1);
			return;
		}
	}
}

// keyboard event handler
void MyGLWidget::keyPressEvent(QKeyEvent *e){
	//a good place to process keystrokes
	int i = 1;
	glm::mat4 tra;
	if (*invertMode) {
		i = -1;
	}
	Node* p;
	glm::mat4 rot;
	switch(e->key())
	{
	case Qt::Key::Key_Shift:
		*invertMode = true;
		break;
	case Qt::Key::Key_Q:
		break;
	case Qt::Key::Key_R:
		tra = *displayClass->selectedNode->translate;
		displayClass->selectedNode->traverseNodes(2, glm::inverse(tra));
		displayClass->selectedNode->traverseNodes(0, glm::rotate(glm::mat4(), i * 10.0f, glm::vec3(0, 1, 0)));
		displayClass->selectedNode->traverseNodes(2, tra);
		break;
	case Qt::Key::Key_S:
		displayClass->selectedNode->traverseNodes(2, glm::translate(glm::mat4(), glm::vec3(0, 0, -1)));
		break;
	case Qt::Key::Key_W:
		displayClass->selectedNode->traverseNodes(2, glm::translate(glm::mat4(), glm::vec3(0, 0, 1)));
		break;
	case Qt::Key::Key_A:
		displayClass->selectedNode->traverseNodes(2, glm::translate(glm::mat4(), glm::vec3(-1, 0, 0)));
		break;
	case Qt::Key::Key_D:
		displayClass->selectedNode->traverseNodes(2, glm::translate(glm::mat4(), glm::vec3(1, 0, 0)));
		break;
	case Qt::Key::Key_X:
		tra = *displayClass->selectedNode->translate;
		rot = *displayClass->selectedNode->rotate;
		displayClass->selectedNode->traverseNodes(2, glm::inverse(tra));
		displayClass->selectedNode->traverseNodes(2, glm::inverse(rot));
		displayClass->selectedNode->traverseNodes(1, glm::scale(glm::mat4(), glm::vec3(1 + (i * 0.5), 1, 1)));
		displayClass->selectedNode->traverseNodes(2, rot);
		displayClass->selectedNode->traverseNodes(2, tra);
		break;
	case Qt::Key::Key_Y:
		tra = *displayClass->selectedNode->translate;
		rot = *displayClass->selectedNode->rotate;
		displayClass->selectedNode->traverseNodes(2, glm::inverse(tra));
		displayClass->selectedNode->traverseNodes(2, glm::inverse(rot));
		displayClass->selectedNode->traverseNodes(1, glm::scale(glm::mat4(), glm::vec3(1, 1 + (i * 0.5), 1)));
		displayClass->selectedNode->traverseNodes(2, rot);
		displayClass->selectedNode->traverseNodes(2, tra);
		break;
	case Qt::Key::Key_Z:
		tra = *displayClass->selectedNode->translate;
		rot = *displayClass->selectedNode->rotate;
		displayClass->selectedNode->traverseNodes(2, glm::inverse(tra));
		displayClass->selectedNode->traverseNodes(2, glm::inverse(rot));
		displayClass->selectedNode->traverseNodes(1, glm::scale(glm::mat4(), glm::vec3(1, 1, 1 + (i * 0.5))));
		displayClass->selectedNode->traverseNodes(2, rot);
		displayClass->selectedNode->traverseNodes(2, tra);
		break;
	case Qt::Key::Key_N:
		if (displayClass->selectedNode->parent == NULL) {
			if (displayClass->selectedNode->children == NULL || displayClass->selectedNode->children->empty()) {
				return;
			}
			displayClass->selectedNode = displayClass->selectedNode->children->at(0);
			return;
		}
		selectNextNode(false, displayClass->selectedNode);
		break;
	case Qt::Key::Key_E:
		p = displayClass->selectedNode->parent;
		if (p == NULL) {
			exit(0);
		}
		for (int i = 0; i < p->children->size(); i++) {
			if (p->children->at(i) == displayClass->selectedNode) {
				p->children->erase(p->children->begin() + i);
				displayClass->selectedNode = displayClass->graph->rootNode;
				break;
			}
		}
		break;
	case Qt::Key::Key_Escape:
		if (displayClass)
			delete displayClass;
		exit(0);	
		break;
	}
}
#pragma endregion