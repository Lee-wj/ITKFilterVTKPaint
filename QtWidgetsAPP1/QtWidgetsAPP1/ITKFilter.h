#ifndef ITKFILTER_H
#define ITKFILTER_H

#include"itkImage.h"
#include"itkImageFileReader.h"
#include"itkImageFileWriter.h"
#include"itkGDCMImageIO.h"
#include"itkPNGImageIO.h"
#include"itkJPEGImageIO.h"
#include"itkVTKImageToImageFilter.h"
#include"itkImageToVTKImageFilter.h"
#include"itkCastImageFilter.h"
#include"itkRescaleIntensityImageFilter.h"
#include"itkBinaryMedianImageFilter.h"
#include"itkIntensityWindowingImageFilter.h"
#include"itkBinaryErodeImageFilter.h"
#include"itkBinaryBallStructuringElement.h"
#include"itkBinaryDilateImageFilter.h"
#include"itkShiftScaleImageFilter.h"
#include"itkCannyEdgeDetectionImageFilter.h"
#include"itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include"itkDiscreteGaussianImageFilter.h"
#include"itkThresholdImageFilter.h"
#include"itkSigmoidImageFilter.h"
#include"itkNormalizeImageFilter.h"

#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkCamera.h>
#include <vtkImageFlip.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkJPEGReader.h>

#include <QVTKWidget.h>
#include <QMessageBox>

#include"vtkTool.h"

using IntPixelType = signed int;
using CharPixelType = unsigned char;
using FloatPixelType = float;
using DoublePixelType = double;
const unsigned int Dimension = 2;
using IntImageType = itk::Image<IntPixelType, Dimension>;
using CharImageType = itk::Image<CharPixelType, Dimension>;
using FloatImageType = itk::Image<FloatPixelType, Dimension>;
using DoubleImageType=itk::Image<DoublePixelType,Dimension>;
using ReaderType = itk::ImageFileReader<IntImageType>;
using ImageIOType = itk::GDCMImageIO;
//using ImageIOType = itk::PNGImageIO;

static class ITKFilter
{
private:

public:
	static void Painting(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void Painting(CharImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myBinaryMedianImageFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myErodeFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myDilateFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myShiftFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myCannyFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myGaussianFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myDiscreteFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myThresholdFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void mySigmoidFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);
	static void myNormalFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget);

};

#endif 

