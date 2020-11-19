#include "InterCHeck.h" 
#include <QMessageBox>
//#include "image.h"
QString AppPath;
void MyHalconExceptionHandler(const HException& except)
{
	throw except;
}
bool CInterCHeck::LoadCheckParam(CHECKPARAM *checkparam, QString* str)
{
	YAML::Node _param;
	if (nullptr == str)
	{
		_param = YAML::LoadFile(QString(AppPath + "/DefaultModel/CheckParam.yaml").toStdString());
		QSettings configIniRead(AppPath + "\\DefaultModel\\CheckParam.ini", QSettings::IniFormat);
		configIniRead.setIniCodec("UTF-8");
 		QString cameraname = checkparam->c_CameraName;
	}
	else
	{
		_param = YAML::LoadFile(str->toStdString().c_str());
	}
	checkparam->i_BandChannel = _param[checkparam->c_CameraName][QString::fromLocal8Bit("Error_Ò©°åÈ±ÏÝ").toStdString()][QString::fromLocal8Bit("Ò©°åÍ¨µÀ").toStdString()]["value"].as<int>();
	checkparam->i_MinGray_Band = _param[checkparam->c_CameraName][QString::fromLocal8Bit("Error_Ò©°åÈ±ÏÝ").toStdString()][QString::fromLocal8Bit("Ò©°å×îÐ¡ãÐÖµ").toStdString()]["value"].as<int>();

// 	//¶ÁÈ¡¼ì²â²ÎÊýÅäÖÃ
// 	//////////////////////////////////////////////////////////////////////////
// 	//checkparam->i_BandArea = configIniRead.value("/" + cameraname + "/BandArea", "865000").toInt();
// 	//checkparam->i_BandException1_Area = configIniRead.value("/" + cameraname + "/BandException1_Area", "300").toInt();
// 	//checkparam->i_BandException2_Value = configIniRead.value("/" + cameraname + "/BandException2_Value", "110").toInt();
// 	//checkparam->i_BandException3_Area = configIniRead.value("/" + cameraname + "/BandException3_Area", "50").toInt();
// 	//checkparam->i_InterException_Area = configIniRead.value("/" + cameraname + "/InterException_Area", "50").toInt();
// 	//checkparam->d_InterException_Open = configIniRead.value("/" + cameraname + "/InterException_Open", "1.5").toDouble();
// 	//checkparam->i_PillRadius = configIniRead.value("/" + cameraname + "/PillRadius", "85").toInt();
// 	//checkparam->i_BandException2_Radius = configIniRead.value("/" + cameraname + "/BandException2_Radius", "0").toInt();
// 	
// 	checkparam->i_BandChannel = configIniRead.value("/" + cameraname + "/BandChannel", "0").toInt();
// 	checkparam->i_MinGray_Band = configIniRead.value("/" + cameraname + "/MinGray_Band", "100").toInt();
// 	checkparam->i_MinArea_Band = configIniRead.value("/" + cameraname + "/MinArea_Band", "680000").toInt();
// 	checkparam->i_CapsulesChannel = configIniRead.value("/" + cameraname + "/CapsulesChannel", "4").toInt();
// 	checkparam->i_MinGray_Capsules = configIniRead.value("/" + cameraname + "/MinGray_Capsules", "70").toInt();
// 	checkparam->d_OR_Capsules = configIniRead.value("/" + cameraname + "/OR_Capsules", "2.5").toDouble();
// 	checkparam->i_MinArea_Capsules = configIniRead.value("/" + cameraname + "/MinArea_Capsules", "10000").toInt();
// 	checkparam->i_MaxArea_Capsules = configIniRead.value("/" + cameraname + "/MaxArea_Capsules", "99999").toInt();
// 	checkparam->i_Number_Capsules = configIniRead.value("/" + cameraname + "/Number_Capsules", "10").toInt();
// 	checkparam->i_BatchChannel = configIniRead.value("/" + cameraname + "/BatchChannel", "0").toInt();
// 	checkparam->i_LengthAdd_1 = configIniRead.value("/" + cameraname + "/LengthAdd_1", "30").toInt();
// 	checkparam->i_LengthAdd_2 = configIniRead.value("/" + cameraname + "/LengthAdd_2", "1000").toInt();
// 	checkparam->d_ER_Batch = configIniRead.value("/" + cameraname + "/ER_Batch", "10").toDouble();
// 	checkparam->i_MaskHeight_Batch = configIniRead.value("/" + cameraname + "/MaksHeight_Batch", "11").toInt();
// 	checkparam->i_MaskWidth_Batch = configIniRead.value("/" + cameraname + "/MaksWidth_Batch", "11").toInt();
// 	checkparam->i_MinGray_Batch = configIniRead.value("/" + cameraname + "/MinGray_Batch", "15").toInt();
// 	checkparam->d_OR_Batch = configIniRead.value("/" + cameraname + "/OR_Batch", "1").toDouble();
// 	checkparam->d_CR_Batch = configIniRead.value("/" + cameraname + "/CR_Batch", "5").toDouble();
// 	checkparam->i_MinArea_Batch = configIniRead.value("/" + cameraname + "/MinArea_Batch", "150").toInt();
// 	checkparam->i_MaxArea_Batch = configIniRead.value("/" + cameraname + "/MaxArea_Batch", "99999").toInt();
// 	//checkparam->i_MinArea_BatchDefect = configIniRead.value("/" + cameraname + "/MinArea_BatchDefect", "50").toInt();
// 	//checkparam->i_MaxArea_BatchDefect = configIniRead.value("/" + cameraname + "/MaxArea_BatchDefect", "99999").toInt();
// 	checkparam->i_Channel_CapsulesDefect = configIniRead.value("/" + cameraname + "/Channel_CapsulesDefect", "1").toInt();
// 	checkparam->d_ER_RegionCapsules = configIniRead.value("/" + cameraname + "/ER_RegionCapsules", "12.5").toDouble();
// 	checkparam->i_MaxGray_CapsulesRed = configIniRead.value("/" + cameraname + "/MaxGray_CapsulesRed", "128").toInt();
// 	checkparam->i_MinArea_CapsulesRed = configIniRead.value("/" + cameraname + "/MinArea_CapsulesRed", "1500").toInt();
// 	checkparam->i_MaxArea_CapsulesRed = configIniRead.value("/" + cameraname + "/MaxArea_CapsulesRed", "99999").toInt();
// 	checkparam->d_DR_CapsulesRed = configIniRead.value("/" + cameraname + "/DR_CapsulesRed", "3").toDouble();
// 	checkparam->d_DR_CapsulesYellow = configIniRead.value("/" + cameraname + "/DR_CapsulesYellow", "3").toDouble();
// 	checkparam->i_Channel_RegionRed = configIniRead.value("/" + cameraname + "/Channel_RegionRed", "4").toInt();
// 	checkparam->i_MaskHeight_RegionRed = configIniRead.value("/" + cameraname + "/MaksHeight_RegionRed", "11").toInt();
// 	checkparam->i_MaskWidth_RegionRed = configIniRead.value("/" + cameraname + "/MaksWidth_RegionRed", "11").toInt();
// 	checkparam->i_MinGray_RegionRed = configIniRead.value("/" + cameraname + "/MinGray_RegionRed", "60").toInt();
// 	checkparam->i_MaxGray_RegionRed = configIniRead.value("/" + cameraname + "/MaxGray_RegionRed", "100").toInt();
// 	checkparam->d_CR_RegionRed = configIniRead.value("/" + cameraname + "/CR_RegionRed", "1").toDouble();
// 	checkparam->d_OR_RegionRed = configIniRead.value("/" + cameraname + "/OR_RegionRed", "1").toDouble();
// 	checkparam->i_MinArea_RegionRed = configIniRead.value("/" + cameraname + "/MinArea_RegionRed", "20").toInt();
// 	checkparam->i_MaxArea_RegionRed = configIniRead.value("/" + cameraname + "/MaxArea_RegionRed", "2000").toInt();
// 	checkparam->i_Channel_RedDefect1 = configIniRead.value("/" + cameraname + "/Channel_RedDefect1", "5").toInt();
// 	checkparam->i_Channel_RedDefect2 = configIniRead.value("/" + cameraname + "/Channel_RedDefect2", "4").toInt();
// 	checkparam->i_MaskHeight_RedDefect = configIniRead.value("/" + cameraname + "/MaksHeight_RedDefect", "5").toInt();
// 	checkparam->i_MaskWidth_RedDefect = configIniRead.value("/" + cameraname + "/MaksWidth_RedDefect", "5").toInt();
// 	checkparam->i_MinGray_RedDefect = configIniRead.value("/" + cameraname + "/MinGray_RedDefect", "35").toInt();
// 	checkparam->d_ER_RedDefect = configIniRead.value("/" + cameraname + "/ER_RedDefect", "3.5").toDouble();
// 	checkparam->d_OR_RedDefect = configIniRead.value("/" + cameraname + "/OR_RedDefect", "1.5").toDouble();
// 	checkparam->i_MinArea_RedDefect = configIniRead.value("/" + cameraname + "/MinArea_RedDefect", "20").toInt();
// 	checkparam->i_MaxArea_RedDefect = configIniRead.value("/" + cameraname + "/MaxArea_RedDefect", "99999").toInt();
// 	checkparam->i_MaskHeight_RegionYellow = configIniRead.value("/" + cameraname + "/MaskHeight_RegionYellow", "5").toInt();
// 	checkparam->i_MaskWidth_RegionYellow = configIniRead.value("/" + cameraname + "/MaskWidth_RegionYellow", "5").toInt();
// 	checkparam->i_MinGray_RegionYellow = configIniRead.value("/" + cameraname + "/MinGray_RegionYellow", "20").toInt();
// 	checkparam->i_MaxGray_RegionYellow = configIniRead.value("/" + cameraname + "/MaxGray_RegionYellow", "185").toInt();
// 	checkparam->d_OR_RegionYellow = configIniRead.value("/" + cameraname + "/OR_RegionYellow", "1.5").toDouble();
// 	checkparam->i_MinArea_RegionYellow = configIniRead.value("/" + cameraname + "/MinArea_RegionYellow", "20").toInt();
// 	checkparam->i_MaxArea_RegionYellow = configIniRead.value("/" + cameraname + "/MaxArea_RegionYellow", "99999").toInt();
// 	checkparam->i_Channel_BandDefect = configIniRead.value("/" + cameraname + "/Channel_BandDefect", "2").toInt();
// 	checkparam->d_DR_BandDefect = configIniRead.value("/" + cameraname + "/DR_BandDefect", "3").toDouble();
// 	checkparam->d_ER_BandDefect = configIniRead.value("/" + cameraname + "/ER_BandDefect", "5").toDouble();
// 	checkparam->i_MaskHeight_BandDefect = configIniRead.value("/" + cameraname + "/MaskHeight_BandDefect", "3").toInt();
// 	checkparam->i_MaskWidth_BandDefect = configIniRead.value("/" + cameraname + "/MaskWidth_BandDefect", "3").toInt();
// 	checkparam->i_MinGray_BandDefect = configIniRead.value("/" + cameraname + "/MinGray_BandDefect", "45").toInt();
// 	checkparam->d_CR_BandDefect = configIniRead.value("/" + cameraname + "/CR_BandDefect", "1.5").toDouble();
// 	checkparam->i_MinWidth_BandDefect = configIniRead.value("/" + cameraname + "/MinWidth_BandDefect", "1").toInt();
// 	checkparam->i_MaxWidth_BandDefect = configIniRead.value("/" + cameraname + "/MaxWidth_BandDefect", "100").toInt();
// 	checkparam->i_MinHeight_BandDefect = configIniRead.value("/" + cameraname + "/MinHeight_BandDefect", "3").toInt();
// 	checkparam->i_MaxHeight_BandDefect = configIniRead.value("/" + cameraname + "/MaxHeight_BandDefect", "100").toInt();
// 	checkparam->i_MinArea_BandDefect = configIniRead.value("/" + cameraname + "/MinArea_BandDefect", "10").toInt();
// 	checkparam->i_MaxArea_BandDefect = configIniRead.value("/" + cameraname + "/MaxArea_BandDefect", "9999").toInt();
// 	checkparam->b_IsOpen = configIniRead.value("/" + cameraname + "/IsOpen", "false").toBool();
// 	checkparam->i_Channel_InCapsules = configIniRead.value("/" + cameraname + "/Channel_InCapsules", "2").toInt();
// 	checkparam->d_ER_InCapsules = configIniRead.value("/" + cameraname + "/ER_InCapsules", "7.5").toDouble();
// 	checkparam->i_MaxGray_InCapsules = configIniRead.value("/" + cameraname + "/MaxGray_InCapsules", "120").toInt();
// 	checkparam->d_OR_InCapsules = configIniRead.value("/" + cameraname + "/OR_InCapsules", ".5").toDouble();
// 	checkparam->i_MinArea_InCapsules = configIniRead.value("/" + cameraname + "/MinArea_InCapsules", "2000").toInt();
// 	checkparam->i_MaxArea_InCapsules = configIniRead.value("/" + cameraname + "/MaxArea_InCapsules", "99999").toInt();
// 	checkparam->i_LengthPlus_1 = configIniRead.value("/" + cameraname + "/LengthPlus_1", "70").toInt();
// 	checkparam->i_LengthPlus_2 = configIniRead.value("/" + cameraname + "/LengthPlus_1", "70").toInt();
// 	checkparam->i_Channel_Middle = configIniRead.value("/" + cameraname + "/Channel_Middle", "0").toInt();
// 	checkparam->i_MaskHeight_Middle = configIniRead.value("/" + cameraname + "/MaskHeight_Middle", "7").toInt();
// 	checkparam->i_MaskWidth_Middle = configIniRead.value("/" + cameraname + "/MaskWidth_Middle", "7").toInt();
// 	checkparam->i_MinGray_Middle = configIniRead.value("/" + cameraname + "/MinGray_Middle", "45").toInt();
// 	checkparam->i_MinWidth_Middle = configIniRead.value("/" + cameraname + "/MinWidth_Middle", "5").toInt();
// 	checkparam->i_MinHeight_Middle = configIniRead.value("/" + cameraname + "/MinHeight_Middle", "3").toInt();
// 	checkparam->d_CR_Middle = configIniRead.value("/" + cameraname + "/CR_Middle", "2.5").toDouble();
// 	checkparam->i_MinArea_Middle = configIniRead.value("/" + cameraname + "/MinArea_Middle", "30").toInt();
// 	checkparam->i_Channel_Plate = configIniRead.value("/" + cameraname + "/Channel_Plate", "0").toInt();
// 	checkparam->i_MaxGray_Plate = configIniRead.value("/" + cameraname + "/MaxGray_Plate", "190").toInt();
// 	checkparam->d_CR_Plate = configIniRead.value("/" + cameraname + "/CR_Plate", "1.5").toDouble();
// 	checkparam->d_OR_Plate_1 = configIniRead.value("/" + cameraname + "/OR_Plate_1", "3.5").toDouble();
// 	checkparam->i_MinArea_Plate_1 = configIniRead.value("/" + cameraname + "/MinArea_Middle_1", "1500").toInt();
// 	checkparam->d_OR_Plate_2 = configIniRead.value("/" + cameraname + "/OR_Plate_2", "1.5").toDouble();
// 	checkparam->i_MinArea_Plate_2 = configIniRead.value("/" + cameraname + "/MinArea_Middle_2", "30").toInt();






	return false;
}
bool CInterCHeck::SaveCheckParam()
{
	m_bchangedparam = true;
	return false;
}
CInterCHeck::CInterCHeck(bool b_test)
{
	m_b_test = b_test;
	m_bchangedparam = false;
	AppPath = qApp->applicationDirPath();
	//test_name = "OCRCheck";
	c_AlgName = "RYcapsule";
	c_AlgVersion = "0.0.1";
	//strcpy(c_AlgName, test_name);
	m_iShowPos = 0;
	USB_Thread = nullptr;
	//QtUsbKey = nullptr;
	m_LEDHANDLE = nullptr;
	//setdlg = nullptr;
	CloseCallBack = nullptr;
	imageCallBack = nullptr;
	textCallback = nullptr;
	saveImageindex = 0;
	saveImageOKindex = 0;
	//ºóÆÚÒÆ¶¯µ½ÆäËûÎ»ÖÃ¡£2018Äê11ÔÂ23ÈÕ13:47:49
	HException::InstallHHandler(&MyHalconExceptionHandler);
	if (b_test)
	{
	}
}
CInterCHeck::~CInterCHeck()
{
	delete[]dataR;
	delete[]dataG;
	delete[]dataB;
	dataR = NULL;
	dataG = NULL;
	dataB = NULL;
}
void CInterCHeck::EndCheck()
{
	QMessageBox::about(NULL, QString::fromLocal8Bit("ÑÏÖØ´íÎó"), QString::fromLocal8Bit("USBkeyÒâÍâ¶Ï¿ª£¡"));
	CloseCallBack();
	delete this;
}
void CInterCHeck::Release()
{
	delete this;
}
char * CInterCHeck::GetCameraName(void)
{
	return m_checkparam.c_CameraName;
}
char * CInterCHeck::GetAlgName(void)
{
	return (char*)c_AlgName;
}
char * CInterCHeck::GetAlgVersion(void)
{
	return (char*)c_AlgVersion;
}
int CInterCHeck::ShowParamDlg(QWidget * parent, bool b_showornot)
{
	QtGuiSetting *setdlg = new QtGuiSetting(nullptr, this);
	setdlg->SetParam(m_checkparam);
	setdlg->SetModelMat(LastImage);
	m_bchangedparam = false;
	QObject::connect(setdlg, SIGNAL(ShouldSaveParam()), this, SLOT(SaveCheckParam()));
	setdlg->exec();
	delete setdlg;
	setdlg = nullptr;
	if (m_bchangedparam)
	{
		return 1;
	}
	return -1;
}
int CInterCHeck::SetParam(int _typeofcamera, char * _cameraName)
{
	m_checkparam.i_TypeOfCamera = _typeofcamera;
	strcpy(m_checkparam.c_CameraName, _cameraName);
	LoadCheckParam(&m_checkparam);
	return 0;
}
int CInterCHeck::ReturnParam(int * _typeofcamera, char & _cameraName)
{
	return 0;
}
int CInterCHeck::InitWindow(int pos, HANDLE _LEDhandle, void* _auhandle)
{
	if (_LEDhandle != NULL)
	{
		m_LEDHANDLE = _LEDhandle;
	}
	m_parent = _auhandle;
	if (m_LEDHANDLE != NULL)
	{
		if (nullptr == USB_Thread)
		{
			USB_Thread = new QThread();
		}
		//if (nullptr == QtUsbKey)
		{
			//QtUsbKey = new QTLiveUSBkey(this);
		}
		//if (nullptr != QtUsbKey && nullptr != USB_Thread)
		{
			//QObject::connect(this, SIGNAL(StartUSBKEY(void*)), QtUsbKey, SLOT(onLiveCheckKey(void*)));
			//QObject::connect(QtUsbKey, SIGNAL(ERRORCONNECT()), this, SLOT(EndCheck()));
			//QtUsbKey->moveToThread(USB_Thread);
			//USB_Thread->start();
			//emit StartUSBKEY(m_LEDHANDLE);
		}
	}
	m_iShowPos = pos;
	return 0;
}
int CInterCHeck::GetCheckPosNo()
{
	return 0;
}
void CInterCHeck::StartCheck(QString camerasign, std::shared_ptr<spd::logger> _daily_logger, int w, int h)
{
	if (_daily_logger != nullptr)
	{
		daily_logger = _daily_logger;
	}
	total_check = 0;
	map<string, string> optin;
	optin.insert(pair<string, string>("conf", (AppPath + "/DefaultModel/conf_inspect.yaml").toLocal8Bit()));
	optin.insert(pair<string, string>("parameter", (AppPath + "/DefaultModel/params.yaml").toLocal8Bit()));
	optin.insert(pair<string, string>("detectors", (AppPath + "/DefaultModel/detectors.yaml").toLocal8Bit()));
	circle_times = m_ShowLabel.size();
	if (dataR != nullptr)
	{
		delete dataR;
		dataR = nullptr;
	}
	if (dataB != nullptr)
	{
		delete dataB;
		dataB = nullptr;
	}
	if (dataG != nullptr)
	{
		delete dataG;
		dataG = nullptr;
	}
	if (dataR == NULL && w != 0 && h != 0)
	{
		dataR = new uchar[w*h];
	}
	if (dataG == NULL && w != 0 && h != 0)
	{
		dataG = new uchar[w*h];
	}
	if (dataB == NULL && w != 0 && h != 0)
	{
		dataB = new uchar[w*h];
	}
	//int ret = pillCheck_init(0);
	//proc.Initialize(optin);
}
void CInterCHeck::StopCheck()
{
}
QString CInterCHeck::GetResult()
{
	return QString();
}
template<typename T>
inline T norm_L2_Sqr(const T* a, const T* b, int n)
{
	double s = 0.f;
	for (int i = 0; i < n; i++) {
		double v = double(a[i] - b[i]);
		s += v * v;
	}
	return s;
}
int CInterCHeck::ComputerBestLabes(std::vector<float> data, double &dsts)
{
	int k_best = -1;
	const int K = m_CenterModel.size();
	const int dims = m_CenterModel[0].size();
	const std::vector<float> sample = data;
	double min_dist = std::numeric_limits<double>::max(); // DBL_MAX
	for (int k = 0; k < K; ++k) {
		const std::vector<float> center = m_CenterModel[k];
		const double dist = norm_L2_Sqr(sample.data(), center.data(), dims);
		if (min_dist > dist) {
			min_dist = dist;
			k_best = k;
		}
	}
	dsts = sqrt(min_dist) / 2500;
	return k_best;
}
bool compColx(const Rect &a, const Rect &b)
{
	return a.x < b.x;
}
bool compColy(const Rect &a, const Rect &b)
{
	return a.y < b.y;
}
Hobject CInterCHeck::Mat2Hobject(Mat& image)
{
	Hobject Hobj = Hobject();
	int hgt = image.rows;
	int wid = image.cols;
	if (dataR == NULL && wid != 0 && hgt != 0)
	{
		dataR = new uchar[wid*hgt];
	}
	if (dataG == NULL && wid != 0 && hgt != 0)
	{
		dataG = new uchar[wid*hgt];
	}
	if (dataB == NULL && wid != 0 && hgt != 0)
	{
		dataB = new uchar[wid*hgt];
	}
	int i;
	//	CV_8UC3
	if (image.type() == CV_8UC3)
	{
		split(image, imgchannel);
		Mat imgB = imgchannel[0];
		Mat imgG = imgchannel[1];
		Mat imgR = imgchannel[2];
		for (i = 0; i < hgt; i++)
		{
			memcpy(dataR + wid * i, imgR.data + imgR.step*i, wid);
			memcpy(dataG + wid * i, imgG.data + imgG.step*i, wid);
			memcpy(dataB + wid * i, imgB.data + imgB.step*i, wid);
		}
		gen_image3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
	}
	//	CV_8UCU1
	else if (image.type() == CV_8UC1)
	{
		uchar* data = new uchar[hgt*wid];
		for (i = 0; i < hgt; i++)
			memcpy(data + wid * i, image.data + image.step*i, wid);
		gen_image1(&Hobj, "byte", wid, hgt, (Hlong)data);
		delete[] data;
		data = NULL;
	}
	return Hobj;
}
Mat CInterCHeck::Hobject2Mat(Hobject Hobj)
{
	HTuple htCh = HTuple();
	HTuple cType;
	Mat Image;
	convert_image_type(Hobj, &Hobj, "byte");
	count_channels(Hobj, &htCh);
	HTuple wid;
	HTuple hgt;
	int W, H;
	if (htCh[0].I() == 1)
	{
		HTuple ptr;
		get_image_pointer1(Hobj, &ptr, &cType, &wid, &hgt);
		W = (Hlong)wid[0].I();
		H = (Hlong)hgt[0].I();
		Image.create(H, W, CV_8UC1);
		uchar* pdata = (uchar*)ptr[0].I();
		memcpy(Image.data, pdata, W*H);
	}
	else if (htCh[0].I() == 3)
	{
		HTuple ptrR, ptrG, ptrB;
		get_image_pointer3(Hobj, &ptrR, &ptrG, &ptrB, &cType, &wid, &hgt);
		W = (Hlong)wid[0].I();
		H = (Hlong)hgt[0].I();
		Image.create(H, W, CV_8UC3);
		vector<Mat> vecM(3);
		vecM[2].create(H, W, CV_8UC1);
		vecM[1].create(H, W, CV_8UC1);
		vecM[0].create(H, W, CV_8UC1);
		uchar* pr = (uchar*)ptrR[0].I();
		uchar* pg = (uchar*)ptrG[0].I();
		uchar* pb = (uchar*)ptrB[0].I();
		memcpy(vecM[2].data, pr, W*H);
		memcpy(vecM[1].data, pg, W*H);
		memcpy(vecM[0].data, pb, W*H);
		merge(vecM, Image);
	}
	return Image;
}
int CInterCHeck::Check(Mat imgpackage, void *checkparam, QString &str)
{
	bool _bresult = TRUE;
	try
	{
		if (checkparam != nullptr)
		{
			m_checkparam = *(CHECKPARAM*)checkparam;
		}
		QString strwarning;
		m_hoLiveImage = Mat2Hobject(imgpackage);
	}
	catch (cv::Exception& e)
	{
		imageCallBack(ui, m_iShowPos, temp, total_check);
		daily_logger->critical(e.what());
		return _bresult;
	}
	catch (HException &e)
	{
		imageCallBack(ui, m_iShowPos, temp, total_check);
		daily_logger->critical(e.message);
		return _bresult;
	}
	catch (...)
	{
	}
	return -1;
}
void CInterCHeck::ShowResult(QVector<double*>& result)
{
}
void CInterCHeck::BeatStart(void)
{
}
void CInterCHeck::BeatEnd(void)
{
}
void * CInterCHeck::GetEncryptHandle()
{
	return nullptr;
}
void CInterCHeck::EnableShow(bool b)
{
}
#include <QDebug>
void CInterCHeck::TESTSETSHOW(void *showlabel)
{
	HTuple hv_WindowID;
	open_window(((QLabel*)showlabel)->y(), ((QLabel*)showlabel)->x(), ((QLabel*)showlabel)->width(), ((QLabel*)showlabel)->height(), (long)((QWidget*)((QLabel*)showlabel)->parent())->winId(), "", "", &hv_WindowID);
	//set_check("father");
	m_ShowLabel.push_back(hv_WindowID);
}
void CInterCHeck::SetResultCallBack(UI_MONITOR uis, CallbackText callbackfun)
{
	if (ui == nullptr)
		ui = uis;
	textCallback = callbackfun;
}
void CInterCHeck::SetShowCallBack(UI_MONITOR uis, CallbackImage callbackfun)
{
	ui = uis;
	imageCallBack = callbackfun;
}
void CInterCHeck::SetCloseCallBack(CallbackClose callbackfun)
{
	CloseCallBack = callbackfun;
}
bool CInterCHeck::OtherBeforeCheck(Mat imgpackage)
{
	if (CloseCallBack == nullptr)
	{
		return false;
	}
	imgpackage.copyTo(MatToShow);
	imgpackage.copyTo(LastImage);
	total_check++;
}
bool CInterCHeck::OtherAfterCheck()
{
	//imageCallBack(ui, m_iShowPos, MatToShow, total_check);
	HTuple hv_Width, hv_Height;
	if (total_check < circle_times)
	{
		get_image_pointer1(m_hoLiveImage, _, _, &hv_Width, &hv_Height);
		set_part(m_ShowLabel[total_check%circle_times], 0, 0, hv_Height - 1, hv_Width - 1);
	}
	return true;
}

void CPPExpDefaultExceptionHandler(const Halcon::HException& except)
{
	throw except;
}
void calculate_lines_gauss_parameters(Halcon::HTuple hv_MaxLineWidth, Halcon::HTuple hv_Contrast,
	Halcon::HTuple *hv_Sigma, Halcon::HTuple *hv_Low, Halcon::HTuple *hv_High)
{

	// Local control variables 
	HTuple  hv_ContrastHigh, hv_ContrastLow, hv_HalfWidth;
	HTuple  hv_Help;


	// Install default exception handler 
	HException::InstallHHandler(&CPPExpDefaultExceptionHandler);

	//Check control parameters
	if (0 != ((hv_MaxLineWidth.Num()) != 1))
	{
		throw HException("Wrong number of values of control parameter: 1");
	}
	if (0 != ((hv_MaxLineWidth.IsNumber()).Not()))
	{
		throw HException("Wrong type of control parameter: 1");
	}
	if (0 != (hv_MaxLineWidth <= 0))
	{
		throw HException("Wrong value of control parameter: 1");
	}
	if (0 != (HTuple((hv_Contrast.Num()) != 1).And((hv_Contrast.Num()) != 2)))
	{
		throw HException("Wrong number of values of control parameter: 2");
	}
	if (0 != (((hv_Contrast.IsNumber()).Min()) == 0))
	{
		throw HException("Wrong type of control parameter: 2");
	}
	//Set and check ContrastHigh
	hv_ContrastHigh = hv_Contrast.Select(0);
	if (0 != (hv_ContrastHigh < 0))
	{
		throw HException("Wrong value of control parameter: 2");
	}
	//Set or derive ContrastLow
	if (0 != ((hv_Contrast.Num()) == 2))
	{
		hv_ContrastLow = hv_Contrast.Select(1);
	}
	else
	{
		hv_ContrastLow = hv_ContrastHigh / 3.0;
	}
	//Check ContrastLow
	if (0 != (hv_ContrastLow < 0))
	{
		throw HException("Wrong value of control parameter: 2");
	}
	if (0 != (hv_ContrastLow > hv_ContrastHigh))
	{
		throw HException("Wrong value of control parameter: 2");
	}
	//
	//Calculate the parameters Sigma, Low, and High for lines_gauss
	if (0 != (hv_MaxLineWidth < (HTuple(3.0).Sqrt())))
	{
		//Note that LineWidthMax < sqrt(3.0) would result in a Sigma < 0.5,
		//which does not make any sense, because the corresponding smoothing
		//filter mask would be of size 1x1.
		//To avoid this, LineWidthMax is restricted to values greater or equal
		//to sqrt(3.0) and the contrast values are adapted to reflect the fact
		//that lines that are thinner than sqrt(3.0) pixels have a lower contrast
		//in the smoothed image (compared to lines that are sqrt(3.0) pixels wide).
		hv_ContrastLow = (hv_ContrastLow*hv_MaxLineWidth) / (HTuple(3.0).Sqrt());
		hv_ContrastHigh = (hv_ContrastHigh*hv_MaxLineWidth) / (HTuple(3.0).Sqrt());
		hv_MaxLineWidth = HTuple(3.0).Sqrt();
	}
	//Convert LineWidthMax and the given contrast values into the input parameters
	//Sigma, Low, and High required by lines_gauss
	hv_HalfWidth = hv_MaxLineWidth / 2.0;
	(*hv_Sigma) = hv_HalfWidth / (HTuple(3.0).Sqrt());
	hv_Help = ((-2.0*hv_HalfWidth) / ((HTuple(6.283185307178).Sqrt())*((*hv_Sigma).Pow(3.0))))*((-0.5*((hv_HalfWidth / (*hv_Sigma)).Pow(2.0))).Exp());
	(*hv_High) = (hv_ContrastHigh*hv_Help).Abs();
	(*hv_Low) = (hv_ContrastLow*hv_Help).Abs();
	return;
}

int CInterCHeck::RealCheck(QString &result, CHECKPARAM *checkparam, int Wnd = -1)
{
	int i_error = 0, i_null = 0;
	try
	{
		Hobject  ho_Image, ho_R, ho_G, ho_B, ho_H, ho_S;
		Hobject  ho_V, ho_Region, ho_RegionFillUp2, ho_ConnectedRegions;
		Hobject  ho_RegionBand, ho_Rectangle11, ho_RegionDifference19;
		Hobject  ho_RegionOpening20, ho_ConnectedRegions30, ho_SelectedRegions24;
		Hobject  ho_Region26, ho_RegionOpening24, ho_RegionFillUp;
		Hobject  ho_ConnectedRegions35, ho_SelectedRegions32, ho_ImageSub10;
		Hobject  ho_ImageReduced, ho_Regions, ho_RegionIntersection20;
		Hobject  ho_RegionFillUp13, ho_RegionOpening, ho_RegionClosing13;
		Hobject  ho_ConnectedRegions1, ho_SelectedRegions, ho_RegionPill;
		Hobject  ho_RegionUnion13, ho_RegionDilation6, ho_Rectangle8;
		Hobject  ho_RegionDifference23, ho_Rectangle9, ho_EmptyPill;
		Hobject  ho_RegionUnion5, ho_RegionTrans3, ho_RegionDifference3;
		Hobject  ho_TopPills, ho_BottomPills, ho_SortedRegions, ho_ObjectSelected;
		Hobject  ho_ObjectSelected1, ho_RegionDilation1, ho_ConnectedRegions9;
		Hobject  ho_Rectangle2, ho_RegionDifference2, ho_RegionTrans2;
		Hobject  ho_RegionTrans, ho_RegionUnion, ho_Rectangle, ho_RegionDifference;
		Hobject  ho_RegionErosion, ho_ConnectedRegions16, ho_SelectedRegions10;
		Hobject  ho_ImageMax, ho_ImageSub1, ho_Region4, ho_RegionOpening1;
		Hobject  ho_RegionIntersection, ho_RegionClosing, ho_ConnectedRegions3;
		Hobject  ho_SelectedRegions2, ho_RegionTrans1, ho_RegionUnion1;
		Hobject  ho_Rectangle1, ho_RegionIntersection9, ho_ImageReduced4;
		Hobject  ho_Region11, ho_RegionOpening6, ho_ConnectedRegions8;
		Hobject  ho_SelectedRegions5, ho_Circle3, ho_ImageSub12;
		Hobject  ho_Region28, ho_RegionIntersection17, ho_RegionDifference20;
		Hobject  ho_RegionOpening21, ho_RegionClosing10, ho_ConnectedRegions38;
		Hobject  ho_SelectedRegions34, ho_Circle5, ho_RegionShadow;
		Hobject  ho_RegionBatch, ho_RegionUnion14, ho_RegionDifference24;
		Hobject  ho_RegionDifference15, ho_RegionUnion2, ho_Region18;
		Hobject  ho_RegionTrans7, ho_ImageReduced18, ho_ImageOpening;
		Hobject  ho_Lines3, ho_UnionContours, ho_SelectedXLD, ho_Region29;
		Hobject  ho_RegionIntersection23, ho_RegionFillUp10, ho_ConnectedRegions39;
		Hobject  ho_SelectedRegions35, ho_ImageMax7, ho_ImageSub11;
		Hobject  ho_Regions4, ho_RegionIntersection21, ho_RegionFillUp14;
		Hobject  ho_RegionOpening25, ho_ConnectedRegions36, ho_SelectedRegions33;
		Hobject  ho_Region21, ho_RegionIntersection16, ho_RegionClosing9;
		Hobject  ho_ConnectedRegions29, ho_SelectedRegions23, ho_ImageMax6;
		Hobject  ho_ImageSub9, ho_Region25, ho_RegionClosing12, ho_RegionOpening22;
		Hobject  ho_RegionIntersection18, ho_RegionFillUp11, ho_ConnectedRegions32;
		Hobject  ho_BadPill, ho_ObjectSelected3, ho_Rectangle10;
		Hobject  ho_RegionErosion11, ho_ImageReduced19, ho_Lines1;
		Hobject  ho_UnionContours1, ho_SelectedXLD1, ho_Regions3;
		Hobject  ho_RegionIntersection19, ho_RegionFillUp12, ho_RegionOpening23;
		Hobject  ho_ConnectedRegions34, ho_SelectedRegions31, ho_RegionTrans13;
		Hobject  ho_Circle4, ho_RegionDifference22, ho_ConnectedRegions33;
		Hobject  ho_SelectedRegions29, ho_SelectedRegions28, ho_RegionTrans12;
		Hobject  ho_RegionArc, ho_RegionErosion6, ho_ImageReduced12;
		Hobject  ho_RegionFillUp6, ho_RegionOpening15, ho_ConnectedRegions22;
		Hobject  ho_SelectedRegions17, ho_Rectangle4, ho_Rectangle7;
		Hobject  ho_RegionErosion2, ho_ImageReduced1, ho_Region1;
		Hobject  ho_ConnectedRegions4, ho_SelectedRegions4, ho_RegionUnion3;
		Hobject  ho_RegionRed, ho_ImageReduced9, ho_Region15, ho_RegionIntersection6;
		Hobject  ho_RegionFillUp4, ho_RegionOpening9, ho_RegionErosion4;
		Hobject  ho_Region3, ho_RegionYellow, ho_RegionDilation2;
		Hobject  ho_RegionDilation3, ho_RegionCross, ho_ImageReduced2;
		Hobject  ho_ImageMax1, ho_ImageSub3, ho_Regions2, ho_Region2;
		Hobject  ho_RegionIntersection1, ho_RegionClosing1, ho_RegionOpening5;
		Hobject  ho_ConnectedRegions2, ho_SelectedRegions1, ho_ImageReduced5;
		Hobject  ho_ImageMax5, ho_ImageSub7, ho_Region6, ho_RegionErosion3;
		Hobject  ho_RegionDifference8, ho_RegionDifference9, ho_RegionOpening8;
		Hobject  ho_ConnectedRegions15, ho_SelectedRegions9, ho_ConnectedRegions7;
		Hobject  ho_Circle1, ho_ImageReduced3, ho_ImageMax2, ho_ImageSub;
		Hobject  ho_Region5, ho_Region8, ho_RegionIntersection3;
		Hobject  ho_RegionDifference7, ho_ConnectedRegions14, ho_RegionOpening3;
		Hobject  ho_SelectedRegions11, ho_ConnectedRegions10, ho_Circle2;
		Hobject  ho_ImageMax3, ho_ImageSub4, ho_Region9, ho_Region20;
		Hobject  ho_RegionErosion7, ho_RegionIntersection15, ho_RegionFillUp9;
		Hobject  ho_RegionOpening19, ho_ConnectedRegions28, ho_RegionTrans9;
		Hobject  ho_SelectedRegions22, ho_RegionUnion10, ho_RegionErosion8;
		Hobject  ho_RegionDifference18, ho_RegionIntersection13;
		Hobject  ho_ConnectedRegions24, ho_SelectedRegions19, ho_RegionUnion7;
		Hobject  ho_RegionClosing7, ho_ConnectedRegions23, ho_SelectedRegions18;
		Hobject  ho_ImageReduced13, ho_Regions1, ho_RegionClosing8;
		Hobject  ho_RegionFillUp7, ho_RegionOpening17, ho_ConnectedRegions25;
		Hobject  ho_SelectedRegions20, ho_RegionTrans8, ho_RegionUnion8;
		Hobject  ho_RegionDifference17, ho_RegionIntersection14;
		Hobject  ho_RegionFillUp8, ho_RegionOpening18, ho_ConnectedRegions26;
		Hobject  ho_SelectedRegions21, ho_RegionUnion9, ho_ConnectedRegions27;
		Hobject  ho_ConnectedRegions6, ho_Circle, ho_RegionIntersection24;
		Hobject  ho_RegionClosing15, ho_ConnectedRegions40, ho_SelectedRegions36;
		Hobject  ho_Region27, ho_RegionOpening26, ho_RegionClosing14;
		Hobject  ho_ConnectedRegions37, ho_SelectedRegions30, ho_RegionIntersection22;
		Hobject  ho_RegionTrans10, ho_RegionTrans14,ho_RegionUnion15;
		Hobject  ho_ImageSub13, ho_RegionErosion9;

		// Local control variables 
		HTuple  hv_check_pill, hv_check_capsule, hv_ImageFiles;
		HTuple  hv_Index, hv_BaseName, hv_Extension, hv_Directory;
		HTuple  hv_AreaBand, hv_ExpDefaultCtrlDummyVar, hv_Row17;
		HTuple  hv_Column17, hv_Phi5, hv_Length15, hv_Length25;
		HTuple  hv_Number18, hv_Area4, hv_Row9, hv_Column9, hv_Radius4;
		HTuple  hv_NumberPill, hv_Row14, hv_Column14, hv_Phi4, hv_Length14;
		HTuple  hv_Length24, hv_Row15, hv_Column15, hv_Row24, hv_Column24;
		HTuple  hv_Row11, hv_Column11, hv_Row21, hv_Column21, hv_Area1;
		HTuple  hv_Row7, hv_Column7, hv_Mean, hv_Number1, hv_Index1;
		HTuple  hv_Number5, hv_Index2, hv_Number6, hv_Row6, hv_Column6;
		HTuple  hv_Phi2, hv_Length12, hv_Length22, hv_Row, hv_Column;
		HTuple  hv_Phi, hv_Length1, hv_Length2, hv_Number3, hv_Row1;
		HTuple  hv_Column1, hv_Phi1, hv_Length11, hv_Length21, hv_Value;
		HTuple  hv_Value1, hv_ratio, hv_Number4, hv_Row5, hv_Column5;
		HTuple  hv_Radius3, hv_Row19, hv_Column19, hv_Radius6, hv_Number12;
		HTuple  hv_Number15, hv_Number17, hv_Area6, hv_Mean1, hv_Number13;
		HTuple  hv_Index4, hv_Area5, hv_Number14, hv_Row16, hv_Column16;
		HTuple  hv_Row25, hv_Column25, hv_Number9, hv_Row10, hv_Column10;
		HTuple  hv_Radius5, hv_Number11, hv_Number10, hv_Row8, hv_Column8;
		HTuple  hv_Phi3, hv_Length13, hv_Length23, hv_Area3, hv_Number;
		HTuple  hv_Row3, hv_Column3, hv_Radius1, hv_Area, hv_Row4;
		HTuple  hv_Column4, hv_Radius2, hv_Number8, hv_Row2, hv_Column2;
		HTuple  hv_Radius, hv_Number16, hv_Area7, hv_Row18, hv_Column18;
		HTuple  hv_Width, hv_Height;


		hv_check_pill = 1;
		hv_check_capsule = 0;

		//Image Acquisition 01: Code generated by Image Acquisition 01
		get_image_size(m_hoLiveImage, &hv_Width, &hv_Height);
		decompose3(m_hoLiveImage, &ho_R, &ho_G, &ho_B);
		trans_from_rgb(ho_R, ho_G, ho_B, &ho_H, &ho_S, &ho_V, "hsv");
		//***Ò©°å³ß´ç
		threshold(ho_R, &ho_Region, 100, 255);
		fill_up(ho_Region, &ho_RegionFillUp2);
		connection(ho_RegionFillUp2, &ho_ConnectedRegions);
		select_shape_std(ho_ConnectedRegions, &ho_RegionBand, "max_area", 70);
		area_center(ho_RegionBand, &hv_AreaBand, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_AreaBand < 870000))
		{
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÅÝÕÖ°åÈ±Ëð");
			result = QString::fromLocal8Bit("ÅÝÕÖ°åÈ±Ëð");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 80, 10);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "µ±Ç°ÅÝÕÖ°åÃæ»ýÎª£º" + hv_AreaBand);
			return 1;
		}

		smallest_rectangle2(ho_RegionBand, &hv_Row17, &hv_Column17, &hv_Phi5, &hv_Length15,
			&hv_Length25);
		gen_rectangle2(&ho_Rectangle11, hv_Row17, hv_Column17, hv_Phi5, hv_Length15 - 5,
			hv_Length25 - 5);
		difference(ho_Rectangle11, ho_RegionBand, &ho_RegionDifference19);
		opening_circle(ho_RegionDifference19, &ho_RegionOpening20, 1.5);
		connection(ho_RegionOpening20, &ho_ConnectedRegions30);
		select_shape(ho_ConnectedRegions30, &ho_SelectedRegions24, "area", "and", 2000,
			99999);
		count_obj(ho_SelectedRegions24, &hv_Number18);
		if (0 != (hv_Number18 > 0))
		{
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
			disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_SelectedRegions24, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÅÝÕÖ°åÈ±Ëð");
			result = QString::fromLocal8Bit("ÅÝÕÖ°åÈ±Ëð");
			return 1;
			// stop(); only in hdevelop
		}


		//*******ÂÁÄ£
		if (0 != m_checkparam.b_CheckPill)
		{
			threshold(ho_H, &ho_Region26, 230, 255);
			opening_circle(ho_Region26, &ho_RegionOpening24, 1.5);
			fill_up(ho_RegionOpening24, &ho_RegionFillUp); HTuple  hv_check_pill, hv_check_capsule, hv_ImageFiles;

			connection(ho_RegionFillUp, &ho_ConnectedRegions35);
			select_shape_std(ho_ConnectedRegions35, &ho_SelectedRegions32, "max_area",
				70);
			area_center(ho_SelectedRegions32, &hv_Area4, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
			if (0 != (hv_Area4 > 1000))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				disp_obj(ho_SelectedRegions32, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÂÁÄ£½Ó·ìÒì³£");
				result = QString::fromLocal8Bit("ÂÁÄ£½Ó·ìÒì³£");
				return 1;
				// stop(); only in hdevelop

			}
		}
		//********************

		//********************Ò©¼ÁÊýÁ¿
		erosion_circle(ho_RegionBand, &ho_RegionBand, 5.5);
		sub_image(ho_V, ho_B, &ho_ImageSub10, 1, 0);
		reduce_domain(ho_ImageSub10, ho_RegionBand, &ho_ImageReduced);
		threshold(ho_ImageReduced, &ho_Regions, 20, 140);
		intersection(ho_Regions, ho_RegionBand, &ho_RegionIntersection20);
		fill_up(ho_RegionIntersection20, &ho_RegionFillUp13);
		opening_circle(ho_RegionFillUp13, &ho_RegionOpening, 5.5);
		closing_circle(ho_RegionOpening, &ho_RegionClosing13, 5.5);
		connection(ho_RegionClosing13, &ho_ConnectedRegions1);
		select_shape(ho_ConnectedRegions1, &ho_SelectedRegions, "area", "and", 3000,
			99999);
		shape_trans(ho_SelectedRegions, &ho_RegionTrans14, "convex");
		smallest_circle(ho_RegionTrans14, &hv_Row9, &hv_Column9, &hv_Radius4);
		gen_circle(&ho_RegionPill, hv_Row9, hv_Column9, hv_Radius4);
		count_obj(ho_RegionPill, &hv_NumberPill);

		//****Æ¬¼Á
		if (0 != m_checkparam.b_CheckPill)
		{

			if (0 != (hv_NumberPill != 18))
			{

				union1(ho_RegionPill, &ho_RegionUnion13);
				dilation_circle(ho_RegionUnion13, &ho_RegionDilation6, 20.5);
				smallest_rectangle2(ho_RegionUnion13, &hv_Row14, &hv_Column14, &hv_Phi4,
					&hv_Length14, &hv_Length24);
				gen_rectangle2(&ho_Rectangle8, hv_Row14, hv_Column14, hv_Phi4, hv_Length14 + 30,
					hv_Length24 + 30);
				difference(ho_Rectangle8, ho_RegionDilation6, &ho_RegionDifference23);
				inner_rectangle1(ho_RegionDifference23, &hv_Row15, &hv_Column15, &hv_Row24,
					&hv_Column24);
				gen_rectangle1(&ho_Rectangle9, hv_Row15, hv_Column15, hv_Row24, hv_Column24);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");

				disp_obj(ho_Rectangle9, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "È±Æ¬");
				result = QString::fromLocal8Bit("È±Æ¬");
				return 1;
				// stop(); only in hdevelop
				;
			}

		}
		//****

		//****½ºÄÒ
		if (0 != m_checkparam.b_CheckCapsule)
		{
			if (0 != (HTuple(hv_NumberPill != 10).And(hv_NumberPill != 0)))
			{
				gen_empty_obj(&ho_EmptyPill);
				if (0 != (hv_NumberPill == 5))
				{
					union1(ho_RegionPill, &ho_RegionUnion5);
					shape_trans(ho_RegionUnion5, &ho_RegionTrans3, "rectangle2");
					difference(ho_RegionBand, ho_RegionTrans3, &ho_RegionDifference3);
					inner_rectangle1(ho_RegionDifference3, &hv_Row11, &hv_Column11, &hv_Row21,
						&hv_Column21);
					gen_rectangle1(&ho_EmptyPill, hv_Row11, hv_Column11, hv_Row21, hv_Column21);
				}
				else
				{
					gen_empty_obj(&ho_TopPills);
					gen_empty_obj(&ho_BottomPills);
					sort_region(ho_RegionPill, &ho_SortedRegions, "first_point", "true", "row");
					area_center(ho_SortedRegions, &hv_Area1, &hv_Row7, &hv_Column7);
					tuple_mean(hv_Row7, &hv_Mean);
					count_obj(ho_SortedRegions, &hv_Number1);
					{
						HTuple end_val115 = hv_Number1;
						HTuple step_val115 = 1;
						for (hv_Index1 = 1; hv_Index1.Continue(end_val115, step_val115); hv_Index1 += step_val115)
						{
							select_obj(ho_SortedRegions, &ho_ObjectSelected, hv_Index1);
							if (0 != ((hv_Row7.Select(hv_Index1 - 1)) < hv_Mean))
							{
								concat_obj(ho_ObjectSelected, ho_TopPills, &ho_TopPills);
							}
							else
							{
								concat_obj(ho_ObjectSelected, ho_BottomPills, &ho_BottomPills);
							}
						}
					}
					union1(ho_TopPills, &ho_TopPills);
					union1(ho_BottomPills, &ho_BottomPills);
					concat_obj(ho_TopPills, ho_BottomPills, &ho_RegionPill);
					count_obj(ho_RegionPill, &hv_Number5);
					{
						HTuple end_val127 = hv_Number5;
						HTuple step_val127 = 1;
						for (hv_Index2 = 1; hv_Index2.Continue(end_val127, step_val127); hv_Index2 += step_val127)
						{
							select_obj(ho_RegionPill, &ho_ObjectSelected1, hv_Index2);
							dilation_circle(ho_ObjectSelected1, &ho_RegionDilation1, 15);
							connection(ho_RegionDilation1, &ho_ConnectedRegions9);
							count_obj(ho_ConnectedRegions9, &hv_Number6);
							if (0 != (hv_Number6 != 5))
							{
								smallest_rectangle2(ho_ObjectSelected1, &hv_Row6, &hv_Column6, &hv_Phi2,
									&hv_Length12, &hv_Length22);
								gen_rectangle2(&ho_Rectangle2, hv_Row6, hv_Column6, hv_Phi2, hv_Length12 + 20,
									hv_Length22 + 20);
								difference(ho_Rectangle2, ho_RegionDilation1, &ho_RegionDifference2
								);
								shape_trans(ho_RegionDifference2, &ho_RegionTrans2, "inner_rectangle1");
								concat_obj(ho_EmptyPill, ho_RegionTrans2, &ho_EmptyPill);
							}
						}
					}
					union1(ho_EmptyPill, &ho_EmptyPill);
				}

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				disp_obj(ho_EmptyPill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "½ºÄÒ¸öÊý´íÎó");
				result = QString::fromLocal8Bit("½ºÄÒ¸öÊý´íÎó");
				return 1;
				//write_image (Image, 'bmp', 0, 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/Ô­Í¼_'+Index)
				//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/È±ÏÝÍ¼_'+Index)
				// stop(); only in hdevelop

			}
			else if (0 != (hv_NumberPill == 0))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÎÞ½ºÄÒ");
				result = QString::fromLocal8Bit("ÎÞ½ºÄÒ");
				return 1;
				//write_image (Image, 'bmp', 0, 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/Ô­Í¼_'+Index)
				//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/È±ÏÝÍ¼_'+Index)
				// stop(); only in hdevelop

			}
		}
		//**************************************



		//******************¼ì²âÅúºÅ
		shape_trans(ho_RegionPill, &ho_RegionTrans, "convex");
		union1(ho_RegionTrans, &ho_RegionUnion);
		smallest_rectangle2(ho_RegionUnion, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
			&hv_Length2);
		gen_rectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1 + 30, hv_Length2 + 1000);
		difference(ho_RegionBand, ho_Rectangle, &ho_RegionDifference);
		erosion_circle(ho_RegionDifference, &ho_RegionErosion, 10);
		connection(ho_RegionErosion, &ho_ConnectedRegions16);
		select_shape_std(ho_ConnectedRegions16, &ho_SelectedRegions10, "max_area", 70);
		gray_dilation_rect(ho_R, &ho_ImageMax, 11, 11);
		sub_image(ho_ImageMax, ho_R, &ho_ImageSub1, 1, 0);
		threshold(ho_ImageSub1, &ho_Region4, 15, 255);
		//dyn_threshold (ImageMax, G, RegionDynThresh, 10, 'light')
		opening_circle(ho_Region4, &ho_RegionOpening1, 1);
		intersection(ho_RegionOpening1, ho_SelectedRegions10, &ho_RegionIntersection);
		closing_circle(ho_RegionIntersection, &ho_RegionClosing, 5);
		connection(ho_RegionClosing, &ho_ConnectedRegions3);
		select_shape(ho_ConnectedRegions3, &ho_SelectedRegions2, "area", "and", 150,
			99999);
		shape_trans(ho_SelectedRegions2, &ho_RegionTrans1, "convex");
		union1(ho_RegionTrans1, &ho_RegionUnion1);
		count_obj(ho_RegionUnion1, &hv_Number3);
		if (0 != (hv_Number3 == 0))
		{

			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÎÞÅúºÅ");
			result = QString::fromLocal8Bit("ÎÞÅúºÅ");
			return 1;
			// stop(); only in hdevelop
			//write_image (Image, 'bmp', 0, 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/Ô­Í¼_'+Index)
			//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/È±ÏÝÍ¼_'+Index)
		}
		else
		{
			smallest_rectangle2(ho_RegionUnion1, &hv_Row1, &hv_Column1, &hv_Phi1, &hv_Length11,
				&hv_Length21);
			gen_rectangle2(&ho_Rectangle1, hv_Row1, hv_Column1, hv_Phi1, hv_Length11, hv_Length21);
			region_features(ho_Rectangle1, "ra", &hv_Value);
			region_features(ho_RegionBand, "rb", &hv_Value1);
			hv_ratio = hv_Value / hv_Value1;
			if (0 != (hv_ratio < 0.3))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÅúºÅÓ¡Ë¢²»ÇåÎú");
				result = QString::fromLocal8Bit("ÅúºÅÓ¡Ë¢²»ÇåÎú");
				return 1;
				// stop(); only in hdevelop
				//write_image (Image, 'bmp', 0, 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/Ô­Í¼_'+Index)
				//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/È±ÏÝÍ¼_'+Index)
			}
			intersection(ho_Rectangle1, ho_RegionBand, &ho_RegionIntersection9);
			reduce_domain(ho_ImageSub1, ho_RegionIntersection9, &ho_ImageReduced4);
			threshold(ho_ImageReduced4, &ho_Region11, 55, 255);
			opening_circle(ho_Region11, &ho_RegionOpening6, 1.5);
			connection(ho_RegionOpening6, &ho_ConnectedRegions8);
			select_shape(ho_ConnectedRegions8, &ho_SelectedRegions5, "area", "and", 50,
				99999);
			count_obj(ho_SelectedRegions5, &hv_Number4);
			if (0 != (hv_Number4 != 0))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				smallest_circle(ho_SelectedRegions5, &hv_Row5, &hv_Column5, &hv_Radius3);
				gen_circle(&ho_Circle3, hv_Row5, hv_Column5, hv_Radius3);

				disp_obj(ho_Circle3, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÅúºÅÈ±Ëð");
				result = QString::fromLocal8Bit("ÅúºÅÈ±Ëð");
				return 1;
				// stop(); only in hdevelop
				//write_image (Image, 'bmp', 0, 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/Ô­Í¼_'+Index)
				//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/È±ÏÝÍ¼_'+Index)
			}
			else
			{

				//disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				//disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			}

		}


		//***Ò©°åÈ±ÏÝ


		//***********ÅÝÕÖÒõÓ°ÇøÓò
		sub_image(ho_G, ho_S, &ho_ImageSub12, 1, 0);
		threshold(ho_ImageSub12, &ho_Region28, 0, 145);
		intersection(ho_Region28, ho_RegionBand, &ho_RegionIntersection17);
		difference(ho_RegionIntersection17, ho_Rectangle1, &ho_RegionDifference20);
		opening_circle(ho_RegionDifference20, &ho_RegionOpening21, 2.5);
		closing_circle(ho_RegionOpening21, &ho_RegionClosing10, 1.5);
		connection(ho_RegionClosing10, &ho_ConnectedRegions38);
		select_shape(ho_ConnectedRegions38, &ho_SelectedRegions34, "area", "and", 1500,
			99999);
		smallest_circle(ho_SelectedRegions34, &hv_Row19, &hv_Column19, &hv_Radius6);
		gen_circle(&ho_Circle5, hv_Row19, hv_Column19, hv_Radius6);
		difference(ho_Circle5, ho_RegionPill, &ho_RegionShadow);
		//*******************************
		dilation_circle(ho_Rectangle1, &ho_RegionBatch, 5.5);
		union2(ho_Circle5, ho_RegionBatch, &ho_RegionUnion14);
		shape_trans(ho_RegionBand, &ho_RegionTrans10, "convex");
		erosion_rectangle1(ho_RegionTrans10, &ho_RegionErosion9, 5, 1);
		difference(ho_RegionErosion9, ho_RegionUnion14, &ho_RegionDifference24);
		//erosion_circle(ho_RegionDifference24, &ho_RegionErosion9, 10.5);

		//opening_circle (RegionDifference15, RegionOpening16, 1.5)
		//difference (RegionOpening16, RegionUnion2, RegionDifference16)






		//opening_circle (Region18, RegionOpening13, 1.5)
		//connection (RegionOpening13, ConnectedRegions19)
		//select_shape (ConnectedRegions19, SelectedRegions16, 'area', 'and', 2000, 99999)
		//shape_trans (SelectedRegions16, RegionTrans6, 'convex')
		//erosion_circle (RegionTrans6, RegionErosion5, 2.5)

		//dilation_circle (RegionPill, RegionDilation, 3)
		//erosion_circle (RegionBand, RegionErosion1, 5)
		//union2 (RegionTrans7, Rectangle1, RegionUnion4)
		//difference (RegionErosion1, RegionUnion4, RegionLeft)
		//reduce_domain (B, RegionLeft, ImageLeft)


		//********Ò©°åÁÑÎÆ/Ã«·¢
		if (0 != m_checkparam.b_CheckPill)
		{
			reduce_domain(ho_R, ho_RegionDifference24, &ho_ImageReduced18);

			gray_opening_rect(ho_ImageReduced18, &ho_ImageOpening, 7, 7);
			lines_gauss(ho_ImageOpening, &ho_Lines3, 1.5, 2, 5, "dark", "true", "bar-shaped",
				"true");
			//calculate_lines_gauss_parameters(5, 50, &hv_Sigma, &hv_Low, &hv_High);
			//lines_gauss(ho_ImageReduced18, &ho_Lines, hv_Sigma, hv_Low, hv_High, "dark", "true", "bar-shaped",
			//	"true");
			union_adjacent_contours_xld(ho_Lines3, &ho_UnionContours, 10, 1, "attr_keep");
			select_shape_xld(ho_UnionContours, &ho_SelectedXLD, (HTuple("contlength").Append("circularity")),
				"and", (HTuple(30).Append(0)), (HTuple(99999).Append(0.1)));
			count_obj(ho_SelectedXLD, &hv_Number12);
			if (0 != (hv_Number12 > 0))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_SelectedXLD, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "Ò©°åÁÑÎÆ/Ã«·¢");
				result = QString::fromLocal8Bit("Ò©°åÁÑÎÆ/Ã«·¢");
				// stop(); only in hdevelop
				return 1;
			}

		}
		//*****************

		//********Ò©°åÔàÎÛ/ºÚµã
		if (0 != m_checkparam.b_CheckPill)
		{

			threshold(ho_ImageSub12, &ho_Region29, 0, 60);
			intersection(ho_Region29, ho_RegionDifference24, &ho_RegionIntersection23);
			fill_up(ho_RegionIntersection23, &ho_RegionFillUp10);
			connection(ho_RegionFillUp10, &ho_ConnectedRegions39);
			select_shape(ho_ConnectedRegions39, &ho_SelectedRegions35, "area", "and", 20,
				99999);

			//gray_dilation_rect(ho_B, &ho_ImageMax7, 7, 7);
			//sub_image(ho_ImageMax7, ho_B, &ho_ImageSub11, 1, 0);
			//threshold(ho_ImageSub11, &ho_Regions4, 30, 255);
			//intersection(ho_Regions4, ho_RegionErosion9, &ho_RegionIntersection21);
			//fill_up(ho_RegionIntersection21, &ho_RegionFillUp14);
			//opening_circle(ho_RegionFillUp14, &ho_RegionOpening25, 1.5);
			//connection(ho_RegionOpening25, &ho_ConnectedRegions36);
			//select_shape(ho_ConnectedRegions36, &ho_SelectedRegions33, (HTuple("area").Append("circularity")),
			//	"and", (HTuple(40).Append(0.4)), (HTuple(99999).Append(1)));
			count_obj(ho_SelectedRegions35, &hv_Number15);
			if (0 != (hv_Number15 > 0))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");

				disp_obj(ho_SelectedRegions35, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "Ò©°åÔàÎÛ/ºÚµã");
				result = QString::fromLocal8Bit("Ò©°åÔàÎÛ/ºÚµã");
				// stop(); only in hdevelop
				return 1;
			}

		}
		//*****************
		//*****************ÁÑÎÆ2
		if (m_checkparam.b_CheckPill)
		{
			threshold(ho_G, &ho_Region21, 0, 155);
			intersection(ho_Region21, ho_RegionDifference24, &ho_RegionIntersection16);
			closing_circle(ho_RegionIntersection16, &ho_RegionClosing9, 2.5);
			connection(ho_RegionClosing9, &ho_ConnectedRegions29);
			select_shape(ho_ConnectedRegions29, &ho_SelectedRegions23, (HTuple("area").Append("circularity")),
				"and", (HTuple(500).Append(0)), (HTuple(99999).Append(0.03)));
			count_obj(ho_SelectedRegions23, &hv_Number17);
			if (0 != (hv_Number17 > 0))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				disp_obj(ho_SelectedRegions23, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "Ò©°åÁÑÎÆ/Ã«·¢");
				result = QString::fromLocal8Bit("Ò©°åÁÑÎÆ/Ã«·¢");
				return 1;
				// stop(); only in hdevelop
			}
		}
	



		//*****Æ¬¼ÁÄÚ²¿È±ÏÝ
		union1(ho_RegionPill, &ho_RegionPill);
		gray_dilation_rect(ho_G, &ho_ImageMax6, 7, 7);
		sub_image(ho_ImageMax6, ho_G, &ho_ImageSub9, 1, 0);
		threshold(ho_ImageSub9, &ho_Region25, 25, 255);
		closing_circle(ho_Region25, &ho_RegionClosing12, 2.5);
		opening_circle(ho_RegionClosing12, &ho_RegionOpening22, 1.5);
		intersection(ho_RegionPill, ho_RegionOpening22, &ho_RegionIntersection18);
		fill_up(ho_RegionIntersection18, &ho_RegionFillUp11);
		connection(ho_RegionFillUp11, &ho_ConnectedRegions32);







		//**********°ëÆ¬
		if (0 != m_checkparam.b_CheckPill)
		{
			gen_empty_obj(&ho_BadPill);
			area_center(ho_SelectedRegions, &hv_Area6, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
			tuple_mean(hv_Area6, &hv_Mean1);
			count_obj(ho_SelectedRegions, &hv_Number13);
			{
				HTuple end_val270 = hv_Number13;
				HTuple step_val270 = 1;
				for (hv_Index4 = 1; hv_Index4.Continue(end_val270, step_val270); hv_Index4 += step_val270)
				{
					select_obj(ho_SelectedRegions, &ho_ObjectSelected3, hv_Index4);
					area_center(ho_ObjectSelected3, &hv_Area5, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
					if (0 != (((hv_Area5 - hv_Mean1).Abs()) > 5000))
					{
						concat_obj(ho_BadPill, ho_ObjectSelected3, &ho_BadPill);
					}
				}
			}
			count_obj(ho_BadPill, &hv_Number14);
			if (0 != (hv_Number14 > 0))
			{
				smallest_rectangle1(ho_BadPill, &hv_Row16, &hv_Column16, &hv_Row25, &hv_Column25);
				gen_rectangle1(&ho_Rectangle10, hv_Row16 - 30, hv_Column16 - 30, hv_Row25 + 30,
					hv_Column25 + 30);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				disp_obj(ho_Rectangle10, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "°ëÆ¬");
				result = QString::fromLocal8Bit("°ëÆ¬");
				return 1;
				// stop(); only in hdevelop
			}
		}

		//***************

		//***********ÁÑÆ¬
		if (0 != m_checkparam.b_CheckPill)
		{
			erosion_circle(ho_RegionTrans14, &ho_RegionErosion11, 5.5);
			union1(ho_RegionErosion11, &ho_RegionUnion15);
			reduce_domain(ho_G, ho_RegionUnion15, &ho_ImageReduced19);
			lines_gauss(ho_ImageReduced19, &ho_Lines1, 1.5, 1, 5, "dark", "true", "bar-shaped",
				"true");
			union_adjacent_contours_xld(ho_Lines1, &ho_UnionContours1, 10, 1, "attr_keep");
			select_shape_xld(ho_UnionContours1, &ho_SelectedXLD1, (HTuple("contlength").Append("circularity")),
				"and", (HTuple(20).Append(0)), (HTuple(1000).Append(0.2)));
			count_obj(ho_SelectedXLD1, &hv_Number9);
			if (0 != (hv_Number9 > 0))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);

				disp_obj(ho_SelectedXLD1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÁÑÆ¬");
				result = QString::fromLocal8Bit("ÁÑÆ¬");
				return 1;
				// stop(); only in hdevelop
			}
		}

		//*************

		//***********Æ¬¼ÁÈ±Ëð
		if (0 != m_checkparam.b_CheckPill)
		{
			smallest_rectangle2(ho_RegionPill, &hv_Row14, &hv_Column14, &hv_Phi4, &hv_Length14,
				&hv_Length24);
			gen_rectangle2(&ho_Rectangle8, hv_Row14, hv_Column14, hv_Phi4, hv_Length14 + 30,
				hv_Length24 + 30);
			sub_image(ho_B, ho_S, &ho_ImageSub13, 1, 0);
			threshold(ho_ImageSub13, &ho_Regions3, 0, 80);
			intersection(ho_Regions3, ho_Rectangle8, &ho_RegionIntersection19);
			fill_up(ho_RegionIntersection19, &ho_RegionFillUp12);
			opening_circle(ho_RegionFillUp12, &ho_RegionOpening23, 1.5);
			connection(ho_RegionOpening23, &ho_ConnectedRegions34);
			select_shape(ho_ConnectedRegions34, &ho_SelectedRegions31, "area", "and", 5000,
				99999);
			shape_trans(ho_SelectedRegions31, &ho_RegionTrans13, "convex");
			smallest_circle(ho_SelectedRegions31, &hv_Row10, &hv_Column10, &hv_Radius5);
			gen_circle(&ho_Circle4, hv_Row10, hv_Column10, hv_Radius5 - 5);
			difference(ho_Circle4, ho_SelectedRegions31, &ho_RegionDifference22);
			connection(ho_RegionDifference22, &ho_ConnectedRegions33);
			select_shape(ho_ConnectedRegions33, &ho_SelectedRegions29, (HTuple("area").Append("circularity")), "and",
				(HTuple(100).Append(0.1)), (HTuple(99999).Append(1)));
			count_obj(ho_SelectedRegions29, &hv_Number11);
			if (0 != (hv_Number11 > 0))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_SelectedRegions29, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "Æ¬¼ÁÈ±Ëð");
				result = QString::fromLocal8Bit("Æ¬¼ÁÈ±Ëð");
				// stop(); only in hdevelop
				return 1;
			}
		}





		//*************






		//*************ÔàÎÛ/ºÚµã
		if (0 != m_checkparam.b_CheckPill)
		{

			select_shape(ho_ConnectedRegions32, &ho_SelectedRegions28, (HTuple("area").Append("circularity")),
				"and", (HTuple(30).Append(0.25)), (HTuple(99999).Append(1)));
			count_obj(ho_SelectedRegions28, &hv_Number10);
			if (0 != (hv_Number10 != 0))
			{
				shape_trans(ho_SelectedRegions28, &ho_RegionTrans12, "convex");

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");

				disp_obj(ho_RegionTrans12, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "Æ¬¼ÁÔàÎÛ/ºÚµã");
				result = QString::fromLocal8Bit("Æ¬¼ÁÔàÎÛ/ºÚµã");
				return 1;
				// stop(); only in hdevelop
			}
		}

		//********




		//************************





		if (0 != m_checkparam.b_CheckCapsule)
		{
			//*****ÊÇ·ñ¼ì²â½ºÄÒÍ·Î²²¿
			gen_empty_obj(&ho_RegionArc);
			erosion_circle(ho_RegionBand, &ho_RegionErosion6, 7.5);
			reduce_domain(ho_B, ho_RegionErosion6, &ho_ImageReduced12);
			threshold(ho_ImageReduced12, &ho_Region18, 0, 120);
			fill_up(ho_Region18, &ho_RegionFillUp6);
			opening_circle(ho_RegionFillUp6, &ho_RegionOpening15, 1.5);
			connection(ho_RegionOpening15, &ho_ConnectedRegions22);
			select_shape(ho_ConnectedRegions22, &ho_SelectedRegions17, "area", "and", 2000,
				99999);
			shape_trans(ho_SelectedRegions17, &ho_RegionTrans7, "convex");
			//inner_rectangle1 (RegionTrans7, Row13, Column13, Row23, Column23)
			//gen_rectangle1 (Rectangle6, Row13, Column13, Row23, Column23)
			smallest_rectangle2(ho_RegionTrans7, &hv_Row8, &hv_Column8, &hv_Phi3, &hv_Length13,
				&hv_Length23);
			gen_rectangle2(&ho_Rectangle4, hv_Row8, hv_Column8, hv_Phi3, (hv_Length13 / hv_Length13) * 70,
				(hv_Length23 / hv_Length23) * 70);
			gen_rectangle2(&ho_Rectangle7, hv_Row8, hv_Column8, hv_Phi3, hv_Length13, hv_Length23);
			difference(ho_Rectangle7, ho_Rectangle4, &ho_RegionDifference15);








			//*****
			//***½ºÄÒÄÚ²¿È±ÏÝ
			union1(ho_RegionPill, &ho_RegionPill);
			erosion_circle(ho_RegionPill, &ho_RegionErosion2, 12.5);
			reduce_domain(ho_G, ho_RegionErosion2, &ho_ImageReduced1);
			//****ºìÉ«²¿·Ö½ºÄÒ
			threshold(ho_ImageReduced1, &ho_Region1, 0, 128);
			connection(ho_Region1, &ho_ConnectedRegions4);
			select_shape(ho_ConnectedRegions4, &ho_SelectedRegions4, "area", "and", 1500,
				99999);
			union1(ho_SelectedRegions4, &ho_RegionUnion3);
			fill_up(ho_RegionUnion3, &ho_RegionRed);

			reduce_domain(ho_S, ho_RegionPill, &ho_ImageReduced9);
			threshold(ho_ImageReduced9, &ho_Region15, 0, 160);
			intersection(ho_Region15, ho_RegionRed, &ho_RegionIntersection6);
			fill_up(ho_RegionIntersection6, &ho_RegionFillUp4);
			opening_circle(ho_RegionFillUp4, &ho_RegionOpening9, 5);
			erosion_circle(ho_RegionOpening9, &ho_RegionErosion4, 3.5);
			//****»ÆÉ«²¿·Ö½ºÄÒ
			threshold(ho_ImageReduced1, &ho_Region3, 0, 255);
			difference(ho_Region3, ho_RegionRed, &ho_RegionYellow);

			//***ºì»Æ·Ö½çÇøÓò
			dilation_circle(ho_RegionRed, &ho_RegionDilation2, 3);
			dilation_circle(ho_RegionYellow, &ho_RegionDilation3, 3);
			intersection(ho_RegionDilation2, ho_RegionDilation3, &ho_RegionCross);

			//****ºìÉ«½ºÄÒÈ±ÏÝ
			reduce_domain(ho_V, ho_RegionErosion4, &ho_ImageReduced2);
			gray_dilation_rect(ho_ImageReduced2, &ho_ImageMax1, 11, 11);
			sub_image(ho_ImageMax1, ho_ImageReduced2, &ho_ImageSub3, 1, 0);
			threshold(ho_ImageSub3, &ho_Regions2, 60, 255);
			threshold(ho_ImageReduced2, &ho_Region2, 0, 100);
			intersection(ho_Regions2, ho_Region2, &ho_RegionIntersection1);
			closing_circle(ho_RegionIntersection1, &ho_RegionClosing1, 1);
			opening_circle(ho_RegionClosing1, &ho_RegionOpening5, 1);
			connection(ho_RegionOpening5, &ho_ConnectedRegions2);
			select_shape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", 20,
				2000);

			reduce_domain(ho_S, ho_RegionErosion4, &ho_ImageReduced5);
			gray_dilation_rect(ho_ImageReduced5, &ho_ImageMax5, 5, 5);
			sub_image(ho_ImageMax5, ho_ImageReduced5, &ho_ImageSub7, 1, 0);
			//gray_erosion_rect (ImageReduced5, ImageMin, 5, 5)
			//sub_image (ImageReduced5, ImageMin, ImageSub2, 1, 0)
			threshold(ho_ImageSub7, &ho_Region6, 35, 255);

			erosion_circle(ho_RegionErosion4, &ho_RegionErosion3, 3.5);
			difference(ho_RegionOpening9, ho_RegionErosion3, &ho_RegionDifference8);
			difference(ho_Region6, ho_RegionDifference8, &ho_RegionDifference9);
			opening_circle(ho_RegionDifference9, &ho_RegionOpening8, 1.5);
			connection(ho_RegionOpening8, &ho_ConnectedRegions15);
			select_shape(ho_ConnectedRegions15, &ho_SelectedRegions9, "area", "and", 20,
				99999);

			//threshold (ImageReduced5, Region7, 120, 255)
			//intersection (Region6, Region7, RegionIntersection2)
			//opening_circle (RegionIntersection2, RegionOpening4, 1)
			//connection (RegionOpening4, ConnectedRegions5)
			//select_shape (ConnectedRegions5, SelectedRegions6, 'area', 'and', 20, 2000)

			union2(ho_SelectedRegions1, ho_SelectedRegions9, &ho_RegionUnion2);

			difference(ho_RegionUnion2, ho_RegionDifference15, &ho_RegionUnion2);

			area_center(ho_RegionUnion2, &hv_Area3, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
			count_obj(ho_RegionUnion2, &hv_Number);
			if (0 != (hv_Area3 > 0))
			{
				connection(ho_RegionUnion2, &ho_ConnectedRegions7);
				smallest_circle(ho_ConnectedRegions7, &hv_Row3, &hv_Column3, &hv_Radius1);
				gen_circle(&ho_Circle1, hv_Row3, hv_Column3, hv_Radius1);

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				disp_obj(ho_Circle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "½ºÄÒÄÚ²¿È±ÏÝ");
				result = QString::fromLocal8Bit("½ºÄÒÄÚ²¿È±ÏÝ");
				//write_image (Image, 'bmp', 0, 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/Ô­Í¼_'+Index)
				//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/È±ÏÝÍ¼_'+Index)
				// stop(); only in hdevelop
				return 1;
			}
			//***»ÆÉ«½ºÄÒÈ±ÏÝ

			reduce_domain(ho_ImageReduced1, ho_RegionYellow, &ho_ImageReduced3);
			gray_dilation_rect(ho_ImageReduced3, &ho_ImageMax2, 5, 5);
			sub_image(ho_ImageMax2, ho_ImageReduced3, &ho_ImageSub, 1, 0);
			threshold(ho_ImageSub, &ho_Region5, 20, 255);
			threshold(ho_ImageReduced3, &ho_Region8, 0, 185);
			intersection(ho_Region5, ho_Region8, &ho_RegionIntersection3);
			difference(ho_RegionIntersection3, ho_RegionCross, &ho_RegionDifference7);
			connection(ho_RegionDifference7, &ho_ConnectedRegions14);
			opening_circle(ho_RegionDifference7, &ho_RegionOpening3, 1.5);
			select_shape(ho_RegionOpening3, &ho_SelectedRegions11, "area", "and", 20, 99999);

			difference(ho_SelectedRegions11, ho_RegionDifference15, &ho_SelectedRegions11
			);


			area_center(ho_SelectedRegions11, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
			if (0 != (hv_Area > 0))
			{
				connection(ho_RegionOpening3, &ho_ConnectedRegions10);
				smallest_circle(ho_ConnectedRegions10, &hv_Row4, &hv_Column4, &hv_Radius2);
				gen_circle(&ho_Circle2, hv_Row4, hv_Column4, hv_Radius2);

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				disp_obj(ho_Circle2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "½ºÄÒÄÚ²¿È±ÏÝ");
				result = QString::fromLocal8Bit("½ºÄÒÄÚ²¿È±ÏÝ");
				return 1;
				//write_image (Image, 'bmp', 0, 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/Ô­Í¼_'+Index)
				//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/È±ÏÝÍ¼_'+Index)
				// stop(); only in hdevelop
			}
		}

		//***********ÒõÓ°ÇøÓòÈ±ÏÝ
		intersection(ho_Region29, ho_RegionShadow, &ho_RegionIntersection24);
		closing_circle(ho_RegionIntersection24, &ho_RegionClosing15, 2.5);
		connection(ho_RegionClosing15, &ho_ConnectedRegions40);
		select_shape(ho_ConnectedRegions40, &ho_SelectedRegions36, "area", "and", 20,
			99999);
		count_obj(ho_SelectedRegions36, &hv_Number16);
		if (0 != (hv_Number16 > 0))
		{
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_SelectedRegions36, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÔàÎÛ/ºÚµã");
			result = QString::fromLocal8Bit("ÔàÎÛ/ºÚµã");
			return 1;
			// stop(); only in hdevelop

		}
		//********************



		//ÍøÎÆ²»Çå
		if (0 != m_checkparam.b_CheckPill)
		{

			threshold(ho_R, &ho_Region27, 175, 255);
			opening_circle(ho_Region27, &ho_RegionOpening26, 1.5);
			closing_circle(ho_RegionOpening26, &ho_RegionClosing14, 3.5);
			connection(ho_RegionClosing14, &ho_ConnectedRegions37);
			select_shape(ho_ConnectedRegions37, &ho_SelectedRegions30, "area", "and", 10500,
				999999);



			threshold(ho_V, &ho_Region4, 185, 255);
			connection(ho_Region4, &ho_ConnectedRegions6);
			closing_circle(ho_ConnectedRegions6, &ho_RegionClosing1, 10);
			opening_circle(ho_RegionClosing1, &ho_RegionOpening3, 7.5);
			select_shape(ho_RegionOpening3, &ho_SelectedRegions4, "area", "and", 1500,
				999999);
			intersection(ho_SelectedRegions30, ho_SelectedRegions4, &ho_RegionIntersection22
			);
			area_center(ho_RegionIntersection22, &hv_Area7, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
			if (0 != (hv_Area7 > 10000))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_RegionIntersection22, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÍøÎÆ²»Çå/°×°å");
				result = QString::fromLocal8Bit("ÍøÎÆ²»Çå/°×°å");
				return 1;
				// stop(); only in hdevelop
			}
		}
		if (0 != m_checkparam.b_CheckCapsule)
		{
			gray_dilation_rect(ho_R, &ho_ImageMax3, 7, 7);
			sub_image(ho_ImageMax3, ho_R, &ho_ImageSub4, 1, 0);
			threshold(ho_ImageSub4, &ho_Region9, 45, 255);

			threshold(ho_G, &ho_Region20, 0, 170);
			erosion_circle(ho_RegionBand, &ho_RegionErosion7, 15.5);
			intersection(ho_Region20, ho_RegionErosion7, &ho_RegionIntersection15);
			fill_up(ho_RegionIntersection15, &ho_RegionFillUp9);
			opening_circle(ho_RegionFillUp9, &ho_RegionOpening19, 20.5);
			connection(ho_RegionOpening19, &ho_ConnectedRegions28);
			shape_trans(ho_ConnectedRegions28, &ho_RegionTrans9, "convex");
			select_shape(ho_RegionTrans9, &ho_SelectedRegions22, "area", "and", 15000,
				99999);
			union1(ho_SelectedRegions22, &ho_RegionUnion10);
			erosion_circle(ho_RegionUnion10, &ho_RegionErosion8, 5.5);
			difference(ho_Region9, ho_RegionErosion8, &ho_RegionDifference18);

			intersection(ho_RegionDifference18, ho_Rectangle4, &ho_RegionIntersection13
			);
			connection(ho_RegionIntersection13, &ho_ConnectedRegions24);
			select_shape(ho_ConnectedRegions24, &ho_SelectedRegions19, (HTuple("width").Append("height")),
				"and", (HTuple(5).Append(3)), (HTuple(99999).Append(99999)));
			union1(ho_SelectedRegions19, &ho_RegionUnion7);
			closing_circle(ho_RegionUnion7, &ho_RegionClosing7, 2.5);
			connection(ho_RegionClosing7, &ho_ConnectedRegions23);
			select_shape(ho_ConnectedRegions23, &ho_SelectedRegions18, "area", "and", 30,
				99999);



			reduce_domain(ho_R, ho_RegionErosion6, &ho_ImageReduced13);
			threshold(ho_ImageReduced13, &ho_Regions1, 0, 190);
			closing_circle(ho_Regions1, &ho_RegionClosing8, 1.5);
			fill_up(ho_RegionClosing8, &ho_RegionFillUp7);
			opening_circle(ho_RegionFillUp7, &ho_RegionOpening17, 3.5);
			connection(ho_RegionOpening17, &ho_ConnectedRegions25);
			select_shape(ho_ConnectedRegions25, &ho_SelectedRegions20, "area", "and", 1500,
				99999);
			shape_trans(ho_SelectedRegions20, &ho_RegionTrans8, "convex");
			union2(ho_RegionTrans8, ho_Rectangle1, &ho_RegionUnion8);
			difference(ho_RegionErosion6, ho_RegionUnion8, &ho_RegionDifference17);
			intersection(ho_RegionDifference17, ho_Region9, &ho_RegionIntersection14);
			fill_up(ho_RegionIntersection14, &ho_RegionFillUp8);
			opening_circle(ho_RegionFillUp8, &ho_RegionOpening18, 1.5);
			connection(ho_RegionOpening18, &ho_ConnectedRegions26);
			select_shape(ho_ConnectedRegions26, &ho_SelectedRegions21, "area", "and", 30,
				99999);
			union2(ho_SelectedRegions18, ho_SelectedRegions21, &ho_RegionUnion9);
			connection(ho_RegionUnion9, &ho_ConnectedRegions27);
			count_obj(ho_ConnectedRegions27, &hv_Number8);


			//difference (Region9, RegionDilation, RegionDifference6)
			//fill_up (RegionDifference6, RegionFillUp3)
			//closing_circle (RegionFillUp3, RegionClosing3, 1.5)
			//connection (RegionClosing3, ConnectedRegions13)
			//select_shape (ConnectedRegions13, SelectedRegions8, ['width','height','area'], 'and', [1,3,20], [100,100,9999])
			//count_obj (SelectedRegions8, Number2)
			//difference (RegionTrans6, RegionPill, RegionDifference13)
			//dilation_circle (RegionDifference13, RegionDilation4, 5)
			//intersection (RegionDilation4, SelectedRegions8, RegionIntersection12)
			//opening_circle (RegionIntersection12, RegionOpening14, 1.5)
			//difference (SelectedRegions8, RegionOpening14, RegionDifference14)




			//fill_up (Region9, RegionFillUp1)
			//threshold (ImageLeft, Region10, 0, 140)
			//intersection (RegionFillUp1, Region10, RegionIntersection4)
			//opening_circle (RegionIntersection4, RegionOpening2, 1)
			//connection (RegionOpening2, ConnectedRegions6)
			//select_shape (ConnectedRegions6, SelectedRegions3, 'area', 'and', 20, 99999)
			//count_obj (SelectedRegions3, Number2)
			if (0 != (hv_Number8 != 0))
			{
				smallest_circle(ho_ConnectedRegions27, &hv_Row2, &hv_Column2, &hv_Radius);
				gen_circle(&ho_Circle, hv_Row2, hv_Column2, hv_Radius);

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				disp_obj(ho_Circle, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "Ò©°åÈ±ÏÝ");
				result = QString::fromLocal8Bit("Ò©°åÈ±ÏÝ");
				//write_image (Image, 'bmp', 0, 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/Ô­Í¼_'+Index)
				//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/ºì»Æ½ºÄÒ/23428868/NGÍ¼/È±ÏÝÍ¼_'+Index)
				// stop(); only in hdevelop
				return 1;
			}
		}

		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		shape_trans(ho_RegionBand, &ho_RegionBand, "convex");
		disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//disp_obj(ho_PillRegions, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//disp_obj(ho_RegionIntersection4, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "²úÆ·Õý³£");
		//disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		result = "Good";
		return 0;


	}
	catch (HException &e)
	{
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "red");
		set_tposition(m_ShowLabel[total_check%circle_times], 100, 20);
		write_string(m_ShowLabel[total_check%circle_times], e.message);
	}
	catch (Exception e)
	{
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "red");
		set_tposition(m_ShowLabel[total_check%circle_times], 100, 20);
		write_string(m_ShowLabel[total_check%circle_times], e.what());
	}
	catch (...)
	{
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "red");
		set_tposition(m_ShowLabel[total_check%circle_times], 100, 20);
		write_string(m_ShowLabel[total_check%circle_times], "Other Error without catch");
	}
}