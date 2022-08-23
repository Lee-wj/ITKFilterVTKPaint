#include "ImageDCM.h"

ImageDCM::ImageDCM()
{
	image = IntImageType::New();
	gdcmImageIO = ImageIOType::New();
	reader = ReaderType::New();
	reader->SetImageIO(gdcmImageIO);
	renderer = vtkRenderer::New();
	viewer = vtkImageViewer2::New();
}

ImageDCM::ImageDCM(QString fileName, QVTKWidget *vtkWidget)
{
	ImageDCM();
	this->fileName = fileName;
	this->qvtkWidget = vtkWidget;
	this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
}

ImageDCM::~ImageDCM()
{
}

void ImageDCM::openDCM(QString filePath, QVTKWidget *vtkWidget)
{
	bool flag=setFileName(filePath);
	setVTKWidget(vtkWidget);
	this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);//����qvtkWidget����Ⱦ��

	if (flag)
	{
		//itk��ȡͼ��,vtk��ʾ��qvtkWidget��
		ITKFilter::Painting(image, qvtkWidget);
	}
}

bool ImageDCM::setFileName(QString fileName)
{
	this->fileName = fileName;
	if (!fileName.isEmpty())
	{
		//QStringתstd::string
		QByteArray b;
		b.append(fileName);
		char *str = b.data();

		reader->SetFileName(str);
		reader->Update();
		image = reader->GetOutput();
		return true;
	}
	else
	{
		QMessageBox::warning(NULL, "Error", "ͼƬ��ַ��Ч", QMessageBox::Ok);
		return false;
	}

}

void ImageDCM::setVTKWidget(QVTKWidget * vtkWidget)
{
	this->qvtkWidget = vtkWidget;
}
