#include "ITKFilter.h"

//在qvtkWidget中绘制dcm格式的image
void ITKFilter::Painting(IntImageType::Pointer image,QVTKWidget *qvtkWidget)
{
	using ConnectorType = itk::ImageToVTKImageFilter<IntImageType>;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(image);
	connector->Update();		//vtk版本大于5必须加上这句话，否则显示不出图片

	/** The VTK image to display in this window */
	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
		
	//ITK和VTK的Y轴相反，翻转图片
	vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
	flip->SetInputData(connector->GetOutput());
	flip->SetFilteredAxis(1);
	flip->Update();

	viewer->SetInputData(flip->GetOutput());
	viewer->UpdateDisplayExtent();
	viewer->SetRenderWindow(qvtkWidget->GetRenderWindow());
	viewer->SetRenderer(renderer);//设置渲染场景
	viewer->SetupInteractor(qvtkWidget->GetRenderWindow()->GetInteractor());//交互方式
	renderer->ResetCamera();
	renderer->DrawOn();
	renderer->SetBackground(1, 1, 1);
	qvtkWidget->GetRenderWindow()->Render();
}
//在qvtkWidget中绘制JPEG格式的image
void ITKFilter::Painting(CharImageType::Pointer image, QVTKWidget *qvtkWidget)
{
	using ConnectorType = itk::ImageToVTKImageFilter<CharImageType>;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(image);
	connector->Update();				//vtk版本大于5必须加上这句话，否则显示不出图片

	/** The VTK image to display in this window */
	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

	//ITK和VTK的Y轴相反，翻转图片
	vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
	flip->SetInputData(connector->GetOutput());
	flip->SetFilteredAxis(1);
	flip->Update();

	viewer->SetInputData(flip->GetOutput());
	viewer->UpdateDisplayExtent();
	viewer->SetRenderWindow(qvtkWidget->GetRenderWindow());
	viewer->SetRenderer(renderer);//设置渲染场景
	viewer->SetupInteractor(qvtkWidget->GetRenderWindow()->GetInteractor());//交互方式
	renderer->ResetCamera();
	renderer->DrawOn();
	renderer->SetBackground(1, 1, 1);
	qvtkWidget->GetRenderWindow()->Render();
}

//二进制中值滤波 
void ITKFilter::myBinaryMedianImageFilter(IntImageType::Pointer image,QVTKWidget *qvtkWidget)
{

	using IntensityWindowingImageFilterType = itk::IntensityWindowingImageFilter <IntImageType, IntImageType>;
	IntensityWindowingImageFilterType::Pointer intensityFilter = IntensityWindowingImageFilterType::New();
	intensityFilter->SetInput(image);
	intensityFilter->SetWindowMinimum(0);
	intensityFilter->SetWindowMaximum(4096);
	intensityFilter->SetOutputMinimum(0);
	intensityFilter->SetOutputMaximum(255);
	intensityFilter->Update();

	using RescaleFilterType1 = itk::RescaleIntensityImageFilter<IntImageType, IntImageType>;
	RescaleFilterType1::Pointer rescaler1 = RescaleFilterType1::New();
	rescaler1->SetOutputMinimum(0);
	rescaler1->SetOutputMaximum(255);
	rescaler1->SetInput(intensityFilter->GetOutput());

	using ShiftScaleFilterType = itk::ShiftScaleImageFilter<IntImageType, IntImageType>;
	ShiftScaleFilterType::Pointer shiftFilter = ShiftScaleFilterType::New();
	shiftFilter->SetInput(rescaler1->GetOutput());
	//改变参数调节显示灰度 
	shiftFilter->SetScale(3);

	using ThresholdFilterType = itk::ThresholdImageFilter<IntImageType>;
	ThresholdFilterType::Pointer thresholdfilter = ThresholdFilterType::New();
	thresholdfilter->SetOutsideValue(255);
	thresholdfilter->ThresholdAbove(255);//上变下不变
	thresholdfilter->SetInput(shiftFilter->GetOutput());

	using ImageCastType = itk::CastImageFilter<IntImageType, FloatImageType>;
	ImageCastType::Pointer imageCast = ImageCastType::New();
	imageCast->SetInput(thresholdfilter->GetOutput());

	using FilterType = itk::BinaryMedianImageFilter<FloatImageType, FloatImageType>;
	FilterType::Pointer filter = FilterType::New();
	const unsigned int radiusX = 3;
	const unsigned int radiusY = 3;
	IntImageType::SizeType indexRadius;
	indexRadius[0] = radiusX; // radius along x
	indexRadius[1] = radiusY; // radius along y
	filter->SetRadius(indexRadius); 
	filter->SetInput(imageCast->GetOutput());
	filter->Update();

	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, CharImageType>;
	RescaleFilterType::Pointer rescale = RescaleFilterType::New();
	rescale->SetInput(filter->GetOutput());

	CharImageType::Pointer outputImage = rescale->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

void ITKFilter::myErodeFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget)
{
	using StructuringElementType =
		itk::BinaryBallStructuringElement<IntPixelType, Dimension>;
	using ErodeFilterType =
		itk::BinaryErodeImageFilter<IntImageType, FloatImageType, StructuringElementType>;

	StructuringElementType structuringElement;
	structuringElement.SetRadius(1);//3*3的核
	structuringElement.CreateStructuringElement();

	ErodeFilterType::Pointer binaryErode = ErodeFilterType::New();
	binaryErode->SetKernel(structuringElement);
	binaryErode->SetInput(image);
	IntPixelType background = 0;
	IntPixelType foreground = 255;
	binaryErode->SetErodeValue(foreground);
	binaryErode->Update();
	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, CharImageType>;
	RescaleFilterType::Pointer rescale = RescaleFilterType::New();
	rescale->SetInput(binaryErode->GetOutput());
	CharImageType::Pointer outputImage = rescale->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

void ITKFilter::myDilateFilter(IntImageType::Pointer image, QVTKWidget *qvtkWidget)
{
	using StructuringElementType =
		itk::BinaryBallStructuringElement<IntPixelType, Dimension>;
	using DilateFilterType =
		itk::BinaryDilateImageFilter<IntImageType, FloatImageType, StructuringElementType>;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(1);//3*3的核
	structuringElement.CreateStructuringElement();
	DilateFilterType::Pointer binaryDilate = DilateFilterType::New();
	binaryDilate->SetKernel(structuringElement);
	binaryDilate->SetInput(image);
	IntPixelType background = 0;
	IntPixelType foreground = 255;
	binaryDilate->SetDilateValue(foreground);
	binaryDilate->Update();
	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, CharImageType>;
	RescaleFilterType::Pointer rescale = RescaleFilterType::New();
	rescale->SetInput(binaryDilate->GetOutput());
	CharImageType::Pointer outputImage = rescale->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

void ITKFilter::myShiftFilter(IntImageType::Pointer image, QVTKWidget * qvtkWidget)
{
	using ShiftScaleFilterType = itk::ShiftScaleImageFilter<IntImageType, FloatImageType>;
	ShiftScaleFilterType::Pointer shiftFilter = ShiftScaleFilterType::New();//将输入图像的像素值增加一个值并乘一个乘数因子
	shiftFilter->SetScale(1.2);
	shiftFilter->SetShift(25);
	shiftFilter->SetInput(image);
	shiftFilter->Update();
	using CastFilterTypeFloatToInt = itk::CastImageFilter<FloatImageType, CharImageType>;
	CastFilterTypeFloatToInt::Pointer castFilterFloatToInt = CastFilterTypeFloatToInt::New();
	castFilterFloatToInt->SetInput(shiftFilter->GetOutput());
	CharImageType::Pointer outputImage = castFilterFloatToInt->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

void ITKFilter::myCannyFilter(IntImageType::Pointer image, QVTKWidget * qvtkWidget)
{
	using IntensityWindowingImageFilterType = itk::IntensityWindowingImageFilter <IntImageType, IntImageType>;
	IntensityWindowingImageFilterType::Pointer intensityFilter = IntensityWindowingImageFilterType::New();
	intensityFilter->SetInput(image);
	intensityFilter->SetWindowMinimum(0);
	intensityFilter->SetWindowMaximum(4096);
	intensityFilter->SetOutputMinimum(0);
	intensityFilter->SetOutputMaximum(255);
	intensityFilter->Update();

	using RescaleFilterType1 = itk::RescaleIntensityImageFilter<IntImageType, IntImageType>;
	RescaleFilterType1::Pointer rescaler1 = RescaleFilterType1::New();
	rescaler1->SetOutputMinimum(0);
	rescaler1->SetOutputMaximum(255);
	rescaler1->SetInput(intensityFilter->GetOutput());

	using ShiftScaleFilterType = itk::ShiftScaleImageFilter<IntImageType, IntImageType>;
	ShiftScaleFilterType::Pointer shiftFilter = ShiftScaleFilterType::New();
	shiftFilter->SetInput(rescaler1->GetOutput());
	//改变参数调节显示灰度 
	shiftFilter->SetScale(3);

	using ThresholdFilterType = itk::ThresholdImageFilter<IntImageType>;
	ThresholdFilterType::Pointer thresholdfilter = ThresholdFilterType::New();
	thresholdfilter->SetOutsideValue(255);
	thresholdfilter->ThresholdAbove(255);//上变下不变
	thresholdfilter->SetInput(shiftFilter->GetOutput());

	using ImageCastType = itk::CastImageFilter<IntImageType, DoubleImageType>;
	ImageCastType::Pointer imageCast = ImageCastType::New();
	imageCast->SetInput(thresholdfilter->GetOutput());

	float variance = 1.0;			//高斯平滑卷积核大小
	float upperThreshold = 0.0;		//设置一个选择阈值的最大临界点；
	float lowerThreshold = 0.0;		//设置阈值范围的最低临界点

	using CannyFilterType = itk::CannyEdgeDetectionImageFilter<DoubleImageType, DoubleImageType>;
	CannyFilterType::Pointer cannyFilter = CannyFilterType::New();
	cannyFilter->SetVariance(variance);
	cannyFilter->SetUpperThreshold(upperThreshold);
	cannyFilter->SetLowerThreshold(lowerThreshold);
	cannyFilter->SetInput(imageCast->GetOutput());
	cannyFilter->Update();

	using RescaleFilterType = itk::RescaleIntensityImageFilter<DoubleImageType, CharImageType>;
	RescaleFilterType::Pointer rescale = RescaleFilterType::New();
	rescale->SetInput(cannyFilter->GetOutput());

	CharImageType::Pointer outputImage = rescale->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);

}

void ITKFilter::myGaussianFilter(IntImageType::Pointer image, QVTKWidget * qvtkWidget)
{
	using FilterType =
		itk::GradientMagnitudeRecursiveGaussianImageFilter<IntImageType, FloatImageType>;
	FilterType::Pointer filter = FilterType::New();
	const unsigned int sigma = 3;
	filter->SetSigma(sigma);
	filter->SetInput(image);
	filter->Update(); 
	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, CharImageType>;
	RescaleFilterType::Pointer rescale = RescaleFilterType::New();
	rescale->SetInput(filter->GetOutput());
	CharImageType::Pointer outputImage = rescale->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

void ITKFilter::myDiscreteFilter(IntImageType::Pointer image, QVTKWidget * qvtkWidget)
{
	using FilterType = itk::DiscreteGaussianImageFilter<IntImageType, FloatImageType>;
	FilterType::Pointer filter = FilterType::New();
	const int gaussianVariance = 3;
	const int maxKernelWidth = 7;
	filter->SetVariance(gaussianVariance);
	filter->SetMaximumKernelWidth(maxKernelWidth);
	filter->SetInput(image);
	filter->Update();
	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, CharImageType>;
	RescaleFilterType::Pointer rescale = RescaleFilterType::New();
	rescale->SetInput(filter->GetOutput());
	CharImageType::Pointer outputImage = rescale->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

void ITKFilter::myThresholdFilter(IntImageType::Pointer image, QVTKWidget * qvtkWidget)
{
	using FilterType = itk::ThresholdImageFilter<IntImageType>;
	FilterType::Pointer filter = FilterType::New();
	const unsigned int outsideValue = 0;
	const unsigned int insideValue = 255;
	const unsigned int lowerThreshold = 100;
	const unsigned int upperThreshold = 200;
	//SetOutsideValue定义了指向那些亮度值在上下门限范围之外的像素的亮度值
	filter->SetOutsideValue(0);
	//filter->ThresholdBelow(180);//下变上不变
	//filter->ThresholdAbove(200);//上变下不变
	filter->ThresholdOutside(150, 220);//外变内不变
	filter->SetInput(image);
	filter->Update();
	IntImageType::Pointer outputImage = filter->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);

}

void ITKFilter::mySigmoidFilter(IntImageType::Pointer image, QVTKWidget * qvtkWidget)
{
	using SigmoidImageFilter = itk::SigmoidImageFilter<IntImageType, IntImageType>;
	SigmoidImageFilter::Pointer sigmoidFilter = SigmoidImageFilter::New();
	const IntPixelType outputMinimum = 10;
	const IntPixelType outputMaximum = 240;
	sigmoidFilter->SetOutputMinimum(outputMinimum);
	sigmoidFilter->SetOutputMaximum(outputMaximum);
	// α 是和输入亮度窗口成比例的,这个窗口是间距[-3α，3α]。
	const double alpha = 10;
	const double beta = 170;
	sigmoidFilter->SetAlpha(alpha);
	sigmoidFilter->SetBeta(beta);
	sigmoidFilter->SetInput(image);
	sigmoidFilter->Update();
	IntImageType::Pointer outputImage = sigmoidFilter->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

void ITKFilter::myNormalFilter(IntImageType::Pointer image, QVTKWidget * qvtkWidget)  
{
	using NormalizeFilterType = itk::NormalizeImageFilter<IntImageType, FloatImageType>;
	NormalizeFilterType::Pointer normalizeFilter = NormalizeFilterType::New();//将输入图像的像素归一化，减均值除方差
	normalizeFilter->SetInput(image);
	normalizeFilter->Update(); 
	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, CharImageType>;
	RescaleFilterType::Pointer rescale = RescaleFilterType::New();
	rescale->SetInput(normalizeFilter->GetOutput());
	CharImageType::Pointer outputImage = rescale->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

