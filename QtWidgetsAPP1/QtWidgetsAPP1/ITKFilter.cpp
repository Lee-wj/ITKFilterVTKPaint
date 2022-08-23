#include "ITKFilter.h"

//��qvtkWidget�л���dcm��ʽ��image
void ITKFilter::Painting(IntImageType::Pointer image,QVTKWidget *qvtkWidget)
{
	using ConnectorType = itk::ImageToVTKImageFilter<IntImageType>;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(image);
	connector->Update();		//vtk�汾����5���������仰��������ʾ����ͼƬ

	/** The VTK image to display in this window */
	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
		
	//ITK��VTK��Y���෴����תͼƬ
	vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
	flip->SetInputData(connector->GetOutput());
	flip->SetFilteredAxis(1);
	flip->Update();

	viewer->SetInputData(flip->GetOutput());
	viewer->UpdateDisplayExtent();
	viewer->SetRenderWindow(qvtkWidget->GetRenderWindow());
	viewer->SetRenderer(renderer);//������Ⱦ����
	viewer->SetupInteractor(qvtkWidget->GetRenderWindow()->GetInteractor());//������ʽ
	renderer->ResetCamera();
	renderer->DrawOn();
	renderer->SetBackground(1, 1, 1);
	qvtkWidget->GetRenderWindow()->Render();
}
//��qvtkWidget�л���JPEG��ʽ��image
void ITKFilter::Painting(CharImageType::Pointer image, QVTKWidget *qvtkWidget)
{
	using ConnectorType = itk::ImageToVTKImageFilter<CharImageType>;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(image);
	connector->Update();				//vtk�汾����5���������仰��������ʾ����ͼƬ

	/** The VTK image to display in this window */
	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

	//ITK��VTK��Y���෴����תͼƬ
	vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
	flip->SetInputData(connector->GetOutput());
	flip->SetFilteredAxis(1);
	flip->Update();

	viewer->SetInputData(flip->GetOutput());
	viewer->UpdateDisplayExtent();
	viewer->SetRenderWindow(qvtkWidget->GetRenderWindow());
	viewer->SetRenderer(renderer);//������Ⱦ����
	viewer->SetupInteractor(qvtkWidget->GetRenderWindow()->GetInteractor());//������ʽ
	renderer->ResetCamera();
	renderer->DrawOn();
	renderer->SetBackground(1, 1, 1);
	qvtkWidget->GetRenderWindow()->Render();
}

//��������ֵ�˲� 
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
	//�ı����������ʾ�Ҷ� 
	shiftFilter->SetScale(3);

	using ThresholdFilterType = itk::ThresholdImageFilter<IntImageType>;
	ThresholdFilterType::Pointer thresholdfilter = ThresholdFilterType::New();
	thresholdfilter->SetOutsideValue(255);
	thresholdfilter->ThresholdAbove(255);//�ϱ��²���
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
	structuringElement.SetRadius(1);//3*3�ĺ�
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
	structuringElement.SetRadius(1);//3*3�ĺ�
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
	ShiftScaleFilterType::Pointer shiftFilter = ShiftScaleFilterType::New();//������ͼ�������ֵ����һ��ֵ����һ����������
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
	//�ı����������ʾ�Ҷ� 
	shiftFilter->SetScale(3);

	using ThresholdFilterType = itk::ThresholdImageFilter<IntImageType>;
	ThresholdFilterType::Pointer thresholdfilter = ThresholdFilterType::New();
	thresholdfilter->SetOutsideValue(255);
	thresholdfilter->ThresholdAbove(255);//�ϱ��²���
	thresholdfilter->SetInput(shiftFilter->GetOutput());

	using ImageCastType = itk::CastImageFilter<IntImageType, DoubleImageType>;
	ImageCastType::Pointer imageCast = ImageCastType::New();
	imageCast->SetInput(thresholdfilter->GetOutput());

	float variance = 1.0;			//��˹ƽ������˴�С
	float upperThreshold = 0.0;		//����һ��ѡ����ֵ������ٽ�㣻
	float lowerThreshold = 0.0;		//������ֵ��Χ������ٽ��

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
	//SetOutsideValue������ָ����Щ����ֵ���������޷�Χ֮������ص�����ֵ
	filter->SetOutsideValue(0);
	//filter->ThresholdBelow(180);//�±��ϲ���
	//filter->ThresholdAbove(200);//�ϱ��²���
	filter->ThresholdOutside(150, 220);//����ڲ���
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
	// �� �Ǻ��������ȴ��ڳɱ�����,��������Ǽ��[-3����3��]��
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
	NormalizeFilterType::Pointer normalizeFilter = NormalizeFilterType::New();//������ͼ������ع�һ��������ֵ������
	normalizeFilter->SetInput(image);
	normalizeFilter->Update(); 
	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, CharImageType>;
	RescaleFilterType::Pointer rescale = RescaleFilterType::New();
	rescale->SetInput(normalizeFilter->GetOutput());
	CharImageType::Pointer outputImage = rescale->GetOutput();
	ITKFilter::Painting(outputImage, qvtkWidget);
}

