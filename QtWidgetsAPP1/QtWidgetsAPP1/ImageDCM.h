#ifndef IMAGEDCM_H
#define IMAGEDCM_H

#include "ITKFilter.h"

class ImageDCM
{
public:
	IntImageType::Pointer image;
	ImageDCM();
	ImageDCM(QString fileName, QVTKWidget *vtkWidget);
	virtual ~ImageDCM();
	bool setFileName(QString fileName);
	void setVTKWidget(QVTKWidget *vtkWidget);
	void openDCM(QString fileName, QVTKWidget *vtkWidget);

private:
	QString fileName;
	QVTKWidget *qvtkWidget;//ÏÔÊ¾DCMÍ¼ÏñµÄQVTKWidget¿Ø¼þ

	ImageIOType::Pointer gdcmImageIO;
	ReaderType::Pointer reader;

	/** The VTK image to display in this window */
	vtkSmartPointer<vtkImageViewer2> viewer;
	vtkSmartPointer<vtkRenderer> renderer;
};
#endif
