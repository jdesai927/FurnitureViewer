#ifndef STARTERCODE_H
#define STARTERCODE_H

#include <QtGui/QMainWindow>
#include "ui_startercode.h"

class StarterCode : public QMainWindow
{
	Q_OBJECT

public:
	StarterCode(QWidget *parent = 0, Qt::WFlags flags = 0);
	~StarterCode();
	Ui::StarterCodeClass ui;

private:
	
};

#endif // STARTERCODE_H
