#include "QtWidgetsAPP1.h"

QtWidgetsAPP1::QtWidgetsAPP1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//���ڻ�������
	this->setWindowTitle("HelloITK/VTK");
	imgBefore = ui.qvtkWidget_before;
	imgAfter = ui.qvtkWidget_later;
	imgDCM = new ImageDCM();
	paintVolume = new Painting();
	connect(ui.fileButton, SIGNAL(clicked()), this, SLOT(openImageSet()));
	connect(ui.filterButton_median, SIGNAL(clicked()), this, SLOT(slot_medianCilcked()));
	connect(ui.fileButton_Erode, SIGNAL(clicked()), this, SLOT(slot_erodeClicked()));
	connect(ui.fileButton_Dilate, SIGNAL(clicked()), this, SLOT(slot_dilateClicked()));
	connect(ui.fileButton_Shift, SIGNAL(clicked()), this, SLOT(slot_shiftClicked()));
	connect(ui.fileButton_Canny, SIGNAL(clicked()), this, SLOT(slot_cannyClicked()));
	connect(ui.fileButton_Gaussian, SIGNAL(clicked()), this, SLOT(slot_gaussianClicked()));
	connect(ui.fileButton_Discrete, SIGNAL(clicked()), this, SLOT(slot_discreteClicked()));
	connect(ui.fileButton_Threshold, SIGNAL(clicked()), this, SLOT(slot_thresholdClicked()));
	connect(ui.fileButton_Sigmoid, SIGNAL(clicked()), this, SLOT(slot_sigmoidClicked()));
	connect(ui.fileButton_Normal, SIGNAL(clicked()), this, SLOT(slot_normalClicked()));
	connect(ui.paintFixed, SIGNAL(clicked()), this, SLOT(slot_paintFixed()));
	connect(ui.paintGPU, SIGNAL(clicked()), this, SLOT(slot_paintGPU()));
	connect(ui.paintMarching, SIGNAL(clicked()), this, SLOT(slot_paintMarching()));
}

QtWidgetsAPP1::~QtWidgetsAPP1()
{
	
}

QString DICOMToJPEG(char *imgpath);

void QtWidgetsAPP1::Init()
{
	const QSize IMAGE_SIZE(188, 138);
	const QSize ITEM_SIZE(188, 170);

	//QString imgSetPath;				//�ļ���·��
	//imgSetPath = "D:\\Program_Files\\TestData";
	QDir dir(imgSetPath);
	if (!dir.exists())
	{
		qWarning("Cannot find the example directory!");
		return;
	}

	//���ù�����
	//QDir::Files 0x002 �г��ļ���QDir::NoSymLinks 0x008 ���г���������
	//QStringList imgNameList;		//�ļ�Ŀ¼�����е�ͼ���ļ���
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	QStringList filters;
	filters << "*.dcm";//<< "*.jpg";
	dir.setNameFilters(filters);
	imgNameList = dir.entryList();
	if (imgNameList.count() <= 0)
	{
		qWarning("It\'s a empty directory!");
		return;
	}

	//QListWidget *imgListWidget;						//ͼƬԤ������
	imgListWidget = ui.imgListWidget;
	imgListWidget->setIconSize(IMAGE_SIZE);				//���õ�Ԫ���С
	imgListWidget->setResizeMode(QListView::Adjust);	//�������������ö�����ʾ
	imgListWidget->setViewMode(QListView::IconMode);	
	imgListWidget->setMovement(QListView::Static);		//���õ�Ԫ����϶�
	imgListWidget->setSpacing(8);
	//���ô�ֱ��������ʾ��ʽ��ScrollBarAsNeeded��������ʾ��ScrollBarAlwaysOff�����أ�ScrollBarAlwaysOn�����ԣ�
	imgListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	//������Ԫ��
	for (int i = 0; i < imgNameList.count(); i++)
	{
		QByteArray b;
		b.append(imgSetPath + "/" + imgNameList.at(i));
		char *str = b.data();
		//��DICOM��ʽת��ΪPNG��ʽ
		QString JPGimgPath = DICOMToJPEG(str);
		QPixmap pixmap(JPGimgPath);
		//QPixmap pixmap(imgSetPath + "\\" + imgNameList.at(i));
		QListWidgetItem *listWidgetItem = new QListWidgetItem(QIcon(pixmap.scaled(IMAGE_SIZE)), imgNameList.at(i));
		listWidgetItem->setSizeHint(ITEM_SIZE);					//���õ�Ԫ��Ŀ�Ⱥ͸߶�
		imgListWidget->insertItem(i, listWidgetItem);
	}

	//���ڲ���
	QVBoxLayout *imgLayout = new QVBoxLayout;
	imgLayout->addWidget(imgListWidget);
	imgLayout->setMargin(0);
	ui.frame_img->setLayout(imgLayout);

	//���ͼƬ�ź����
	connect(imgListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_itemClicked(QListWidgetItem*)));
}

void QtWidgetsAPP1::slot_itemClicked(QListWidgetItem *item)
{
	//QString filePath = "D:\\Program_Files\\TestData\\10014.dcm";
	QString filePath = imgSetPath + "/" + imgNameList.at(imgListWidget->row(item));
	imgDCM->openDCM(filePath, imgBefore);
}

void QtWidgetsAPP1::openImageSet()
{
	imgSetPath = QFileDialog::getExistingDirectory(this, "Select Directory", "./", QFileDialog::ShowDirsOnly);
	QByteArray b;
	b.append(imgSetPath);
	char *str = b.data();
	paintVolume->setfolder(str);

	if (!imgSetPath.isEmpty())
	{
		ui.filePath->setText(imgSetPath);
		this->Init();
	}
	else
	{
		QMessageBox::warning(this, "Error", "Error",QMessageBox::Ok);
	}
}

QString DICOMToJPEG(char *imgpath)
{
	//��ȡ������׺���ļ���
	std::string path = imgpath;
	int ps = path.find_last_of("/");
	int pe = path.find_last_of(".");
	std::string name = path.substr(ps + 1, pe - ps - 1);

	//ITK��ȡDICOMͼ��
	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(imgpath);
	reader->SetImageIO(gdcmImageIO);
	reader->Update();
	//���ػҶ�ֵ��ת
	IntImageType::Pointer image = reader->GetOutput();
	/*ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();
	ImageType::IndexType index;
	for (int x = 0; x < size[0]; x++)
		for (int y = 0; y < size[1]; y++)
		{
			index[0] = x;
			index[1] = y;
			ImageType::PixelType value = image->GetPixel(index);
			unsigned short test = 4096 - value;
			image->SetPixel(index, test);
		}
	image->Update();*/

	//���ô�λ�ʹ���
	using IntensityWindowingImageFilterType = itk::IntensityWindowingImageFilter <IntImageType, IntImageType>;
	IntensityWindowingImageFilterType::Pointer intensityFilter = IntensityWindowingImageFilterType::New();
	intensityFilter->SetInput(image);
	intensityFilter->SetWindowMinimum(0);
	intensityFilter->SetWindowMaximum(4096);
	intensityFilter->SetOutputMinimum(0);
	intensityFilter->SetOutputMaximum(255);
	intensityFilter->Update();

	//dcmתJPG
	using RescaleFilterType = itk::RescaleIntensityImageFilter<IntImageType, IntImageType>;
	RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
	rescaler->SetOutputMinimum(0);
	rescaler->SetOutputMaximum(255);
	rescaler->SetInput(intensityFilter->GetOutput());

	using ShiftScaleFilterType = itk::ShiftScaleImageFilter<IntImageType, IntImageType>;
	ShiftScaleFilterType::Pointer shiftFilter = ShiftScaleFilterType::New();
	shiftFilter->SetInput(rescaler->GetOutput());
	//�ı����������ʾ�Ҷ� 
  	shiftFilter->SetScale(3);

	using ThresholdFilterType = itk::ThresholdImageFilter<IntImageType>;
	ThresholdFilterType::Pointer thresholdfilter = ThresholdFilterType::New();
	thresholdfilter->SetOutsideValue(255);
	thresholdfilter->ThresholdAbove(255);//�ϱ��²���
	thresholdfilter->SetInput(shiftFilter->GetOutput());

	using ImageCastType = itk::CastImageFilter<IntImageType, CharImageType>;
	ImageCastType::Pointer imageCast = ImageCastType::New();
	imageCast->SetInput(thresholdfilter->GetOutput());

	//��JPG��ʽ��ͼ��洢�ڱ���
	using Writer1Type = itk::ImageFileWriter<CharImageType>;
	Writer1Type::Pointer writer1 = Writer1Type::New();

	std::string writepath="D:\\Program_Files\\TestData\\out\\" + name + ".jpg";
	using jpgType = itk::JPEGImageIO;
	jpgType::Pointer jpgIO = jpgType::New();
	writer1->SetImageIO(jpgIO);
	writer1->SetFileName(writepath);
	writer1->SetInput(imageCast->GetOutput());
	writer1->Update();
	
	QString jpgimgpath= QString::fromStdString(writepath);
	return jpgimgpath;
}

void QtWidgetsAPP1::slot_medianCilcked()
{
	ITKFilter::myBinaryMedianImageFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_erodeClicked()
{
	ITKFilter::myErodeFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_dilateClicked()
{
	ITKFilter::myDilateFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_shiftClicked()
{
	ITKFilter::myShiftFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_cannyClicked()
{
	ITKFilter::myCannyFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_gaussianClicked()
{
	ITKFilter::myGaussianFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_discreteClicked()
{
	ITKFilter::myDiscreteFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_thresholdClicked()
{
	ITKFilter::myThresholdFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_sigmoidClicked()
{
	ITKFilter::mySigmoidFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_normalClicked()
{
	ITKFilter::myNormalFilter(imgDCM->image, ui.qvtkWidget_later);
}

void QtWidgetsAPP1::slot_paintFixed()
{
	if (paintVolume->getfolder() == "")
	{
		QMessageBox::warning(NULL, "Error", "Please select a datafolder first");
		return;
	}
	paintVolume->FixedPaint();
}

void QtWidgetsAPP1::slot_paintGPU()
{
	if (paintVolume->getfolder() == "")
	{
		QMessageBox::warning(NULL, "Error", "Please select a datafolder first");
		return;
	}
	paintVolume->GPUPaint();
}

void QtWidgetsAPP1::slot_paintMarching()
{
	if (paintVolume->getfolder() == "")
	{
		QMessageBox::warning(NULL, "Error", "Please select a datafolder first");
		return;
	}
	paintVolume->MarchingCubesPaint();
}
