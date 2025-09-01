/********************************************************************************
** Form generated from reading UI file 'MutiTableWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUTITABLEWIDGET_H
#define UI_MUTITABLEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MutiTableWidgetClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MutiTableWidgetClass)
    {
        if (MutiTableWidgetClass->objectName().isEmpty())
            MutiTableWidgetClass->setObjectName(QString::fromUtf8("MutiTableWidgetClass"));
        MutiTableWidgetClass->resize(600, 400);
        menuBar = new QMenuBar(MutiTableWidgetClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        MutiTableWidgetClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MutiTableWidgetClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MutiTableWidgetClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MutiTableWidgetClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MutiTableWidgetClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MutiTableWidgetClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MutiTableWidgetClass->setStatusBar(statusBar);

        retranslateUi(MutiTableWidgetClass);

        QMetaObject::connectSlotsByName(MutiTableWidgetClass);
    } // setupUi

    void retranslateUi(QMainWindow *MutiTableWidgetClass)
    {
        MutiTableWidgetClass->setWindowTitle(QCoreApplication::translate("MutiTableWidgetClass", "MutiTableWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MutiTableWidgetClass: public Ui_MutiTableWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUTITABLEWIDGET_H
