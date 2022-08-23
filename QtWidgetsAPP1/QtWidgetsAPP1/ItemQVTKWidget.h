#pragma once
#include<QListWidget.h>

namespace Ui {
	class ItemQVTKWidget;
}

class ItemQVTKWidget:public QListWidgetItem
{
	Q_OBJECT

public:
	explicit ItemQVTKWidget(QVTKWidget *parent = 0);
	~ItemQVTKWidget();

	//…Ë÷√ ˝æ›
	void SetData(const QString& qstrFileName, int iFileSize, const QString& qstrPic);

private:

};

