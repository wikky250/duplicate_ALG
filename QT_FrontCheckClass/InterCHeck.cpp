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
	checkparam->i_BandChannel = configIniRead.value("/" + cameraname + "/BandChannel", "5").toInt();
	checkparam->i_BandThread = configIniRead.value("/" + cameraname + "/BandThread", "128").toInt();
	checkparam->i_PillChannel1 = configIniRead.value("/" + cameraname + "/PillChannel1", "4").toInt();
	checkparam->i_PillThread1 = configIniRead.value("/" + cameraname + "/PillThread1", "30").toInt();
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
	configIniRead.setValue("/" + cameraname + "/BandChannel", checkparam->i_BandChannel);
	configIniRead.setValue("/" + cameraname + "/BandThread", checkparam->i_BandThread);
	configIniRead.setValue("/" + cameraname + "/PillChannel1", checkparam->i_PillChannel1);
	configIniRead.setValue("/" + cameraname + "/PillThread1", checkparam->i_PillThread1);
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
	//return -1;
	try
	{
		int _BandChannel = -1, _BandThread = -1, _PillChannel1 = -1, _PillThread1 = -1;
		if (nullptr!=checkparam)
		{
			_BandChannel = checkparam->i_BandChannel;
			_BandThread = checkparam->i_BandThread;
			_PillChannel1 = checkparam->i_PillChannel1;
			_PillThread1 = checkparam->i_PillThread1;
		}
		else
		{
			_BandChannel = m_checkparam.i_BandChannel;
			_BandThread = m_checkparam.i_BandThread;
			_PillChannel1 = m_checkparam.i_PillChannel1;
			_PillThread1 = m_checkparam.i_PillThread1;
		}
		// Local iconic variables 
		result = "DERROR";
		// Local iconic variables 
		Hobject  ho_ImageMax, ho_ImageSub, ho_ImageMin, ho_ImageSub1;
		Hobject  ho_Region, ho_Regions_black, ho_Regions1, ho_RegionOpening1;
		Hobject  ho_RegionClosing2, ho_RegionBand, ho_Regions2, ho_RegionOpening2;
		Hobject  ho_RegionBandErosion, ho_ImageReduced, ho_Regions;
		Hobject  ho_RegionFillUp, ho_RegionOpening, ho_ConnectedRegions;
		Hobject  ho_SelectedRegions, ho_RegionUnion, ho_RegionDilation;
		Hobject  ho_RegionDifference1, ho_ImageReduced1, ho_Regions3;
		Hobject  ho_RegionOpening3, ho_ImageMin1, ho_ImageSub2, ho_Regions4;
		Hobject  ho_ConnectedRegions1, ho_RegionTrans1, ho_SelectedRegions1;
		Hobject  ho_RegionUnion3, ho_RegionUnion2, ho_RegionDilation1;
		Hobject  ho_RegionDifference2, ho_PillRegion, ho_ObjectSelected;
		Hobject  ho_RegionOpening5, ho_RegionTrans, ho_RegionDifference;
		Hobject  ho_RegionOpening6, ho_RegionIntersection, ho_RegionOpening7;
		Hobject  ho_RegionUnion1, ho_RegionFillUp1, ho_ConnectedRegions2;


		// Local control variables 
		HTuple  hv_ImageFiles, hv_Index, hv_Area, hv_ExpDefaultCtrlDummyVar;
		HTuple  hv_Area1, hv_Number, hv_Area2, hv_Area4, hv_Row;
		HTuple  hv_Column, hv_m_bEach, hv_Index1, hv_Area3;




		decompose3(m_hoLiveImage, &ho_ImageChannel[0], &ho_ImageChannel[1], &ho_ImageChannel[2]);
		trans_from_rgb(ho_ImageChannel[0], ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageChannel[3], &ho_ImageChannel[4], &ho_ImageChannel[5], "hsv");
		gray_dilation_rect(ho_ImageChannel[1], &ho_ImageMax, 5, 5);
		sub_image(ho_ImageMax, ho_ImageChannel[1], &ho_ImageSub, 1, 0);
		gray_erosion_rect(ho_ImageSub, &ho_ImageMin, 11, 11);
		sub_image(ho_ImageSub, ho_ImageMin, &ho_ImageSub1, 1, 0);
		threshold(ho_ImageSub1, &ho_Region, 30, 255);
		closing_circle(ho_Region, &ho_Regions_black, 5.5); 




		threshold(ho_ImageChannel[_BandChannel], &ho_Regions1, _BandThread, 255);
		opening_circle(ho_Regions1, &ho_RegionOpening1, 3.5);
		closing_circle(ho_RegionOpening1, &ho_RegionClosing2, 20.5);
		fill_up(ho_RegionClosing2, &ho_RegionFillUp1);
		connection(ho_RegionFillUp1, &ho_ConnectedRegions2);
		select_shape_std(ho_ConnectedRegions2, &ho_RegionBand, "max_area", 70);
		area_center(ho_RegionBand, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_Area < 1340000))
		{
			disp_obj(m_hoLiveImage, Wnd==-1?m_ShowLabel[0]:Wnd);
			set_draw(Wnd==-1?m_ShowLabel[0]:Wnd, "margin");
			set_color(Wnd==-1?m_ShowLabel[0]:Wnd, "red");
			set_line_width(Wnd==-1?m_ShowLabel[0]:Wnd, 6);
			disp_obj(ho_RegionBand, Wnd==-1?m_ShowLabel[0]:Wnd);
			set_tposition(Wnd==-1?m_ShowLabel[0]:Wnd, 10, 10);
			result = QString::fromLocal8Bit("铝膜版面错误");
			write_string(Wnd==-1?m_ShowLabel[0]:Wnd, "铝膜版面错误");
			return -1;
			// stop(); only in hdevelop
		}
		threshold(ho_ImageChannel[4], &ho_Regions2, 87, 255);
		opening_circle(ho_Regions2, &ho_RegionOpening2, 3.5);
		area_center(ho_RegionOpening2, &hv_Area1, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_Area1 > 1000))
		{
			disp_obj(m_hoLiveImage, Wnd==-1?m_ShowLabel[0]:Wnd);
			set_draw(Wnd==-1?m_ShowLabel[0]:Wnd, "margin");
			set_color(Wnd==-1?m_ShowLabel[0]:Wnd, "red");
			disp_obj(ho_RegionOpening2, Wnd==-1?m_ShowLabel[0]:Wnd);
			set_tposition(Wnd==-1?m_ShowLabel[0]:Wnd, 10, 10);
			result = QString::fromLocal8Bit("铝膜接缝异常");
			write_string(Wnd==-1?m_ShowLabel[0]:Wnd, "铝膜接缝异常");
			return 1;
			// stop(); only in hdevelop
		}
		erosion_circle(ho_RegionBand, &ho_RegionBandErosion, 3.5);
		reduce_domain(ho_ImageChannel[_PillChannel1], ho_RegionBandErosion, &ho_ImageReduced);
		threshold(ho_ImageReduced, &ho_Regions, _PillThread1, 255);
		fill_up(ho_Regions, &ho_RegionFillUp);
		opening_circle(ho_RegionFillUp, &ho_RegionOpening, 3.5);
		connection(ho_RegionOpening, &ho_ConnectedRegions);
		select_shape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 16500,
			99999);
		count_obj(ho_SelectedRegions, &hv_Number);
		if (0 != (hv_Number != 18))
		{
			disp_obj(m_hoLiveImage, Wnd==-1?m_ShowLabel[0]:Wnd);
			set_draw(Wnd==-1?m_ShowLabel[0]:Wnd, "margin");
			set_color(Wnd==-1?m_ShowLabel[0]:Wnd, "red");
			disp_obj(ho_SelectedRegions, Wnd==-1?m_ShowLabel[0]:Wnd);
			set_tposition(Wnd==-1?m_ShowLabel[0]:Wnd, 10, 10);
			result = QString::fromLocal8Bit("药剂个数错误");
			write_string(Wnd==-1?m_ShowLabel[0]:Wnd, "药剂个数错误");
			return 1;
			// stop(); only in hdevelop
		}
		union1(ho_SelectedRegions, &ho_RegionUnion);
		dilation_circle(ho_RegionUnion, &ho_RegionDilation, 3.5);
		difference(ho_RegionBandErosion, ho_RegionDilation, &ho_RegionDifference1);
		reduce_domain(ho_ImageChannel[2], ho_RegionDifference1, &ho_ImageReduced1);
		threshold(ho_ImageReduced1, &ho_Regions3, 0, 160);
		opening_circle(ho_Regions3, &ho_RegionOpening3, 2.5);
		area_center(ho_RegionOpening3, &hv_Area2, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != hv_Area2)
		{
			disp_obj(m_hoLiveImage, Wnd==-1?m_ShowLabel[0]:Wnd);
			set_draw(Wnd==-1?m_ShowLabel[0]:Wnd, "margin");
			set_color(Wnd==-1?m_ShowLabel[0]:Wnd, "red");
			disp_obj(ho_RegionOpening3, Wnd==-1?m_ShowLabel[0]:Wnd);
			set_tposition(Wnd==-1?m_ShowLabel[0]:Wnd, 10, 10);
			result = QString::fromLocal8Bit("版面异常");
			write_string(Wnd==-1?m_ShowLabel[0]:Wnd, "版面异常");
			return 1;
			// stop(); only in hdevelop
		}
		gray_erosion_rect(ho_ImageReduced1, &ho_ImageMin1, 11, 11);
		sub_image(ho_ImageReduced1, ho_ImageMin1, &ho_ImageSub2, 1, 0);
		threshold(ho_ImageSub2, &ho_Regions4, 70, 255);
		connection(ho_Regions4, &ho_ConnectedRegions1);
		shape_trans(ho_ConnectedRegions1, &ho_RegionTrans1, "convex");
		select_shape(ho_RegionTrans1, &ho_SelectedRegions1, "area", "and", 30, 999);
		union1(ho_SelectedRegions1, &ho_RegionUnion3);

		union1(ho_SelectedRegions, &ho_RegionUnion2);
		dilation_circle(ho_RegionUnion2, &ho_RegionDilation1, 25.5);
		difference(ho_RegionUnion3, ho_RegionDilation1, &ho_RegionDifference2);

		area_center(ho_RegionDifference2, &hv_Area4, &hv_Row, &hv_Column);
		if (0 != (hv_Area4.Num()))
		{
			if (0 != (hv_Area4 > 50))
			{
				disp_obj(m_hoLiveImage, Wnd==-1?m_ShowLabel[0]:Wnd);
				set_draw(Wnd==-1?m_ShowLabel[0]:Wnd, "margin");
				set_color(Wnd==-1?m_ShowLabel[0]:Wnd, "red");
				disp_obj(ho_RegionDifference2, Wnd==-1?m_ShowLabel[0]:Wnd);
				set_tposition(3600, 10, 10);
				result = QString::fromLocal8Bit("版面异常2");
				write_string(3600, "版面异常2");
				return 1;
			}
		}
		sort_region(ho_SelectedRegions, &ho_PillRegion, "character", "true", "row");
		hv_m_bEach = 1;
		{
			HTuple end_val93 = hv_Number;
			HTuple step_val93 = 1;
			for (hv_Index1 = 1; hv_Index1.Continue(end_val93, step_val93); hv_Index1 += step_val93)
			{
				//if (m_bEach==0)
				  //break
				//endif
				select_obj(ho_PillRegion, &ho_ObjectSelected, hv_Index1);
				opening_circle(ho_ObjectSelected, &ho_RegionOpening5, 3.5);
				shape_trans(ho_RegionOpening5, &ho_RegionTrans, "convex");
				difference(ho_RegionTrans, ho_RegionOpening5, &ho_RegionDifference);
				opening_circle(ho_RegionDifference, &ho_RegionOpening6, 2.5);
				intersection(ho_Regions_black, ho_RegionTrans, &ho_RegionIntersection);
				opening_circle(ho_RegionIntersection, &ho_RegionOpening7, 1.5);
				union2(ho_RegionOpening6, ho_RegionOpening7, &ho_RegionUnion1);
				//opening_circle (RegionUnion1, RegionOpening4, 1.5)
				area_center(ho_RegionUnion1, &hv_Area3, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
				if (0 != (hv_Area3 > 0))
				{
					disp_obj(m_hoLiveImage, Wnd==-1?m_ShowLabel[0]:Wnd);
					set_draw(Wnd==-1?m_ShowLabel[0]:Wnd, "margin");
					set_color(Wnd==-1?m_ShowLabel[0]:Wnd, "red");
					disp_obj(ho_RegionUnion1, Wnd==-1?m_ShowLabel[0]:Wnd);
					set_tposition(Wnd==-1?m_ShowLabel[0]:Wnd, 10, 10);
					result = QString::fromLocal8Bit("片剂内部异常");
					write_string(Wnd==-1?m_ShowLabel[0]:Wnd, "片剂内部异常");
					return 1;
				}
			}
		}
		disp_obj(m_hoLiveImage, Wnd==-1?m_ShowLabel[0]:Wnd);
		result = "Good";
		return 0;
		if (Wnd == -1)
		{
		}
		else
		{
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
}
