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
	checkparam->i_BandThread = configIniRead.value("/" + cameraname + "/BandThread", "100").toInt();
	checkparam->i_BandChannel = configIniRead.value("/" + cameraname + "/BandChannel", "2").toInt();
	checkparam->i_ConvexThread = configIniRead.value("/" + cameraname + "/ConvexThread", "25").toInt();
	checkparam->i_ConvexOpenCore = configIniRead.value("/" + cameraname + "/ConvexOpenCore", "15").toInt() / 10.0;
	checkparam->i_1stChannel = configIniRead.value("/" + cameraname + "/1stChannel", "1").toInt();
	checkparam->i_1stThread = configIniRead.value("/" + cameraname + "/1stThread", "80").toInt();
	checkparam->i_2ndChannel = configIniRead.value("/" + cameraname + "/2ndChannel", "3").toInt();
	checkparam->i_2ndThread = configIniRead.value("/" + cameraname + "/2ndThread", "120").toInt();
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
	configIniRead.setValue("/" + cameraname + "/BandThread", checkparam->i_BandThread);
	configIniRead.setValue("/" + cameraname + "/BandChannel", checkparam->i_BandChannel);
	configIniRead.setValue("/" + cameraname + "/1stChannel", checkparam->i_1stChannel);
	configIniRead.setValue("/" + cameraname + "/1stThread", checkparam->i_1stThread);
	configIniRead.setValue("/" + cameraname + "/2ndChannel", checkparam->i_2ndChannel);
	configIniRead.setValue("/" + cameraname + "/2ndThread", checkparam->i_2ndThread);
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
int CInterCHeck::Check(Mat imgpackage, CHECKPARAM *checkparam, QString &str)
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
int CInterCHeck::RealCheck(QString &result, CHECKPARAM *checkparam, int Wnd = -1)
{
	int i_error = 0, i_null = 0;
	//return -1;
	try
	{
		// Local iconic variables 
		QStringList qslre;
		// 
		// 		if (b_test)
		// 		{
		// 			if ((total_check / circle_times) % 2 == 0)
		// 				qslre << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR";
		// 			else
		// 				qslre << "Good" << "Good" << "Good" << "Good" << "Good" << "Good";
		// 			result = qslre.join(",");
		// 			return -1;
		// 		}
		qslre << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR";
		result = qslre.join(",");
		// Local iconic variables 
		Hobject ho_ImageChannel[6];
		Hobject  ho_Image1, ho_Image2, ho_Image3;
		Hobject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
		Hobject  ho_ImageSub2, ho_EmptyRegion_Out, ho_EmptyRegion_OCR;
		Hobject  ho_EmptyRegion_Top, ho_EmptyRegion_Inner, ho_EmptyRegion_TopUP;
		Hobject  ho_EmptyRegion_Intensity, ho_ImageMax2, ho_Regions;
		Hobject  ho_Regions_Leak, ho_Region, ho_RegionOpening2, ho_RegionOpening;
		Hobject  ho_ConnectedRegions2, ho_SelectedRegions1, ho_RegionOpening_CAPOOT;
		Hobject  ho_ImageReduced, ho_ImageMax, ho_ImageSub, ho_Region1;
		Hobject  ho_RegionClosing, ho_Regions_Convex, ho_Regionx;
		Hobject  ho_Region_1st, ho_Regionsx, ho_Regions_2nd, ho_Region_OCR;
		Hobject  ho_Rectangle, ho_RegionIntersection5, ho_ConnectedRegions;
		Hobject  ho_SelectedRegions, ho_RegionOpening8, ho_RegionTrans2;
		Hobject  ho_RegionIntersection, ho_RegionErosion, ho_RegionIntersection4;
		Hobject  ho_Regions_LeakTOPError, ho_RegionErosion3, ho_RegionIntersection3;
		Hobject  ho_RegionOpening5, ho_Rectangle2, ho_RegionIntersection7;
		Hobject  ho_ConnectedRegions4, ho_SelectedRegions5, ho_RegionOpening9;
		Hobject  ho_RegionTrans3, ho_Rectangle1, ho_RegionIntersection8;
		Hobject  ho_RegionErosion1, ho_RegionIntersection2, ho_Regions_LeakTOPError2;
		Hobject  ho_RegionErosion6, ho_RegionIntersection9, ho_RegionOpening10;
		// Local control variables 
		HTuple  hv_ImageFiles, hv_Index, hv_Pointer, hv_Type;
		HTuple  hv_Width, hv_Height, hv_Tuple_Error, hv_Co_Index;
		HTuple  hv_Indices, hv_C_s, hv_R_s, hv_R_sEnd, hv_Row11;
		HTuple  hv_Column11, hv_Row21, hv_Column21, hv_Area4, hv_Row3;
		HTuple  hv_Column3, hv_Area5, hv_Row7, hv_Column7, hv_Row12;
		HTuple  hv_Column12, hv_Row22, hv_Column22, hv_Area6, hv_Row6;
		HTuple  hv_Column6, hv_Area7, hv_Row9, hv_Column9, hv_Area2;
		HTuple  hv_Row4, hv_Column4, hv_Area3, hv_Row5, hv_Column5;
		HTuple  hv_Area, hv_Row, hv_Column, hv_Area8, hv_Min, hv_Max, hv_Range;
		HTuple hv_BandThread, hv_MiddleThread, hv_UPBoundary, hv_DOWNBoundary;
		HTuple hv_LeakingThread, hv_LeakingRadios;
		int _iBandChannel, _i1stChannel, _i1stThread, _i2ndChannnel, _i2ndThread;
		int	_iConvexThread;
		double _iConvexOpenCore;
		if (nullptr != checkparam)
		{
			hv_BandThread = checkparam->i_BandThread;
			_iBandChannel = checkparam->i_BandChannel;
			_iConvexThread = checkparam->i_ConvexThread;
			_iConvexOpenCore = checkparam->i_ConvexOpenCore;
			_i1stChannel = checkparam->i_1stChannel;
			_i1stThread = checkparam->i_1stThread;
			_i2ndChannnel = checkparam->i_2ndChannel;
			_i2ndThread = checkparam->i_2ndThread;
		}
		else
		{
			hv_BandThread = m_checkparam.i_BandThread;
			_iBandChannel = m_checkparam.i_BandChannel;
			_iConvexThread = m_checkparam.i_ConvexThread;
			_iConvexOpenCore = m_checkparam.i_ConvexOpenCore;
			_i1stChannel = m_checkparam.i_1stChannel;
			_i1stThread = m_checkparam.i_1stThread;
			_i2ndChannnel = m_checkparam.i_2ndChannel;
			_i2ndThread = m_checkparam.i_2ndThread;
		}
		get_image_pointer1(m_hoLiveImage, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
		decompose3(m_hoLiveImage, &ho_ImageChannel[0], &ho_ImageChannel[1], &ho_ImageChannel[2]);
		trans_from_rgb(ho_ImageChannel[0], ho_ImageChannel[1], ho_ImageChannel[2],
			&ho_ImageChannel[3], &ho_ImageChannel[4], &ho_ImageChannel[5], "hsv");
		if (total_check < circle_times)
		{
			set_part(m_ShowLabel[total_check%circle_times], 0, 0, hv_Height - 1, hv_Width - 1);
		}
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		gen_empty_region(&ho_EmptyRegion_Out);
		gen_empty_region(&ho_EmptyRegion_Top);
		gen_empty_region(&ho_EmptyRegion_Inner);
		gen_empty_region(&ho_EmptyRegion_TopUP);
		gen_empty_region(&ho_EmptyRegion_Intensity);
		hv_Tuple_Error = HTuple();
		//Image Acquisition 01: Do something
		gray_dilation_rect(ho_ImageChannel[2], &ho_ImageMax2, 5, 5);
		sub_image(ho_ImageMax2, ho_ImageChannel[2], &ho_ImageSub2, 1, 0);
		threshold(ho_ImageSub2, &ho_Regions, 70, 255);
		closing_circle(ho_Regions, &ho_Regions_Leak, 1.5);
		threshold(ho_ImageChannel[2], &ho_Region, 100, 255);
		opening_circle(ho_Region, &ho_Region, 3.5);
		threshold(ho_ImageChannel[_iBandChannel], &ho_RegionOpening2, hv_BandThread, 255);
		fill_up(ho_RegionOpening2, &ho_RegionOpening2);
		closing_circle(ho_RegionOpening2, &ho_RegionOpening2, 3.5);
		opening_circle(ho_RegionOpening2, &ho_RegionOpening, 30.5);
		connection(ho_RegionOpening, &ho_ConnectedRegions2);
		select_shape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", 10000,
			99999);
		union1(ho_SelectedRegions1, &ho_RegionOpening_CAPOOT);
		reduce_domain(ho_ImageChannel[2], ho_RegionOpening_CAPOOT, &ho_ImageReduced);
		gray_dilation_rect(ho_ImageReduced, &ho_ImageMax, 5, 5);
		sub_image(ho_ImageMax, ho_ImageReduced, &ho_ImageSub, 1, 0);
		threshold(ho_ImageSub, &ho_Region1, _iConvexThread, 255);
		closing_circle(ho_Region1, &ho_RegionClosing, 1.5);
		opening_circle(ho_RegionClosing, &ho_Regions_Convex, _iConvexOpenCore);
		//双色第一部分
		threshold(ho_ImageChannel[_i1stChannel], &ho_Regionx, _i1stThread, 255);
		opening_circle(ho_Regionx, &ho_Region_1st, 1.5);
		//双色第二部分^
		threshold(ho_ImageChannel[_i2ndChannnel], &ho_Regionsx, _i2ndThread, 254);
		opening_circle(ho_Regionsx, &ho_Regions_2nd, 1.5);
		//ocr部分
		threshold(ho_ImageChannel[0], &ho_Region_OCR, 0, 120);
		for (hv_Co_Index = 1; hv_Co_Index <= 6; hv_Co_Index += 1)
		{
			tuple_find(hv_Tuple_Error, hv_Co_Index, &hv_Indices);
			if (0 != (HTuple(hv_Indices != -1).And((hv_Indices.Num()) != 0)))
			{
				continue;
			}
			hv_C_s = ((hv_Width / 6)*(hv_Co_Index - 1)) - 10;
			//if ((Index+1)%3==0)
			hv_R_s = 0;
			hv_R_sEnd = hv_Height / 2;
			gen_rectangle1(&ho_Rectangle, hv_R_s, hv_C_s, hv_R_sEnd, ((hv_Width / 6)*hv_Co_Index) + 10);
			min_max_gray(ho_Rectangle, ho_ImageChannel[5], 0, &hv_Min, &hv_Max, &hv_Range);
			intersection(ho_RegionOpening_CAPOOT, ho_Rectangle, &ho_RegionIntersection5
			);
			area_center(ho_RegionIntersection5, &hv_Area8, _, _);
			if (0 != (hv_Max < 200))
			{
				//concat_obj(ho_EmptyRegion_Intensity, ho_RegionIntersection4, &ho_EmptyRegion_Intensity);
				hv_Tuple_Error = hv_Tuple_Error.Concat(hv_Co_Index);
				qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("NULL");
				i_null++;
				continue;
			}
			else
			{
				if (0 != (HTuple(hv_Area8 == 0).Or((hv_Area8.Num()) == 0)))
				{
					hv_Tuple_Error = hv_Tuple_Error.Concat(hv_Co_Index);
					qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("YS");
					i_error++;
					continue;
				}
			}
			connection(ho_RegionIntersection5, &ho_ConnectedRegions);
			select_shape_std(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
			opening_circle(ho_SelectedRegions, &ho_RegionOpening8, 10.5);
			shape_trans(ho_RegionOpening8, &ho_RegionTrans2, "convex");
			smallest_rectangle1(ho_RegionTrans2, &hv_Row11, &hv_Column11, &hv_Row21,
				&hv_Column21);
			gen_rectangle1(&ho_Rectangle, hv_Row11, hv_Column11, ((hv_Row21 - hv_Row11)*0.2) + hv_Row11,
				hv_Column21);
			intersection(ho_RegionTrans2, ho_Rectangle, &ho_RegionIntersection);
			erosion_circle(ho_RegionIntersection, &ho_RegionErosion, 7.5);
			union2(ho_EmptyRegion_Out, ho_RegionErosion, &ho_EmptyRegion_Out);
			intersection(ho_Regions_Convex, ho_RegionErosion, &ho_RegionIntersection4
			);
			opening_circle(ho_RegionIntersection4, &ho_Regions_LeakTOPError, 1.5);
			area_center(ho_RegionIntersection4, &hv_Area4, &hv_Row3, &hv_Column3);
			if (0 != (hv_Area4 > 10))
			{
				union2(ho_EmptyRegion_Intensity, ho_RegionIntersection4, &ho_EmptyRegion_Intensity
				);
				hv_Tuple_Error = hv_Tuple_Error.Concat(hv_Co_Index);
				qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("sunkenTOP_LA");
				i_error++;
				continue;
			}
			erosion_circle(ho_RegionIntersection, &ho_RegionErosion3, 5.5);
			intersection(ho_RegionErosion3, ho_Regions_Leak, &ho_RegionIntersection3);
			opening_circle(ho_RegionIntersection3, &ho_RegionOpening5, 1.5);
			area_center(ho_RegionOpening5, &hv_Area5, &hv_Row7, &hv_Column7);
			if (0 != (hv_Area5 > 0))
			{
				union2(ho_EmptyRegion_Inner, ho_RegionOpening5, &ho_EmptyRegion_Inner);
				hv_Tuple_Error = hv_Tuple_Error.Concat(hv_Co_Index);
				qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("impurityTOP_LA");
				i_error++;
				continue;
			}
			hv_R_s = hv_Height / 2;
			hv_R_sEnd = hv_Height;
			gen_rectangle1(&ho_Rectangle2, hv_R_s, hv_C_s, hv_R_sEnd, ((hv_Width / 6)*hv_Co_Index) + 10);
			intersection(ho_RegionOpening_CAPOOT, ho_Rectangle2, &ho_RegionIntersection7
			);
			connection(ho_RegionIntersection7, &ho_ConnectedRegions4);
			select_shape_std(ho_ConnectedRegions4, &ho_SelectedRegions5, "max_area",
				70);
			fill_up(ho_SelectedRegions5, &ho_SelectedRegions5);
			opening_circle(ho_SelectedRegions5, &ho_RegionOpening9, 10.5);
			shape_trans(ho_RegionOpening9, &ho_RegionTrans3, "convex");
			smallest_rectangle1(ho_RegionTrans3, &hv_Row12, &hv_Column12, &hv_Row22,
				&hv_Column22);
			gen_rectangle1(&ho_Rectangle1, ((hv_Row22 - hv_Row12)*0.8) + hv_Row12, hv_Column12,
				hv_Row22, hv_Column22);
			intersection(ho_RegionTrans3, ho_Rectangle1, &ho_RegionIntersection8);
			erosion_circle(ho_RegionIntersection8, &ho_RegionErosion1, 7.5);
			union2(ho_EmptyRegion_Out, ho_RegionErosion1, &ho_EmptyRegion_Out);
			intersection(ho_Regions_Convex, ho_RegionErosion1, &ho_RegionIntersection2
			);
			opening_circle(ho_RegionIntersection2, &ho_Regions_LeakTOPError2, 1.5);
			area_center(ho_RegionIntersection2, &hv_Area6, &hv_Row6, &hv_Column6);
			if (0 != (hv_Area6 > 10))
			{
				union2(ho_EmptyRegion_Intensity, ho_RegionIntersection2, &ho_EmptyRegion_Intensity
				);
				hv_Tuple_Error = hv_Tuple_Error.Concat(hv_Co_Index);
				qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("sunkenDown_LA");
				i_error++;
				continue;
			}
			erosion_circle(ho_RegionIntersection8, &ho_RegionErosion6, 3.5);
			//union2 (EmptyRegion_Out, RegionErosion6, EmptyRegion_Out)
			intersection(ho_RegionErosion6, ho_Regions_Leak, &ho_RegionIntersection9);
			opening_circle(ho_RegionIntersection9, &ho_RegionOpening10, 1.5);
			area_center(ho_RegionOpening10, &hv_Area7, &hv_Row9, &hv_Column9);
			if (0 != (hv_Area7 > 0))
			{
				union2(ho_EmptyRegion_Inner, ho_RegionOpening10, &ho_EmptyRegion_Inner);
				hv_Tuple_Error = hv_Tuple_Error.Concat(hv_Co_Index);
				qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("impurityDown_LA");
				i_error++;
				continue;
			}
			qslre[5 - (hv_Co_Index[0].I() - 1)] = QString::fromLocal8Bit("Good");
		}
		result = qslre.join(",");
		if (Wnd == -1)
		{
			// 			if (((total_check + 1) % circle_times == 0))
			// 			{
			// 				disp_obj(ho_ImageSub3, m_ShowLabel[total_check%circle_times]);
			// 				disp_obj(ho_Regions_LeakTOP, m_ShowLabel[total_check%circle_times]);
			// 				set_tposition(m_ShowLabel[total_check%circle_times], 10, 10);
			// 				set_color(m_ShowLabel[total_check%circle_times], "gold");
			// 				write_string(m_ShowLabel[total_check%circle_times], "运行");
			// 			}
			// 
			// 			else
			{
				disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
				set_tposition(m_ShowLabel[total_check%circle_times], 10, 10);
				set_color(m_ShowLabel[total_check%circle_times], "gold");
				write_string(m_ShowLabel[total_check%circle_times], "运行");
				area_center(ho_EmptyRegion_Intensity, &hv_Area2, &hv_Row4, &hv_Column4);
				area_center(ho_EmptyRegion_Inner, &hv_Area3, &hv_Row5, &hv_Column5);
				area_center(ho_EmptyRegion_Top, &hv_Area, &hv_Row, &hv_Column);
				set_line_width(m_ShowLabel[total_check%circle_times], 1);
				set_draw(m_ShowLabel[total_check%circle_times], "margin");
				set_color(m_ShowLabel[total_check%circle_times], "green");
				disp_obj(ho_EmptyRegion_Out, m_ShowLabel[total_check%circle_times]);
				//disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
				set_tposition(m_ShowLabel[total_check%circle_times], 30, 10);
				write_string(m_ShowLabel[total_check%circle_times], hv_Tuple_Error);
				set_draw(m_ShowLabel[total_check%circle_times], "margin");
				set_color(m_ShowLabel[total_check%circle_times], "gold");
				set_line_width(m_ShowLabel[total_check%circle_times], 1);
				//disp_obj(ho_EmptyRegion_Out, m_ShowLabel[total_check%circle_times]);
				if (0 != (hv_Area > 0))
				{
					set_draw(m_ShowLabel[total_check%circle_times], "margin");
					set_color(m_ShowLabel[total_check%circle_times], "red");
					set_line_width(m_ShowLabel[total_check%circle_times], 3);
					disp_obj(ho_EmptyRegion_Top, m_ShowLabel[total_check%circle_times]);
				}
				if (0 != (hv_Area2 > 0))
				{
					set_draw(m_ShowLabel[total_check%circle_times], "margin");
					set_color(m_ShowLabel[total_check%circle_times], "red");
					set_line_width(m_ShowLabel[total_check%circle_times], 3);
					disp_obj(ho_EmptyRegion_Intensity, m_ShowLabel[total_check%circle_times]);
				}
				if (0 != (hv_Area3 > 0))
				{
					set_draw(m_ShowLabel[total_check%circle_times], "margin");
					set_color(m_ShowLabel[total_check%circle_times], "coral");
					set_line_width(m_ShowLabel[total_check%circle_times], 3);
					disp_obj(ho_EmptyRegion_Inner, m_ShowLabel[total_check%circle_times]);
				}
			}
		}
		else
		{
			set_part(HTuple(Wnd), 0, 0, hv_Height - 1, hv_Width - 1);
			disp_obj(m_hoLiveImage, HTuple(Wnd));
			set_tposition(HTuple(Wnd), 100, 100);
			set_color(HTuple(Wnd), "gold");
			write_string(HTuple(Wnd), "测试");
			set_draw(HTuple(Wnd), "margin");
			set_line_width(HTuple(Wnd), 3);
			set_color(HTuple(Wnd), "red");
			// 			disp_obj(ho_EmptyObject_Leaking, HTuple(Wnd));
			// 			disp_obj(ho_EmptyObject_Other, HTuple(Wnd));
			// 			set_color(HTuple(Wnd), "yellow");
			// 			disp_obj(ho_EmptyObjectHeight, HTuple(Wnd));
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
	if (i_null == 6)
	{
		return -1;
	}
	return i_error;
}
