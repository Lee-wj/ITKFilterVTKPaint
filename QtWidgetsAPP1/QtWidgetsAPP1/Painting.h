#ifndef PAINTING_H
#define PAINTING_H

#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellPicker.h>
#include <vtkImagePlaneWidget.h>
#include <vtkMarchingCubes.h>
#include <vtkImageCast.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageMapToColors.h>
#include <vtkProperty.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkAxesActor.h>

#include<vtkMarchingCubes.h>
#include<vtkPolyDataMapper.h>
#include<vtkStripper.h>
#include<vtkActor.h>
#include<vtkRenderingVolumeOpenGL2ObjectFactory.h>
#include<vtkRenderingOpenGL2ObjectFactory.h>
#include<vtkCamera.h>
#include<vtkMetaImageReader.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkOutlineFilter.h>

#include"vtkTool.h"

class Painting
{
private: 
	std::string folder;

public :
	Painting();
	Painting(std::string folder);
	void setfolder(std::string folder);
	std::string getfolder();
	void GPUPaint();
	void FixedPaint();
	void MarchingCubesPaint();
};

#endif

