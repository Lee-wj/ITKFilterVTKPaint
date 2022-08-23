#include "Painting.h"

VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2); //����no override found for 'vtkRayCastImageDisplayHelper'.

Painting::Painting()
{
	this->folder = "";
}

Painting::Painting(std::string folder)
{
	this->folder = folder;
}

void Painting::setfolder(std::string folder)
{
	this->folder = folder;
}

std::string Painting::getfolder()
{
	return folder;
}

vtkSmartPointer<vtkImageData> DataTransform(vtkSmartPointer<vtkDICOMImageReader> reader)
{
	/*VTK�г��õ���VTKImageCast���ͽ���ͼ����������ת����
	һЩ���õ���ͼ�����ӣ������ݶ����ӣ��ڼ���ʱ���ھ��ȵĿ��ǣ��Ὣ����洢Ϊfloat����double���͡�
	������ͼ����ʾʱ��һ����Ҫͼ��Ϊunsigned char���͡�
	������������ת��ʱ��ֻ�ǽ����ͽ���ǿ��ת������û�н��б���������*/
	vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData((vtkDataObject*)reader->GetOutput());
	//��������Ҫ��ͼ��ת���ɲ�ͬ�����ͽ��м���ʱ��ֻ��Ҫ��SetOutputScalarTypeToXXX()���ó���Ӧ��������ͼ��ɡ�
	cast->SetOutputScalarTypeToFloat();
	//cast->SetOutputScalarTypeToUnsignedChar();
	cast->Update();
	return cast->GetOutput();
}

vtkSmartPointer<vtkVolumeProperty> SetProperty()
{
	/*ͨ��vtkVolumeProperty���������������ӰЧ����Shading������ӰЧ����Ҫ�ܻ�����ϵ����ɢ���ϵ���������ϵ���͸߹�ǿ���ĸ�����Ӱ�졣
	vtkVolumeProperty::SetAmbient()���û�����ϵ��
	vtkVolumeProperty::SetDiffuse()����ɢ���ϵ��
	vtkVolumeProperty::SetSpecular()���÷����ϵ��
	һ������£�����ϵ���ĺ�Ӧ��Ϊ1��
	������ʱ��������ƹ�����Ϊ��������ȣ���ֵ֮�ͻ����1��*/
	/*���⣬����һ�������Ǹ߹�ǿ�ȣ�Specular Power��, ���ڿ�������Ƶ����ƽ���̶ȡ�
	������VTKVolumeProperty::SetSpecularPower()���øò�����*/

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();//�����������
	volumeProperty->SetInterpolationTypeToLinear();

	/*��������ϵ���У���������ϵ��ռ����ʱ����ӰЧ�������ԡ�
	��ɢ���ϵ��ռ����ʱ����ʾЧ����Ƚϴ��
	�������ϵ��ռ����ʱ����ʾЧ����ȽϹ⻬��
	����ӰЧ���ر�ʱ����ͬ�ڻ�����ϵ��Ϊ1.0����������ϵ��Ϊ�㡣
	��Ȼ��ӰЧ�������Դ����Ŀ��λ�ú���ɫ�йء�*/
	volumeProperty->ShadeOn();						//vtkVolumeProperty��Ĭ�����ǹر���ӰЧ���ģ������Ҫ��ʽ����ShadeOn()����������ӰЧ����
	volumeProperty->SetAmbient(0.4);				//������ϵ��
	volumeProperty->SetDiffuse(0.6);				//ɢ���ϵ��,������
	volumeProperty->SetSpecular(0.2);				//�����ϵ��,���淴�� 

	//���ò�͸����
	/*��͸���ȴ��亯����һ���ֶ����Ա���ӳ�亯����
	���øú����ɽ�����ͶӰ�����еĲ�����Ҷ�ֵӳ��Ϊ��ͬ�Ĳ�͸����ֵ������������ɫֵ*/
	/*vtkPiecewiseFunction�ඨ��������Էֶκ�����
	��֧���������÷�ʽ����һ�ַ�ʽ��ֱ����Ӷϵ㣻
	�ڶ�����ֱ�����һ���߶Σ�����������ϵ㡣*/
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	//Defines a piecewise(�ֶ�) function mapping.
	//ֱ����Ӷϵ�
	//int AddPoint(double x, double y);
	/*��һ������xΪ�Ա�����������ָ�Ҷ�ֵ��y����ӳ��ֵ������ָ��͸���ȡ�
	ִ�гɹ��󣬷��ص�ǰ��ӵĶϵ��index����ֵ����0��ʼ�������򷵻� - 1.*/
	//����������͸���ȶϵ�(60, 0.00)��(140, 0.40)��(180, 0.90)��
	//�������ǣ����Ҷ�ֵС��60ʱ����͸������Ӧ����Ϊ0��
	//���Ҷ�ֵ����60~140ʱ��ͨ������ӳ�䵽0.0~0.40֮�䣻
	//���Ҷ�ֵ����140~180ʱ������ӳ����0.40~0.90��
	//���Ҷ�ֵ����180��ʱ����͸����ӳ�䵽0.90~1.00��һ��ֵ��
	/*compositeOpacity->AddPoint(70.0, 0.0);
	compositeOpacity->AddPoint(599.0, 0);
	compositeOpacity->AddPoint(600.0, 0);
	compositeOpacity->AddPoint(1195.0, 0);
	compositeOpacity->AddPoint(1200, .2);
	compositeOpacity->AddPoint(1300, .3);
	compositeOpacity->AddPoint(2000, .3);
	compositeOpacity->AddPoint(4095.0, 1.0);*/

	compositeOpacity->AddPoint(60, 0.00);
	compositeOpacity->AddPoint(140, 0.40);
	compositeOpacity->AddPoint(180, 0.90);
	volumeProperty->SetScalarOpacity(compositeOpacity);

	//������ɫ����
	//int AddRGBPoint(double x, double r, double g, double b);
	//����ʵ��һ��RGB�ϵ����ӣ�����x��ʾ���ػҶ�ֵ��r,g,bΪӳ���RGB����,��ΧΪ��0,1��
	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
	/*color->AddRGBPoint(0.0, 0.5, 0.0, 0.0);
	color->AddRGBPoint(600.0, 1.0, 0.5, 0.5);
	color->AddRGBPoint(1280.0, 0.5, 0.5, 0.3);
	color->AddRGBPoint(1960.0, 0.5, 0.77, 0.6);
	color->AddRGBPoint(4095.0, 0.5, 0.9, 0.9);*/

	color->AddRGBPoint(0, 0, 0, 0);
	color->AddRGBPoint(64, 1, 0.52, 0.30);
	color->AddRGBPoint(190.0, 1, 1, 1);
	color->AddRGBPoint(220, 0.2, 0.2, 0.2);
	volumeProperty->SetColor(color);

	return volumeProperty;
}

void PaintingMethod(vtkVolumeMapper * mapper, vtkVolumeProperty * property)
{
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	//represents a volume(data & properties) in a rendered scene
	volume->SetMapper(mapper);
	volume->SetProperty(property);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	
	renderer->SetBackground(1, 1, 1);//���ñ�����ɫ��
	renderWindow->SetSize(1000, 600);
	renderer->AddVolume(volume);
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("VolumeRendering");
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderer->ResetCamera();
	renderWindow->Render();
	renderWindowInteractor->Start();
}

void Painting::GPUPaint()
{
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDataByteOrderToLittleEndian();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();

	//����GPU���ٵĹ���Ͷ��������㷨
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapperGpu = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapperGpu->SetInputData(DataTransform(reader));
	//volumeMapperGpu->SetSampleDistance(1.0);			//���ù��߲�������
	//volumeMapperGpu->SetImageSampleDistance(0.5);		//����ͼ���������
	//volumeMapperGpu->SetAutoAdjustSampleDistances(1);

	PaintingMethod(volumeMapperGpu, SetProperty());
}

void Painting::FixedPaint()
{
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDataByteOrderToLittleEndian();
	reader->SetDirectoryName(folder.c_str());
	//reader->SetDirectoryName("D:\\Program_Files\\TestData\\in");
	reader->Update();

	//�̶����ص��������ӳ����
	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	volumeMapper->SetInputData(DataTransform(reader));

	PaintingMethod(volumeMapper, SetProperty());
}

void Painting::MarchingCubesPaint()
{
	vtkObjectFactory::RegisterFactory(vtkRenderingOpenGL2ObjectFactory::New());
	vtkObjectFactory::RegisterFactory(vtkRenderingVolumeOpenGL2ObjectFactory::New());

	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->SetDataByteOrderToLittleEndian();
	reader->Update();

	vtkSmartPointer<vtkMarchingCubes> marchingcube = vtkSmartPointer<vtkMarchingCubes>::New();
	marchingcube->SetInputConnection(reader->GetOutputPort());//��ö�ȡ�����ݵĵ㼯��
	marchingcube->SetValue(0, 200);//Setting the threshold;
	marchingcube->ComputeNormalsOn();//������淨����;

	vtkSmartPointer<vtkStripper> Stripper = vtkSmartPointer<vtkStripper>::New();
	Stripper->SetInputConnection(marchingcube->GetOutputPort());//��ȡ����Ƭ

	vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();//������Ƭӳ��Ϊ�������ݣ�
	Mapper->SetInputConnection(Stripper->GetOutputPort());
	Mapper->ScalarVisibilityOff();


	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();//Created a actor;
	actor->SetMapper(Mapper);//���Ƥ����������
	actor->GetProperty()->SetDiffuseColor(1, .49, .25);//����Ƥ����ɫ��
	actor->GetProperty()->SetSpecular(0.3);//�����ʣ�
	actor->GetProperty()->SetOpacity(1.0);//͸���ȣ�
	actor->GetProperty()->SetSpecularPower(20);//�����ǿ�ȣ�
	actor->GetProperty()->SetColor(1, 0, 0);//���ýǵ���ɫ��
	actor->GetProperty()->SetRepresentationToWireframe();//�߿�

	//vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();//Setting the Camera;
	//camera->SetViewUp(0, 0, -1);//����������Ϸ���
	//camera->SetPosition(0, 1, 0);//λ�ã���������ϵ�����λ�ã�
	//camera->SetFocalPoint(0, 0, 0);//���㣬��������ϵ�������������
	//camera->ComputeViewPlaneNormal();//������ƽ�淽�򣬻��ڵ�ǰ��λ�úͽ��㣻

	vtkSmartPointer<vtkOutlineFilter> outfilterline = vtkSmartPointer<vtkOutlineFilter>::New();
	outfilterline->SetInputConnection(reader->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> outmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	outmapper->SetInputConnection(outfilterline->GetOutputPort());
	vtkSmartPointer<vtkActor> OutlineActor = vtkSmartPointer<vtkActor>::New();
	OutlineActor->SetMapper(outmapper);
	OutlineActor->GetProperty()->SetColor(0, 0, 0);//�߿���ɫ

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();//WINDOW��
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();//wininteratcor;
	renWin->AddRenderer(ren);
	iren->SetRenderWindow(renWin);
	ren->AddActor(actor);
	ren->AddActor(OutlineActor);
	//ren->SetActiveCamera(camera);//������Ⱦ���������
	ren->ResetCamera();
	ren->ResetCameraClippingRange();

	//camera->Dolly(1.5);//ʹ��Dolly()������������ƽ�淨���ƶ������
	ren->SetBackground(1, 1, 1);//���ñ�����ɫ��
	renWin->SetSize(1000, 600);
	renWin->SetWindowName("ActorRendering");

	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	renWin->Render();
	iren->Initialize();
	iren->Start();
}


