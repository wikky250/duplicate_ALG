#include "InterCHeck.h" 
#include <QMessageBox>
//#include "image.h"
QString AppPath;




bool CInterCHeck::LoadCheckParam(CHECKPARAM * checkparam)
{
	QSettings configIniRead(AppPath + "\\DefaultModel\\CheckParam.ini", QSettings::IniFormat);
	configIniRead.setIniCodec("UTF-8");
	QString cameraname = checkparam->c_CameraName;
	//读取检测参数配置
	//////////////////////////////////////////////////////////////////////////
	checkparam->i_BandThread = configIniRead.value("/" + cameraname + "/BandThread", "120").toInt();

	checkparam->i_Low_LimitOut = configIniRead.value("/" + cameraname + "/Low_LimitOut", "59").toInt();
	checkparam->i_High_LimitOut = configIniRead.value("/" + cameraname + "/High_LimitOut", "59").toInt();
	checkparam->i_Low_LimitIn = configIniRead.value("/" + cameraname + "/Low_LimitIn", "50").toInt();
	checkparam->i_High_LimitIn = configIniRead.value("/" + cameraname + "/High_LimitIn", "59").toInt();


	checkparam->i_Innerthread = configIniRead.value("/" + cameraname + "/Innerthread", "180").toInt();
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
	//写入检测参数配置
	//////////////////////////////////////////////////////////////////////////
	configIniRead.setValue("/" + cameraname + "/tempThread", checkparam->i_BandThread);
	configIniRead.setValue("/" + cameraname + "/Low_LimitOut", checkparam->i_Low_LimitOut);
	configIniRead.setValue("/" + cameraname + "/High_LimitOut", checkparam->i_High_LimitOut);
	configIniRead.setValue("/" + cameraname + "/Low_LimitIn", checkparam->i_Low_LimitIn);
	configIniRead.setValue("/" + cameraname + "/High_LimitIn", checkparam->i_High_LimitIn);
	configIniRead.setValue("/" + cameraname + "/Innerthread", checkparam->i_Innerthread);
	//////////////////////////////////////////////////////////////////////////
	return false;
}
CInterCHeck::CInterCHeck(bool b_test)
{
	m_b_test = b_test;
	m_bchangedparam = false;
	AppPath = qApp->applicationDirPath();
	test_name = "OCRCheck";
	c_AlgName = new char[20];
	strcpy(c_AlgName, test_name);
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
	return c_AlgName;
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

void CInterCHeck::StartCheck(QString camerasign, std::shared_ptr<spd::logger> _daily_logger)
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
	int i;
	//	CV_8UC3
	if (image.type() == CV_8UC3)
	{
		split(image, imgchannel);
		Mat imgB = imgchannel[0];
		Mat imgG = imgchannel[1];
		Mat imgR = imgchannel[2];
		if (dataR == NULL)
		{
			dataR = new uchar[hgt*wid];
		}
		if (dataG == NULL)
		{
			dataG = new uchar[hgt*wid];
		}
		if (dataB == NULL)
		{
			dataB = new uchar[hgt*wid];
		}
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
bool CInterCHeck::Check(Mat imgpackage, CHECKPARAM *checkparam, QString &str)
{
	bool _bresult = TRUE;
	try
	{
		if (checkparam != nullptr)
		{
			m_checkparam = *checkparam;
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
	return _bresult;
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
	return true;
}

bool CInterCHeck::RealCheck(QString &result, CHECKPARAM *checkparam, int Wnd = -1)
{
	bool re = true;
	try
	{
		// Local iconic variables 
		QStringList qslre;
		qslre << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR";
		result = qslre.join(",");

		// Local iconic variables 
		Hobject  ho_Image, ho_Image1, ho_Image2, ho_Image3;
		Hobject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
		Hobject  ho_Region, ho_Region_Cap, ho_RegionFillUp1, ho_RegionOpening3;
		Hobject  ho_Region_OCR, ho_Regionx, ho_Region_1st, ho_RegionIntersection4;
		Hobject  ho_RegionClosing4, ho_ConnectedRegions5, ho_Regionsx;
		Hobject  ho_Regions_2nd, ho_RegionIntersection5, ho_RegionFillUp2;
		Hobject  ho_ConnectedRegions6, ho_SelectedRegions4, ho_RegionClosing5;
		Hobject  ho_EmptyObject_Area, ho_EmptyObjectWidth, ho_EmptyObjectHeight;
		Hobject  ho_EmptyObject_Inner, ho_EmptyObject_Leaking, ho_EmptyObject_Top;
		Hobject  ho_EmptyObject_Other, ho_Rectangle, ho_RegionIntersection;
		Hobject  ho_ConnectedRegions, ho_SelectedRegions, ho_RegionIntersection1;
		Hobject  ho_RegionClosing1, ho_ConnectedRegions1, ho_SelectedRegions1;
		Hobject  ho_RegionTrans_Out_1st, ho_Rectangle2, ho_RegionIntersection2;
		Hobject  ho_RegionClosing2, ho_ConnectedRegions2, ho_SelectedRegions2;
		Hobject  ho_RegionTrans_Out_2nd, ho_Rectangle3, ho_RegionTrans;
		Hobject  ho_RegionFillUp, ho_Region_CapOut, ho_ImageReduced;
		Hobject  ho_ImageMin, ho_ImageSub, ho_Regions, ho_RegionClosing;
		Hobject  ho_RegionOpening1, ho_ConnectedRegions3, ho_SelectedRegions3;
		Hobject  ho_RegionUnion1, ho_Rectangle5, ho_Rectangle6, ho_RegionDifference;
		Hobject  ho_ConnectedRegions7, ho_RegionIntersection6, ho_ObjectSelected;
		Hobject  ho_RegionAffineTrans, ho_ObjectSelected1, ho_RegionDifference1;
		Hobject  ho_RegionOpening4;


		// Local control variables 
		HTuple  hv_ImageFiles, hv_Index, hv_Width, hv_Height;
		HTuple  hv_Area3, hv_Row3, hv_Column3, hv_Area_White_Min;
		HTuple  hv_Area_White_Max, hv_Area4, hv_Row4, hv_Column4;
		HTuple  hv_Area_Blue_Min, hv_Area_Blue_Max, hv_b_Width;
		HTuple  hv_b_Height, hv_b_Inner, hv_b_Leaking, hv_b_Top;
		HTuple  hv_b_Other, hv_Low_LimitOut, hv_High_LimitOut, hv_Low_LimitIn;
		HTuple  hv_High_LimitIn, hv_Co_Index, hv_b_Skip, hv_Ro_Index;
		HTuple  hv_R_s, hv_C_s, hv_Area1, hv_ExpDefaultCtrlDummyVar;
		HTuple  hv_Row13, hv_Column13, hv_Row23, hv_Column23, hv_tmp_H;
		HTuple  hv_Center_R, hv_AreaSR1, hv_Row11, hv_Column11;
		HTuple  hv_Row21, hv_Column21, hv_Cent_R, hv_High_Limit;
		HTuple  hv_Low_Limit, hv_AreaSR2, hv_Row12, hv_Column12;
		HTuple  hv_Row22, hv_Column22, hv_Area, hv_Row5, hv_Column5;
		HTuple  hv_Phi, hv_Length1, hv_Length2, hv_HomMat2DIdentity;
		HTuple  hv_p, hv_HomMat2DRotate, hv_Area2, hv_Row, hv_Column;



		hv_Low_LimitOut = 0;
		hv_High_LimitOut = 0;
		hv_Low_LimitIn = 0;
		hv_High_LimitIn = 0;
		HTuple hv_BandThread = 0;
		HTuple hv_InnerThread = 0;
		if (nullptr != checkparam)
		{
			hv_Low_LimitOut = checkparam->i_Low_LimitOut;
			hv_High_LimitOut = checkparam->i_High_LimitOut;
			hv_Low_LimitIn = checkparam->i_Low_LimitIn;
			hv_High_LimitIn = checkparam->i_High_LimitIn;
			hv_BandThread = checkparam->i_BandThread;
			hv_InnerThread = checkparam->i_Innerthread;
		}
		else
		{
			hv_Low_LimitOut = m_checkparam.i_Low_LimitOut;
			hv_High_LimitOut = m_checkparam.i_High_LimitOut;
			hv_Low_LimitIn = m_checkparam.i_Low_LimitIn;
			hv_High_LimitIn = m_checkparam.i_High_LimitIn;
			hv_BandThread = m_checkparam.i_BandThread;
			hv_InnerThread = m_checkparam.i_Innerthread;
		}
		get_image_size(m_hoLiveImage, &hv_Width, &hv_Height);

		if (total_check < circle_times)
		{
			set_part(m_ShowLabel[total_check%circle_times], 0, 0, hv_Height - 1, hv_Width - 1);
		}
		//disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		decompose3(m_hoLiveImage, &ho_Image1, &ho_Image2, &ho_Image3);
		trans_from_rgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2, &ho_ImageResult3, "hsv");

		threshold(ho_ImageResult3, &ho_Region, hv_BandThread, 255);
		//opening_circle(ho_Region, &ho_RegionOpening, 3.5);
		union1(ho_Region, &ho_Region_Cap);
		fill_up(ho_Region_Cap, &ho_Region_Cap);
		opening_circle(ho_Region_Cap, &ho_Region_Cap, 30.5);


		//字符部分
		threshold(ho_Image1, &ho_Region_OCR, 0, 140);
		//双色第一部分
		threshold(ho_ImageResult2, &ho_Regionx, 0, 70);
		opening_circle(ho_Regionx, &ho_Region_1st, 1.5);
		//双色第二部分
		threshold(ho_Image1, &ho_Regionsx, 55, 120);
		opening_circle(ho_Regionsx, &ho_Regions_2nd, 3.5);


		//Image Acquisition 01: Do something
		//面积检测
		gen_empty_obj(&ho_EmptyObject_Area);
		//宽度检测
		gen_empty_obj(&ho_EmptyObjectWidth);
		hv_b_Width = 0;
		//高度检测
		gen_empty_obj(&ho_EmptyObjectHeight);
		hv_b_Height = 0;
		//内部缺陷检测
		gen_empty_obj(&ho_EmptyObject_Inner);
		hv_b_Inner = 0;
		//漏粉检测
		gen_empty_obj(&ho_EmptyObject_Leaking);
		hv_b_Leaking = 0;
		//其它
		gen_empty_obj(&ho_EmptyObject_Other);
		hv_b_Other = 0;
		for (hv_Co_Index = 1; hv_Co_Index <= 6; hv_Co_Index += 1)
		{
			//跳过第二排检测标志
			hv_b_Skip = 0;
			for (hv_Ro_Index = 1; hv_Ro_Index <= 2; hv_Ro_Index += 1)
			{
				if (0 != (hv_b_Skip == 1))
				{
					continue;
				}
				hv_R_s = ((hv_Height / 2)*(hv_Ro_Index - 1)) - 20;
				hv_C_s = ((hv_Width / 6)*(hv_Co_Index - 1)) - 20;
				if (0 != (hv_R_s < 0))
				{
					hv_R_s = 0;
				}
				if (0 != (hv_C_s < 0))
				{
					hv_C_s = 0;
				}
				gen_rectangle1(&ho_Rectangle, hv_R_s, hv_C_s, ((hv_Height / 2)*hv_Ro_Index) + 20,
					((hv_Width / 6)*hv_Co_Index) + 20);

				//外轮廓筛查
				intersection(ho_Region_Cap, ho_Rectangle, &ho_RegionIntersection);
				connection(ho_RegionIntersection, &ho_ConnectedRegions);
				select_shape_std(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
				area_center(ho_SelectedRegions, &hv_Area1, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
				if (0 != (hv_Area1 < 15000))
				{
					hv_b_Skip = 1;
					qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("DNull");
					re = false;
					continue;

				}
				smallest_rectangle1(ho_SelectedRegions, &hv_Row13, &hv_Column13, &hv_Row23,
					&hv_Column23);
				hv_tmp_H = hv_Row23 - hv_Row13;
				hv_Center_R = ((hv_Row23 - hv_Row13) / 2) + hv_Row13;
				if (0 != (HTuple(hv_tmp_H > 260).Or(hv_tmp_H < 230)))
				{
					union2(ho_EmptyObjectHeight, ho_SelectedRegions, &ho_EmptyObjectHeight);
					qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("高度整体异常");
					hv_b_Skip = 1;
					re = false;
					continue;
				}

				//shape_trans (SelectedRegions, RegionTrans_FIN_OUT, 'convex')

				//1st
				intersection(ho_SelectedRegions, ho_Region_1st, &ho_RegionIntersection1);
				connection(ho_RegionIntersection1, &ho_ConnectedRegions1);
				select_shape_std(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area",
					70);
				//shape_trans (SelectedRegions1, RegionTrans_Out_1st, 'convex')
				closing_circle(ho_SelectedRegions1, &ho_RegionTrans_Out_1st, 50.5);
				smallest_rectangle1(ho_RegionTrans_Out_1st, &hv_Row11, &hv_Column11, &hv_Row21,
					&hv_Column21);
				hv_tmp_H = hv_Row21 - hv_Row11;
				hv_Cent_R = ((hv_Row21 - hv_Row11) / 2) + hv_Row11;
				if (0 != (HTuple(HTuple(hv_Cent_R < hv_Center_R).And(hv_Ro_Index == 1)).Or(HTuple(hv_Cent_R > hv_Center_R).And(hv_Ro_Index == 2))))
				{
					hv_High_Limit = hv_High_LimitOut;
					hv_Low_Limit = hv_Low_LimitOut;
				}
				else
				{
					hv_High_Limit = hv_High_LimitIn;
					hv_Low_Limit = hv_Low_LimitIn;

				}
				if (0 != (HTuple(hv_tmp_H > hv_High_Limit).Or(hv_tmp_H < hv_Low_Limit)))
				{
					//gen_rectangle1(&ho_Rectangle2, hv_Row11, hv_Column11, hv_Row21, hv_Column21);
					// stop(); only in hdevelop
					concat_obj(ho_EmptyObjectHeight, ho_SelectedRegions, &ho_EmptyObjectHeight);
					//concat_obj (EmptyObjectHeight, RegionTrans_Out_1st, EmptyObjectHeight)
					hv_b_Skip = 1;
					qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("高度异常");
					re = false;
					continue;
				}







				//2nd
				intersection(ho_SelectedRegions, ho_Regions_2nd, &ho_RegionIntersection2);
				connection(ho_RegionIntersection2, &ho_ConnectedRegions2);
				select_shape_std(ho_ConnectedRegions2, &ho_SelectedRegions2, "max_area",
					70);
				//shape_trans (SelectedRegions2, RegionTrans_Out_2nd, 'convex')
				closing_circle(ho_SelectedRegions2, &ho_RegionTrans_Out_2nd, 50.5);
				smallest_rectangle1(ho_RegionTrans_Out_2nd, &hv_Row12, &hv_Column12, &hv_Row22,
					&hv_Column22);
				hv_tmp_H = hv_Row22 - hv_Row12;
				hv_Cent_R = ((hv_Row22 - hv_Row12) / 2) + hv_Row12;
				if (0 != (HTuple(HTuple(hv_Cent_R < hv_Center_R).And(hv_Ro_Index == 1)).Or(HTuple(hv_Cent_R > hv_Center_R).And(hv_Ro_Index == 2))))
				{
					hv_High_Limit = hv_High_LimitOut;
					hv_Low_Limit = hv_Low_LimitOut;
				}
				else
				{
					hv_High_Limit = hv_High_LimitIn;
					hv_Low_Limit = hv_Low_LimitIn;

				}
				if (0 != (HTuple(hv_tmp_H > hv_High_Limit).Or(hv_tmp_H < hv_Low_Limit)))
				{
					//
					//	disp_obj(ho_Image, m_ShowLabel[total_check%circle_times]);
					//gen_rectangle1(&ho_Rectangle3, hv_Row12, hv_Column12, hv_Row22, hv_Column22);
					// stop(); only in hdevelop
					concat_obj(ho_EmptyObjectHeight, ho_SelectedRegions, &ho_EmptyObjectHeight
					);
					//concat_obj (EmptyObjectHeight, RegionTrans_Out_2nd, EmptyObjectHeight)
					hv_b_Skip = 1;
					qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("高度异常");
					re = false;
					continue;
				}





				//union2out
				//union2 (RegionTrans_Out_1st, RegionTrans_Out_2nd, RegionUnion)
				shape_trans(ho_SelectedRegions, &ho_RegionTrans, "convex");
				fill_up(ho_RegionTrans, &ho_RegionFillUp);
				erosion_circle(ho_RegionFillUp, &ho_Region_CapOut, 3.5);
				//胶囊内OCR 需要区分在哪个区域上
// 				inner_rectangle1(ho_RegionTrans_Out_1st, &hv_Row1, &hv_Column1, &hv_Row2,
// 					&hv_Column2);
// 				gen_rectangle1(&ho_Rectangle1, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
// 				intersection(ho_Rectangle1, ho_Region_OCR, &ho_RegionIntersection3);
// 				closing_rectangle1(ho_RegionIntersection3, &ho_RegionClosing3, 5, 1);
// 				opening_circle(ho_RegionClosing3, &ho_RegionOpening2, 3.5);

				reduce_domain(ho_Image3, ho_Region_CapOut, &ho_ImageReduced);
				gray_dilation_rect(ho_ImageReduced, &ho_ImageMin, 5, 5);
				sub_image(ho_ImageMin, ho_ImageReduced, &ho_ImageSub, 1, 0);
				threshold(ho_ImageSub, &ho_Regions, hv_InnerThread, 255);
				//difference (Regions, RegionOpening2, RegionDifference)
				closing_circle(ho_Regions, &ho_RegionClosing, 1.5);
				opening_circle(ho_RegionClosing, &ho_RegionOpening1, 1.5);
				connection(ho_RegionOpening1, &ho_ConnectedRegions3);
				select_shape(ho_ConnectedRegions3, &ho_SelectedRegions3, "area", "and",10,99999);
				union1(ho_SelectedRegions3, &ho_RegionUnion1);

				area_center(ho_RegionUnion1, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
				if (0 != (hv_Area > 0))	
				{
					//stop ()
					concat_obj(ho_EmptyObject_Leaking, ho_RegionOpening1, &ho_EmptyObject_Leaking
					);
					concat_obj(ho_EmptyObject_Other, ho_Region_CapOut, &ho_EmptyObject_Other
					);
					hv_b_Skip = 1;
					qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("内部异常");
					re = false;
					continue;
				}


				smallest_rectangle2(ho_RegionFillUp, &hv_Row5, &hv_Column5, &hv_Phi, &hv_Length1,
					&hv_Length2);
				gen_rectangle2(&ho_Rectangle5, hv_Row5, hv_Column5, hv_Phi, hv_Length1*1.10,
					hv_Length2*0.9);
				gen_rectangle2(&ho_Rectangle6, hv_Row5, hv_Column5, hv_Phi, hv_Length1*0.9,
					hv_Length2);
				difference(ho_Rectangle5, ho_Rectangle6, &ho_RegionDifference);
				connection(ho_RegionDifference, &ho_ConnectedRegions7);
				difference(ho_ConnectedRegions7, ho_Region_CapOut, &ho_RegionIntersection6
				);
				select_obj(ho_RegionIntersection6, &ho_ObjectSelected, 1);
				hom_mat2d_identity(&hv_HomMat2DIdentity);
				if (0 != (hv_Phi > 0))
				{
					hv_p = -(HTuple(180).Rad());
				}
				else
				{
					hv_p = HTuple(180).Rad();
				}
				hom_mat2d_rotate(hv_HomMat2DIdentity, hv_p, hv_Row5, hv_Column5, &hv_HomMat2DRotate);
				affine_trans_region(ho_ObjectSelected, &ho_RegionAffineTrans, hv_HomMat2DRotate,
					"nearest_neighbor");
				select_obj(ho_RegionIntersection6, &ho_ObjectSelected1, 2);
				symm_difference(ho_RegionAffineTrans, ho_ObjectSelected1, &ho_RegionDifference1
				);
				opening_circle(ho_RegionDifference1, &ho_RegionOpening4, 2.5);
				area_center(ho_RegionOpening4, &hv_Area2, &hv_Row, &hv_Column);
				if (0 != (hv_Area2 > 10))
				{
					hv_b_Top = 1;
					concat_obj(ho_EmptyObject_Top, ho_RegionOpening4, &ho_EmptyObject_Top);
					hv_b_Skip = 1;
					qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("顶凹");
					re = false;
					continue;
				}
			}
			if (hv_b_Skip == 0)
			{
				qslre[5 - (hv_Co_Index[0].I() - 1)] = "Good";
			}
		}
		result = qslre.join(",");

		if (Wnd==-1)
		{
			disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
			set_tposition(m_ShowLabel[total_check%circle_times], 100, 100);
			set_color(m_ShowLabel[total_check%circle_times], "gold");
			write_string(m_ShowLabel[total_check%circle_times], "运行");
			set_draw(m_ShowLabel[total_check%circle_times], "margin");
			set_line_width(m_ShowLabel[total_check%circle_times], 3);
			set_color(m_ShowLabel[total_check%circle_times], "red");
			disp_obj(ho_EmptyObject_Leaking, m_ShowLabel[total_check%circle_times]);
			disp_obj(ho_EmptyObject_Other, m_ShowLabel[total_check%circle_times]);
			set_color(m_ShowLabel[total_check%circle_times], "yellow");
			disp_obj(ho_EmptyObjectHeight, m_ShowLabel[total_check%circle_times]);
			set_color(m_ShowLabel[total_check%circle_times], "blue");
			disp_obj(ho_EmptyObject_Top, m_ShowLabel[total_check%circle_times]);
		}
		else
		{
			set_part(HTuple(Wnd), 0, 0, hv_Height - 1, hv_Width - 1);
			disp_obj(m_hoLiveImage, HTuple(Wnd));
			set_tposition(HTuple(Wnd), 100, 100);
			set_color(HTuple(Wnd), "gold");
			write_string(HTuple(Wnd),"测试");
			set_draw(HTuple(Wnd), "margin");
			set_line_width(HTuple(Wnd), 3);
			set_color(HTuple(Wnd), "red");
			disp_obj(ho_EmptyObject_Leaking, HTuple(Wnd));
			disp_obj(ho_EmptyObject_Other, HTuple(Wnd));
			set_color(HTuple(Wnd), "yellow");
			disp_obj(ho_EmptyObjectHeight, HTuple(Wnd));
		}
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

	//disp_image(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
	return false;
}
