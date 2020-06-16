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
	checkparam->i_tempThread = configIniRead.value("/" + cameraname + "/tempThread", "150").toInt();
	checkparam->i_CapsuleCount = configIniRead.value("/" + cameraname + "/CapsuleCount", "6").toInt();
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
	configIniRead.setValue("/" + cameraname + "/tempThread", checkparam->i_tempThread);
	configIniRead.setValue("/" + cameraname + "/CapsuleCount", checkparam->i_CapsuleCount);
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
	QtGuiSetting *setdlg = new QtGuiSetting(parent, this);
	setdlg->SetParam(m_checkparam);
	setdlg->SetModelMat(LastImage);
	m_bchangedparam = false;
	QObject::connect(setdlg, SIGNAL(ShouldSaveParam(CHECKPARAM*)), this, SLOT(SaveCheckParam(CHECKPARAM*)));
	setdlg->exec();
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
Hobject Mat2Hobject(Mat& image)
{
	Hobject Hobj = Hobject();
	int hgt = image.rows;
	int wid = image.cols;
	int i;
	//	CV_8UC3
	if (image.type() == CV_8UC3)
	{
		vector<Mat> imgchannel;
		split(image, imgchannel);
		Mat imgB = imgchannel[0];
		Mat imgG = imgchannel[1];
		Mat imgR = imgchannel[2];
		uchar* dataR = new uchar[hgt*wid];
		uchar* dataG = new uchar[hgt*wid];
		uchar* dataB = new uchar[hgt*wid];
		for (i = 0; i < hgt; i++)
		{
			memcpy(dataR + wid * i, imgR.data + imgR.step*i, wid);
			memcpy(dataG + wid * i, imgG.data + imgG.step*i, wid);
			memcpy(dataB + wid * i, imgB.data + imgB.step*i, wid);
		}
		gen_image3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
		delete[]dataR;
		delete[]dataG;
		delete[]dataB;
		dataR = NULL;
		dataG = NULL;
		dataB = NULL;
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
Mat Hobject2Mat(Hobject Hobj)
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
//模乘运算，返回值 x=a*b mod n
static inline unsigned __int64 MulMod(unsigned __int64 a, unsigned __int64 b, unsigned __int64 n)
{
	return a * b % n;
}
//模幂运算，返回值 x=base^pow mod n
static unsigned __int64 PowMod(unsigned __int64 &base, unsigned __int64 &pow, unsigned __int64 &n)
{
	unsigned __int64    a = base, b = pow, c = 1;
	while (b)
	{
		while (!(b & 1))
		{
			b >>= 1;            //a=a * a % n;    //函数看起来可以处理64位的整数，但由于这里a*a在a>=2^32时已经造成了溢出，因此实际处理范围没有64位
			a = MulMod(a, a, n);
		}        b--;        //c=a * c % n;        //这里也会溢出，若把64位整数拆为两个32位整数不知是否可以解决这个问题。
		c = MulMod(a, c, n);
	}    return c;
}

/*
Stein法求最大公约数
*/
static unsigned __int64 SteinGcd(unsigned __int64 &p, unsigned __int64 &q)
{
	unsigned __int64    a = p > q ? p : q;
	unsigned __int64    b = p < q ? p : q;
	unsigned __int64    t, r = 1;
	if (p == q)
	{
		return p;           //两数相等，最大公约数就是本身
	}
	else
	{
		while ((!(a & 1)) && (!(b & 1)))
		{
			r <<= 1;          //a、b为偶数时，gcd(a,b)=2*gcd(a/2,b/2)
			a >>= 1;
			b >>= 1;
		}        if (!(a & 1))
		{
			t = a;            //a为偶数，交换a，b
			a = b;
			b = t;
		}        do
		{
			while (!(b & 1))
			{
				b >>= 1;      //b为偶数，a为奇数时，gcd(b,a)=gcd(b/2,a)
			}            if (b < a)
			{
				t = a;        //b小于a，交换a，b
				a = b;
				b = t;
			}            b = (b - a) >> 1; //b、a都是奇数，gcd(b,a)=gcd((b-a)/2,a)
		} while (b);
		return r * a;
	}
}
/*
已知a、b，求x，满足a*x =1 (mod b)
相当于求解a*x-b*y=1的最小整数解
*/
static unsigned __int64 Euclid(unsigned __int64 &a, unsigned __int64 &b)
{
	unsigned __int64    m, e, i, j, x, y;
	long                xx, yy;
	m = b; e = a; x = 0; y = 1; xx = 1; yy = 1;
	while (e)
	{
		i = m / e; j = m % e;
		m = e; e = j; j = y; y *= i;
		if (xx == yy)
		{
			if (x > y)
				y = x - y;
			else {
				y -= x;
				yy = 0;
			}
		}
		else
		{
			y += x;
			xx = 1 - xx;
			yy = 1 - yy;
		}        x = j;
	}
	if (xx == 0)
		x = b - x;
	return x;
}


void * CInterCHeck::GetEncryptHandle()
{
	HANDLE _auhandle;

	QSettings configIniRead(AppPath + "\\ProgramSet.ini", QSettings::IniFormat);
	QString q_seed = configIniRead.value("/SystemConfig/SEED", "NoRead").toString();
	QString q_key = configIniRead.value("/SystemConfig/KEY", "NoRead").toString();
	string seed, key;
	unsigned __int64    p, q;   //两个素数，不参与加密解密运算
	unsigned __int64    f;      //f=(p-1)*(q-1)，不参与加密解密运算
	unsigned __int64    n, e;   //公匙，n=p*q，gcd(e,f)=1
	unsigned __int64    d;      //私匙，e*d=1 (mod f)，gcd(n,d)=1
	unsigned __int64    s;      //块长，满足2^s<=n的最大的s，即log2(n)
	unsigned __int64    t;

	if (0 == strcmp(q_seed.toStdString().c_str(), "NoRead"))
	{
		return (void*)-1;
	}
	else
	{
		p = 127/*RandomPrime(16)*/;          //随机生成两个素数
		q = 167/*RandomPrime(16)*/;
		n = p * q;
		f = (p - 1) * (q - 1);
		e;
		do
		{
			e = 1024/*g_Rnd.Random(65536)*/;  //小于2^16，65536=2^16
			e |= 1;                   //保证最低位是1，即保证是奇数，因f一定是偶数，要互素，只能是奇数
		} while (SteinGcd(e, f) != 1);
		d = Euclid(e, f);
		s = 0;
		t = n >> 1;
		while (t)
		{
			s++;                    //s=log2(n)
			t >>= 1;
		}
		char *pDec = new char[100];
		char *pEnc = new char[100];
		int sizeseed = strlen(q_seed.toStdString().c_str());
		char seg[] = " "; /*分隔符这里为逗号comma，分隔符可以为你指定的，如分号，空格等*/
		int i = 0;
		char temp[255];

		strcpy_s(temp, q_seed.toStdString().c_str());
		char *substr = strtok(temp, seg);

		while (substr != NULL) {
			unsigned __int64 x = 0;
			sscanf(substr, "%x", &x);
			pDec[i++] = PowMod(x, d, n);		//e,n加密；d,n解密
			substr = strtok(NULL, seg);/*在第一次调用时，strtok()必需给予参数str字符串，
									   往后的调用则将参数str设置成NULL。每次调用成功则返回被分割出片段的指针。*/
		}
		seed = pDec;

	}
	if (0 == strcmp(q_key.toStdString().c_str(), "NoRead"))
	{
		return (void*)-1;
	}
	else
	{
		p = 127/*RandomPrime(16)*/;          //随机生成两个素数
		q = 163/*RandomPrime(16)*/;
		n = p * q;
		f = (p - 1) * (q - 1);
		e = 1024;
		do
		{
			e = 1024/*g_Rnd.Random(65536)*/;  //小于2^16，65536=2^16
			e |= 1;                   //保证最低位是1，即保证是奇数，因f一定是偶数，要互素，只能是奇数
		} while (SteinGcd(e, f) != 1);
		d = Euclid(e, f);
		s = 0;
		t = n >> 1;
		while (t)
		{
			s++;                    //s=log2(n)
			t >>= 1;
		}
		char *pDec = new char[100];
		char *pEnc = new char[100];
		int sizeseed = strlen(q_key.toStdString().c_str());
		char seg[] = " "; /*分隔符这里为逗号comma，分隔符可以为你指定的，如分号，空格等*/
		int i = 0;
		char temp[255];

		strcpy_s(temp, q_key.toStdString().c_str());
		char *substr = strtok(temp, seg);

		while (substr != NULL) {
			unsigned __int64 x = 0;
			sscanf(substr, "%x", &x);
			pDec[i++] = PowMod(x, d, n);
			substr = strtok(NULL, seg);/*在第一次调用时，strtok()必需给予参数str字符串，
									   往后的调用则将参数str设置成NULL。每次调用成功则返回被分割出片段的指针。*/
		}
		key = pDec;
	}

	DWORD	dwCount;
	/*
	int i_key = R1_Find((BYTE*)seed.c_str(), &dwCount);
	if (i_key == 0 && dwCount > 0)
	{
		i_key = R1_Open(&_auhandle, (BYTE*)seed.c_str(), 0);
		BYTE	tryCount;
		i_key = R1_VerifySoPin(_auhandle, (BYTE*)key.c_str(), &tryCount);
		if (R1_S_SUCCESS == i_key)
		{
			return _auhandle;
		}
		return (HANDLE)-1;
	}
	else*/
	return (void*)-1;
}

void CInterCHeck::EnableShow(bool)
{
}

void CInterCHeck::TESTSETSHOW(void *showlabel)
{
	HTuple hv_WindowID;
	open_window(((QLabel*)showlabel)->y(), ((QLabel*)showlabel)->x(), ((QLabel*)showlabel)->width(), ((QLabel*)showlabel)->height(), (long)((QWidget*)((QLabel*)showlabel)->parent())->winId(), "", "", &hv_WindowID);
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
}

bool CInterCHeck::OtherAfterCheck()
{
	//imageCallBack(ui, m_iShowPos, MatToShow, total_check);
	total_check++;
	return true;
}

bool CInterCHeck::RealCheck()
{
	// Local iconic variables 
	Hobject  ho_EmptyObjectWidth, ho_EmptyObjectHeight;
	Hobject  ho_EmptyObject_Inner, ho_EmptyObject_Leaking, ho_Rectangle6;
	Hobject  ho_Image, ho_Image1, ho_Image2, ho_Image3, ho_ImageResult1;
	Hobject  ho_ImageResult2, ho_ImageResult3, ho_Region, ho_RegionFillUp;
	Hobject  ho_ConnectedRegions, ho_SelectedRegions3, ho_SortedRegions;
	Hobject  ho_SortedRegions2, ho_RegionErosion2, ho_ObjectSelected1;
	Hobject  ho_RegionErosion4, ho_ImageReduced4, ho_Region2;
	Hobject  ho_RegionOpening1, ho_RegionErosion3, ho_ImageReduced3;
	Hobject  ho_Regions, ho_RegionOpening, ho_RegionTrans1, ho_RegionDifference2;
	Hobject  ho_Rectangle5;


	// Local control variables 
	HTuple  hv_ImageFiles, hv_Index, hv_Width, hv_Height;
	HTuple  hv_Index1, hv_Error_Index, hv_ExpDefaultCtrlDummyVar;
	HTuple  hv_Length1, hv_Length2, hv_Number2, hv_Number4;
	HTuple  hv_Index5, hv_b_shouldBreak, hv_Index2, hv_b_eachW;
	HTuple  hv_b_eachH, hv_b_eachLeak, hv_b_eachInner, hv_Area5;
	HTuple  hv_Row14, hv_Column14, hv_Row24, hv_Column24, hv_outH;
	HTuple  hv_outW, hv_Row13, hv_Column13, hv_Row23, hv_Column23;
	HTuple  hv_w, hv_h, hv_cc, hv_ch, hv_s, hv_Area_W, hv_Area_H;
	HTuple  hv_Area_Inner, hv_Area_Leaking, hv_Indices;


	get_image_size(m_hoLiveImage, &hv_Width, &hv_Height);
	set_part(m_ShowLabel[total_check%circle_times], 0, 0, hv_Height - 1, hv_Width - 1);
	//宽度检测
	gen_empty_obj(&ho_EmptyObjectWidth);
	//高度检测
	gen_empty_obj(&ho_EmptyObjectHeight);
	gen_empty_obj(&ho_EmptyObject_Inner);
	gen_empty_obj(&ho_EmptyObject_Leaking);
	//Image Acquisition 01: Do something
	for (hv_Index1 = 0; hv_Index1 <= 1; hv_Index1 += 1)
	{
		if (0 != (hv_Index1 == 0))
		{
			hv_Error_Index = HTuple();
		}
		gen_rectangle1(&ho_Rectangle6, (hv_Height / 2)*hv_Index1, 0, (hv_Height / 2)*(hv_Index1 + 1),
			hv_Width);
		reduce_domain(m_hoLiveImage, ho_Rectangle6, &ho_Image);
		decompose3(ho_Image, &ho_Image1, &ho_Image2, &ho_Image3);
		trans_from_rgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2,
			&ho_ImageResult3, "hsv");
		threshold(ho_Image1, &ho_Region, 135, 255);
		fill_up(ho_Region, &ho_RegionFillUp);
		connection(ho_RegionFillUp, &ho_ConnectedRegions);
		select_shape(ho_ConnectedRegions, &ho_SelectedRegions3, "area", "and", 13000,
			99999);
		sort_region(ho_SelectedRegions3, &ho_SortedRegions, "character", "true", "row");
		smallest_rectangle2(ho_SelectedRegions3, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar,
			&hv_ExpDefaultCtrlDummyVar, &hv_Length1, &hv_Length2);
		count_obj(ho_SortedRegions, &hv_Number2);

		sort_region(ho_SelectedRegions3, &ho_SortedRegions2, "character", "true", "row");
		erosion_circle(ho_SortedRegions2, &ho_RegionErosion2, 5.5);
		count_obj(ho_RegionErosion2, &hv_Number4);
		{
			HTuple end_val34 = hv_Number4;
			HTuple step_val34 = 1;
			for (hv_Index5 = 1; hv_Index5.Continue(end_val34, step_val34); hv_Index5 += step_val34)
			{
				hv_b_shouldBreak = 0;
				if (0 != (hv_Index1 != 0))
				{
					{
						HTuple end_val37 = (hv_Error_Index.Num()) - 1;
						HTuple step_val37 = 1;
						for (hv_Index2 = 0; hv_Index2.Continue(end_val37, step_val37); hv_Index2 += step_val37)
						{
							if (0 != ((hv_Error_Index.Select(hv_Index2)) == hv_Index5))
							{
								hv_b_shouldBreak = 1;
								break;
							}
						}
					}
				}
				if (0 != (hv_b_shouldBreak == 1))
				{
					continue;
				}
				hv_b_eachW = 0;
				hv_b_eachH = 0;
				hv_b_eachLeak = 0;
				hv_b_eachInner = 0;
				select_obj(ho_SortedRegions2, &ho_ObjectSelected1, hv_Index5);
				smallest_rectangle2(ho_ObjectSelected1, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar,
					&hv_ExpDefaultCtrlDummyVar, &hv_Length1, &hv_Length2);
				if (0 != (hv_Length2 > 41))
				{
					union2(ho_EmptyObjectWidth, ho_ObjectSelected1, &ho_EmptyObjectWidth);
					hv_b_eachW = 1;
				}
				if (0 != (hv_Length1 > 110))
				{
					union2(ho_EmptyObjectHeight, ho_ObjectSelected1, &ho_EmptyObjectHeight);
					hv_b_eachH = 1;
				}
				erosion_circle(ho_ObjectSelected1, &ho_RegionErosion4, 3.5);
				reduce_domain(ho_Image1, ho_RegionErosion4, &ho_ImageReduced4);
				threshold(ho_ImageReduced4, &ho_Region2, 0, 180);
				opening_circle(ho_Region2, &ho_RegionOpening1, 0.5);
				area_center(ho_RegionOpening1, &hv_Area5, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
				if (0 != (hv_Area5.Num()))
				{
					if (0 != (hv_Area5 > 0))
					{
						concat_obj(ho_EmptyObject_Leaking, ho_RegionOpening1, &ho_EmptyObject_Leaking
						);
						concat_obj(ho_EmptyObject_Leaking, ho_ObjectSelected1, &ho_EmptyObject_Leaking
						);
						hv_b_eachLeak = 1;
					}
				}

				//检测内部弯折缺陷需要腐蚀特定值 3.5
				erosion_circle(ho_ObjectSelected1, &ho_RegionErosion3, 3.5);
				reduce_domain(ho_ImageResult2, ho_RegionErosion3, &ho_ImageReduced3);
				threshold(ho_ImageReduced3, &ho_Regions, 0, 150);
				opening_circle(ho_Regions, &ho_RegionOpening, 1.5);
				smallest_rectangle1(ho_RegionOpening, &hv_Row14, &hv_Column14, &hv_Row24,
					&hv_Column24);
				hv_outH = hv_Row24 - hv_Row14;
				hv_outW = hv_Column24 - hv_Column14;
				shape_trans(ho_RegionOpening, &ho_RegionTrans1, "convex");
				difference(ho_RegionTrans1, ho_RegionOpening, &ho_RegionDifference2);
				inner_rectangle1(ho_RegionDifference2, &hv_Row13, &hv_Column13, &hv_Row23,
					&hv_Column23);
				gen_rectangle1(&ho_Rectangle5, hv_Row13, hv_Column13, hv_Row23, hv_Column23);
				hv_w = hv_Column23 - hv_Column13;
				hv_h = hv_Row23 - hv_Row13;
				hv_cc = hv_Column13 + (hv_w / 2);
				hv_ch = hv_Row13 + (hv_h / 2);
				hv_s = (hv_outH / 5) * 3;
				if (0 != (hv_h < ((hv_outH / 10) * 6)))
				{
					//union2 (EmptyObject_Inner, Rectangle5, EmptyObject_Inner)
					//union2 (EmptyObject_Inner, ObjectSelected4, EmptyObject_Inner)
					//b_eachInner := 1
				}

				if (0 != (HTuple(HTuple(hv_b_eachW.Or(hv_b_eachH)).Or(hv_b_eachInner)).Or(hv_b_eachLeak)))
				{
					hv_Error_Index = hv_Error_Index.Concat(hv_Index5);
				}
			}
		}

	}

	area_center(ho_EmptyObjectWidth, &hv_Area_W, _, _);
	area_center(ho_EmptyObjectHeight, &hv_Area_H, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);

	area_center(ho_EmptyObject_Inner, &hv_Area_Inner, &hv_ExpDefaultCtrlDummyVar,
		&hv_ExpDefaultCtrlDummyVar);
	area_center(ho_EmptyObject_Leaking, &hv_Area_Leaking, &hv_ExpDefaultCtrlDummyVar,
		&hv_ExpDefaultCtrlDummyVar);


	if (0 != (HTuple(HTuple(HTuple(hv_Area_W > 0).Or(hv_Area_H > 0)).Or(hv_Area_Inner.Num())).Or(hv_Area_Leaking.Num())))
	{
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		set_draw(m_ShowLabel[total_check%circle_times], "margin");
		set_line_width(m_ShowLabel[total_check%circle_times], 5);
		set_color(m_ShowLabel[total_check%circle_times], "blue");
		set_tposition(3600, 24, 20);
		write_string(3600, "宽度异常");
		disp_obj(ho_EmptyObjectWidth, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "yellow");
		set_tposition(3600, 64, 20);
		write_string(3600, "高度异常");
		disp_obj(ho_EmptyObjectHeight, m_ShowLabel[total_check%circle_times]);
		disp_obj(ho_EmptyObjectWidth, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "cyan");
		set_tposition(3600, 104, 20);
		write_string(3600, "胶囊内部异常");
		disp_obj(ho_EmptyObject_Inner, m_ShowLabel[total_check%circle_times]);
		set_color(m_ShowLabel[total_check%circle_times], "white");
		set_tposition(3600, 144, 20);
		write_string(3600, "胶囊内杂质");
		disp_obj(ho_EmptyObject_Leaking, m_ShowLabel[total_check%circle_times]);
		tuple_sort(hv_Error_Index, &hv_Indices);
		set_colored(m_ShowLabel[total_check%circle_times], 12);
	}
	else
	{
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
	}

	//disp_image(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
	return false;
}
