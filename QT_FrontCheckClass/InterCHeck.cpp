#include "InterCHeck.h" 
#include <QMessageBox>
//#include "image.h"
QString AppPath;
bool CInterCHeck::LoadCheckParam(CHECKPARAM * checkparam)
{
	QSettings configIniRead(AppPath + "\\DefaultModel\\CheckParam.ini", QSettings::IniFormat);
	configIniRead.setIniCodec("UTF-8");
	QString cameraname = checkparam->c_CameraName;
	//¶ÁÈ¡¼ì²â²ÎÊýÅäÖÃ
	//////////////////////////////////////////////////////////////////////////
	//checkparam->i_BandArea = configIniRead.value("/" + cameraname + "/BandArea", "865000").toInt();
	//checkparam->i_BandException1_Area = configIniRead.value("/" + cameraname + "/BandException1_Area", "300").toInt();
	//checkparam->i_BandException2_Value = configIniRead.value("/" + cameraname + "/BandException2_Value", "110").toInt();
	//checkparam->i_BandException3_Area = configIniRead.value("/" + cameraname + "/BandException3_Area", "50").toInt();
	//checkparam->i_InterException_Area = configIniRead.value("/" + cameraname + "/InterException_Area", "50").toInt();
	//checkparam->d_InterException_Open = configIniRead.value("/" + cameraname + "/InterException_Open", "1.5").toDouble();
	//checkparam->i_PillRadius = configIniRead.value("/" + cameraname + "/PillRadius", "85").toInt();
	//checkparam->i_BandException2_Radius = configIniRead.value("/" + cameraname + "/BandException2_Radius", "0").toInt();
	
	checkparam->i_BandChannel = configIniRead.value("/" + cameraname + "/BandChannel", "0").toInt();
	checkparam->i_MinGray_Band = configIniRead.value("/" + cameraname + "/MinGray_Band", "100").toInt();
	checkparam->i_MinArea_Band = configIniRead.value("/" + cameraname + "/MinArea_Band", "740000").toInt();
	checkparam->i_CapsulesChannel = configIniRead.value("/" + cameraname + "/CapsulesChannel", "4").toInt();
	checkparam->i_MinGray_Capsules = configIniRead.value("/" + cameraname + "/MinGray_Capsules", "70").toInt();
	checkparam->d_OR_Capsules = configIniRead.value("/" + cameraname + "/OR_Capsules", "2.5").toDouble();
	checkparam->i_MinArea_Capsules = configIniRead.value("/" + cameraname + "/MinArea_Capsules", "10000").toInt();
	checkparam->i_MaxArea_Capsules = configIniRead.value("/" + cameraname + "/MaxArea_Capsules", "99999").toInt();
	checkparam->i_Number_Capsules = configIniRead.value("/" + cameraname + "/Number_Capsules", "10").toInt();
	checkparam->i_BatchChannel = configIniRead.value("/" + cameraname + "/BatchChannel", "0").toInt();
	checkparam->i_LengthAdd_1 = configIniRead.value("/" + cameraname + "/LengthAdd_1", "30").toInt();
	checkparam->i_LengthAdd_2 = configIniRead.value("/" + cameraname + "/LengthAdd_2", "1000").toInt();
	checkparam->d_ER_Batch = configIniRead.value("/" + cameraname + "/ER_Batch", "10").toDouble();
	checkparam->i_MaskHeight_Batch = configIniRead.value("/" + cameraname + "/MaksHeight_Batch", "11").toInt();
	checkparam->i_MaskWidth_Batch = configIniRead.value("/" + cameraname + "/MaksWidth_Batch", "11").toInt();
	checkparam->i_MinGray_Batch = configIniRead.value("/" + cameraname + "/MinGray_Batch", "15").toInt();
	checkparam->d_OR_Batch = configIniRead.value("/" + cameraname + "/OR_Batch", "1").toDouble();
	checkparam->d_CR_Batch = configIniRead.value("/" + cameraname + "/CR_Batch", "5").toDouble();
	checkparam->i_MinArea_Batch = configIniRead.value("/" + cameraname + "/MinArea_Batch", "150").toInt();
	checkparam->i_MaxArea_Batch = configIniRead.value("/" + cameraname + "/MaxArea_Batch", "99999").toInt();
	//checkparam->i_MinArea_BatchDefect = configIniRead.value("/" + cameraname + "/MinArea_BatchDefect", "50").toInt();
	//checkparam->i_MaxArea_BatchDefect = configIniRead.value("/" + cameraname + "/MaxArea_BatchDefect", "99999").toInt();
	checkparam->i_Channel_CapsulesDefect = configIniRead.value("/" + cameraname + "/Channel_CapsulesDefect", "1").toInt();
	checkparam->d_ER_RegionCapsules = configIniRead.value("/" + cameraname + "/ER_RegionCapsules", "12.5").toDouble();
	checkparam->i_MaxGray_CapsulesRed = configIniRead.value("/" + cameraname + "/MaxGray_CapsulesRed", "128").toInt();
	checkparam->i_MinArea_CapsulesRed = configIniRead.value("/" + cameraname + "/MinArea_CapsulesRed", "1500").toInt();
	checkparam->i_MaxArea_CapsulesRed = configIniRead.value("/" + cameraname + "/MaxArea_CapsulesRed", "99999").toInt();
	checkparam->d_DR_CapsulesRed = configIniRead.value("/" + cameraname + "/DR_CapsulesRed", "3").toDouble();
	checkparam->d_DR_CapsulesYellow = configIniRead.value("/" + cameraname + "/DR_CapsulesYellow", "3").toDouble();
	checkparam->i_Channel_RegionRed = configIniRead.value("/" + cameraname + "/Channel_RegionRed", "4").toInt();
	checkparam->i_MaskHeight_RegionRed = configIniRead.value("/" + cameraname + "/MaksHeight_RegionRed", "11").toInt();
	checkparam->i_MaskWidth_RegionRed = configIniRead.value("/" + cameraname + "/MaksWidth_RegionRed", "11").toInt();
	checkparam->i_MinGray_RegionRed = configIniRead.value("/" + cameraname + "/MinGray_RegionRed", "60").toInt();
	checkparam->i_MaxGray_RegionRed = configIniRead.value("/" + cameraname + "/MaxGray_RegionRed", "100").toInt();
	checkparam->d_CR_RegionRed = configIniRead.value("/" + cameraname + "/CR_RegionRed", "1").toDouble();
	checkparam->d_OR_RegionRed = configIniRead.value("/" + cameraname + "/OR_RegionRed", "1").toDouble();
	checkparam->i_MinArea_RegionRed = configIniRead.value("/" + cameraname + "/MinArea_RegionRed", "20").toInt();
	checkparam->i_MaxArea_RegionRed = configIniRead.value("/" + cameraname + "/MaxArea_RegionRed", "2000").toInt();
	checkparam->i_Channel_RedDefect1 = configIniRead.value("/" + cameraname + "/Channel_RedDefect1", "5").toInt();
	checkparam->i_Channel_RedDefect2 = configIniRead.value("/" + cameraname + "/Channel_RedDefect2", "4").toInt();
	checkparam->i_MaskHeight_RedDefect = configIniRead.value("/" + cameraname + "/MaksHeight_RedDefect", "5").toInt();
	checkparam->i_MaskWidth_RedDefect = configIniRead.value("/" + cameraname + "/MaksWidth_RedDefect", "5").toInt();
	checkparam->i_MinGray_RedDefect = configIniRead.value("/" + cameraname + "/MinGray_RedDefect", "35").toInt();
	checkparam->d_ER_RedDefect = configIniRead.value("/" + cameraname + "/ER_RedDefect", "3.5").toDouble();
	checkparam->d_OR_RedDefect = configIniRead.value("/" + cameraname + "/OR_RedDefect", "1.5").toDouble();
	checkparam->i_MinArea_RedDefect = configIniRead.value("/" + cameraname + "/MinArea_RedDefect", "10").toInt();
	checkparam->i_MaxArea_RedDefect = configIniRead.value("/" + cameraname + "/MaxArea_RedDefect", "99999").toInt();
	checkparam->i_MaskHeight_RegionYellow = configIniRead.value("/" + cameraname + "/MaskHeight_RegionYellow", "5").toInt();
	checkparam->i_MaskWidth_RegionYellow = configIniRead.value("/" + cameraname + "/MaskWidth_RegionYellow", "5").toInt();
	checkparam->i_MinGray_RegionYellow = configIniRead.value("/" + cameraname + "/MinGray_RegionYellow", "20").toInt();
	checkparam->i_MaxGray_RegionYellow = configIniRead.value("/" + cameraname + "/MaxGray_RegionYellow", "185").toInt();
	checkparam->d_OR_RegionYellow = configIniRead.value("/" + cameraname + "/OR_RegionYellow", "1.5").toDouble();
	checkparam->i_MinArea_RegionYellow = configIniRead.value("/" + cameraname + "/MinArea_RegionYellow", "10").toInt();
	checkparam->i_MaxArea_RegionYellow = configIniRead.value("/" + cameraname + "/MaxArea_RegionYellow", "99999").toInt();
	checkparam->i_Channel_BandDefect = configIniRead.value("/" + cameraname + "/Channel_BandDefect", "2").toInt();
	checkparam->d_DR_BandDefect = configIniRead.value("/" + cameraname + "/DR_BandDefect", "3").toDouble();
	checkparam->d_ER_BandDefect = configIniRead.value("/" + cameraname + "/ER_BandDefect", "5").toDouble();
	checkparam->i_MaskHeight_BandDefect = configIniRead.value("/" + cameraname + "/MaskHeight_BandDefect", "3").toInt();
	checkparam->i_MaskWidth_BandDefect = configIniRead.value("/" + cameraname + "/MaskWidth_BandDefect", "3").toInt();
	checkparam->i_MinGray_BandDefect = configIniRead.value("/" + cameraname + "/MinGray_BandDefect", "45").toInt();
	checkparam->d_CR_BandDefect = configIniRead.value("/" + cameraname + "/CR_BandDefect", "1.5").toDouble();
	checkparam->i_MinWidth_BandDefect = configIniRead.value("/" + cameraname + "/MinWidth_BandDefect", "1").toInt();
	checkparam->i_MaxWidth_BandDefect = configIniRead.value("/" + cameraname + "/MaxWidth_BandDefect", "100").toInt();
	checkparam->i_MinHeight_BandDefect = configIniRead.value("/" + cameraname + "/MinHeight_BandDefect", "3").toInt();
	checkparam->i_MaxHeight_BandDefect = configIniRead.value("/" + cameraname + "/MaxHeight_BandDefect", "100").toInt();
	checkparam->i_MinArea_BandDefect = configIniRead.value("/" + cameraname + "/MinArea_BandDefect", "10").toInt();
	checkparam->i_MaxArea_BandDefect = configIniRead.value("/" + cameraname + "/MaxArea_BandDefect", "9999").toInt();









	//////////////////////////////////////////////////////////////////////////
	return false;
}
bool CInterCHeck::SaveCheckParam(CHECKPARAM * checkparam)
{
	m_bchangedparam = true;
	m_checkparam = *checkparam;
	QSettings configIniRead(AppPath + "\\DefaultModel\\CheckParam.ini", QSettings::IniFormat);
	QString cameraname = checkparam->c_CameraName;
	QString sss = configIniRead.value("/" + cameraname + "/OperateCore", "NoRead").toString();
	strcpy(checkparam->c_OperateCore, configIniRead.value("/" + cameraname + "/OperateCore", "NoRead").toString().toStdString().c_str());
	//Ð´Èë¼ì²â²ÎÊýÅäÖÃ
	//////////////////////////////////////////////////////////////////////////
	//configIniRead.setValue("/" + cameraname + "/BandArea", checkparam->i_BandArea);
	//configIniRead.setValue("/" + cameraname + "/BandException1_Area", checkparam->i_BandException1_Area);
	//configIniRead.setValue("/" + cameraname + "/BandException2_Value", checkparam->i_BandException2_Value);
	//configIniRead.setValue("/" + cameraname + "/BandException3_Area", checkparam->i_BandException3_Area);
	//configIniRead.setValue("/" + cameraname + "/InterException_Area", checkparam->i_InterException_Area);
	//configIniRead.setValue("/" + cameraname + "/InterException_Open", checkparam->d_InterException_Open);
	//configIniRead.setValue("/" + cameraname + "/PillRadius", checkparam->i_PillRadius);
	//configIniRead.setValue("/" + cameraname + "/BandException2_Radius", checkparam->i_BandException2_Radius);
	configIniRead.setValue("/" + cameraname + "/BandChannel", checkparam->i_BandChannel);
	configIniRead.setValue("/" + cameraname + "/MinGray_Band", checkparam->i_MinGray_Band);
	configIniRead.setValue("/" + cameraname + "/MinArea_Band", checkparam->i_MinArea_Band);
	configIniRead.setValue("/" + cameraname + "/CapsulesChannel", checkparam->i_CapsulesChannel);
	configIniRead.setValue("/" + cameraname + "/MinGray_Capsules", checkparam->i_MinGray_Capsules);
	configIniRead.setValue("/" + cameraname + "/OR_Capsules", checkparam->d_OR_Capsules);
	configIniRead.setValue("/" + cameraname + "/MaxArea_Capsules", checkparam->i_MaxArea_Capsules);
	configIniRead.setValue("/" + cameraname + "/Number_Capsules", checkparam->i_Number_Capsules);
	configIniRead.setValue("/" + cameraname + "/BatchChannel", checkparam->i_BatchChannel);
	configIniRead.setValue("/" + cameraname + "/LengthAdd_1", checkparam->i_LengthAdd_1);
	configIniRead.setValue("/" + cameraname + "/LengthAdd_2", checkparam->i_LengthAdd_2);
	configIniRead.setValue("/" + cameraname + "/ER_Batch", checkparam->d_ER_Batch);
	configIniRead.setValue("/" + cameraname + "/MaskHeight_Batch", checkparam->i_MaskHeight_Batch);
	configIniRead.setValue("/" + cameraname + "/MaskWidth_Batch", checkparam->i_MaskWidth_Batch);
	configIniRead.setValue("/" + cameraname + "/MinGray_Batch", checkparam->i_MinGray_Batch);
	configIniRead.setValue("/" + cameraname + "/OR_Batch", checkparam->d_OR_Batch);
	configIniRead.setValue("/" + cameraname + "/CR_Batch", checkparam->d_CR_Batch);
	configIniRead.setValue("/" + cameraname + "/MinArea_Batch", checkparam->i_MinArea_Batch);
	configIniRead.setValue("/" + cameraname + "/MaxArea_Batch", checkparam->i_MaxArea_Batch);
	//configIniRead.setValue("/" + cameraname + "/MinArea_BatchDefect", checkparam->i_MinArea_BatchDefect);
	//configIniRead.setValue("/" + cameraname + "/MaxArea_BatchDefect", checkparam->i_MaxArea_BatchDefect);
	configIniRead.setValue("/" + cameraname + "/Channel_CapsulesDefect", checkparam->i_Channel_CapsulesDefect);
	configIniRead.setValue("/" + cameraname + "/ER_RegionCapsules", checkparam->d_ER_RegionCapsules);
	configIniRead.setValue("/" + cameraname + "/MaxGray_CapsulesRed", checkparam->i_MaxGray_CapsulesRed);
	configIniRead.setValue("/" + cameraname + "/MinArea_CapsulesRed", checkparam->i_MinArea_CapsulesRed);
	configIniRead.setValue("/" + cameraname + "/MaxArea_CapsulesRed", checkparam->i_MaxArea_CapsulesRed);
	configIniRead.setValue("/" + cameraname + "/DR_CapsulesRed", checkparam->d_DR_CapsulesRed);
	configIniRead.setValue("/" + cameraname + "/DR_CapsulesYellow", checkparam->d_DR_CapsulesYellow);
	configIniRead.setValue("/" + cameraname + "/Channel_RegionRed", checkparam->i_Channel_RegionRed);
	configIniRead.setValue("/" + cameraname + "/MaskHeight_RegionRed", checkparam->i_MaskHeight_RegionRed);
	configIniRead.setValue("/" + cameraname + "/MaskWidth_RegionRed", checkparam->i_MaskWidth_RegionRed);
	configIniRead.setValue("/" + cameraname + "/MinGray_RegionRed", checkparam->i_MinGray_RegionRed);
	configIniRead.setValue("/" + cameraname + "/MaxGray_RegionRed", checkparam->i_MaxGray_RegionRed);
	configIniRead.setValue("/" + cameraname + "/CR_RegionRed", checkparam->d_CR_RegionRed);
	configIniRead.setValue("/" + cameraname + "/OR_RegionRed", checkparam->d_OR_RegionRed);
	configIniRead.setValue("/" + cameraname + "/MinArea_RegionRed", checkparam->i_MinArea_RegionRed);
	configIniRead.setValue("/" + cameraname + "/MaxArea_RegionRed", checkparam->i_MaxArea_RegionRed);
	configIniRead.setValue("/" + cameraname + "/Channel_RedDefect1", checkparam->i_Channel_RedDefect1);
	configIniRead.setValue("/" + cameraname + "/Channel_RedDefect2", checkparam->i_Channel_RedDefect2);
	configIniRead.setValue("/" + cameraname + "/MaskHeight_RedDefect", checkparam->i_MaskHeight_RedDefect);
	configIniRead.setValue("/" + cameraname + "/MaskWidth_RedDefect", checkparam->i_MaskWidth_RedDefect);
	configIniRead.setValue("/" + cameraname + "/MinGray_RedDefect", checkparam->i_MinGray_RedDefect);
	configIniRead.setValue("/" + cameraname + "/ER_RedDefect", checkparam->d_ER_RedDefect);
	configIniRead.setValue("/" + cameraname + "/OR_RedDefect", checkparam->d_OR_RedDefect);
	configIniRead.setValue("/" + cameraname + "/MinArea_RedDefect", checkparam->i_MinArea_RedDefect);
	configIniRead.setValue("/" + cameraname + "/MaxArea_RedDefect", checkparam->i_MaxArea_RedDefect);
	configIniRead.setValue("/" + cameraname + "/MaskHeight_RegionYellow", checkparam->i_MaskHeight_RegionYellow);
	configIniRead.setValue("/" + cameraname + "/MaskWidth_RegionYellow", checkparam->i_MaskWidth_RegionYellow);
	configIniRead.setValue("/" + cameraname + "/MinGray_RegionYellow", checkparam->i_MinGray_RegionYellow);
	configIniRead.setValue("/" + cameraname + "/MaxGray_RegionYellow", checkparam->i_MaxGray_RegionYellow);
	configIniRead.setValue("/" + cameraname + "/OR_RegionYellow", checkparam->d_OR_RegionYellow);
	configIniRead.setValue("/" + cameraname + "/MinArea_RegionYellow", checkparam->i_MinArea_RegionYellow);
	configIniRead.setValue("/" + cameraname + "/MaxArea_RegionYellow", checkparam->i_MaxArea_RegionYellow);
	configIniRead.setValue("/" + cameraname + "/Channel_BandDefect", checkparam->i_Channel_BandDefect);
	configIniRead.setValue("/" + cameraname + "/DR_BandDefect", checkparam->d_DR_BandDefect);
	configIniRead.setValue("/" + cameraname + "/ER_BandDefect", checkparam->d_ER_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MaskHeight_BandDefect", checkparam->i_MaskHeight_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MaskWidth_BandDefect", checkparam->i_MaskWidth_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MinGray_BandDefect", checkparam->i_MinGray_BandDefect);
	configIniRead.setValue("/" + cameraname + "/CR_BandDefect", checkparam->d_CR_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MinWidth_BandDefect", checkparam->i_MinWidth_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MaxWidth_BandDefect", checkparam->i_MaxWidth_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MinHeight_BandDefect", checkparam->i_MinHeight_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MaxHeight_BandDefect", checkparam->i_MaxHeight_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MinArea_BandDefect", checkparam->i_MinArea_BandDefect);
	configIniRead.setValue("/" + cameraname + "/MaxArea_BandDefect", checkparam->i_MaxArea_BandDefect);
	//////////////////////////////////////////////////////////////////////////
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
	QObject::connect(setdlg, SIGNAL(ShouldSaveParam(CHECKPARAM*)), this, SLOT(SaveCheckParam(CHECKPARAM*)));
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
int CInterCHeck::RealCheck(QString &result, CHECKPARAM *checkparam, int Wnd = -1)
{
	int i_error = 0, i_null = 0;
	try
	{
		Hobject  ho_Image;
		Hobject  ho_V, ho_Region, ho_RegionFillUp2, ho_ConnectedRegions;
		Hobject  ho_RegionBand, ho_ImageReduced, ho_Regions, ho_RegionOpening;
		Hobject  ho_RegionFillUp, ho_ConnectedRegions1, ho_SelectedRegions;
		Hobject  ho_RegionPill, ho_EmptyPill, ho_RegionUnion5, ho_RegionTrans3;
		Hobject  ho_RegionDifference3, ho_TopPills, ho_BottomPills;
		Hobject  ho_SortedRegions, ho_ObjectSelected, ho_ObjectSelected1;
		Hobject  ho_RegionDilation1, ho_ConnectedRegions9, ho_Rectangle2;
		Hobject  ho_RegionDifference2, ho_RegionTrans2, ho_RegionTrans;
		Hobject  ho_RegionUnion, ho_Rectangle, ho_RegionDifference;
		Hobject  ho_RegionErosion, ho_ConnectedRegions16, ho_SelectedRegions10;
		Hobject  ho_ImageMax, ho_ImageSub1, ho_Region4, ho_RegionOpening1;
		Hobject  ho_RegionIntersection, ho_RegionClosing, ho_ConnectedRegions3;
		Hobject  ho_SelectedRegions2, ho_RegionTrans1, ho_RegionUnion1;
		Hobject  ho_Rectangle1, ho_ImageReduced4, ho_Region11, ho_RegionOpening6;
		Hobject  ho_ConnectedRegions8, ho_SelectedRegions5, ho_Circle3;
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
		Hobject  ho_ConnectedRegions15, ho_SelectedRegions9, ho_RegionUnion2;
		Hobject  ho_ConnectedRegions7, ho_Circle1, ho_ImageReduced3;
		Hobject  ho_ImageMax2, ho_ImageSub, ho_Region5, ho_Region8;
		Hobject  ho_RegionIntersection3, ho_RegionDifference7, ho_ConnectedRegions14;
		Hobject  ho_RegionOpening3, ho_SelectedRegions11, ho_ConnectedRegions10;
		Hobject  ho_Circle2, ho_RegionDilation, ho_RegionErosion1;
		Hobject  ho_RegionUnion4, ho_RegionLeft, ho_ImageLeft, ho_ImageMax3;
		Hobject  ho_ImageSub4, ho_Region9, ho_RegionDifference6;
		Hobject  ho_RegionFillUp3, ho_RegionClosing3, ho_ConnectedRegions13;
		Hobject  ho_SelectedRegions8, ho_Circle, ho_ImageReduced7;
		Hobject  ho_ImageOpening, ho_ImageSub5, ho_ImageResult, ho_ImageSub6;
		Hobject  ho_Region13, ho_Region12, ho_ConnectedRegions11;
		Hobject  ho_RegionTrans4, ho_RegionIntersection5, ho_RegionDifference1;
		Hobject  ho_ImageReduced6, ho_ImageMax4, ho_RegionDynThresh1, ho_RegionIntersection9;


		// Local control variables 
		HTuple  hv_ImageFiles, hv_Index, hv_BaseName;
		HTuple  hv_Extension, hv_Directory, hv_AreaBand, hv_ExpDefaultCtrlDummyVar;
		HTuple  hv_NumberPill, hv_Row11, hv_Column11, hv_Row21;
		HTuple  hv_Column21, hv_Area1, hv_Row7, hv_Column7, hv_Mean;
		HTuple  hv_Number1, hv_Index1, hv_Number5, hv_Index2, hv_Number6;
		HTuple  hv_Row6, hv_Column6, hv_Phi2, hv_Length12, hv_Length22;
		HTuple  hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2;
		HTuple  hv_Number3, hv_Row1, hv_Column1, hv_Phi1, hv_Length11;
		HTuple  hv_Length21, hv_Value, hv_Value1, hv_ratio, hv_Number4;
		HTuple  hv_Row5, hv_Column5, hv_Radius3, hv_Number, hv_Row3;
		HTuple  hv_Column3, hv_Radius1, hv_Area, hv_Row4, hv_Column4;
		HTuple  hv_Radius2, hv_Number2, hv_Row2, hv_Column2, hv_Radius;
		HTuple  hv_UsedThreshold;
		HTuple  hv_Width, hv_Height;


		//Image Acquisition 01: Code generated by Image Acquisition 01
		get_image_size(m_hoLiveImage, &hv_Width, &hv_Height);
		set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 2);
		set_font(Wnd == -1 ? m_ShowLabel[0] : Wnd, "-Mono-20-*-*-*-*-1-");
		//Image Acquisition 01: Do something
		decompose3(m_hoLiveImage, &ho_ImageChannel[0], &ho_ImageChannel[1], &ho_ImageChannel[2]);
		trans_from_rgb(ho_ImageChannel[0], ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageChannel[3], &ho_ImageChannel[4], &ho_ImageChannel[5], "hsv");

		//***Ò©°å³ß´ç
		threshold(ho_ImageChannel[m_checkparam.i_BandChannel], &ho_Region, m_checkparam.i_MinGray_Band, 255);
		fill_up(ho_Region, &ho_RegionFillUp2);
		connection(ho_RegionFillUp2, &ho_ConnectedRegions);
		select_shape_std(ho_ConnectedRegions, &ho_RegionBand, "max_area", 70);
		area_center(ho_RegionBand, &hv_AreaBand, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_AreaBand < m_checkparam.i_MinArea_Band))
		{
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			result = QString::fromLocal8Bit("°æÃæ³ß´ç´íÎó");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "°æÃæ³ß´ç´íÎó");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 80, 10);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "µ±Ç°Ò©°åÃæ»ýÎª£º" + hv_AreaBand);

			return 1;
			// stop(); only in hdevelop
		}


		//**½ºÄÒÊýÁ¿
		reduce_domain(ho_ImageChannel[m_checkparam.i_CapsulesChannel], ho_RegionBand, &ho_ImageReduced);
		threshold(ho_ImageReduced, &ho_Regions, m_checkparam.i_MinGray_Capsules, 255);
		opening_circle(ho_Regions, &ho_RegionOpening, m_checkparam.d_OR_Capsules);
		fill_up(ho_RegionOpening, &ho_RegionFillUp);
		connection(ho_RegionFillUp, &ho_ConnectedRegions1);
		select_shape(ho_ConnectedRegions1, &ho_SelectedRegions, "area", "and", m_checkparam.i_MinArea_Capsules,
			m_checkparam.i_MaxArea_Capsules);
		shape_trans(ho_SelectedRegions, &ho_RegionPill, "convex");
		count_obj(ho_RegionPill, &hv_NumberPill);
		if (0 != (HTuple(hv_NumberPill != m_checkparam.i_Number_Capsules).And(hv_NumberPill != 0)))
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
					HTuple end_val56 = hv_Number1;
					HTuple step_val56 = 1;
					for (hv_Index1 = 1; hv_Index1.Continue(end_val56, step_val56); hv_Index1 += step_val56)
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
					HTuple end_val68 = hv_Number5;
					HTuple step_val68 = 1;
					for (hv_Index2 = 1; hv_Index2.Continue(end_val68, step_val68); hv_Index2 += step_val68)
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
							difference(ho_Rectangle2, ho_RegionDilation1, &ho_RegionDifference2);
							shape_trans(ho_RegionDifference2, &ho_RegionTrans2, "inner_rectangle1");
							concat_obj(ho_EmptyPill, ho_RegionTrans2, &ho_EmptyPill);
						}
					}
				}
				union1(ho_EmptyPill, &ho_EmptyPill);
			}
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_EmptyPill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			result = QString::fromLocal8Bit("½ºÄÒ¸öÊý´íÎó");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "½ºÄÒ¸öÊý´íÎó");

			return 1;

			// stop(); only in hdevelop
		}
		else if (0 != (hv_NumberPill == 0))
		{
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			result = QString::fromLocal8Bit("ÎÞ½ºÄÒ");
			disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÎÞ½ºÄÒ");

			return 1;
			// stop(); only in hdevelop
		}
		//**¼ì²âÅúºÅ
		shape_trans(ho_RegionPill, &ho_RegionTrans, "convex");
		union1(ho_RegionTrans, &ho_RegionUnion);
		smallest_rectangle2(ho_RegionUnion, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
			&hv_Length2);
		gen_rectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1 + m_checkparam.i_LengthAdd_1, hv_Length2 + m_checkparam.i_LengthAdd_2);
		difference(ho_RegionBand, ho_Rectangle, &ho_RegionDifference);
		erosion_circle(ho_RegionDifference, &ho_RegionErosion, m_checkparam.d_ER_Batch);
		connection(ho_RegionErosion, &ho_ConnectedRegions16);
		select_shape_std(ho_ConnectedRegions16, &ho_SelectedRegions10, "max_area", 70);
		gray_dilation_rect(ho_ImageChannel[m_checkparam.i_BatchChannel], &ho_ImageMax, m_checkparam.i_MaskHeight_Batch, m_checkparam.i_MaskWidth_Batch);
		sub_image(ho_ImageMax, ho_ImageChannel[m_checkparam.i_BatchChannel], &ho_ImageSub1, 1, 0);
		threshold(ho_ImageSub1, &ho_Region4, m_checkparam.i_MinGray_Batch, 255);
		//dyn_threshold (ImageMax, G, RegionDynThresh, 10, 'light')
		opening_circle(ho_Region4, &ho_RegionOpening1, m_checkparam.d_OR_Batch);
		intersection(ho_RegionOpening1, ho_SelectedRegions10, &ho_RegionIntersection);
		closing_circle(ho_RegionIntersection, &ho_RegionClosing, m_checkparam.d_CR_Batch);
		connection(ho_RegionClosing, &ho_ConnectedRegions3);
		select_shape(ho_ConnectedRegions3, &ho_SelectedRegions2, "area", "and", m_checkparam.i_MinArea_Batch,
			m_checkparam.i_MaxArea_Batch);
		shape_trans(ho_SelectedRegions2, &ho_RegionTrans1, "convex");
		union1(ho_RegionTrans1, &ho_RegionUnion1);
		count_obj(ho_RegionUnion1, &hv_Number3);
		if (0 != (hv_Number3 == 0))
		{
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			result = QString::fromLocal8Bit("ÎÞÅúºÅ");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÎÞÅúºÅ");

			// stop(); only in hdevelop
			return 1;
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
				//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				result = QString::fromLocal8Bit("ÅúºÅÓ¡Ë¢²»Çå");
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÅúºÅÓ¡Ë¢²»ÇåÎú");

				return 1;
				// stop(); only in hdevelop
			}
			intersection(ho_Rectangle1, ho_RegionBand, &ho_RegionIntersection9);
			reduce_domain(ho_ImageSub1, ho_RegionIntersection9, &ho_ImageReduced4);
			threshold(ho_ImageReduced4, &ho_Region11, 50, 255);
			opening_circle(ho_Region11, &ho_RegionOpening6, 1.5);
			connection(ho_RegionOpening6, &ho_ConnectedRegions8);
			select_shape(ho_ConnectedRegions8, &ho_SelectedRegions5, "area", "and", 50,
				99999);
			count_obj(ho_SelectedRegions5, &hv_Number4);
			if (0 != (hv_Number4 != 0))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				smallest_circle(ho_SelectedRegions5, &hv_Row5, &hv_Column5, &hv_Radius3);
				gen_circle(&ho_Circle3, hv_Row5, hv_Column5, hv_Radius3);
				disp_obj(ho_Circle3, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				result = QString::fromLocal8Bit("ÅúºÅÈ±Ëð");
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ÅúºÅÈ±Ëð");
				return 1;
				// stop(); only in hdevelop
			}
			//else
			//{
			//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			//	disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			//}

		}
		//***½ºÄÒÄÚ²¿È±ÏÝ
		union1(ho_RegionPill, &ho_RegionPill);
		erosion_circle(ho_RegionPill, &ho_RegionErosion2, m_checkparam.d_ER_BandDefect);
		reduce_domain(ho_ImageChannel[m_checkparam.i_Channel_CapsulesDefect], ho_RegionErosion2, &ho_ImageReduced1);
		//****ºìÉ«²¿·Ö½ºÄÒ
		threshold(ho_ImageReduced1, &ho_Region1, 0, m_checkparam.i_MaxGray_CapsulesRed);
		connection(ho_Region1, &ho_ConnectedRegions4);
		select_shape(ho_ConnectedRegions4, &ho_SelectedRegions4, "area", "and", m_checkparam.i_MinArea_CapsulesRed,
			m_checkparam.i_MaxArea_CapsulesRed);
		union1(ho_SelectedRegions4, &ho_RegionUnion3);
		fill_up(ho_RegionUnion3, &ho_RegionRed);

		reduce_domain(ho_ImageChannel[m_checkparam.i_Channel_RegionRed ], ho_RegionPill, &ho_ImageReduced9);
		threshold(ho_ImageReduced9, &ho_Region15, 0, 160);
		intersection(ho_Region15, ho_RegionRed, &ho_RegionIntersection6);
		fill_up(ho_RegionIntersection6, &ho_RegionFillUp4);
		opening_circle(ho_RegionFillUp4, &ho_RegionOpening9, 5);
		erosion_circle(ho_RegionOpening9, &ho_RegionErosion4, 3.5);
		//****»ÆÉ«²¿·Ö½ºÄÒ
		threshold(ho_ImageReduced1, &ho_Region3, 0, 255);
		difference(ho_Region3, ho_RegionRed, &ho_RegionYellow);

		//***ºì»Æ·Ö½çÇøÓò
		dilation_circle(ho_RegionRed, &ho_RegionDilation2, m_checkparam.d_DR_CapsulesRed);
		dilation_circle(ho_RegionYellow, &ho_RegionDilation3, m_checkparam.d_DR_CapsulesYellow);
		intersection(ho_RegionDilation2, ho_RegionDilation3, &ho_RegionCross);

		//****ºìÉ«½ºÄÒÈ±ÏÝ
		reduce_domain(ho_ImageChannel[m_checkparam.i_Channel_RedDefect1], ho_RegionErosion4, &ho_ImageReduced2);
		gray_dilation_rect(ho_ImageReduced2, &ho_ImageMax1, m_checkparam.i_MaskHeight_RedDefect, m_checkparam.i_MaskWidth_RedDefect);
		sub_image(ho_ImageMax1, ho_ImageReduced2, &ho_ImageSub3, 1, 0);
		threshold(ho_ImageSub3, &ho_Regions2, m_checkparam.i_MinGray_RegionRed, 255);
		threshold(ho_ImageReduced2, &ho_Region2, 0, m_checkparam.i_MaxGray_RegionRed);
		intersection(ho_Regions2, ho_Region2, &ho_RegionIntersection1);
		closing_circle(ho_RegionIntersection1, &ho_RegionClosing1, m_checkparam.d_CR_RegionRed);
		opening_circle(ho_RegionClosing1, &ho_RegionOpening5, m_checkparam.d_OR_RegionRed);
		connection(ho_RegionOpening5, &ho_ConnectedRegions2);
		select_shape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", m_checkparam.i_MinArea_RegionRed, m_checkparam.i_MaxArea_RegionRed);

		reduce_domain(ho_ImageChannel[m_checkparam.i_Channel_RedDefect2], ho_RegionErosion4, &ho_ImageReduced5);
		gray_dilation_rect(ho_ImageReduced5, &ho_ImageMax5, m_checkparam.i_MaskHeight_RedDefect, m_checkparam.i_MaskWidth_RedDefect);
		sub_image(ho_ImageMax5, ho_ImageReduced5, &ho_ImageSub7, 1, 0);
		//gray_erosion_rect (ImageReduced5, ImageMin, 5, 5)
		//sub_image (ImageReduced5, ImageMin, ImageSub2, 1, 0)
		threshold(ho_ImageSub7, &ho_Region6, m_checkparam.i_MinGray_RedDefect, 255);

		erosion_circle(ho_RegionErosion4, &ho_RegionErosion3, m_checkparam.d_ER_RedDefect);
		difference(ho_RegionOpening9, ho_RegionErosion3, &ho_RegionDifference8);
		difference(ho_Region6, ho_RegionDifference8, &ho_RegionDifference9);
		opening_circle(ho_RegionDifference9, &ho_RegionOpening8, m_checkparam.d_OR_RedDefect);
		connection(ho_RegionOpening8, &ho_ConnectedRegions15);
		select_shape(ho_ConnectedRegions15, &ho_SelectedRegions9, "area", "and", m_checkparam.i_MinArea_RedDefect,
			m_checkparam.i_MaxArea_RedDefect);

		//threshold (ImageReduced5, Region7, 120, 255)
		//intersection (Region6, Region7, RegionIntersection2)
		//opening_circle (RegionIntersection2, RegionOpening4, 1)
		//connection (RegionOpening4, ConnectedRegions5)
		//select_shape (ConnectedRegions5, SelectedRegions6, 'area', 'and', 20, 2000)

		union2(ho_SelectedRegions1, ho_SelectedRegions9, &ho_RegionUnion2);
		count_obj(ho_RegionUnion2, &hv_Number);
		if (0 != (hv_Number != 0))
		{
			connection(ho_RegionUnion2, &ho_ConnectedRegions7);
			smallest_circle(ho_ConnectedRegions7, &hv_Row3, &hv_Column3, &hv_Radius1);
			gen_circle(&ho_Circle1, hv_Row3, hv_Column3, hv_Radius1);
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_Circle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			result = QString::fromLocal8Bit("ºìÉ«½ºÄÒÈ±ÏÝ");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ºìÉ«½ºÄÒÈ±ÏÝ");

			return 1;
			// stop(); only in hdevelop
		}
		//***»ÆÉ«½ºÄÒ
		reduce_domain(ho_ImageReduced1, ho_RegionYellow, &ho_ImageReduced3);
		gray_dilation_rect(ho_ImageReduced3, &ho_ImageMax2, m_checkparam.i_MaskHeight_RegionYellow, m_checkparam.i_MaskWidth_RegionYellow);
		sub_image(ho_ImageMax2, ho_ImageReduced3, &ho_ImageSub, 1, 0);
		threshold(ho_ImageSub, &ho_Region5, m_checkparam.i_MinGray_RegionYellow, 255);
		threshold(ho_ImageReduced3, &ho_Region8, 0, m_checkparam.i_MaxGray_RegionYellow);
		intersection(ho_Region5, ho_Region8, &ho_RegionIntersection3);
		difference(ho_RegionIntersection3, ho_RegionCross, &ho_RegionDifference7);
		connection(ho_RegionDifference7, &ho_ConnectedRegions14);
		opening_circle(ho_RegionDifference7, &ho_RegionOpening3, m_checkparam.d_OR_RegionYellow);
		select_shape(ho_RegionOpening3, &ho_SelectedRegions11, "area", "and", m_checkparam.i_MinArea_RegionYellow, m_checkparam.i_MaxArea_RegionYellow);
		area_center(ho_SelectedRegions11, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_Area.Num()))
		{
			connection(ho_SelectedRegions11, &ho_ConnectedRegions10);
			smallest_circle(ho_ConnectedRegions10, &hv_Row4, &hv_Column4, &hv_Radius2);
			gen_circle(&ho_Circle2, hv_Row4, hv_Column4, hv_Radius2);
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_Circle2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			result = QString::fromLocal8Bit("»ÆÉ«½ºÄÒÈ±ÏÝ");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "»ÆÉ«½ºÄÒÈ±ÏÝ");

			// stop(); only in hdevelop
			return 1;
		}
		//***Ò©°åÈ±ÏÝ
		dilation_circle(ho_RegionPill, &ho_RegionDilation, m_checkparam.d_DR_BandDefect);
		erosion_circle(ho_RegionBand, &ho_RegionErosion1, m_checkparam.d_ER_BandDefect);
		union2(ho_RegionPill, ho_Rectangle1, &ho_RegionUnion4);
		difference(ho_RegionErosion1, ho_RegionUnion4, &ho_RegionLeft);
		reduce_domain(ho_ImageChannel[m_checkparam.i_Channel_BandDefect], ho_RegionLeft, &ho_ImageLeft);
		gray_dilation_rect(ho_ImageLeft, &ho_ImageMax3, m_checkparam.i_MaskHeight_BandDefect, m_checkparam.i_MaskWidth_BandDefect);
		sub_image(ho_ImageMax3, ho_ImageLeft, &ho_ImageSub4, 1, 0);
		threshold(ho_ImageSub4, &ho_Region9, m_checkparam.i_MinGray_BandDefect, 255);
		difference(ho_Region9, ho_RegionDilation, &ho_RegionDifference6);
		fill_up(ho_RegionDifference6, &ho_RegionFillUp3);
		closing_circle(ho_RegionFillUp3, &ho_RegionClosing3, m_checkparam.d_CR_BandDefect);
		connection(ho_RegionClosing3, &ho_ConnectedRegions13);
		select_shape(ho_ConnectedRegions13, &ho_SelectedRegions8, (HTuple("width").Append("height").Append("area")),
			"and", (HTuple(m_checkparam.i_MinWidth_BandDefect).Append(m_checkparam.i_MinHeight_BandDefect).Append(m_checkparam.i_MinArea_BandDefect)),
			(HTuple(m_checkparam.i_MaxWidth_BandDefect).Append(m_checkparam.i_MaxHeight_BandDefect).Append(m_checkparam.i_MaxArea_BandDefect)));
		count_obj(ho_SelectedRegions8, &hv_Number2);
		if (0 != (hv_Number2 != 0))
		{
			smallest_circle(ho_SelectedRegions8, &hv_Row2, &hv_Column2, &hv_Radius);
			gen_circle(&ho_Circle, hv_Row2, hv_Column2, hv_Radius);
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_Circle, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			result = QString::fromLocal8Bit("Ò©°åÈ±ÏÝ");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "Ò©°åÈ±ÏÝ");
			// stop(); only in hdevelop
			return 1;
		}

		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
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