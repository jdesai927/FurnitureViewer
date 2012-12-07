#include "glew.h"
#include "startercode.h"

using namespace std;

#include <QtGui/QApplication>

char* filename;
char* raytraceconfig;

int main(int argc, char *argv[])
{
	if (argc < 3) {
		std::cout << "Error: Invalid number of arguments. Exiting.\n" << std::endl;
		exit(-1);
	}
	QApplication a(argc, argv);
	StarterCode w;
	filename = argv[1];
	raytraceconfig = argv[2];
	w.show();
	return a.exec();
}
