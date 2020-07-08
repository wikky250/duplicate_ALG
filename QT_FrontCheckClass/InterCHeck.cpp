#include "InterCHeck.h" 
#include <QMessageBox>
//#include "image.h"
QString AppPath;




bool CInterCHeck::LoadCheckParam(CHECKPARAM * checkparam)
{
	QSettings configIniRead(AppPath + "\\DefaultModel\\CheckParam.ini", QSettings::IniFormat);
	configIniRead.setIniCodec("UTF-8");
	QString cameraname = checkparam->c_CameraName;
	//��ȡ����������
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
	//д�����������
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
	//�����ƶ�������λ�á�2018��11��23��13:47:49
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
	QMessageBox::about(NULL, QString::fromLocal8Bit("���ش���"), QString::fromLocal8Bit("USBkey����Ͽ���"));
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
//ģ�����㣬����ֵ x=a*b mod n
static inline unsigned __int64 MulMod(unsigned __int64 a, unsigned __int64 b, unsigned __int64 n)
{
	return a * b % n;
}
//ģ�����㣬����ֵ x=base^pow mod n
static unsigned __int64 PowMod(unsigned __int64 &base, unsigned __int64 &pow, unsigned __int64 &n)
{
	unsigned __int64    a = base, b = pow, c = 1;
	while (b)
	{
		while (!(b & 1))
		{
			b >>= 1;            //a=a * a % n;    //�������������Դ���64λ������������������a*a��a>=2^32ʱ�Ѿ��������������ʵ�ʴ���Χû��64λ
			a = MulMod(a, a, n);
		}        b--;        //c=a * c % n;        //����Ҳ�����������64λ������Ϊ����32λ������֪�Ƿ���Խ��������⡣
		c = MulMod(a, c, n);
	}    return c;
}

/*
Stein�������Լ��
*/
static unsigned __int64 SteinGcd(unsigned __int64 &p, unsigned __int64 &q)
{
	unsigned __int64    a = p > q ? p : q;
	unsigned __int64    b = p < q ? p : q;
	unsigned __int64    t, r = 1;
	if (p == q)
	{
		return p;           //������ȣ����Լ�����Ǳ���
	}
	else
	{
		while ((!(a & 1)) && (!(b & 1)))
		{
			r <<= 1;          //a��bΪż��ʱ��gcd(a,b)=2*gcd(a/2,b/2)
			a >>= 1;
			b >>= 1;
		}        if (!(a & 1))
		{
			t = a;            //aΪż��������a��b
			a = b;
			b = t;
		}        do
		{
			while (!(b & 1))
			{
				b >>= 1;      //bΪż����aΪ����ʱ��gcd(b,a)=gcd(b/2,a)
			}            if (b < a)
			{
				t = a;        //bС��a������a��b
				a = b;
				b = t;
			}            b = (b - a) >> 1; //b��a����������gcd(b,a)=gcd((b-a)/2,a)
		} while (b);
		return r * a;
	}
}
/*
��֪a��b����x������a*x =1 (mod b)
�൱�����a*x-b*y=1����С������
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
	unsigned __int64    p, q;   //������������������ܽ�������
	unsigned __int64    f;      //f=(p-1)*(q-1)����������ܽ�������
	unsigned __int64    n, e;   //���ף�n=p*q��gcd(e,f)=1
	unsigned __int64    d;      //˽�ף�e*d=1 (mod f)��gcd(n,d)=1
	unsigned __int64    s;      //�鳤������2^s<=n������s����log2(n)
	unsigned __int64    t;

	if (0 == strcmp(q_seed.toStdString().c_str(), "NoRead"))
	{
		return (void*)-1;
	}
	else
	{
		p = 127/*RandomPrime(16)*/;          //���������������
		q = 167/*RandomPrime(16)*/;
		n = p * q;
		f = (p - 1) * (q - 1);
		e;
		do
		{
			e = 1024/*g_Rnd.Random(65536)*/;  //С��2^16��65536=2^16
			e |= 1;                   //��֤���λ��1������֤����������fһ����ż����Ҫ���أ�ֻ��������
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
		char seg[] = " "; /*�ָ�������Ϊ����comma���ָ�������Ϊ��ָ���ģ���ֺţ��ո��*/
		int i = 0;
		char temp[255];

		strcpy_s(temp, q_seed.toStdString().c_str());
		char *substr = strtok(temp, seg);

		while (substr != NULL) {
			unsigned __int64 x = 0;
			sscanf(substr, "%x", &x);
			pDec[i++] = PowMod(x, d, n);		//e,n���ܣ�d,n����
			substr = strtok(NULL, seg);/*�ڵ�һ�ε���ʱ��strtok()����������str�ַ�����
									   ����ĵ����򽫲���str���ó�NULL��ÿ�ε��óɹ��򷵻ر��ָ��Ƭ�ε�ָ�롣*/
		}
		seed = pDec;

	}
	if (0 == strcmp(q_key.toStdString().c_str(), "NoRead"))
	{
		return (void*)-1;
	}
	else
	{
		p = 127/*RandomPrime(16)*/;          //���������������
		q = 163/*RandomPrime(16)*/;
		n = p * q;
		f = (p - 1) * (q - 1);
		e = 1024;
		do
		{
			e = 1024/*g_Rnd.Random(65536)*/;  //С��2^16��65536=2^16
			e |= 1;                   //��֤���λ��1������֤����������fһ����ż����Ҫ���أ�ֻ��������
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
		char seg[] = " "; /*�ָ�������Ϊ����comma���ָ�������Ϊ��ָ���ģ���ֺţ��ո��*/
		int i = 0;
		char temp[255];

		strcpy_s(temp, q_key.toStdString().c_str());
		char *substr = strtok(temp, seg);

		while (substr != NULL) {
			unsigned __int64 x = 0;
			sscanf(substr, "%x", &x);
			pDec[i++] = PowMod(x, d, n);
			substr = strtok(NULL, seg);/*�ڵ�һ�ε���ʱ��strtok()����������str�ַ�����
									   ����ĵ����򽫲���str���ó�NULL��ÿ�ε��óɹ��򷵻ر��ָ��Ƭ�ε�ָ�롣*/
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

bool CInterCHeck::RealCheck(QString &result)
{
	try
	{
		// Local iconic variables 
		QStringList qslre;
		qslre << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR";
		// 		if (m_b_test)
		// 		{
		// 			qslre << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR" << "DERROR";
		//  		}
		// 		else
		// 		{
		// 			if (m_iShowPos ==0)
		// 			{
		// 				if ((total_check/circle_times)%2==0)
		// 					qslre << "Good" << "DERROR" << "Good" << "DERROR" << "Good" << "DERROR";				
		// 				else
		// 					qslre << "DERROR" << "Good" << "DERROR" << "Good" << "DERROR" << "Good";
		// 			}
		// 			if (m_iShowPos == 1)
		// 			{
		// 				if ((total_check / circle_times) % 2 == 0)
		// 					qslre << "Good" << "Good" << "Good" << "DERROR" << "DERROR" << "DERROR";
		// 				else
		// 					qslre << "DERROR" << "DERROR" << "DERROR" << "Good" << "Good" << "Good";
		// 
		// 			}
		// 			if (m_iShowPos == 2)
		// 			{
		// 				if ((total_check / circle_times) % 2 == 0)
		// 					qslre << "DERROR" << "DERROR" << "DERROR" << "Good" << "Good" << "Good";
		// 				else
		// 					qslre << "Good" << "Good" << "Good" << "DERROR" << "DERROR" << "DERROR";
		// 			}
		// 			if (m_iShowPos == 3)
		// 			{
		// 				if ((total_check / circle_times) % 2 == 0)
		// 					qslre << "DERROR" << "Good" << "DERROR" << "Good" << "DERROR" << "Good";
		// 				else
		// 					qslre << "Good" << "DERROR" << "Good" << "DERROR" << "Good" << "DERROR";
		// 			}
		// 			if (m_iShowPos == 4)
		// 			{
		// 				if ((total_check / circle_times) % 2 == 0)
		// 					qslre << "Good" << "Good" << "DERROR" << "DERROR" << "Good" << "Good";
		// 				else
		// 					qslre << "DERROR" << "DERROR" << "Good" << "Good" << "DERROR" << "DERROR";
		// 			}
		// 		}
		result = qslre.join(",");
		Hobject  ho_EmptyObjectWidth, ho_EmptyObjectHeight;
		Hobject  ho_EmptyObject_Inner, ho_EmptyObject_Leaking, ho_EmptyObject_Other;
		Hobject  ho_Rectangle6, ho_Image, ho_Image1, ho_Image2, ho_Image3;
		Hobject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
		Hobject  ho_Region, ho_Rectangle, ho_RegionIntersection;
		Hobject  ho_ConnectedRegions, ho_SelectedRegions3, ho_RegionFillUp;
		Hobject  ho_RegionErosion2, ho_RegionErosion4, ho_ImageReduced4;
		Hobject  ho_Region2, ho_RegionOpening1, ho_RegionErosion3;
		Hobject  ho_ImageReduced3, ho_Regions, ho_RegionOpening;
		Hobject  ho_RegionTrans1, ho_RegionDifference2, ho_Rectangle5;


		// Local control variables 
		HTuple  hv_in, hv_ImageFiles, hv_Index, hv_Error_Index;
		HTuple  hv_Width, hv_Height, hv_Index1, hv_Index3, hv_b_eachW;
		HTuple  hv_b_eachH, hv_b_eachLeak, hv_b_eachInner, hv_b_eachOther;
		HTuple  hv_b_eachLoss, hv_ExpDefaultCtrlDummyVar, hv_Length1;
		HTuple  hv_Length2, hv_Number2, hv_Number4, hv_b_shouldBreak;
		HTuple  hv_Index2, hv_Mean, hv_Deviation, hv_Area5, hv_Row14;
		HTuple  hv_Column14, hv_Row24, hv_Column24, hv_outH, hv_outW;
		HTuple  hv_Row13, hv_Column13, hv_Row23, hv_Column23, hv_w;
		HTuple  hv_h, hv_cc, hv_ch, hv_s, hv_Area_Inner, hv_Area_Leaking;

		get_image_size(m_hoLiveImage, &hv_Width, &hv_Height);

		//��ȼ��
		gen_empty_obj(&ho_EmptyObjectWidth);
		//�߶ȼ��
		gen_empty_obj(&ho_EmptyObjectHeight);
		gen_empty_obj(&ho_EmptyObject_Inner);
		gen_empty_obj(&ho_EmptyObject_Leaking);
		gen_empty_obj(&ho_EmptyObject_Other);

		if (total_check < circle_times)
		{
			set_part(m_ShowLabel[total_check%circle_times], 0, 0, hv_Height - 1, hv_Width - 1);
		}

		//Image Acquisition 01: Do something
		for (hv_Index1 = 0; hv_Index1 <= 1; hv_Index1 += 1)
		{
			gen_rectangle1(&ho_Rectangle6, (hv_Height / 2)*hv_Index1, 0, (hv_Height / 2)*(hv_Index1 + 1),
				hv_Width);
			reduce_domain(m_hoLiveImage, ho_Rectangle6, &ho_Image);
			decompose3(ho_Image, &ho_Image1, &ho_Image2, &ho_Image3);
			trans_from_rgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2,
				&ho_ImageResult3, "hsv");
			threshold(ho_Image1, &ho_Region, 128, 255);
			for (hv_Index3 = 1; hv_Index3 <= 6; hv_Index3 += 1)
			{
				hv_b_eachW = 0;
				hv_b_eachH = 0;
				hv_b_eachLeak = 0;
				hv_b_eachInner = 0;
				hv_b_eachOther = 0;
				hv_b_eachLoss = 0;
				gen_rectangle1(&ho_Rectangle, 0, (hv_Width / 6)*(hv_Index3 - 1), hv_Height, (hv_Width / 6)*hv_Index3);
				intersection(ho_Rectangle, ho_Region, &ho_RegionIntersection);
				connection(ho_RegionIntersection, &ho_ConnectedRegions);
				select_shape(ho_ConnectedRegions, &ho_SelectedRegions3, "area", "and", 10000,
					99999);
				fill_up(ho_SelectedRegions3, &ho_RegionFillUp);
				smallest_rectangle2(ho_RegionFillUp, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar,
					&hv_ExpDefaultCtrlDummyVar, &hv_Length1, &hv_Length2);
				count_obj(ho_RegionFillUp, &hv_Number2);
				erosion_circle(ho_RegionFillUp, &ho_RegionErosion2, 5.5);
				count_obj(ho_RegionErosion2, &hv_Number4);

				hv_b_shouldBreak = 0;
				if (0 != (hv_Index1 != 0))
				{
					{
						HTuple end_val46 = (hv_Error_Index.Num()) - 1;
						HTuple step_val46 = 1;
						for (hv_Index2 = 0; hv_Index2.Continue(end_val46, step_val46); hv_Index2 += step_val46)
						{
							if (0 != ((hv_Error_Index.Select(hv_Index2)) == hv_Index3))
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
				if (0 != (hv_Number4 == 0))
				{
					hv_b_eachLoss = 1;
					hv_Error_Index = hv_Error_Index.Concat(hv_Index3);
					qslre[5 - (hv_Index3[0].I() - 1)] = "Loss ERROR";
				}
				else
				{
					smallest_rectangle2(ho_RegionErosion2, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar,
						&hv_ExpDefaultCtrlDummyVar, &hv_Length1, &hv_Length2);
					if (0 != (hv_Length2 > 41))
					{
						union2(ho_EmptyObjectWidth, ho_RegionErosion2, &ho_EmptyObjectWidth);
						hv_b_eachW = 1;
					}
					if (0 != (hv_Length1 > 110))
					{
						union2(ho_EmptyObjectHeight, ho_RegionErosion2, &ho_EmptyObjectHeight
						);
						hv_b_eachH = 1;
					}
					erosion_circle(ho_RegionErosion2, &ho_RegionErosion4, 5.5);

					intensity(ho_RegionErosion4, ho_Image2, &hv_Mean, &hv_Deviation);
					if (0 != (hv_Mean > 150))
					{
						hv_b_eachOther = 1;
						concat_obj(ho_EmptyObject_Other, ho_RegionErosion4, &ho_EmptyObject_Other
						);
					}


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
							concat_obj(ho_EmptyObject_Leaking, ho_RegionErosion2, &ho_EmptyObject_Leaking
							);
							hv_b_eachLeak = 1;
						}
					}

					//����ڲ�����ȱ����Ҫ��ʴ�ض�ֵ 3.5
					erosion_circle(ho_RegionErosion2, &ho_RegionErosion3, 3.5);
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
// 				if (0 != (HTuple(HTuple(HTuple(HTuple(hv_b_eachW.Or(hv_b_eachH)).Or(hv_b_eachLeak)).Or(hv_b_eachInner)).Or(hv_b_eachOther)).Or(hv_b_eachLoss)))
// 				{
// 					hv_Error_Index = hv_Error_Index.Concat(hv_Index3);
// 				}

				if (0 != (HTuple(hv_b_eachOther)))
				{
					hv_Error_Index = hv_Error_Index.Concat(hv_Index3);
					qslre[5 - (hv_Index3[0].I() - 1)] = "YSERROR";
				}
				else
				{
					qslre[5 - (hv_Index3[0].I() - 1)] = "Good";
				}
				}
				area_center(ho_EmptyObject_Inner, &hv_Area_Inner, &hv_ExpDefaultCtrlDummyVar,
					&hv_ExpDefaultCtrlDummyVar);
				area_center(ho_EmptyObject_Leaking, &hv_Area_Leaking, &hv_ExpDefaultCtrlDummyVar,
					&hv_ExpDefaultCtrlDummyVar);

			}
		}
		disp_obj(m_hoLiveImage, m_ShowLabel[total_check%circle_times]);
		result = qslre.join(",");
		if (0 != ((hv_Error_Index.Num()) > 0))
		{

// 			set_draw(m_ShowLabel[total_check%circle_times], "margin");
// 
// 			set_line_width(m_ShowLabel[total_check%circle_times], 5);
// 
// 			set_color(m_ShowLabel[total_check%circle_times], "blue");
// 			set_tposition(m_ShowLabel[total_check%circle_times], 24, 20);
// 			write_string(m_ShowLabel[total_check%circle_times], "����쳣");
// 
// 
// 			disp_obj(ho_EmptyObjectWidth, m_ShowLabel[total_check%circle_times]);
// 
// 			set_color(m_ShowLabel[total_check%circle_times], "yellow");
// 			set_tposition(m_ShowLabel[total_check%circle_times], 64, 20);
// 			write_string(m_ShowLabel[total_check%circle_times], "�߶��쳣");
// 
// 			disp_obj(ho_EmptyObjectHeight, m_ShowLabel[total_check%circle_times]);
// 
// 			disp_obj(ho_EmptyObjectWidth, m_ShowLabel[total_check%circle_times]);
// 
// 			set_color(m_ShowLabel[total_check%circle_times], "cyan");
// 			set_tposition(m_ShowLabel[total_check%circle_times], 104, 20);
// 			write_string(m_ShowLabel[total_check%circle_times], "�����ڲ��쳣");
// 
// 			disp_obj(ho_EmptyObject_Inner, m_ShowLabel[total_check%circle_times]);
// 
// 			set_color(m_ShowLabel[total_check%circle_times], "white");
// 			set_tposition(m_ShowLabel[total_check%circle_times], 144, 20);
// 			write_string(m_ShowLabel[total_check%circle_times], "����������");
// 
// 			disp_obj(ho_EmptyObject_Leaking, m_ShowLabel[total_check%circle_times]);
// 			tuple_sort(hv_Error_Index, &hv_Error_Index);
			// stop(); only in hdevelop

			set_colored(m_ShowLabel[total_check%circle_times], 12);
		}
		else
		{
			set_color(m_ShowLabel[total_check%circle_times], "red");
			set_tposition(m_ShowLabel[total_check%circle_times], 100, 20);
			write_string(m_ShowLabel[total_check%circle_times], HTuple("ͼ�����") + HTuple(total_check));
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
