/********************************************************************************
** Form generated from reading UI file 'startercode.ui'
**
** Created: Tue Nov 27 15:55:24 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTERCODE_H
#define UI_STARTERCODE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include "myglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_StarterCodeClass
{
public:
    MyGLWidget *centralWidget;

    void setupUi(QMainWindow *StarterCodeClass)
    {
        if (StarterCodeClass->objectName().isEmpty())
            StarterCodeClass->setObjectName(QString::fromUtf8("StarterCodeClass"));
        StarterCodeClass->resize(640, 480);
        centralWidget = new MyGLWidget(StarterCodeClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        StarterCodeClass->setCentralWidget(centralWidget);

        retranslateUi(StarterCodeClass);

        QMetaObject::connectSlotsByName(StarterCodeClass);
    } // setupUi

    void retranslateUi(QMainWindow *StarterCodeClass)
    {
        StarterCodeClass->setWindowTitle(QApplication::translate("StarterCodeClass", "Scene Graph Starter Code QT", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StarterCodeClass: public Ui_StarterCodeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTERCODE_H
