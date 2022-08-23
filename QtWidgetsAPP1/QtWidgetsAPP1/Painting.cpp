#include "Painting.h"

VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2); //错误：no override found for 'vtkRayCastImageDisplayHelper'.

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
	/*VTK中常用到的VTKImageCast类型进行图像数据类型转换。
	一些常用到的图像算子（例如梯度算子）在计算时出于精度的考虑，会将结果存储为float或者double类型。
	但是在图像显示时，一般需要图像为unsigned char类型。
	这个类进行类型转换时，只是将类型进行强制转换，而没有进行比例的缩放*/
	vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData((vtkDataObject*)reader->GetOutput());
	//当我们需要把图像转换成不同的类型进行计算时，只需要把SetOutputScalarTypeToXXX()设置成相应的输出类型即可。
	cast->SetOutputScalarTypeToFloat();
	//cast->SetOutputScalarTypeToUnsignedChar();
	cast->Update();
	return cast->GetOutput();
}

vtkSmartPointer<vtkVolumeProperty> SetProperty()
{
	/*通过vtkVolumeProperty可以设置体绘制阴影效果（Shading）。阴影效果主要受环境光系数、散射光系数、反射光系数和高光强度四个参数影响。
	vtkVolumeProperty::SetAmbient()设置环境光系数
	vtkVolumeProperty::SetDiffuse()设置散射光系数
	vtkVolumeProperty::SetSpecular()设置反射光系数
	一般情况下，三个系数的和应该为1。
	但是有时候，在体绘制过程中为了提高亮度，三值之和会大于1。*/
	/*另外，还有一个参数是高光强度（Specular Power）, 用于控制体绘制的外观平滑程度。
	可用于VTKVolumeProperty::SetSpecularPower()设置该参数。*/

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();//定义对象属性
	volumeProperty->SetInterpolationTypeToLinear();

	/*在这三个系数中，当环境光系数占主导时，阴影效果不明显。
	当散射光系数占主导时，显示效果会比较粗燥；
	但反射光系数占主导时，显示效果会比较光滑。
	当阴影效果关闭时，等同于环境光系数为1.0，其他两个系数为零。
	当然阴影效果还与光源的数目、位置和颜色有关。*/
	volumeProperty->ShadeOn();						//vtkVolumeProperty中默认是是关闭阴影效果的，因此需要显式调用ShadeOn()函数来打开阴影效果。
	volumeProperty->SetAmbient(0.4);				//环境光系数
	volumeProperty->SetDiffuse(0.6);				//散射光系数,漫反射
	volumeProperty->SetSpecular(0.2);				//反射光系数,镜面反射 

	//设置不透明度
	/*不透明度传输函数是一个分段线性标量映射函数，
	利用该函数可将光线投影过程中的采样点灰度值映射为不同的不透明度值，决定最终颜色值*/
	/*vtkPiecewiseFunction类定义标量线性分段函数，
	其支持两种设置方式，第一种方式是直接添加断点；
	第二种是直接添加一条线段，即添加两个断点。*/
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	//Defines a piecewise(分段) function mapping.
	//直接添加断点
	//int AddPoint(double x, double y);
	/*第一个参数x为自变量，这里是指灰度值；y则是映射值，这里指不透明度。
	执行成功后，返回当前添加的断点的index索引值（从0开始），否则返回 - 1.*/
	//下面三个不透明度断点(60, 0.00)、(140, 0.40)、(180, 0.90)。
	//其意义是，当灰度值小于60时，不透明导读应设置为0；
	//当灰度值介于60~140时，通过线性映射到0.0~0.40之间；
	//当灰度值介于140~180时，线性映射至0.40~0.90；
	//当灰度值大于180度时，不透明度映射到0.90~1.00的一个值。
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

	//设置颜色属性
	//int AddRGBPoint(double x, double r, double g, double b);
	//可以实现一个RGB断点的添加，其中x表示像素灰度值，r,g,b为映射的RGB分量,范围为（0,1）
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
	
	renderer->SetBackground(1, 1, 1);//设置背景颜色；
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

	//基于GPU加速的光线投射体绘制算法
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapperGpu = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapperGpu->SetInputData(DataTransform(reader));
	//volumeMapperGpu->SetSampleDistance(1.0);			//设置光线采样距离
	//volumeMapperGpu->SetImageSampleDistance(0.5);		//设置图像采样步长
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

	//固定像素点的体数据映射器
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
	marchingcube->SetInputConnection(reader->GetOutputPort());//获得读取的数据的点集；
	marchingcube->SetValue(0, 200);//Setting the threshold;
	marchingcube->ComputeNormalsOn();//计算表面法向量;

	vtkSmartPointer<vtkStripper> Stripper = vtkSmartPointer<vtkStripper>::New();
	Stripper->SetInputConnection(marchingcube->GetOutputPort());//获取三角片

	vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();//将三角片映射为几何数据；
	Mapper->SetInputConnection(Stripper->GetOutputPort());
	Mapper->ScalarVisibilityOff();


	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();//Created a actor;
	actor->SetMapper(Mapper);//获得皮肤几何数据
	actor->GetProperty()->SetDiffuseColor(1, .49, .25);//设置皮肤颜色；
	actor->GetProperty()->SetSpecular(0.3);//反射率；
	actor->GetProperty()->SetOpacity(1.0);//透明度；
	actor->GetProperty()->SetSpecularPower(20);//反射光强度；
	actor->GetProperty()->SetColor(1, 0, 0);//设置角的颜色；
	actor->GetProperty()->SetRepresentationToWireframe();//线框；

	//vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();//Setting the Camera;
	//camera->SetViewUp(0, 0, -1);//设置相机向上方向；
	//camera->SetPosition(0, 1, 0);//位置：世界坐标系，相机位置；
	//camera->SetFocalPoint(0, 0, 0);//焦点，世界坐标系，控制相机方向；
	//camera->ComputeViewPlaneNormal();//重置视平面方向，基于当前的位置和焦点；

	vtkSmartPointer<vtkOutlineFilter> outfilterline = vtkSmartPointer<vtkOutlineFilter>::New();
	outfilterline->SetInputConnection(reader->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> outmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	outmapper->SetInputConnection(outfilterline->GetOutputPort());
	vtkSmartPointer<vtkActor> OutlineActor = vtkSmartPointer<vtkActor>::New();
	OutlineActor->SetMapper(outmapper);
	OutlineActor->GetProperty()->SetColor(0, 0, 0);//线框颜色

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();//WINDOW；
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();//wininteratcor;
	renWin->AddRenderer(ren);
	iren->SetRenderWindow(renWin);
	ren->AddActor(actor);
	ren->AddActor(OutlineActor);
	//ren->SetActiveCamera(camera);//设置渲染器的相机；
	ren->ResetCamera();
	ren->ResetCameraClippingRange();

	//camera->Dolly(1.5);//使用Dolly()方法延伸着视平面法向移动相机；
	ren->SetBackground(1, 1, 1);//设置背景颜色；
	renWin->SetSize(1000, 600);
	renWin->SetWindowName("ActorRendering");

	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	renWin->Render();
	iren->Initialize();
	iren->Start();
}


