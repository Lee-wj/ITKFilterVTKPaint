#ifndef QTWIDGETSAPP1_H
#define QTWIDGETSAPP1_H

#include<QtWidgets/QMainWindow>
#include<QListWidget>
#include<QLabel>
#include<QFileDialog>
#include<QMessageBox>
#include<QDir>
#include<QStringList>
#include<QListWidgetItem>
#include<QVBoxLayout>
#include<QBuffer>

#include<string>
#include "ui_QtWidgetsAPP1.h"

#include"ImageDCM.h"
#include"Painting.h"
#include"ITKFilter.h"

class QtWidgetsAPP1 : public QMainWindow
{
	Q_OBJECT

public:
	QtWidgetsAPP1(QWidget *parent = Q_NULLPTR);
	~QtWidgetsAPP1();

private:
	Ui::QtWidgetsAPP1Class ui;
	QStringList imgNameList;		//文件目录下所有的图像文件名
	QListWidget *imgListWidget;		//图片预览窗口
	QString imgSetPath;				//文件夹路径

	QVTKWidget *imgBefore;
	QVTKWidget *imgAfter;;
	ImageDCM *imgDCM;
	Painting *paintVolume;

	void Init();

private slots:
	void openImageSet();
	void slot_itemClicked(QListWidgetItem*);
	void slot_medianCilcked();
	void slot_erodeClicked();
	void slot_dilateClicked();
	void slot_shiftClicked();
	void slot_cannyClicked();
	void slot_gaussianClicked();
	void slot_discreteClicked();
	void slot_thresholdClicked();
	void slot_sigmoidClicked();
	void slot_normalClicked();
	void slot_paintFixed();
	void slot_paintGPU();
	void slot_paintMarching();
};
#endif