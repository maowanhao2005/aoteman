/********************************************************************************
** Form generated from reading UI file 'mp4.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MP4_H
#define UI_MP4_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mp4
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mp4)
    {
        if (mp4->objectName().isEmpty())
            mp4->setObjectName("mp4");
        mp4->resize(800, 600);
        centralwidget = new QWidget(mp4);
        centralwidget->setObjectName("centralwidget");
        mp4->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mp4);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        mp4->setMenuBar(menubar);
        statusbar = new QStatusBar(mp4);
        statusbar->setObjectName("statusbar");
        mp4->setStatusBar(statusbar);

        retranslateUi(mp4);

        QMetaObject::connectSlotsByName(mp4);
    } // setupUi

    void retranslateUi(QMainWindow *mp4)
    {
        mp4->setWindowTitle(QCoreApplication::translate("mp4", "mp4", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mp4: public Ui_mp4 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MP4_H
