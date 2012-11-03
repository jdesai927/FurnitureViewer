// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#ifndef MYGLWIDGET
#define MYGLWIDGET

#include "DisplayClass.h"
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <fstream>
#include <QTimer>
#include <QKeyEvent>

class MyGLWidget : public QGLWidget {
	
	Q_OBJECT

private:

	//Animation/transformation stuff
	QTimer* timer;

	// Display
	
	
public:
	DisplayClass *displayClass;
	bool* invertMode;
	Node* selectedNode;
	MyGLWidget(QWidget*);
	~MyGLWidget(void);

protected:
	Node* setPar(Node*,Node*, int, float*, int);
	std::istringstream getNumStream(std::string);
	glm::vec3 getVec(std::string);
	void selectNextNode(bool,Node*);
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int, int);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
};

#endif