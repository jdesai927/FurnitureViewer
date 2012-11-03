#include "glew.h"
#include "startercode.h"

using namespace std;

#include <QtGui/QApplication>

char* filename;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StarterCode w;
	filename = argv[1];

	w.show();
	return a.exec();
}
