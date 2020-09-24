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
	try
	{
		Hobject  ho_Image, ho_R, ho_G, ho_B, ho_H, ho_S;
		Hobject  ho_V, ho_Region, ho_RegionFillUp, ho_ConnectedRegions;
		Hobject  ho_SelectedRegions, ho_RegionBand, ho_BandErosion;
		Hobject  ho_ImageSub2, ho_ImageReduced, ho_Region1, ho_RegionFillUp1;
		Hobject  ho_RegionOpening, ho_ConnectedRegions1, ho_RegionClosing4;
		Hobject  ho_PillRegions, ho_PillDilation, ho_RegionDifference1;
		Hobject  ho_ImageReduced1, ho_Region3, ho_RegionOpening2;
		Hobject  ho_RegionFillUp2, ho_ConnectedRegions5, ho_RegionSeam;
		Hobject  ho_RegionTrans3, ho_RegionUnion1, ho_Rectangle;
		Hobject  ho_RegionDifference, ho_RegionErosion, ho_ImageMax;
		Hobject  ho_ImageSub, ho_Region2, ho_RegionOpening1, ho_RegionIntersection;
		Hobject  ho_ConnectedRegions3, ho_RegionTrans, ho_SelectedRegions2;
		Hobject  ho_RegionData, ho_RegionUnion, ho_RegionClosing;
		Hobject  ho_ConnectedRegions4, ho_SelectedRegions3, ho_Region4;
		Hobject  ho_ConnectedRegions6, ho_RegionClosing1, ho_RegionOpening3;
		Hobject  ho_SelectedRegions4, ho_RegionIntersection1, ho_RegionLeft;
		Hobject  ho_ImageReduced2, ho_ImageMax1, ho_ImageSub1, ho_ImageReduced3;
		Hobject  ho_Region5, ho_RegionFillUp3, ho_RegionClosing2;
		Hobject  ho_RegionOpening4, ho_ConnectedRegions7, ho_RegionTrans1;
		Hobject  ho_SelectedRegions5, ho_BandStrange, ho_PillInter;
		Hobject  ho_PillImage, ho_ImageMax2, ho_ImageSub3, ho_Region6;
		Hobject  ho_RegionOpening6, ho_RegionClosing3, ho_ConnectedRegions9;
		Hobject  ho_SelectedRegions6, ho_ConnectedRegions8, ho_SortedRegions;
		Hobject  ho_RegionOpening5, ho_ObjectSelected, ho_Circle;
		Hobject  ho_RegionDifference2, ho_RegionOpening7, ho_RegionErosion2;
		Hobject  ho_RegionIntersection3, ho_RegionTrans4,ho_RegionFillUp4, ho_RegionDilation;
		Hobject  ho_ImageReduced4, ho_Region7, ho_RegionOpening8, ho_RegionDynThresh;
		Hobject  ho_PillDilation1, ho_PillDilation2, ho_ImageMax3, ho_RegionErosion3;

		// Local control variables 
		HTuple  hv_ImageFiles, hv_Index, hv_Area, hv_ExpDefaultCtrlDummyVar;
		HTuple  hv_Number, hv_Area1, hv_Row, hv_Column, hv_Phi;
		HTuple  hv_Length1, hv_Length2, hv_Number1, hv_Row1, hv_Column1;
		HTuple  hv_Phi1, hv_Length11, hv_Length21, hv_Area2, hv_Area3;
		HTuple  hv_Index1, hv_Row2, hv_Column2, hv_Radius, hv_Area4;
		HTuple  hv_Area5, hv_Number2, hv_Area6;

		//read_image(&ho_Image, hv_ImageFiles.Select(hv_Index));
		//Image Acquisition 01: Do something
		//**转换颜色空间：R,G,B,H,S,V


		//ho_ImageChannel的0,1,2,3,4,5分别对应R,G,B,H,S,V
		//m_hoLiveImage为传入的原图
		decompose3(m_hoLiveImage, &ho_ImageChannel[0], &ho_ImageChannel[1], &ho_ImageChannel[2]);
		trans_from_rgb(ho_ImageChannel[0], ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageChannel[3], &ho_ImageChannel[4], &ho_ImageChannel[5], "hsv");
		//decompose3(ho_Image, &ho_R, &ho_G, &ho_B);
		//trans_from_rgb(ho_R, ho_G, ho_B, &ho_H, &ho_S, &ho_V, "hsv");

		//**找到整体药版区域
		threshold(ho_ImageChannel[0], &ho_Region, 120, 255);
		fill_up(ho_Region, &ho_RegionFillUp);
		connection(ho_RegionFillUp, &ho_ConnectedRegions);
		select_shape_std(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
		shape_trans(ho_SelectedRegions, &ho_RegionBand, "convex");

		//***版面错误
		area_center(ho_RegionBand, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_Area < 1360000))
		{
			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
			set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 6);
			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			result = QString::fromLocal8Bit("铝模版面错误");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "铝模版面错误");
			return 1;
			// stop(); only in hdevelop
			//continue
		}

		//**药剂数量
		erosion_circle(ho_RegionBand, &ho_BandErosion, 7.5);
		sub_image(ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageSub2, 1, 0);
		reduce_domain(ho_ImageSub2, ho_BandErosion, &ho_ImageReduced);
		threshold(ho_ImageReduced, &ho_Region1, 25, 255);
		fill_up(ho_Region1, &ho_RegionFillUp1);
		opening_circle(ho_RegionFillUp1, &ho_RegionOpening, 3.5);
		connection(ho_RegionOpening, &ho_ConnectedRegions1);
		closing_circle(ho_ConnectedRegions1, &ho_RegionClosing4, 3.5);
		select_shape(ho_RegionClosing4, &ho_PillRegions, (HTuple("area").Append("circularity")),
			"and", (HTuple(16500).Append(0.8)), (HTuple(22500).Append(1.0)));
		//shape_trans (PillRegions, PillRegions, 'convex')
		count_obj(ho_PillRegions, &hv_Number);
		if (0 != (hv_Number != 18))
		{

			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");

			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

			disp_obj(ho_PillRegions, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			result = QString::fromLocal8Bit("药剂数量错误");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "药剂数量错误");
			return 1;
			// stop(); only in hdevelop
			//continue
		}

		//**去除药剂区域

		dilation_circle(ho_PillRegions, &ho_PillDilation, 25.5);
		shape_trans(ho_PillDilation, &ho_RegionTrans4, "convex");
		difference(ho_BandErosion, ho_RegionTrans4, &ho_RegionDifference1);

		//**铝模接缝异常
		reduce_domain(ho_ImageChannel[3], ho_RegionDifference1, &ho_ImageReduced1);
		threshold(ho_ImageReduced1, &ho_Region3, 230, 255);
		opening_circle(ho_Region3, &ho_RegionOpening2, 1.5);
		fill_up(ho_RegionOpening2, &ho_RegionFillUp2);
		connection(ho_RegionFillUp2, &ho_ConnectedRegions5);
		select_shape_std(ho_ConnectedRegions5, &ho_RegionSeam, "max_area", 70);
		area_center(ho_RegionSeam, &hv_Area1, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_Area1 > 10000))
		{

			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");

			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

			set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 4);

			disp_obj(ho_RegionSeam, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			result = QString::fromLocal8Bit("铝模接缝异常");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "铝模接缝异常");
			return 1;
			// stop(); only in hdevelop
			//continue
		}

		//**喷印日期
		shape_trans(ho_PillRegions, &ho_RegionTrans3, "convex");
		union1(ho_RegionTrans3, &ho_RegionUnion1);
		smallest_rectangle2(ho_RegionUnion1, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
			&hv_Length2);
		gen_rectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2 + 500);
		difference(ho_RegionBand, ho_Rectangle, &ho_RegionDifference);
		//connection (RegionDifference, ConnectedRegions2)

		//select_shape_std (ConnectedRegions2, SelectedRegions1, 'max_area', 70)
		erosion_circle(ho_RegionDifference, &ho_RegionErosion, 15);
		gray_dilation_rect(ho_ImageChannel[1], &ho_ImageMax, 7, 7);
		sub_image(ho_ImageMax, ho_ImageChannel[1], &ho_ImageSub, 1, 0);
		threshold(ho_ImageSub, &ho_Region2, 10, 255);
		opening_circle(ho_Region2, &ho_RegionOpening1, 2);
		intersection(ho_RegionErosion, ho_RegionOpening1, &ho_RegionIntersection);
		connection(ho_RegionIntersection, &ho_ConnectedRegions3);
		shape_trans(ho_ConnectedRegions3, &ho_RegionTrans, "convex");
		select_shape(ho_RegionTrans, &ho_SelectedRegions2, "area", "and", 200, 99999);
		count_obj(ho_SelectedRegions2, &hv_Number1);
		if (0 != (hv_Number1 == 0))
		{

			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			result = QString::fromLocal8Bit("无喷印日期");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "无喷印日期");
			return 1;
			gen_empty_obj(&ho_RegionData);
			// stop(); only in hdevelop
			//continue
		}
		else
		{
			union1(ho_SelectedRegions2, &ho_RegionUnion);
			closing_rectangle1(ho_RegionUnion, &ho_RegionClosing, 1, 100);
			connection(ho_RegionClosing, &ho_ConnectedRegions4);
			select_shape_std(ho_ConnectedRegions4, &ho_SelectedRegions3, "max_area", 70);
			smallest_rectangle2(ho_SelectedRegions3, &hv_Row1, &hv_Column1, &hv_Phi1, &hv_Length11,
				&hv_Length21);
			gen_rectangle2(&ho_RegionData, hv_Row1, hv_Column1, hv_Phi1, hv_Length11, hv_Length21);
			//喷印日期缺陷
			reduce_domain(ho_ImageChannel[5], ho_RegionData, &ho_ImageReduced4);
			/*local_threshold(ho_ImageReduced4, &ho_Region7, "adapted_std_deviation", "dark",
				"range", 40);*/
			gray_dilation_rect(ho_ImageReduced4, &ho_ImageMax3, 11, 11);
			dyn_threshold(ho_ImageReduced4, ho_ImageMax3, &ho_RegionDynThresh, 45, "dark");
			fill_up(ho_RegionDynThresh, &ho_RegionFillUp4);
			opening_circle(ho_RegionFillUp4, &ho_RegionOpening8, 3.5);
			area_center(ho_RegionOpening8, &hv_Area6, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
			if (0 != (hv_Area6 > 100))
			{
				disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");
				set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
				disp_obj(ho_RegionOpening8, Wnd == -1 ? m_ShowLabel[0] : Wnd);
				set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
				result = QString::fromLocal8Bit("喷印日期缺陷");
				write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "喷印日期缺陷");
				return 1;
			}


		}


		//**压合缺陷
		threshold(ho_ImageChannel[5], &ho_Region4, 190, 255);
		connection(ho_Region4, &ho_ConnectedRegions6);
		closing_circle(ho_ConnectedRegions6, &ho_RegionClosing1, 10);
		opening_circle(ho_RegionClosing1, &ho_RegionOpening3, 7.5);
		select_shape(ho_RegionOpening3, &ho_SelectedRegions4, "area", "and", 1500, 99999);
		intersection(ho_RegionDifference1, ho_SelectedRegions4, &ho_RegionIntersection1
		);
		area_center(ho_RegionIntersection1, &hv_Area2, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_Area2 > 5000))
		{

			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");

			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

			disp_obj(ho_RegionIntersection1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			result = QString::fromLocal8Bit("压合缺陷");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "压合缺陷");
			return 1;
			// stop(); only in hdevelop
			//continue
		}
		//***去除药剂区域和日期去区域
		difference(ho_RegionDifference1, ho_RegionData, &ho_RegionLeft);

		//***版面异常
		reduce_domain(ho_ImageChannel[2], ho_RegionLeft, &ho_ImageReduced2);
		gray_dilation_rect(ho_ImageReduced2, &ho_ImageMax1, 7, 7);
		sub_image(ho_ImageMax1, ho_ImageReduced2, &ho_ImageSub1, 1, 0);
		reduce_domain(ho_ImageSub1, ho_BandErosion, &ho_ImageReduced3);
		threshold(ho_ImageReduced3, &ho_Region5, 40, 255);
		fill_up(ho_Region5, &ho_RegionFillUp3);
		closing_circle(ho_RegionFillUp3, &ho_RegionClosing2, 3.5);
		opening_circle(ho_RegionClosing2, &ho_RegionOpening4, 2.5);
		connection(ho_RegionOpening4, &ho_ConnectedRegions7);
		shape_trans(ho_ConnectedRegions7, &ho_RegionTrans1, "convex");
		select_shape(ho_RegionTrans1, &ho_SelectedRegions5, "area", "and", 100, 99999);
		union1(ho_SelectedRegions5, &ho_BandStrange);
		area_center(ho_BandStrange, &hv_Area3, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (0 != (hv_Area3.Num()))
		{

			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");

			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

			disp_obj(ho_BandStrange, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			result = QString::fromLocal8Bit("版面异常");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "版面异常");
			return 1;
			// stop(); only in hdevelop
			//continue
		}


		//**片剂内部异常
		gen_empty_obj(&ho_PillInter);
		//dilation_circle (PillDilation, PillDilation1, 10)
		erosion_circle(ho_PillDilation, &ho_RegionErosion3, 10.5);
		shape_trans(ho_RegionErosion3, &ho_PillDilation1, "convex");
		union1(ho_PillDilation1, &ho_PillDilation2);
		//union1 (PillRegions, PillRegions)
		reduce_domain(ho_ImageChannel[1], ho_PillDilation2, &ho_PillImage);
		//texture_laws (PillImage, ImageTexture, 'le', 3, 5)

		gray_dilation_rect(ho_PillImage, &ho_ImageMax2, 5, 5);
		sub_image(ho_ImageMax2, ho_PillImage, &ho_ImageSub3, 1, 0);
		threshold(ho_ImageSub3, &ho_Region6, 25, 255);
		//fill_up (Region6, RegionFillUp5)
		opening_circle(ho_Region6, &ho_RegionOpening6, 1);
		closing_circle(ho_RegionOpening6, &ho_RegionClosing3, 3.5);
		connection(ho_RegionClosing3, &ho_ConnectedRegions9);
		select_shape(ho_ConnectedRegions9, &ho_SelectedRegions6, "area", "and", 50, 99999);


		connection(ho_PillRegions, &ho_ConnectedRegions8);
		sort_region(ho_ConnectedRegions8, &ho_SortedRegions, "character", "true", "row");
		opening_circle(ho_SortedRegions, &ho_RegionOpening5, 5);
		{
			HTuple end_val183 = hv_Number;
			HTuple step_val183 = 1;
			for (hv_Index1 = 1; hv_Index1.Continue(end_val183, step_val183); hv_Index1 += step_val183)
			{
				select_obj(ho_RegionOpening5, &ho_ObjectSelected, hv_Index1);
				//shape_trans (ObjectSelected, RegionTrans2, 'convex')
				//erosion_circle (RegionTrans2, RegionErosion1, 10)
				//intersection (RegionErosion1, SelectedRegions6, RegionIntersection2)
				smallest_circle(ho_ObjectSelected, &hv_Row2, &hv_Column2, &hv_Radius);
				gen_circle(&ho_Circle, hv_Row2, hv_Column2, hv_Radius - 3);
				difference(ho_Circle, ho_ObjectSelected, &ho_RegionDifference2);
				opening_circle(ho_RegionDifference2, &ho_RegionOpening7, 2.5);
				area_center(ho_RegionOpening7, &hv_Area4, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
				dilation_circle(ho_Circle, &ho_RegionDilation, 5.5);
				intersection(ho_RegionDilation, ho_SelectedRegions6, &ho_RegionIntersection3
				);
				area_center(ho_RegionIntersection3, &hv_Area5, &hv_ExpDefaultCtrlDummyVar,
					&hv_ExpDefaultCtrlDummyVar);
				if (0 != (hv_Area5 > 40))
				{
					concat_obj(ho_PillInter, ho_RegionIntersection3, &ho_PillInter);
				}
				if (0 != (hv_Area4 > 50))
				{
					concat_obj(ho_PillInter, ho_RegionOpening7, &ho_PillInter);
				}
			}
		}
		//union2 (SelectedRegions6, PillInter, PillInter)
		count_obj(ho_PillInter, &hv_Number2);
		if (0 != (hv_Number2 != 0))
		{

			disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);

			set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "fill");

			set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");

			disp_obj(ho_PillInter, Wnd == -1 ? m_ShowLabel[0] : Wnd);
			set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
			result = QString::fromLocal8Bit("片剂内部错误");
			write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "片剂内部错误");
			return 1;
			// stop(); only in hdevelop
			//continue
		}
		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "blue");
		set_line_width(Wnd == -1 ? m_ShowLabel[0] : Wnd, 2);
		disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		disp_obj(ho_PillRegions, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		disp_obj(ho_RegionData, Wnd == -1 ? m_ShowLabel[0] : Wnd);

		//disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
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