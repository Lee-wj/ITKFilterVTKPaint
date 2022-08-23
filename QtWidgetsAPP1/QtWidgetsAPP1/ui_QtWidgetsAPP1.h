/********************************************************************************
** Form generated from reading UI file 'QtWidgetsAPP1Hp4284.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef QTWIDGETSAPP1HP4284_H
#define QTWIDGETSAPP1HP4284_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_QtWidgetsAPP1Class
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QPushButton *fileButton;
    QLineEdit *filePath;
    QFrame *frame_img;
    QListWidget *imgListWidget;
    QPushButton *paintFixed;
    QPushButton *paintGPU;
    QPushButton *paintMarching;
    QFrame *frame_ImageShow;
    QVTKWidget *qvtkWidget_later;
    QVTKWidget *qvtkWidget_before;
    QFrame *frame_ITKFIilter;
    QPushButton *filterButton_median;
    QPushButton *fileButton_Erode;
    QPushButton *fileButton_Dilate;
    QPushButton *fileButton_Shift;
    QPushButton *fileButton_Canny;
    QPushButton *fileButton_Gaussian;
    QPushButton *fileButton_Discrete;
    QPushButton *fileButton_Threshold;
    QPushButton *fileButton_Sigmoid;
    QPushButton *fileButton_Normal;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtWidgetsAPP1Class)
    {
        if (QtWidgetsAPP1Class->objectName().isEmpty())
            QtWidgetsAPP1Class->setObjectName(QStringLiteral("QtWidgetsAPP1Class"));
        QtWidgetsAPP1Class->resize(1508, 864);
        centralWidget = new QWidget(QtWidgetsAPP1Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 10, 651, 761));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        fileButton = new QPushButton(frame);
        fileButton->setObjectName(QStringLiteral("fileButton"));
        fileButton->setGeometry(QRect(40, 20, 141, 51));
        filePath = new QLineEdit(frame);
        filePath->setObjectName(QStringLiteral("filePath"));
        filePath->setGeometry(QRect(240, 20, 351, 51));
        frame_img = new QFrame(frame);
        frame_img->setObjectName(QStringLiteral("frame_img"));
        frame_img->setGeometry(QRect(10, 160, 631, 591));
        frame_img->setFrameShape(QFrame::StyledPanel);
        frame_img->setFrameShadow(QFrame::Raised);
        imgListWidget = new QListWidget(frame_img);
        imgListWidget->setObjectName(QStringLiteral("imgListWidget"));
        imgListWidget->setGeometry(QRect(0, 0, 631, 581));
        paintFixed = new QPushButton(frame);
        paintFixed->setObjectName(QStringLiteral("paintFixed"));
        paintFixed->setGeometry(QRect(40, 90, 141, 51));
        paintGPU = new QPushButton(frame);
        paintGPU->setObjectName(QStringLiteral("paintGPU"));
        paintGPU->setGeometry(QRect(240, 90, 141, 51));
        paintMarching = new QPushButton(frame);
        paintMarching->setObjectName(QStringLiteral("paintMarching"));
        paintMarching->setGeometry(QRect(440, 90, 141, 51));
        frame_ImageShow = new QFrame(centralWidget);
        frame_ImageShow->setObjectName(QStringLiteral("frame_ImageShow"));
        frame_ImageShow->setGeometry(QRect(670, 10, 561, 761));
        frame_ImageShow->setFrameShape(QFrame::StyledPanel);
        frame_ImageShow->setFrameShadow(QFrame::Raised);
        qvtkWidget_later = new QVTKWidget(frame_ImageShow);
        qvtkWidget_later->setObjectName(QStringLiteral("qvtkWidget_later"));
        qvtkWidget_later->setGeometry(QRect(20, 390, 521, 351));
        qvtkWidget_before = new QVTKWidget(frame_ImageShow);
        qvtkWidget_before->setObjectName(QStringLiteral("qvtkWidget_before"));
        qvtkWidget_before->setGeometry(QRect(20, 20, 521, 351));
        frame_ITKFIilter = new QFrame(centralWidget);
        frame_ITKFIilter->setObjectName(QStringLiteral("frame_ITKFIilter"));
        frame_ITKFIilter->setGeometry(QRect(1249, 29, 221, 721));
        frame_ITKFIilter->setFrameShape(QFrame::StyledPanel);
        frame_ITKFIilter->setFrameShadow(QFrame::Raised);
        filterButton_median = new QPushButton(frame_ITKFIilter);
        filterButton_median->setObjectName(QStringLiteral("filterButton_median"));
        filterButton_median->setGeometry(QRect(40, 20, 141, 51));
        fileButton_Erode = new QPushButton(frame_ITKFIilter);
        fileButton_Erode->setObjectName(QStringLiteral("fileButton_Erode"));
        fileButton_Erode->setGeometry(QRect(40, 90, 141, 51));
        fileButton_Dilate = new QPushButton(frame_ITKFIilter);
        fileButton_Dilate->setObjectName(QStringLiteral("fileButton_Dilate"));
        fileButton_Dilate->setGeometry(QRect(40, 160, 141, 51));
        fileButton_Shift = new QPushButton(frame_ITKFIilter);
        fileButton_Shift->setObjectName(QStringLiteral("fileButton_Shift"));
        fileButton_Shift->setGeometry(QRect(40, 230, 141, 51));
        fileButton_Canny = new QPushButton(frame_ITKFIilter);
        fileButton_Canny->setObjectName(QStringLiteral("fileButton_Canny"));
        fileButton_Canny->setGeometry(QRect(40, 300, 141, 51));
        fileButton_Gaussian = new QPushButton(frame_ITKFIilter);
        fileButton_Gaussian->setObjectName(QStringLiteral("fileButton_Gaussian"));
        fileButton_Gaussian->setGeometry(QRect(40, 370, 141, 51));
        fileButton_Discrete = new QPushButton(frame_ITKFIilter);
        fileButton_Discrete->setObjectName(QStringLiteral("fileButton_Discrete"));
        fileButton_Discrete->setGeometry(QRect(40, 440, 141, 51));
        fileButton_Threshold = new QPushButton(frame_ITKFIilter);
        fileButton_Threshold->setObjectName(QStringLiteral("fileButton_Threshold"));
        fileButton_Threshold->setGeometry(QRect(40, 510, 141, 51));
        fileButton_Sigmoid = new QPushButton(frame_ITKFIilter);
        fileButton_Sigmoid->setObjectName(QStringLiteral("fileButton_Sigmoid"));
        fileButton_Sigmoid->setGeometry(QRect(40, 580, 141, 51));
        fileButton_Normal = new QPushButton(frame_ITKFIilter);
        fileButton_Normal->setObjectName(QStringLiteral("fileButton_Normal"));
        fileButton_Normal->setGeometry(QRect(40, 650, 141, 51));
        QtWidgetsAPP1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtWidgetsAPP1Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1508, 30));
        QtWidgetsAPP1Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtWidgetsAPP1Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtWidgetsAPP1Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtWidgetsAPP1Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtWidgetsAPP1Class->setStatusBar(statusBar);

        retranslateUi(QtWidgetsAPP1Class);

        QMetaObject::connectSlotsByName(QtWidgetsAPP1Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtWidgetsAPP1Class)
    {
        QtWidgetsAPP1Class->setWindowTitle(QApplication::translate("QtWidgetsAPP1Class", "QtWidgetsAPP1", Q_NULLPTR));
        fileButton->setText(QApplication::translate("QtWidgetsAPP1Class", "\345\257\274\345\205\245\345\233\276\347\211\207\351\233\206\345\220\210", Q_NULLPTR));
        paintFixed->setText(QApplication::translate("QtWidgetsAPP1Class", "\345\233\272\345\256\232\345\203\217\347\264\240\347\273\230\345\210\266", Q_NULLPTR));
        paintGPU->setText(QApplication::translate("QtWidgetsAPP1Class", "GPU\345\212\240\351\200\237\347\273\230\345\210\266", Q_NULLPTR));
        paintMarching->setText(QApplication::translate("QtWidgetsAPP1Class", "\351\235\242\347\273\230\345\210\266", Q_NULLPTR));
        filterButton_median->setText(QApplication::translate("QtWidgetsAPP1Class", "\344\270\255\345\200\274\346\273\244\346\263\242", Q_NULLPTR));
        fileButton_Erode->setText(QApplication::translate("QtWidgetsAPP1Class", "\345\275\242\346\200\201\345\255\246\350\205\220\350\232\200", Q_NULLPTR));
        fileButton_Dilate->setText(QApplication::translate("QtWidgetsAPP1Class", "\345\275\242\346\200\201\345\255\246\350\206\250\350\203\200", Q_NULLPTR));
        fileButton_Shift->setText(QApplication::translate("QtWidgetsAPP1Class", "\347\272\277\346\200\247\351\230\210\345\200\274\345\217\230\346\215\242", Q_NULLPTR));
        fileButton_Canny->setText(QApplication::translate("QtWidgetsAPP1Class", "Canny\350\276\271\347\274\230\346\243\200\346\265\213", Q_NULLPTR));
        fileButton_Gaussian->setText(QApplication::translate("QtWidgetsAPP1Class", "\350\276\271\347\274\230\346\243\200\346\265\213(Gaussian)", Q_NULLPTR));
        fileButton_Discrete->setText(QApplication::translate("QtWidgetsAPP1Class", "\347\246\273\346\225\243\351\253\230\346\226\257\346\250\241\347\263\212", Q_NULLPTR));
        fileButton_Threshold->setText(QApplication::translate("QtWidgetsAPP1Class", "\351\230\210\345\200\274\350\277\207\346\273\244", Q_NULLPTR));
        fileButton_Sigmoid->setText(QApplication::translate("QtWidgetsAPP1Class", "\351\235\236\347\272\277\346\200\247\346\273\244\346\263\242", Q_NULLPTR));
        fileButton_Normal->setText(QApplication::translate("QtWidgetsAPP1Class", "\345\203\217\347\264\240\345\275\222\344\270\200\345\214\226", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtWidgetsAPP1Class: public Ui_QtWidgetsAPP1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // QTWIDGETSAPP1HP4284_H
