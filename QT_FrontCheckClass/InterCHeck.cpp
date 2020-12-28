#include "InterCHeck.h" 
#include <QMessageBox>
//#include "image.h"
void MyHalconExceptionHandler(const HException& except)
{
	throw except;
}
bool CInterCHeck::SaveCheckParam()
{
	LoadCheckParam(&m_checkparam);
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
	//后期移动到其他位置。2018年11月23日13:47:49
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
	QMessageBox::about(NULL, QString::fromLocal8Bit("严重错误"), QString::fromLocal8Bit("USBkey意外断开！"));
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
	setdlg->SetParam(m_checkparam.c_CameraName);
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

int CInterCHeck::RealCheck(QString &result, CHECKPARAM *checkparam, int Wnd = -1,QString* pos)
{
	int i_error = 0, i_null = 0;
	
	try
	{
		Hobject  ho_Image, ho_R, ho_G, ho_B, ho_H, ho_S;
		Hobject  ho_V, ho_Region, ho_RegionFillUp2, ho_ConnectedRegions;
		Hobject  ho_RegionBand, ho_Rectangle11, ho_RegionDifference19;
		Hobject  ho_RegionOpening20, ho_ConnectedRegions30, ho_SelectedRegions24;
		Hobject  ho_ImageSub10, ho_ImageReduced, ho_Regions, ho_RegionIntersection20;
		Hobject  ho_RegionFillUp13, ho_RegionOpening, ho_RegionClosing13;
		Hobject  ho_ConnectedRegions1, ho_Circle8, ho_SelectedRegions;
		Hobject  ho_RegionTrans14, ho_RegionPill, ho_error_pill;
		Hobject  ho_ObjectSelected4, ho_Rectangle13, ho_RegionUnion13;
		Hobject  ho_RegionDilation6, ho_Rectangle8, ho_RegionDifference23;
		Hobject  ho_Rectangle9, ho_EmptyPill, ho_RegionUnion5, ho_RegionTrans3;
		Hobject  ho_RegionDifference3, ho_TopPills, ho_BottomPills;
		Hobject  ho_SortedRegions, ho_ObjectSelected, ho_ObjectSelected1;
		Hobject  ho_RegionDilation1, ho_ConnectedRegions9, ho_Rectangle2;
		Hobject  ho_RegionDifference2, ho_RegionTrans2, ho_RegionTrans;
		Hobject  ho_RegionUnion, ho_Rectangle, ho_RegionDifference;
		Hobject  ho_RegionErosion, ho_ConnectedRegions16, ho_SelectedRegions10;
		Hobject  ho_ImageMax, ho_ImageSub1, ho_Region4, ho_RegionOpening1;
		Hobject  ho_RegionIntersection, ho_RegionClosing, ho_ConnectedRegions3;
		Hobject  ho_SelectedRegions2, ho_RegionTrans1, ho_RegionUnion1;
		Hobject  ho_Rectangle1, ho_RegionIntersection9, ho_ImageReduced4;
		Hobject  ho_Region11, ho_RegionOpening6, ho_ConnectedRegions8;
		Hobject  ho_SelectedRegions5, ho_Circle3, ho_ImageSub12;
		Hobject  ho_RegionShadow, ho_RegionBatch, ho_RegionTrans10;
		Hobject  ho_RegionErosion9, ho_RegionDifference24, ho_RegionDifference28;
		Hobject  ho_RegionOpening31, ho_ImageMax8, ho_ImageSub15;
		Hobject  ho_Regions5, ho_RegionDifference29, ho_RegionIntersection25;
		Hobject  ho_RegionOpening27, ho_ConnectedRegions41, ho_SelectedRegions37;
		Hobject  ho_RegionTrans15, ho_RegionUnion17, ho_ConnectedRegions42;
		Hobject  ho_Circle7, ho_RegionUnion18, ho_RegionDifference15;
		Hobject  ho_RegionUnion2, ho_Region18, ho_RegionTrans7, ho_ImageReduced18;
		Hobject  ho_ImageOpening, ho_Lines3, ho_UnionContours, ho_SelectedXLD;
		Hobject  ho_Region29, ho_RegionIntersection23, ho_RegionFillUp10;
		Hobject  ho_ConnectedRegions39, ho_SelectedRegions35, ho_RegionDifference27;
		Hobject  ho_RegionOpening30, ho_RegionUnion19, ho_RegionUnion20;
		Hobject  ho_Region21, ho_RegionIntersection16, ho_RegionClosing9;
		Hobject  ho_ConnectedRegions29, ho_SelectedRegions23, ho_ImageMax6;
		Hobject  ho_ImageSub9, ho_Region25, ho_RegionClosing12, ho_RegionOpening22;
		Hobject  ho_RegionIntersection18, ho_RegionFillUp11, ho_ConnectedRegions32;
		Hobject  ho_Circle6, ho_BadPill, ho_ObjectSelected3, ho_Rectangle10;
		Hobject  ho_RegionErosion11, ho_RegionUnion15, ho_ImageReduced19;
		Hobject  ho_Lines1, ho_UnionContours1, ho_SelectedXLD1, ho_ImageSub13;
		Hobject  ho_Regions3, ho_RegionIntersection19, ho_RegionFillUp12;
		Hobject  ho_RegionOpening23, ho_ConnectedRegions34, ho_SelectedRegions31;
		Hobject  ho_RegionTrans13, ho_Circle4, ho_RegionDifference22;
		Hobject  ho_ConnectedRegions33, ho_SelectedRegions29, ho_SelectedRegions28;
		Hobject  ho_RegionTrans12, ho_RegionArc, ho_RegionErosion6;
		Hobject  ho_ImageReduced12, ho_RegionFillUp6, ho_RegionOpening15;
		Hobject  ho_ConnectedRegions22, ho_SelectedRegions17, ho_Rectangle4;
		Hobject  ho_Rectangle7, ho_RegionErosion2, ho_ImageReduced1;
		Hobject  ho_Region1, ho_ConnectedRegions4, ho_SelectedRegions4;
		Hobject  ho_RegionUnion3, ho_RegionRed, ho_ImageReduced9;
		Hobject  ho_Region15, ho_RegionIntersection6, ho_RegionFillUp4;
		Hobject  ho_RegionOpening9, ho_RegionErosion4, ho_Region3;
		Hobject  ho_RegionYellow, ho_RegionDilation2, ho_RegionDilation3;
		Hobject  ho_RegionCross, ho_ImageReduced2, ho_ImageMax1;
		Hobject  ho_ImageSub3, ho_Regions2, ho_Region2, ho_RegionIntersection1;
		Hobject  ho_RegionClosing1, ho_RegionOpening5, ho_ConnectedRegions2;
		Hobject  ho_SelectedRegions1, ho_ImageReduced5, ho_ImageMax5;
		Hobject  ho_ImageSub7, ho_Region6, ho_RegionErosion3, ho_RegionDifference8;
		Hobject  ho_RegionDifference9, ho_RegionOpening8, ho_ConnectedRegions15;
		Hobject  ho_SelectedRegions9, ho_ConnectedRegions7, ho_Circle1;
		Hobject  ho_ImageReduced3, ho_ImageMax2, ho_ImageSub, ho_Region5;
		Hobject  ho_Region8, ho_RegionIntersection3, ho_RegionDifference7;
		Hobject  ho_ConnectedRegions14, ho_RegionOpening3, ho_SelectedRegions11;
		Hobject  ho_ConnectedRegions10, ho_Circle2, ho_ImageMax3;
		Hobject  ho_ImageSub4, ho_Region9, ho_Region20, ho_RegionErosion7;
		Hobject  ho_RegionIntersection15, ho_RegionFillUp9, ho_RegionOpening19;
		Hobject  ho_ConnectedRegions28, ho_RegionTrans9, ho_SelectedRegions22;
		Hobject  ho_RegionUnion10, ho_RegionErosion8, ho_RegionDifference18;
		Hobject  ho_RegionIntersection13, ho_ConnectedRegions24;
		Hobject  ho_SelectedRegions19, ho_RegionUnion7, ho_RegionClosing7;
		Hobject  ho_ConnectedRegions23, ho_SelectedRegions18, ho_ImageReduced13;
		Hobject  ho_Regions1, ho_RegionClosing8, ho_RegionFillUp7;
		Hobject  ho_RegionOpening17, ho_ConnectedRegions25, ho_SelectedRegions20;
		Hobject  ho_RegionTrans8, ho_RegionUnion8, ho_RegionDifference17;
		Hobject  ho_RegionIntersection14, ho_RegionFillUp8, ho_RegionOpening18;
		Hobject  ho_ConnectedRegions26, ho_SelectedRegions21, ho_RegionUnion9;
		Hobject  ho_ConnectedRegions27, ho_ConnectedRegions6, ho_Circle;
		Hobject  ho_ImageSub16, ho_ImageScaleMax1, ho_Region31, ho_RegionIntersection24;
		Hobject  ho_RegionFillUp15, ho_RegionOpening28, ho_ConnectedRegions40;
		Hobject  ho_SelectedRegions36, ho_ImageSub14, ho_RegionDifference26;
		Hobject  ho_RegionDifference25, ho_ImageReduced20, ho_ImageOpening1;
		Hobject  ho_Lines2, ho_SelectedXLD2, ho_UnionContours2, ho_Region30;
		Hobject  ho_RegionOpening26, ho_RegionClosing14, ho_ConnectedRegions37;
		Hobject  ho_SelectedRegions30, ho_RegionIntersection22, ho_RegionUnion16;
		Hobject  ho_ImageSub17, ho_RegionErosion10, ho_Region27;
		Hobject  ho_RegionIntersection26, ho_RegionOpening32, ho_ConnectedRegions43;
		Hobject  ho_SelectedRegions38, ho_RegionTrans16, ho_ImageSub18;
		Hobject  ho_ImageMin1, ho_ImageSub19, ho_Region32, ho_RegionIntersection27;
		Hobject  ho_RegionFillUp17, ho_RegionOpening33, ho_RegionClosing16;
		Hobject  ho_ConnectedRegions44, ho_SelectedRegions39, ho_RegionTrans17;
		Hobject  ho_RegionUnion21, ho_RegionDifference30, ho_RegionOpening34;
		Hobject  ho_ImageMax9, ho_ImageSub20, ho_Region33, ho_RegionOpening35;
		Hobject  ho_RegionErosion12, ho_RegionDifference31,ho_RegionClosing17,ho_ConnectedRegions45;

		// Local control variables 
		HTuple  hv_check_pill, hv_check_capsule, hv_ImageFiles;
		HTuple  hv_Index, hv_BaseName, hv_Extension, hv_Directory;
		HTuple  hv_AreaBand, hv_ExpDefaultCtrlDummyVar, hv_Row17;
		HTuple  hv_Column17, hv_Phi5, hv_Length15, hv_Length25;
		HTuple  hv_Number18, hv_Row27, hv_Column27, hv_Radius9;
		HTuple  hv_Row9, hv_Column9, hv_Radius4, hv_NumberPill;
		HTuple  hv_Mean5, hv_Deviation3, hv_Mean6, hv_Index5, hv_Mean4;
		HTuple  hv_Deviation, hv_Number20, hv_Row110, hv_Column110;
		HTuple  hv_Row29, hv_Column29, hv_Row14, hv_Column14, hv_Phi4;
		HTuple  hv_Length14, hv_Length24, hv_Row15, hv_Column15;
		HTuple  hv_Row24, hv_Column24, hv_Row11, hv_Column11, hv_Row21;
		HTuple  hv_Column21, hv_Area1, hv_Row7, hv_Column7, hv_Mean;
		HTuple  hv_Number1, hv_Index1, hv_Number5, hv_Index2, hv_Number6;
		HTuple  hv_Row6, hv_Column6, hv_Phi2, hv_Length12, hv_Length22;
		HTuple  hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2;
		HTuple  hv_Number3, hv_Row1, hv_Column1, hv_Phi1, hv_Length11;
		HTuple  hv_Length21, hv_Value, hv_Value1, hv_ratio, hv_Number4;
		HTuple  hv_Row5, hv_Column5, hv_Radius3, hv_Row26, hv_Column26;
		HTuple  hv_Radius8, hv_Number12, hv_Area8, hv_Row28, hv_Column28;
		HTuple  hv_Number17, hv_Row20, hv_Column20, hv_Radius7;
		HTuple  hv_Area6, hv_Mean1, hv_Number13, hv_Index4, hv_Area5;
		HTuple  hv_Number14, hv_Row16, hv_Column16, hv_Row25, hv_Column25;
		HTuple  hv_Number9, hv_Row10, hv_Column10, hv_Radius5, hv_Number11;
		HTuple  hv_Number10, hv_Row8, hv_Column8, hv_Phi3, hv_Length13;
		HTuple  hv_Length23, hv_Area3, hv_Number, hv_Row3, hv_Column3;
		HTuple  hv_Radius1, hv_Area, hv_Row4, hv_Column4, hv_Radius2;
		HTuple  hv_Number8, hv_Row2, hv_Column2, hv_Radius, hv_Number16;
		HTuple  hv_Number19, hv_Area7, hv_Row18, hv_Column18, hv_Value2;
		HTuple  hv_Number21;

		HTuple  hv_Width, hv_Height;
		QString _pos;
		if (nullptr==pos)
		{
			_pos = "";
		}
		else
		{
			_pos = *pos;
		}
		CHECKPARAM _param;
		if (nullptr==checkparam)
		{
			_param = m_checkparam;
		}
		else
		{
			_param = *checkparam;
		}
		set_font(3600, "-Mono-20-*-*-*-*-1-");
		hv_check_pill = 1;
		hv_check_capsule = 0;
		//Image Acquisition 01: Code generated by Image Acquisition 01
		get_image_size(m_hoLiveImage, &hv_Width, &hv_Height);
		decompose3(m_hoLiveImage, &ho_ImageChannel[0], &ho_ImageChannel[1], &ho_ImageChannel[2]);
		trans_from_rgb(ho_ImageChannel[0], ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageChannel[3], &ho_ImageChannel[4], &ho_ImageChannel[5], "hsv");
		//***药板尺寸
	/*	ho_R = ho_ImageChannel[0];
		ho_G = ho_ImageChannel[1];
		ho_B = ho_ImageChannel[2];
		ho_H = ho_ImageChannel[3];
		ho_S = ho_ImageChannel[4];
		ho_V = ho_ImageChannel[5];*/

		threshold(ho_ImageChannel[_param.i_Channel_Band], &ho_Region, _param.i_MinGray_Band, 255);
		if (QString::fromLocal8Bit("Error_泡罩板缺陷+泡罩板通道")== _pos)
		{
			disp_obj(ho_ImageChannel[_param.i_Channel_Band], Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}
		if (QString::fromLocal8Bit("Error_泡罩板缺陷+泡罩板最小灰度") == _pos)
		{
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			disp_obj(ho_Region, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}
		fill_up(ho_Region, &ho_RegionFillUp2);
		connection(ho_RegionFillUp2, &ho_ConnectedRegions);
		select_shape_std(ho_ConnectedRegions, &ho_RegionBand, "max_area", 70);
		area_center(ho_RegionBand, &hv_AreaBand, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (QString::fromLocal8Bit("Error_泡罩板缺陷+泡罩板最小面积") == _pos)
		{
			if (0 != (hv_AreaBand < _param.i_MinArea_Band))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 2);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "泡罩板缺损");
				result = QString::fromLocal8Bit("泡罩板缺损");
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 80, 10);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "当前泡罩板面积为：" + hv_AreaBand);
				return 0;
			}
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 2);
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
			disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "正常");
			result = QString::fromLocal8Bit("正常");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 80, 10);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "当前泡罩板面积为：" + hv_AreaBand);
			return 0;
		}
		if (0 != (hv_AreaBand < _param.i_MinArea_Band))
		{
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "泡罩板缺损");
			result = QString::fromLocal8Bit("泡罩板缺损");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 80, 10);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "当前泡罩板面积为：" + hv_AreaBand);
			return 1;
		}
		smallest_rectangle2(ho_RegionBand, &hv_Row17, &hv_Column17, &hv_Phi5, &hv_Length15,
			&hv_Length25);
		gen_rectangle2(&ho_Rectangle11, hv_Row17, hv_Column17, hv_Phi5, hv_Length15 -_param.i_PlusLength_Rectangle,
			hv_Length25 - _param.i_PlusLength_Rectangle);
		if (QString::fromLocal8Bit("Error_泡罩板缺陷+最小外接矩形增量") == _pos)
		{
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			disp_obj(ho_Rectangle11, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}
		difference(ho_Rectangle11, ho_RegionBand, &ho_RegionDifference19);
		opening_circle(ho_RegionDifference19, &ho_RegionOpening20, 1.5);
		connection(ho_RegionOpening20, &ho_ConnectedRegions30);
		select_shape(ho_ConnectedRegions30, &ho_SelectedRegions24, "area", "and", _param.i_MinArea_BandDefect,
			99999);
		if (QString::fromLocal8Bit("Error_泡罩板缺陷+泡罩板缺陷最小面积") == _pos)
		{
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			disp_obj(ho_SelectedRegions24, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}
		count_obj(ho_SelectedRegions24, &hv_Number18);
		if (0 != (hv_Number18 > 0))
		{
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			//set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
			//disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);

			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_SelectedRegions24, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "泡罩板缺损");
			result = QString::fromLocal8Bit("泡罩板缺损");
			return 1;
			// stop(); only in hdevelop
		}
	
	
		//********************药剂数量
		erosion_circle(ho_RegionBand, &ho_RegionBand, 5.5);
		sub_image(ho_ImageChannel[_param.i_Channel1_NumPill], ho_ImageChannel[_param.i_Channel2_NumPill], &ho_ImageSub10, 1, 0);
		if (QString::fromLocal8Bit("Error_片剂数量+片剂数量通道1") == _pos)
		{
			disp_obj(ho_ImageChannel[_param.i_Channel1_NumPill], Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}
		if (QString::fromLocal8Bit("Error_片剂数量+片剂数量通道2") == _pos)
		{
			disp_obj(ho_ImageChannel[_param.i_Channel2_NumPill], Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}
		reduce_domain(ho_ImageSub10, ho_RegionBand, &ho_ImageReduced);
		threshold(ho_ImageReduced, &ho_Regions, _param.i_MinGray_NumPill, 255);
		if (QString::fromLocal8Bit("Error_片剂数量+最小灰度") == _pos)
		{
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			disp_obj(ho_Regions, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}
		intersection(ho_Regions, ho_RegionBand, &ho_RegionIntersection20);
		fill_up(ho_RegionIntersection20, &ho_RegionFillUp13);
		opening_circle(ho_RegionFillUp13, &ho_RegionOpening, 5.5);
		closing_circle(ho_RegionOpening, &ho_RegionClosing13, 5.5);
		connection(ho_RegionClosing13, &ho_ConnectedRegions1);
		inner_circle(ho_ConnectedRegions1, &hv_Row27, &hv_Column27, &hv_Radius9);
		gen_circle(&ho_Circle8, hv_Row27, hv_Column27, hv_Radius9*1.1);
		
		
		
		select_shape(ho_Circle8, &ho_SelectedRegions, "area", "and", _param.i_MinArea_NumPill, 99999);
		if (QString::fromLocal8Bit("Error_片剂数量+最小面积") == _pos)
		{
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			disp_obj(ho_SelectedRegions, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}
		shape_trans(ho_SelectedRegions, &ho_RegionTrans14, "convex");
		smallest_circle(ho_RegionTrans14, &hv_Row9, &hv_Column9, &hv_Radius4);
		gen_circle(&ho_RegionPill, hv_Row9, hv_Column9, hv_Radius4);
		count_obj(ho_RegionPill, &hv_NumberPill);
		if (QString::fromLocal8Bit("Error_片剂数量+片剂个数") == _pos)
		{
			set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
			if (0 != (hv_NumberPill != _param.i_Number_NumPill))
			{
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				disp_obj(ho_RegionPill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			disp_obj(ho_RegionPill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			return 0;
		}



		//***********片剂缺损
		if (0 != _param.b_Lack)
		{
			smallest_rectangle2(ho_RegionPill, &hv_Row14, &hv_Column14, &hv_Phi4, &hv_Length14,
				&hv_Length24);
			gen_rectangle2(&ho_Rectangle8, hv_Row14, hv_Column14, hv_Phi4, hv_Length14 + _param.i_RectMove_Lack,
				hv_Length24 + _param.i_RectMove_Lack);
			if (QString::fromLocal8Bit("Error_片剂缺损+仿射矩形增量") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Rectangle8, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			sub_image(ho_ImageChannel[_param.i_Channel1_Lack], ho_ImageChannel[_param.i_Channel2_Lack], &ho_ImageSub13, 1, 0);
			if (QString::fromLocal8Bit("Error_片剂缺损+颜色通道1") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel1_Lack], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_片剂缺损+颜色通道2") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel2_Lack], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			threshold(ho_ImageSub13, &ho_Regions3, 0, _param.i_MaxGray_Lack);
			if (QString::fromLocal8Bit("Error_片剂缺损+最大灰度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Regions3, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			intersection(ho_Regions3, ho_ConnectedRegions1, &ho_RegionIntersection19);
			fill_up(ho_RegionIntersection19, &ho_RegionFillUp12);
			opening_circle(ho_RegionFillUp12, &ho_RegionOpening23, 1.5);
			connection(ho_RegionOpening23, &ho_ConnectedRegions34);
			select_shape(ho_ConnectedRegions34, &ho_SelectedRegions31, "area", "and", _param.i_MinArea1_Lack,
				99999);
			if (QString::fromLocal8Bit("Error_片剂缺损+筛选圆最小面积") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions31, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			shape_trans(ho_SelectedRegions31, &ho_RegionTrans13, "convex");
			smallest_circle(ho_RegionTrans13, &hv_Row10, &hv_Column10, &hv_Radius5);
			gen_circle(&ho_Circle4, hv_Row10, hv_Column10, hv_Radius5 - _param.i_RadiusMove_Lack);
			if (QString::fromLocal8Bit("Error_片剂缺损+最小内接圆半径增量") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Circle4, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			difference(ho_Circle4, ho_SelectedRegions31, &ho_RegionDifference22);
			connection(ho_RegionDifference22, &ho_ConnectedRegions33);
			select_shape(ho_ConnectedRegions33, &ho_SelectedRegions29, (HTuple("area").Append("circularity")),
				"and", (HTuple(_param.i_MinArea2_Lack).Append(_param.d_Circularity_Lack)), (HTuple(99999).Append(1)));
			if (QString::fromLocal8Bit("Error_片剂缺损+筛选缺陷最小面积") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions29, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_片剂缺损+筛选缺陷最小圆度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions29, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			count_obj(ho_SelectedRegions29, &hv_Number11);
			if (0 != (hv_Number11 > 0))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_SelectedRegions29, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "片剂缺损");
				result = QString::fromLocal8Bit("片剂缺损");
				// stop(); only in hdevelop
				return 1;
			}
		}





		//*************

		if (_param.b_Powder)
		{
			gen_empty_obj(&ho_error_pill);
			intensity(ho_RegionPill, ho_ImageChannel[_param.i_Channel_Powder], &hv_Mean5, &hv_Deviation3);
			if (QString::fromLocal8Bit("Error_夹粉+颜色通道") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel_Powder], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			tuple_mean(hv_Mean5, &hv_Mean6);
			{
				HTuple end_val104 = hv_NumberPill;
				HTuple step_val104 = 1;
				for (hv_Index5 = 1; hv_Index5.Continue(end_val104, step_val104); hv_Index5 += step_val104)
				{
					select_obj(ho_RegionPill, &ho_ObjectSelected4, hv_Index5);
					intensity(ho_ObjectSelected4, ho_ImageChannel[_param.i_Channel_Powder], &hv_Mean4, &hv_Deviation);
					if (0 != (((hv_Mean4 - hv_Mean6).Abs()) > _param.i_DifMean_Powder))
					{
						concat_obj(ho_error_pill, ho_ObjectSelected4, &ho_error_pill);
					}
				}
			}
			count_obj(ho_error_pill, &hv_Number20);
			if (QString::fromLocal8Bit("Error_夹粉+灰度均值差值") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 2);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_error_pill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (0 != (hv_Number20 > 0))
			{
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				smallest_rectangle1(ho_error_pill, &hv_Row110, &hv_Column110, &hv_Row29, &hv_Column29);
				gen_rectangle1(&ho_Rectangle13, hv_Row110 - 10, hv_Column110 - 10, hv_Row29 + 10,
					hv_Column29 + 10);
				disp_obj(ho_Rectangle13, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "夹粉");
				result = QString::fromLocal8Bit("夹粉");
				return 1;
				// stop(); only in hdevelop
			}
		}



		

		//****片剂
		if (0 != _param.b_NumPill)
		{

			if (0 != (hv_NumberPill != _param.i_Number_NumPill))
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
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "缺片");
				result = QString::fromLocal8Bit("缺片");
				return 1;
				// stop(); only in hdevelop
			}

		}
		//****

		//****胶囊
		//if (0 != _param.b_CheckCapsule)
		//{
		//	if (0 != (HTuple(hv_NumberPill != 10).And(hv_NumberPill != 0)))
		//	{
		//		gen_empty_obj(&ho_EmptyPill);
		//		if (0 != (hv_NumberPill == 5))
		//		{
		//			union1(ho_RegionPill, &ho_RegionUnion5);
		//			shape_trans(ho_RegionUnion5, &ho_RegionTrans3, "rectangle2");
		//			difference(ho_RegionBand, ho_RegionTrans3, &ho_RegionDifference3);
		//			inner_rectangle1(ho_RegionDifference3, &hv_Row11, &hv_Column11, &hv_Row21,
		//				&hv_Column21);
		//			gen_rectangle1(&ho_EmptyPill, hv_Row11, hv_Column11, hv_Row21, hv_Column21);
		//		}
		//		else
		//		{
		//			gen_empty_obj(&ho_TopPills);
		//			gen_empty_obj(&ho_BottomPills);
		//			sort_region(ho_RegionPill, &ho_SortedRegions, "first_point", "true", "row");
		//			area_center(ho_SortedRegions, &hv_Area1, &hv_Row7, &hv_Column7);
		//			tuple_mean(hv_Row7, &hv_Mean);
		//			count_obj(ho_SortedRegions, &hv_Number1);
		//			{
		//				HTuple end_val115 = hv_Number1;
		//				HTuple step_val115 = 1;
		//				for (hv_Index1 = 1; hv_Index1.Continue(end_val115, step_val115); hv_Index1 += step_val115)
		//				{
		//					select_obj(ho_SortedRegions, &ho_ObjectSelected, hv_Index1);
		//					if (0 != ((hv_Row7.Select(hv_Index1 - 1)) < hv_Mean))
		//					{
		//						concat_obj(ho_ObjectSelected, ho_TopPills, &ho_TopPills);
		//					}
		//					else
		//					{
		//						concat_obj(ho_ObjectSelected, ho_BottomPills, &ho_BottomPills);
		//					}
		//				}
		//			}
		//			union1(ho_TopPills, &ho_TopPills);
		//			union1(ho_BottomPills, &ho_BottomPills);
		//			concat_obj(ho_TopPills, ho_BottomPills, &ho_RegionPill);
		//			count_obj(ho_RegionPill, &hv_Number5);
		//			{
		//				HTuple end_val127 = hv_Number5;
		//				HTuple step_val127 = 1;
		//				for (hv_Index2 = 1; hv_Index2.Continue(end_val127, step_val127); hv_Index2 += step_val127)
		//				{
		//					select_obj(ho_RegionPill, &ho_ObjectSelected1, hv_Index2);
		//					dilation_circle(ho_ObjectSelected1, &ho_RegionDilation1, 15);
		//					connection(ho_RegionDilation1, &ho_ConnectedRegions9);
		//					count_obj(ho_ConnectedRegions9, &hv_Number6);
		//					if (0 != (hv_Number6 != 5))
		//					{
		//						smallest_rectangle2(ho_ObjectSelected1, &hv_Row6, &hv_Column6, &hv_Phi2,
		//							&hv_Length12, &hv_Length22);
		//						gen_rectangle2(&ho_Rectangle2, hv_Row6, hv_Column6, hv_Phi2, hv_Length12 + 20,
		//							hv_Length22 + 20);
		//						difference(ho_Rectangle2, ho_RegionDilation1, &ho_RegionDifference2
		//						);
		//						shape_trans(ho_RegionDifference2, &ho_RegionTrans2, "inner_rectangle1");
		//						concat_obj(ho_EmptyPill, ho_RegionTrans2, &ho_EmptyPill);
		//					}
		//				}
		//			}
		//			union1(ho_EmptyPill, &ho_EmptyPill);
		//		}

		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

		//		set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");

		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

		//		disp_obj(ho_EmptyPill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "胶囊个数错误");
		//		result = QString::fromLocal8Bit("胶囊个数错误");
		//		return 1;
		//		//write_image (Image, 'bmp', 0, 'D:/work/红黄胶囊/23428868/NG图/原图_'+Index)
		//		//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/红黄胶囊/23428868/NG图/缺陷图_'+Index)
		//		// stop(); only in hdevelop

		//	}
		//	else if (0 != (hv_NumberPill == 0))
		//	{

		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

		//		set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");

		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

		//		disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "无胶囊");
		//		result = QString::fromLocal8Bit("无胶囊");
		//		return 1;
		//		//write_image (Image, 'bmp', 0, 'D:/work/红黄胶囊/23428868/NG图/原图_'+Index)
		//		//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/红黄胶囊/23428868/NG图/缺陷图_'+Index)
		//		// stop(); only in hdevelop

		//	}
		//}
		//**************************************



		//******************检测批号
		if (_param.b_Batch)
		{
			shape_trans(ho_RegionPill, &ho_RegionTrans, "convex");
			union1(ho_RegionTrans, &ho_RegionUnion);
			smallest_rectangle2(ho_RegionUnion, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
				&hv_Length2);
			gen_rectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1 + _param.i_PlusLength1_Batch, hv_Length2 + _param.i_PlusLength2_Batch);
			if (QString::fromLocal8Bit("Error_批号+最小外接矩形增量1") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 2);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Rectangle, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_批号+最小外接矩形增量2") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 2);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Rectangle, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			difference(ho_RegionBand, ho_Rectangle, &ho_RegionDifference);
			erosion_circle(ho_RegionDifference, &ho_RegionErosion, 10);
			connection(ho_RegionErosion, &ho_ConnectedRegions16);
			select_shape_std(ho_ConnectedRegions16, &ho_SelectedRegions10, "max_area", 70);
			gray_dilation_rect(ho_ImageChannel[_param.i_Channel_Batch], &ho_ImageMax, 15, 15);
			sub_image(ho_ImageMax, ho_ImageChannel[_param.i_Channel_Batch], &ho_ImageSub1, 1, 0);
			if (QString::fromLocal8Bit("Error_批号+批号通道") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel_Batch], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			threshold(ho_ImageSub1, &ho_Region4, _param.i_MinGray_Batch, 255);
			if (QString::fromLocal8Bit("Error_批号+最小灰度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Region4, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			//dyn_threshold (ImageMax, G, RegionDynThresh, 10, 'light')
			opening_circle(ho_Region4, &ho_RegionOpening1, _param.d_Opening_Batch);
			if (QString::fromLocal8Bit("Error_批号+开运算参数") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionOpening1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			intersection(ho_RegionOpening1, ho_RegionErosion, &ho_RegionIntersection);
			closing_circle(ho_RegionIntersection, &ho_RegionClosing, _param.d_Closing_Batch);
			if (QString::fromLocal8Bit("Error_批号+闭运算参数") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionClosing, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			connection(ho_RegionClosing, &ho_ConnectedRegions3);
			select_shape(ho_ConnectedRegions3, &ho_SelectedRegions2, "area", "and", _param.i_MinArea_Batch,
				99999);
			if (QString::fromLocal8Bit("Error_批号+最小面积") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			shape_trans(ho_SelectedRegions2, &ho_RegionTrans1, "convex");
			union1(ho_RegionTrans1, &ho_RegionUnion1);
			count_obj(ho_RegionUnion1, &hv_Number3);
			if (0 != (hv_Number3 == 0))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "无批号");
				result = QString::fromLocal8Bit("无批号");
				return 1;
				// stop(); only in hdevelop
				//write_image (Image, 'bmp', 0, 'D:/work/红黄胶囊/23428868/NG图/原图_'+Index)
				//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/红黄胶囊/23428868/NG图/缺陷图_'+Index)
			}
			else
			{
				closing_rectangle1(ho_RegionUnion1, &ho_RegionClosing17, 1, 1000);
				connection(ho_RegionClosing17, &ho_ConnectedRegions45);
				smallest_rectangle2(ho_ConnectedRegions45, &hv_Row1, &hv_Column1, &hv_Phi1, &hv_Length11,
					&hv_Length21);
				gen_rectangle2(&ho_Rectangle1, hv_Row1, hv_Column1, hv_Phi1, hv_Length11, hv_Length21);
				region_features(ho_Rectangle1, "ra", &hv_Value);
				region_features(ho_RegionBand, "rb", &hv_Value1);
				hv_ratio = hv_Value / hv_Value1;
				if (0 != (hv_ratio < 0.2))
				{

					disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

					set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

					disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
					set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
					write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "批号印刷不清晰");
					result = QString::fromLocal8Bit("批号印刷不清晰");
					return 1;
					// stop(); only in hdevelop
					//write_image (Image, 'bmp', 0, 'D:/work/红黄胶囊/23428868/NG图/原图_'+Index)
					//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/红黄胶囊/23428868/NG图/缺陷图_'+Index)
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
					write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "批号缺损");
					result = QString::fromLocal8Bit("批号缺损");
					return 1;
					// stop(); only in hdevelop
					//write_image (Image, 'bmp', 0, 'D:/work/红黄胶囊/23428868/NG图/原图_'+Index)
					//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/红黄胶囊/23428868/NG图/缺陷图_'+Index)
				}
				else
				{

					//disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
					//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
					//disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				}

			}
		}
		


		//***药板缺陷


		//***********泡罩阴影区域
		if (_param.b_Shadow)
		{
			sub_image(ho_ImageChannel[_param.i_Channel2_Shadow], ho_ImageChannel[_param.i_Channel3_Shadow], &ho_ImageSub12, 1, 0);
			if (QString::fromLocal8Bit("Error_阴影区域+阴影通道2") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel2_Shadow], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_阴影区域+阴影通道3") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel3_Shadow], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			//threshold (ImageSub12, Region28, 0, 145)
			//intersection (Region28, RegionBand, RegionIntersection17)
			//difference (RegionIntersection17, Rectangle1, RegionDifference20)
			//opening_circle (RegionDifference20, RegionOpening21, 2.5)
			//closing_circle (RegionOpening21, RegionClosing10, 1.5)
			//connection (RegionClosing10, ConnectedRegions38)
			//select_shape (ConnectedRegions38, SelectedRegions34, 'area', 'and', 1500, 99999)
			//smallest_circle (SelectedRegions34, Row19, Column19, Radius6)
			//gen_circle (Circle5, Row19, Column19, Radius6)
			//difference (Circle5, RegionPill, RegionShadow)
			//*******************************
			//dilation_circle (Rectangle1, RegionBatch, 5.5)
			//union2 (Circle5, RegionBatch, RegionUnion14)
			//shape_trans (RegionBand, RegionTrans10, 'convex')
			//erosion_rectangle1 (RegionTrans10, RegionErosion9, 5, 1)
			//difference (RegionErosion9, RegionUnion14, RegionDifference24)
			difference(ho_ConnectedRegions1, ho_Circle8, &ho_RegionDifference28);
			opening_circle(ho_RegionDifference28, &ho_RegionOpening31, 2.5);

			gray_dilation_rect(ho_ImageChannel[_param.i_Channel1_Shadow], &ho_ImageMax8, 31, 31);
			if (QString::fromLocal8Bit("Error_阴影区域+阴影通道1") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel1_Shadow], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			sub_image(ho_ImageMax8, ho_ImageChannel[_param.i_Channel1_Shadow], &ho_ImageSub15, 1, 0);
			threshold(ho_ImageSub15, &ho_Regions5, _param.i_MinGray_Shadow, 255);
			if (QString::fromLocal8Bit("Error_阴影区域+最小灰度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Regions5, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			difference(ho_Regions5, ho_RegionOpening31, &ho_RegionDifference29);
			intersection(ho_RegionDifference29, ho_RegionBand, &ho_RegionIntersection25);
			opening_circle(ho_RegionIntersection25, &ho_RegionOpening27, _param.d_Opening_Shadow);
			if (QString::fromLocal8Bit("Error_阴影区域+开运算参数") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionOpening27, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			connection(ho_RegionOpening27, &ho_ConnectedRegions41);
			select_shape(ho_ConnectedRegions41, &ho_SelectedRegions37, ((HTuple("width").Append("height")).Append("area")),
				"and", ((HTuple(_param.i_Width_Shadow).Append(_param.i_Length_Shadow)).Append(_param.i_Area_Shadow)), ((HTuple(99999).Append(99999)).Append(99999)));
			if (QString::fromLocal8Bit("Error_阴影区域+筛选最小宽度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions37, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_阴影区域+筛选最小长度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions37, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_阴影区域+筛选最小面积") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions37, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			shape_trans(ho_SelectedRegions37, &ho_RegionTrans15, "convex");
			//closing_circle (SelectedRegions37, RegionClosing15, 70.5)
			//fill_up (RegionClosing15, RegionFillUp16)
			//opening_circle (RegionFillUp16, RegionOpening29, 30.5)
			union1(ho_RegionTrans15, &ho_RegionUnion17);
			connection(ho_RegionUnion17, &ho_ConnectedRegions42);
			inner_circle(ho_ConnectedRegions42, &hv_Row26, &hv_Column26, &hv_Radius8);
			gen_circle(&ho_Circle7, hv_Row26, hv_Column26, (hv_Radius8 / hv_Radius8) * _param.i_Radius_Shadow);
			if (QString::fromLocal8Bit("Error_阴影区域+最小内接圆半径") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Circle7, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			union1(ho_Circle7, &ho_RegionUnion18);
			dilation_circle(ho_Rectangle1, &ho_RegionBatch, 5.5);
			//union2 (Circle5, RegionBatch, RegionUnion14)
			shape_trans(ho_RegionBand, &ho_RegionTrans10, "convex");
			erosion_rectangle1(ho_RegionTrans10, &ho_RegionErosion9, 5, 1);
			difference(ho_RegionErosion9, ho_RegionUnion18, &ho_RegionDifference24);
			difference(ho_RegionUnion18, ho_RegionPill, &ho_RegionShadow);
		}
		

		//********药板裂纹/毛发
		if (0 != _param.b_Crack)
		{
			difference(ho_RegionDifference24, ho_RegionDifference, &ho_RegionDifference31);
			reduce_domain(ho_ImageChannel[_param.i_Channel_Crack], ho_RegionDifference31, &ho_ImageReduced18);
			if (QString::fromLocal8Bit("Error_药板裂纹/毛发+颜色通道") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel_Crack], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			gray_opening_rect(ho_ImageReduced18, &ho_ImageOpening, 7, 7);
			lines_gauss(ho_ImageOpening, &ho_Lines3, 1.5, 2, 5, "dark", "true", "bar-shaped",
				"true");
			//calculate_lines_gauss_parameters(5, 50, &hv_Sigma, &hv_Low, &hv_High);
			//lines_gauss(ho_ImageReduced18, &ho_Lines, hv_Sigma, hv_Low, hv_High, "dark", "true", "bar-shaped",
			//	"true");
			union_adjacent_contours_xld(ho_Lines3, &ho_UnionContours, 10, 1, "attr_keep");
			select_shape_xld(ho_UnionContours, &ho_SelectedXLD, (HTuple("contlength").Append("circularity")),
				"and", (HTuple(_param.i_Length_Crack).Append(0)), (HTuple(99999).Append(_param.d_Circularity_Lack)));
			if (QString::fromLocal8Bit("Error_药板裂纹/毛发+筛选最小长度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedXLD, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_药板裂纹/毛发+筛选最大圆度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedXLD, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			count_obj(ho_SelectedXLD, &hv_Number12);
			if (0 != (hv_Number12 > 0))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_SelectedXLD, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "药板裂纹/毛发");
				result = QString::fromLocal8Bit("药板裂纹/毛发");
				// stop(); only in hdevelop
				return 1;
			}

		}
		//*****************

		//********药板脏污/黑点
		if (0 != _param.b_Dirty)
		{

			threshold(ho_ImageSub12, &ho_Region29, 0, _param.i_MaxGray_Dirty);
			difference(ho_RegionDifference24, ho_RegionBatch, &ho_RegionDifference30);
			intersection(ho_Region29, ho_RegionDifference30, &ho_RegionIntersection23);
			opening_circle(ho_RegionIntersection23, &ho_RegionOpening34, 1.5);
			fill_up(ho_RegionOpening34, &ho_RegionFillUp10);
			connection(ho_RegionFillUp10, &ho_ConnectedRegions39);
			select_shape(ho_ConnectedRegions39, &ho_SelectedRegions35, (HTuple("area").Append("circularity")),
				"and", (HTuple(_param.i_MinArea_BlackSpot).Append(_param.d_Circularity_BlackSpot)), (HTuple(99999).Append(1)));
			if (QString::fromLocal8Bit("Error_药板脏污/黑点+筛选黑点最小面积") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions35, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_药板脏污/黑点+筛选黑点最小圆度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions35, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}

			difference(ho_ConnectedRegions1, ho_Circle8, &ho_RegionDifference27);
			opening_circle(ho_RegionDifference27, &ho_RegionOpening30, 7.5);
			union2(ho_SelectedRegions35, ho_RegionOpening30, &ho_RegionUnion19);
			union1(ho_RegionUnion19, &ho_RegionUnion20);



			//gray_dilation_rect (B, ImageMax7, 7, 7)
			//sub_image (ImageMax7, B, ImageSub11, 1, 0)
			//threshold (ImageSub11, Regions4, 30, 255)
			//intersection (Regions4, RegionDifference24, RegionIntersection21)
			//fill_up (RegionIntersection21, RegionFillUp14)
			//opening_circle (RegionFillUp14, RegionOpening25, 1.5)
			//connection (RegionOpening25, ConnectedRegions36)
			//select_shape (ConnectedRegions36, SelectedRegions33, ['area','circularity'], 'and', [40,0.4], [99999,1])
			//count_obj (RegionUnion19, Number15)
			area_center(ho_RegionUnion20, &hv_Area8, &hv_Row28, &hv_Column28);
			if (0 != (hv_Area8 > _param.i_MinArea_Dirty))
			{

				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");

				disp_obj(ho_SelectedRegions35, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "药板脏污/黑点");
				result = QString::fromLocal8Bit("药板脏污/黑点");
				// stop(); only in hdevelop
				return 1;
			}

		}
		//*****************
		//*****************裂纹2
		if (_param.b_Fissure)
		{
			gray_dilation_rect(ho_ImageChannel[_param.i_Channel_Fissure], &ho_ImageMax9, 11, 11);
			if (QString::fromLocal8Bit("Error_裂纹2+颜色通道") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel_Fissure], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			sub_image(ho_ImageMax9, ho_ImageChannel[_param.i_Channel_Fissure], &ho_ImageSub20, 1, 0);
			threshold(ho_ImageSub20, &ho_Region33, _param.i_MinGray_Fissure, 255);
			if (QString::fromLocal8Bit("Error_裂纹2+最小灰度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Region33, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			threshold(ho_ImageChannel[_param.i_Channel_Fissure], &ho_Region21, 0, 155);
			intersection(ho_Region33, ho_RegionDifference24, &ho_RegionIntersection16);
			closing_circle(ho_RegionIntersection16, &ho_RegionClosing9, 2.5);
			opening_circle(ho_RegionClosing9, &ho_RegionOpening35, 1.5);

			connection(ho_RegionOpening35, &ho_ConnectedRegions29);
			select_shape(ho_ConnectedRegions29, &ho_SelectedRegions23, (HTuple("area").Append("circularity")),
				"and", (HTuple(_param.i_MinArea_Fissure).Append(0)), (HTuple(99999).Append(_param.d_Circularity_Fissure)));
			if (QString::fromLocal8Bit("Error_裂纹2+筛选最小面积") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions23, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_裂纹2+筛选最大圆度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions23, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			count_obj(ho_SelectedRegions23, &hv_Number17);
			if (0 != (hv_Number17 > 0))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				disp_obj(ho_SelectedRegions23, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "药板裂纹/毛发");
				result = QString::fromLocal8Bit("药板裂纹/毛发");
				return 1;
				// stop(); only in hdevelop
			}
		}




		//*****片剂内部缺陷
		if (_param.b_Iner)
		{
			union1(ho_RegionPill, &ho_RegionPill);
			erosion_circle(ho_RegionPill, &ho_RegionErosion12, 5.5);


			sub_image(ho_ImageChannel[_param.i_Channel1_Iner], ho_ImageChannel[_param.i_Channel2_Iner], &ho_ImageSub9, 1, 0);
			if (QString::fromLocal8Bit("Error_片剂内部缺陷+颜色通道1") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel1_Iner], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_片剂内部缺陷+颜色通道2") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel2_Iner], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			threshold(ho_ImageSub9, &ho_Region25, 0, _param.i_MaxGray_Iner);
			if (QString::fromLocal8Bit("Error_片剂内部缺陷+最小灰度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Region25, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			closing_circle(ho_Region25, &ho_RegionClosing12, _param.d_Closing_Iner);
			if (QString::fromLocal8Bit("Error_片剂内部缺陷+闭运算参数") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionClosing12, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			opening_circle(ho_RegionClosing12, &ho_RegionOpening22, _param.d_Opening_Iner);
			if (QString::fromLocal8Bit("Error_片剂内部缺陷+开运算参数") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionOpening22, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			intersection(ho_RegionErosion12, ho_RegionOpening22, &ho_RegionIntersection18);
			fill_up(ho_RegionIntersection18, &ho_RegionFillUp11);
			connection(ho_RegionFillUp11, &ho_ConnectedRegions32);
			//smallest_circle(ho_ConnectedRegions32, &hv_Row20, &hv_Column20, &hv_Radius7);
			//gen_circle(&ho_Circle6, hv_Row20, hv_Column20, hv_Radius7);
		}
		







		//**********半片
		if (0 != _param.b_Half)
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
					if (0 != (((hv_Area5 - hv_Mean1).Abs()) > _param.i_MinArea_Half))
					{
						concat_obj(ho_BadPill, ho_ObjectSelected3, &ho_BadPill);
					}
				}
				if (QString::fromLocal8Bit("Error_半片+最小面积") == _pos)
				{
					set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
					set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
					set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
					disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
					disp_obj(ho_BadPill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
					return 0;
				}
			}
			count_obj(ho_BadPill, &hv_Number14);
			if (0 != (hv_Number14 > 0))
			{
				smallest_rectangle1(ho_BadPill, &hv_Row16, &hv_Column16, &hv_Row25, &hv_Column25);
				gen_rectangle1(&ho_Rectangle10, hv_Row16 -_param.i_RectMove1_Half, hv_Column16 -_param.i_RectMove1_Half, hv_Row25 + _param.i_RectMove2_Half,
					hv_Column25 + _param.i_RectMove2_Half);
				if (QString::fromLocal8Bit("Error_半片+仿射矩形增量1") == _pos)
				{
					set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
					set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
					set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
					disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
					disp_obj(ho_Rectangle10, Wnd == -1 ? m_ShowLabel[0] : Wnd);
					return 0;
				}
				if (QString::fromLocal8Bit("Error_半片+仿射矩形增量2") == _pos)
				{
					set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
					set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
					set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
					disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
					disp_obj(ho_Rectangle10, Wnd == -1 ? m_ShowLabel[0] : Wnd);
					return 0;
				}
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

				disp_obj(ho_Rectangle10, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "半片");
				result = QString::fromLocal8Bit("半片");
				return 1;
				// stop(); only in hdevelop
			}
		}

		//***************

		//***********裂片
		if (0 != _param.b_Break)
		{
			erosion_circle(ho_RegionTrans14, &ho_RegionErosion11, _param.d_Erosion_Break);
			if (QString::fromLocal8Bit("Error_裂片+圆腐蚀参数") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionErosion11, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			union1(ho_RegionErosion11, &ho_RegionUnion15);
			reduce_domain(ho_ImageChannel[_param.i_Channel_Break], ho_RegionUnion15, &ho_ImageReduced19);
			if (QString::fromLocal8Bit("Error_裂片+颜色通道") == _pos)
			{
			
				disp_obj(ho_ImageChannel[_param.i_Channel_Break], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			lines_gauss(ho_ImageReduced19, &ho_Lines1, 1.5, 1, 5, "dark", "true", "bar-shaped",
				"true");
			union_adjacent_contours_xld(ho_Lines1, &ho_UnionContours1, 10, 1, "attr_keep");
			select_shape_xld(ho_UnionContours1, &ho_SelectedXLD1, (HTuple("contlength").Append("circularity")),
				"and", (HTuple(_param.i_Length_Break).Append(0)), (HTuple(1000).Append(_param.d_Circularity_Break)));
			if (QString::fromLocal8Bit("Error_裂片+筛选裂片最小长度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedXLD1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_裂片+筛选裂片最大圆度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedXLD1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			count_obj(ho_SelectedXLD1, &hv_Number9);
			if (0 != (hv_Number9 > 0))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				disp_obj(ho_SelectedXLD1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "裂片");
				result = QString::fromLocal8Bit("裂片");
				return 1;
				// stop(); only in hdevelop
			}
		}

		//*************

		





		//*************脏污/黑点
		if (0 != _param.b_PillDirty)
		{
			select_shape(ho_ConnectedRegions32, &ho_SelectedRegions28, (HTuple("area").Append("circularity")),
				"and", (HTuple(_param.i_MinArea_PillDirty).Append(_param.d_Circularity_PillDirty)), (HTuple(99999).Append(1)));
			if (QString::fromLocal8Bit("Error_片剂脏污/黑点+最小面积") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions28, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_片剂脏污/黑点+最小圆度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions28, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			count_obj(ho_SelectedRegions28, &hv_Number10);
			if (0 != (hv_Number10 != 0))
			{
				shape_trans(ho_SelectedRegions28, &ho_RegionTrans12, "convex");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				disp_obj(ho_RegionTrans12, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "片剂脏污/黑点");
				result = QString::fromLocal8Bit("片剂脏污/黑点");
				return 1;
				// stop(); only in hdevelop
			}
		}
		//压泡
		if (_param.b_PressedBubble)
		{
			sub_image(ho_ImageChannel[_param.i_Channel1_PressedBubble], ho_ImageChannel[_param.i_Channel2_PressedBubble], &ho_ImageSub17, 1, 0);
			if (QString::fromLocal8Bit("Error_压泡+颜色通道1") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel1_PressedBubble], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_压泡+颜色通道2") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel2_PressedBubble], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_压泡+颜色通道3") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel3_PressedBubble], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			erosion_circle(ho_RegionPill, &ho_RegionErosion10, _param.d_Erosion_PressedBubble);
			if (QString::fromLocal8Bit("Error_压泡+圆腐蚀参数") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionErosion10, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}

			threshold(ho_ImageSub17, &ho_Region27, 0, _param.i_MaxGray_PressedBubble);
			if (QString::fromLocal8Bit("Error_压泡+最大灰度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Region27, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			intersection(ho_Region27, ho_RegionErosion10, &ho_RegionIntersection26);
			opening_circle(ho_RegionIntersection26, &ho_RegionOpening32, _param.d_Opening1_PressedBubble);
			if (QString::fromLocal8Bit("Error_压泡+开运算参数1") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionOpening32, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			connection(ho_RegionOpening32, &ho_ConnectedRegions43);
			select_shape(ho_ConnectedRegions43, &ho_SelectedRegions38, "area", "and", _param.i_MinArea1_PressedBubble,
				99999);
			if (QString::fromLocal8Bit("Error_压泡+最小面积1") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions38, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			shape_trans(ho_SelectedRegions38, &ho_RegionTrans16, "convex");




			sub_image(ho_ImageChannel[_param.i_Channel1_PressedBubble], ho_ImageChannel[_param.i_Channel3_PressedBubble], &ho_ImageSub18, 1, 0);
			gray_erosion_rect(ho_ImageSub18, &ho_ImageMin1, 11, 11);
			sub_image(ho_ImageSub18, ho_ImageMin1, &ho_ImageSub19, 1, 0);
			threshold(ho_ImageSub19, &ho_Region32, _param.i_MinGray_PressedBubble, 255);
			if (QString::fromLocal8Bit("Error_压泡+最小灰度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Region32, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			intersection(ho_Region32, ho_RegionErosion10, &ho_RegionIntersection27);
			fill_up(ho_RegionIntersection27, &ho_RegionFillUp17);
			opening_circle(ho_RegionFillUp17, &ho_RegionOpening33, _param.d_Opeing2_PressedBubble);
			if (QString::fromLocal8Bit("Error_压泡+开运算参数2") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionOpening33, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			closing_circle(ho_RegionOpening33, &ho_RegionClosing16, _param.d_Closing_Batch);
			if (QString::fromLocal8Bit("Error_压泡+闭运算参数") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionClosing16, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			connection(ho_RegionClosing16, &ho_ConnectedRegions44);
			select_shape(ho_ConnectedRegions44, &ho_SelectedRegions39, "area", "and", _param.i_MinArea2_PressedBubble,
				99999);
			if (QString::fromLocal8Bit("Error_压泡+最小面积2") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions39, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			shape_trans(ho_SelectedRegions39, &ho_RegionTrans17, "convex");

			union2(ho_RegionTrans16, ho_RegionTrans17, &ho_RegionUnion21);
			count_obj(ho_RegionUnion21, &hv_Number21);
			if (0 != (hv_Number21 > 0))
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_RegionUnion21, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "压泡");
				result = QString::fromLocal8Bit("压泡");
				return 1;
				// stop(); only in hdevelop
			}

		}
		


		//********




		//************************





		//if (0 != _param.b_CheckCapsule)
		//{
		//	//*****是否检测胶囊头尾部
		//	gen_empty_obj(&ho_RegionArc);
		//	erosion_circle(ho_RegionBand, &ho_RegionErosion6, 7.5);
		//	reduce_domain(ho_B, ho_RegionErosion6, &ho_ImageReduced12);
		//	threshold(ho_ImageReduced12, &ho_Region18, 0, 120);
		//	fill_up(ho_Region18, &ho_RegionFillUp6);
		//	opening_circle(ho_RegionFillUp6, &ho_RegionOpening15, 1.5);
		//	connection(ho_RegionOpening15, &ho_ConnectedRegions22);
		//	select_shape(ho_ConnectedRegions22, &ho_SelectedRegions17, "area", "and", 2000,
		//		99999);
		//	shape_trans(ho_SelectedRegions17, &ho_RegionTrans7, "convex");
		//	//inner_rectangle1 (RegionTrans7, Row13, Column13, Row23, Column23)
		//	//gen_rectangle1 (Rectangle6, Row13, Column13, Row23, Column23)
		//	smallest_rectangle2(ho_RegionTrans7, &hv_Row8, &hv_Column8, &hv_Phi3, &hv_Length13,
		//		&hv_Length23);
		//	gen_rectangle2(&ho_Rectangle4, hv_Row8, hv_Column8, hv_Phi3, (hv_Length13 / hv_Length13) * 70,
		//		(hv_Length23 / hv_Length23) * 70);
		//	gen_rectangle2(&ho_Rectangle7, hv_Row8, hv_Column8, hv_Phi3, hv_Length13, hv_Length23);
		//	difference(ho_Rectangle7, ho_Rectangle4, &ho_RegionDifference15);








		//	//*****
		//	//***胶囊内部缺陷
		//	union1(ho_RegionPill, &ho_RegionPill);
		//	erosion_circle(ho_RegionPill, &ho_RegionErosion2, 12.5);
		//	reduce_domain(ho_G, ho_RegionErosion2, &ho_ImageReduced1);
		//	//****红色部分胶囊
		//	threshold(ho_ImageReduced1, &ho_Region1, 0, 128);
		//	connection(ho_Region1, &ho_ConnectedRegions4);
		//	select_shape(ho_ConnectedRegions4, &ho_SelectedRegions4, "area", "and", 1500,
		//		99999);
		//	union1(ho_SelectedRegions4, &ho_RegionUnion3);
		//	fill_up(ho_RegionUnion3, &ho_RegionRed);

		//	reduce_domain(ho_S, ho_RegionPill, &ho_ImageReduced9);
		//	threshold(ho_ImageReduced9, &ho_Region15, 0, 160);
		//	intersection(ho_Region15, ho_RegionRed, &ho_RegionIntersection6);
		//	fill_up(ho_RegionIntersection6, &ho_RegionFillUp4);
		//	opening_circle(ho_RegionFillUp4, &ho_RegionOpening9, 5);
		//	erosion_circle(ho_RegionOpening9, &ho_RegionErosion4, 3.5);
		//	//****黄色部分胶囊
		//	threshold(ho_ImageReduced1, &ho_Region3, 0, 255);
		//	difference(ho_Region3, ho_RegionRed, &ho_RegionYellow);

		//	//***红黄分界区域
		//	dilation_circle(ho_RegionRed, &ho_RegionDilation2, 3);
		//	dilation_circle(ho_RegionYellow, &ho_RegionDilation3, 3);
		//	intersection(ho_RegionDilation2, ho_RegionDilation3, &ho_RegionCross);

		//	//****红色胶囊缺陷
		//	reduce_domain(ho_V, ho_RegionErosion4, &ho_ImageReduced2);
		//	gray_dilation_rect(ho_ImageReduced2, &ho_ImageMax1, 11, 11);
		//	sub_image(ho_ImageMax1, ho_ImageReduced2, &ho_ImageSub3, 1, 0);
		//	threshold(ho_ImageSub3, &ho_Regions2, 60, 255);
		//	threshold(ho_ImageReduced2, &ho_Region2, 0, 100);
		//	intersection(ho_Regions2, ho_Region2, &ho_RegionIntersection1);
		//	closing_circle(ho_RegionIntersection1, &ho_RegionClosing1, 1);
		//	opening_circle(ho_RegionClosing1, &ho_RegionOpening5, 1);
		//	connection(ho_RegionOpening5, &ho_ConnectedRegions2);
		//	select_shape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", 20,
		//		2000);

		//	reduce_domain(ho_S, ho_RegionErosion4, &ho_ImageReduced5);
		//	gray_dilation_rect(ho_ImageReduced5, &ho_ImageMax5, 5, 5);
		//	sub_image(ho_ImageMax5, ho_ImageReduced5, &ho_ImageSub7, 1, 0);
		//	//gray_erosion_rect (ImageReduced5, ImageMin, 5, 5)
		//	//sub_image (ImageReduced5, ImageMin, ImageSub2, 1, 0)
		//	threshold(ho_ImageSub7, &ho_Region6, 35, 255);

		//	erosion_circle(ho_RegionErosion4, &ho_RegionErosion3, 3.5);
		//	difference(ho_RegionOpening9, ho_RegionErosion3, &ho_RegionDifference8);
		//	difference(ho_Region6, ho_RegionDifference8, &ho_RegionDifference9);
		//	opening_circle(ho_RegionDifference9, &ho_RegionOpening8, 1.5);
		//	connection(ho_RegionOpening8, &ho_ConnectedRegions15);
		//	select_shape(ho_ConnectedRegions15, &ho_SelectedRegions9, "area", "and", 20,
		//		99999);

		//	//threshold (ImageReduced5, Region7, 120, 255)
		//	//intersection (Region6, Region7, RegionIntersection2)
		//	//opening_circle (RegionIntersection2, RegionOpening4, 1)
		//	//connection (RegionOpening4, ConnectedRegions5)
		//	//select_shape (ConnectedRegions5, SelectedRegions6, 'area', 'and', 20, 2000)

		//	union2(ho_SelectedRegions1, ho_SelectedRegions9, &ho_RegionUnion2);

		//	difference(ho_RegionUnion2, ho_RegionDifference15, &ho_RegionUnion2);

		//	area_center(ho_RegionUnion2, &hv_Area3, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		//	count_obj(ho_RegionUnion2, &hv_Number);
		//	if (0 != (hv_Area3 > 0))
		//	{
		//		connection(ho_RegionUnion2, &ho_ConnectedRegions7);
		//		smallest_circle(ho_ConnectedRegions7, &hv_Row3, &hv_Column3, &hv_Radius1);
		//		gen_circle(&ho_Circle1, hv_Row3, hv_Column3, hv_Radius1);

		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

		//		disp_obj(ho_Circle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "胶囊内部缺陷");
		//		result = QString::fromLocal8Bit("胶囊内部缺陷");
		//		//write_image (Image, 'bmp', 0, 'D:/work/红黄胶囊/23428868/NG图/原图_'+Index)
		//		//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/红黄胶囊/23428868/NG图/缺陷图_'+Index)
		//		// stop(); only in hdevelop
		//		return 1;
		//	}
		//	//***黄色胶囊缺陷

		//	reduce_domain(ho_ImageReduced1, ho_RegionYellow, &ho_ImageReduced3);
		//	gray_dilation_rect(ho_ImageReduced3, &ho_ImageMax2, 5, 5);
		//	sub_image(ho_ImageMax2, ho_ImageReduced3, &ho_ImageSub, 1, 0);
		//	threshold(ho_ImageSub, &ho_Region5, 20, 255);
		//	threshold(ho_ImageReduced3, &ho_Region8, 0, 185);
		//	intersection(ho_Region5, ho_Region8, &ho_RegionIntersection3);
		//	difference(ho_RegionIntersection3, ho_RegionCross, &ho_RegionDifference7);
		//	connection(ho_RegionDifference7, &ho_ConnectedRegions14);
		//	opening_circle(ho_RegionDifference7, &ho_RegionOpening3, 1.5);
		//	select_shape(ho_RegionOpening3, &ho_SelectedRegions11, "area", "and", 20, 99999);

		//	difference(ho_SelectedRegions11, ho_RegionDifference15, &ho_SelectedRegions11
		//	);


		//	area_center(ho_SelectedRegions11, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		//	if (0 != (hv_Area > 0))
		//	{
		//		connection(ho_RegionOpening3, &ho_ConnectedRegions10);
		//		smallest_circle(ho_ConnectedRegions10, &hv_Row4, &hv_Column4, &hv_Radius2);
		//		gen_circle(&ho_Circle2, hv_Row4, hv_Column4, hv_Radius2);

		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

		//		disp_obj(ho_Circle2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "胶囊内部缺陷");
		//		result = QString::fromLocal8Bit("胶囊内部缺陷");
		//		return 1;
		//		//write_image (Image, 'bmp', 0, 'D:/work/红黄胶囊/23428868/NG图/原图_'+Index)
		//		//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/红黄胶囊/23428868/NG图/缺陷图_'+Index)
		//		// stop(); only in hdevelop
		//	}
		//}

		//***********阴影区域缺陷
		if (_param.b_ShadowDefect)
		{
			sub_image(ho_ImageChannel[_param.i_Channel1_ShadowDefect], ho_ImageChannel[_param.i_Channel2_ShadowDefect], &ho_ImageSub16, 1, 0);
			if (QString::fromLocal8Bit("Error_阴影区域缺陷+颜色通道1") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel1_ShadowDefect], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_阴影区域缺陷+颜色通道2") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel2_ShadowDefect], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			scale_image_max(ho_ImageSub16, &ho_ImageScaleMax1);
			threshold(ho_ImageScaleMax1, &ho_Region31, 0, _param.i_MaxGray_ShadowDefect);
			if (QString::fromLocal8Bit("Error_阴影区域缺陷+最大灰度") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_Region31, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			intersection(ho_Region31, ho_RegionShadow, &ho_RegionIntersection24);
			fill_up(ho_RegionIntersection24, &ho_RegionFillUp15);
			opening_circle(ho_RegionFillUp15, &ho_RegionOpening28, 2.5);
			/*closing_circle(ho_RegionIntersection24, &ho_RegionClosing15, 2.5);*/
			connection(ho_RegionOpening28, &ho_ConnectedRegions40);
			select_shape(ho_ConnectedRegions40, &ho_SelectedRegions36, "area", "and", _param.i_MinArea_ShadowDefect,
				99999);
			if (QString::fromLocal8Bit("Error_阴影区域缺陷+最小面积") == _pos)
			{
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedRegions36, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			count_obj(ho_SelectedRegions36, &hv_Number16);
			if (0 != (hv_Number16 > 0))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_SelectedRegions36, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "异物/脏污");
				result = QString::fromLocal8Bit("异物/脏污");
				return 1;
				// stop(); only in hdevelop

			}
		}
		
		//********************
		// 皱板

		if (_param.b_Folds)
		{
			sub_image(ho_ImageChannel[_param.i_Channel1_Folds], ho_ImageChannel[_param.i_Channel2_Folds], &ho_ImageSub14, 1, 0);
			if (QString::fromLocal8Bit("Error_皱板+颜色通道1") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel1_Folds], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_皱板+颜色通道2") == _pos)
			{
				disp_obj(ho_ImageChannel[_param.i_Channel2_Folds], Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			difference(ho_RegionBand, ho_RegionBatch, &ho_RegionDifference26);
			difference(ho_RegionDifference26, ho_RegionUnion18, &ho_RegionDifference25);
			reduce_domain(ho_ImageSub14, ho_RegionDifference25, &ho_ImageReduced20);
			gray_opening_rect(ho_ImageReduced20, &ho_ImageOpening1, 7, 7);
			lines_gauss(ho_ImageOpening1, &ho_Lines2, 1.5, 3, 4, "dark", "true", "bar-shaped",
				"true");
			select_shape_xld(ho_Lines2, &ho_SelectedXLD2, (HTuple("contlength").Append("circularity")),
				"and", (HTuple(_param.i_Length_Folds).Append(0)), (HTuple(99999).Append(_param.d_Circularity_Folds)));
			if (QString::fromLocal8Bit("Error_皱板+最小长度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedXLD2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			if (QString::fromLocal8Bit("Error_皱板+最大圆度") == _pos)
			{
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 3);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				disp_obj(ho_SelectedXLD2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				return 0;
			}
			count_obj(ho_SelectedXLD2, &hv_Number19);
			if (0 != (hv_Number19 > 0))
			{
				union_adjacent_contours_xld(ho_SelectedXLD2, &ho_UnionContours2, 10, 1, "attr_keep");
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 2);
				disp_obj(ho_UnionContours2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "皱板");
				result = QString::fromLocal8Bit("皱板");
				return 1;
				// stop(); only in hdevelop
			}
		}
		


		//网纹不清
		if (0 != _param.b_Unclear)
		{
			threshold(ho_ImageSub14, &ho_Region27, 170, 255);
			opening_circle(ho_Region27, &ho_RegionOpening26, 1.5);
			closing_circle(ho_RegionOpening26, &ho_RegionClosing14, 3.5);
			connection(ho_RegionClosing14, &ho_ConnectedRegions37);
			select_shape(ho_ConnectedRegions37, &ho_SelectedRegions30, "area", "and", 10500,
				999999);

			

			threshold(ho_ImageChannel[_param.i_Channel_Unclear], &ho_Region4, 175, 255);
			connection(ho_Region4, &ho_ConnectedRegions6);
			closing_circle(ho_ConnectedRegions6, &ho_RegionClosing1, 10);
			opening_circle(ho_RegionClosing1, &ho_RegionOpening3, 7.5);
			select_shape(ho_RegionOpening3, &ho_SelectedRegions4, "area", "and", 1500,
				999999);
			intersection(ho_SelectedRegions30, ho_SelectedRegions4, &ho_RegionIntersection22
			);
			union1(ho_RegionIntersection22, &ho_RegionUnion16);
			area_center(ho_RegionUnion16, &hv_Area7, &hv_Row18, &hv_Column18);
			region_features(ho_RegionUnion16, "circularity", &hv_Value2);
			if (0 != (HTuple(hv_Area7 > 10000).And(hv_Value2 > 0.15)))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_RegionIntersection22, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "网纹不清/白板");
				result = QString::fromLocal8Bit("网纹不清/白板");
				return 1;
				// stop(); only in hdevelop
			}
		}
		
		//if (0 != _param.b_CheckCapsule)
		//{
		//	gray_dilation_rect(ho_R, &ho_ImageMax3, 7, 7);
		//	sub_image(ho_ImageMax3, ho_R, &ho_ImageSub4, 1, 0);
		//	threshold(ho_ImageSub4, &ho_Region9, 45, 255);

		//	threshold(ho_G, &ho_Region20, 0, 170);
		//	erosion_circle(ho_RegionBand, &ho_RegionErosion7, 15.5);
		//	intersection(ho_Region20, ho_RegionErosion7, &ho_RegionIntersection15);
		//	fill_up(ho_RegionIntersection15, &ho_RegionFillUp9);
		//	opening_circle(ho_RegionFillUp9, &ho_RegionOpening19, 20.5);
		//	connection(ho_RegionOpening19, &ho_ConnectedRegions28);
		//	shape_trans(ho_ConnectedRegions28, &ho_RegionTrans9, "convex");
		//	select_shape(ho_RegionTrans9, &ho_SelectedRegions22, "area", "and", 15000,
		//		99999);
		//	union1(ho_SelectedRegions22, &ho_RegionUnion10);
		//	erosion_circle(ho_RegionUnion10, &ho_RegionErosion8, 5.5);
		//	difference(ho_Region9, ho_RegionErosion8, &ho_RegionDifference18);

		//	intersection(ho_RegionDifference18, ho_Rectangle4, &ho_RegionIntersection13
		//	);
		//	connection(ho_RegionIntersection13, &ho_ConnectedRegions24);
		//	select_shape(ho_ConnectedRegions24, &ho_SelectedRegions19, (HTuple("width").Append("height")),
		//		"and", (HTuple(5).Append(3)), (HTuple(99999).Append(99999)));
		//	union1(ho_SelectedRegions19, &ho_RegionUnion7);
		//	closing_circle(ho_RegionUnion7, &ho_RegionClosing7, 2.5);
		//	connection(ho_RegionClosing7, &ho_ConnectedRegions23);
		//	select_shape(ho_ConnectedRegions23, &ho_SelectedRegions18, "area", "and", 30,
		//		99999);



		//	reduce_domain(ho_R, ho_RegionErosion6, &ho_ImageReduced13);
		//	threshold(ho_ImageReduced13, &ho_Regions1, 0, 190);
		//	closing_circle(ho_Regions1, &ho_RegionClosing8, 1.5);
		//	fill_up(ho_RegionClosing8, &ho_RegionFillUp7);
		//	opening_circle(ho_RegionFillUp7, &ho_RegionOpening17, 3.5);
		//	connection(ho_RegionOpening17, &ho_ConnectedRegions25);
		//	select_shape(ho_ConnectedRegions25, &ho_SelectedRegions20, "area", "and", 1500,
		//		99999);
		//	shape_trans(ho_SelectedRegions20, &ho_RegionTrans8, "convex");
		//	union2(ho_RegionTrans8, ho_Rectangle1, &ho_RegionUnion8);
		//	difference(ho_RegionErosion6, ho_RegionUnion8, &ho_RegionDifference17);
		//	intersection(ho_RegionDifference17, ho_Region9, &ho_RegionIntersection14);
		//	fill_up(ho_RegionIntersection14, &ho_RegionFillUp8);
		//	opening_circle(ho_RegionFillUp8, &ho_RegionOpening18, 1.5);
		//	connection(ho_RegionOpening18, &ho_ConnectedRegions26);
		//	select_shape(ho_ConnectedRegions26, &ho_SelectedRegions21, "area", "and", 30,
		//		99999);
		//	union2(ho_SelectedRegions18, ho_SelectedRegions21, &ho_RegionUnion9);
		//	connection(ho_RegionUnion9, &ho_ConnectedRegions27);
		//	count_obj(ho_ConnectedRegions27, &hv_Number8);


		//	//difference (Region9, RegionDilation, RegionDifference6)
		//	//fill_up (RegionDifference6, RegionFillUp3)
		//	//closing_circle (RegionFillUp3, RegionClosing3, 1.5)
		//	//connection (RegionClosing3, ConnectedRegions13)
		//	//select_shape (ConnectedRegions13, SelectedRegions8, ['width','height','area'], 'and', [1,3,20], [100,100,9999])
		//	//count_obj (SelectedRegions8, Number2)
		//	//difference (RegionTrans6, RegionPill, RegionDifference13)
		//	//dilation_circle (RegionDifference13, RegionDilation4, 5)
		//	//intersection (RegionDilation4, SelectedRegions8, RegionIntersection12)
		//	//opening_circle (RegionIntersection12, RegionOpening14, 1.5)
		//	//difference (SelectedRegions8, RegionOpening14, RegionDifference14)




		//	//fill_up (Region9, RegionFillUp1)
		//	//threshold (ImageLeft, Region10, 0, 140)
		//	//intersection (RegionFillUp1, Region10, RegionIntersection4)
		//	//opening_circle (RegionIntersection4, RegionOpening2, 1)
		//	//connection (RegionOpening2, ConnectedRegions6)
		//	//select_shape (ConnectedRegions6, SelectedRegions3, 'area', 'and', 20, 99999)
		//	//count_obj (SelectedRegions3, Number2)
		//	if (0 != (hv_Number8 != 0))
		//	{
		//		smallest_circle(ho_ConnectedRegions27, &hv_Row2, &hv_Column2, &hv_Radius);
		//		gen_circle(&ho_Circle, hv_Row2, hv_Column2, hv_Radius);

		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

		//		disp_obj(ho_Circle, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "药板缺陷");
		//		result = QString::fromLocal8Bit("药板缺陷");
		//		//write_image (Image, 'bmp', 0, 'D:/work/红黄胶囊/23428868/NG图/原图_'+Index)
		//		//dump_window (Wnd == -1 ? m_ShowLabel[0] : Wnd, 'bmp', 'D:/work/红黄胶囊/23428868/NG图/缺陷图_'+Index)
		//		// stop(); only in hdevelop
		//		return 1;
		//	}
		//}

		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		shape_trans(ho_RegionBand, &ho_RegionBand, "convex");
		disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//disp_obj(ho_PillRegions, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//disp_obj(ho_RegionIntersection4, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "产品正常");
		//disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		result = "Good";
		return 0;


	}
	catch (HException &e)
	{
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "red");
		set_tposition(m_ShowLabel[total_check%circle_times], 100, 20);
		write_string(m_ShowLabel[total_check%circle_times], "其他缺陷");
	}
	catch (Exception e)
	{
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "red");
		set_tposition(m_ShowLabel[total_check%circle_times], 100, 20);
		write_string(m_ShowLabel[total_check%circle_times], "其他缺陷");
	}
	catch (...)
	{
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "red");
		set_tposition(m_ShowLabel[total_check%circle_times], 100, 20);
		write_string(m_ShowLabel[total_check%circle_times], "其他缺陷");
	}
}