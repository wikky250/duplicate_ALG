#include "MultThread_Run.h"
#include "OffLine_Demo.h"
#include <QLabel>
extern QVector<HANDLE> WriteSign;
extern int g_iCameraTotal;
extern HANDLE g_hCom;
extern QString AppPath;
extern QDateTime g_current_time;
bool bDeleting = false;

QVector<QtBaseCameraClass*> g_MultCamera;
QVector<QThread*> m_MultGetThread;

//map which to load from disk temporary
QMap<QString, QLibrary*> dllQMap;

//plc connect module
QtPLCControl* lib_PLC = nullptr;
QThread* lib_PLCThread = nullptr;
QVector<MultDecodeThread_Run*> m_MultDecodeThread;
QVector<QThread*> QTH_MultDecodeThread;


QVector<CatchImageFromCam*>m_MultGetImageFromCam;
QVector<QThread*>m_MultGetImageFromCamThread;


//Thread to save
MultSaveThread_Run* m_MultSaveThread = nullptr;
QThread* QTH_MultSaveThread = nullptr;
//THread to Summary
MultSummaryThread_Run* m_MultSummaryThread = nullptr;
QThread* QTH_MultSummaryThread = nullptr;
//THread to Show
QMutex g_MutexShowRefresh;
//cv::Mat g_mToShow[MAX_CAMERACOUNT];
QVector < MultShowThread_run*> m_MultShowThread;
QVector < QThread*> QTH_MultShowThread;


void CallbackGetImg(void* puser)
{
	((CatchImageFromCam*)puser)->GetImageFromCam();
	return;
};

#pragma region MultDecodeThread
QVector<IMAGE_INGREDIENT*>g_GetedImage[MAX_CAMERACOUNT];

QVector<IMAGE_SIMPLEINGREDIENT*>g_ShowBUfferImage[MAX_CAMERACOUNT];



int g_iGetFromCam[MAX_CAMERACOUNT];					//reset by NewUI_Demo::CleanAlg()

int g_iDecodeFineshed[MAX_CAMERACOUNT];				//reset by NewUI_Demo::CleanAlg()

MultDecodeThread_Run::MultDecodeThread_Run(QObject* parent, void* user)
	: QObject(parent)
{
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<cv::Mat>("cv::Mat&");//注册opencv函数，在槽函数中避免出错
	QueryPerformanceFrequency(&freq);
	_CheckClass = nullptr;
}
MultDecodeThread_Run::~MultDecodeThread_Run()
{
	if (_CheckClass != nullptr)
	{
		if (destructor != nullptr)
		{
			destructor(_CheckClass);
			destructor = nullptr;
		}
		_CheckClass = nullptr;
	}
}
int MultDecodeThread_Run::ResetDecodeImage(int indexcam, bool b)
{
	m_bStop = false;
	i_captotal = 0;
	return -1;
}
bool MultDecodeThread_Run::unloadDll()
{
	if (_CheckClass != nullptr)
	{
		if (destructor != nullptr)
		{
			destructor(_CheckClass);
			destructor = nullptr;
		}
		_CheckClass = nullptr;
	}
	return false;
}

int MultDecodeThread_Run::ThreadDecodeImageMat(int camIndex, int _index)
{
	int _icurrentRead = _index;
	if (camIndex != m_iCamIndex || _icurrentRead % ALGNUM != m_iAlgIndex)
	{
		return -1;
	}
	int delaytostop = 0;
	{
		{
			tempingredient = *g_GetedImage[m_iCamIndex].at(_icurrentRead % CAMREACATCHBUFFER);
			if (tempingredient._bGetIMG && !tempingredient._bDecodeResult)
			{
				if (i_captotal == 0)
				{
					w = tempingredient.w;
					h = tempingredient.h;
					c = tempingredient.c;
					_CheckClass->StartCheck(m_sCameraSign, nullptr, w, h, c);
					m = cv::Mat(h, w, c == 1 ? CV_8UC1 : CV_8UC3);
				}
				memcpy((char*)m.data, tempingredient._cImageData, w * h * c);
				ResultStruct strResult;
				if (nullptr != _CheckClass)
				{
					_CheckClass->Check(m, nullptr, strResult);
				}
				QueryPerformanceCounter(&nEndTime);
				tempingredient._msspend = (double)(nEndTime.QuadPart - tempingredient.nBeginTime.QuadPart) / (double)nFreq.QuadPart * 1000;
				emit OUTRESULTSUMMARY(strResult.error_describe, m_iCamIndex, _icurrentRead);
				emit ERRORMESSAGE(TYPE_SYSTEM, QString("Thread %1 ThreadDecodeImageMat ,Image Index %2, Alg Time %3")\
					.arg(QString::number(m_iAlgIndex))\
					.arg(QString::number(tempingredient._iIndexofImg))\
					.arg(QString::number(tempingredient._msspend, 'f', 2)), e_TRACE);
				memcpy(tempingredient._cImageShowData, (char*)m.data, w * h * c);
				tempingredient._bDecodeResult = true;
				tempingredient._result = strResult;
				*g_GetedImage[m_iCamIndex][_icurrentRead % CAMREACATCHBUFFER] = tempingredient;
				emit SAVESIGNAL(m_iCamIndex, _icurrentRead);
				emit SignaltoShowThread(m_iCamIndex, _icurrentRead);
				++i_captotal;
				g_iDecodeFineshed[m_iCamIndex]++;
			}
		}
	}
	return 0;
}

int MultDecodeThread_Run::FinishedCheck()
{
	m_bStop = true;
	return 0;
}

void MultDecodeThread_Run::SetTimeCheck()
{
}
void MultDecodeThread_Run::SetMultIndex(int camindex, int algindex)
{
	m_iCamIndex = camindex;
	m_iAlgIndex = algindex;
	i_captotal = 0;
	QueryPerformanceFrequency(&nFreq);
	return;
}
void MultDecodeThread_Run::SetCameraInfo(CAMERAINFO_F& inf)
{
	m_sCameraSign = inf.c_CameraSign;
	w = inf.i_FrameWidth;
	h = inf.i_FrameHeight;
}
#pragma endregion
#pragma region MultInit_Run
MultInit_Run::MultInit_Run(QWidget* parent, void* user)
	:QWidget(parent)
{
	_parent = user;
}
MultInit_Run::~MultInit_Run()
{
}
QString GetFileProductName(QString fullName)
{
	DWORD dwLen = 0;
	char* lpData = NULL;

	BOOL bSuccess = FALSE;
	QString fileInfomation;
	dwLen = GetFileVersionInfoSize(fullName.toStdWString().c_str(), 0);
	if (0 == dwLen)
	{
		return "";
	}
	lpData = new char[dwLen + 1];

	bSuccess = GetFileVersionInfo(fullName.toStdWString().c_str(), 0, dwLen, lpData);
	if (!bSuccess)
	{
		delete[] lpData;
		return "";
	}

	LPVOID lpBuffer = NULL;
	UINT uLen = 0;
	bSuccess = VerQueryValue(lpData,
		(TEXT("\\VarFileInfo\\Translation")),
		&lpBuffer,
		&uLen);
	QString strTranslation, str1, str2;
	unsigned short int* p = (unsigned short int*)lpBuffer;
	str1.setNum(*p, 16);
	str1 = "000" + str1;
	strTranslation += str1.mid(str1.size() - 4, 4);
	str2.setNum(*(++p), 16);
	str2 = "000" + str2;
	strTranslation += str2.mid(str2.size() - 4, 4);
	QString code = "\\StringFileInfo\\" + strTranslation + "\\ProductName";//FileVersion
	bSuccess = VerQueryValue(lpData,
		(code.toStdWString().c_str()),
		&lpBuffer,
		&uLen);
	if (!bSuccess)
	{
		delete[] lpData;
		return "";
	}
	fileInfomation += QString::fromUtf16((const unsigned short int*)lpBuffer);
	delete[] lpData;//此处不需要释放
	return fileInfomation;
}
bool MultInit_Run::ThreadInitAlgo()
{
	QDir finder(AppPath);
	if (!finder.exists())
	{
		return -1;
	}
	finder.setFilter(QDir::Files | QDir::NoSymLinks);
	QFileInfoList list = finder.entryInfoList();
	int file_count = list.count();
	if (file_count <= 0)
	{
		return -1;
	}
	QString Liststring_list;
	for (int i = 0; i < file_count; i++)
	{
		QFileInfo file_info = list.at(i);
		QString suffix = file_info.suffix();//文件后缀
		if (QString::compare(suffix, QString("dll"), Qt::CaseInsensitive) == 0)//CaseInsensitive不区分大小写比较 QString::compare两个参数相等时为0
		{
			QString absolute_file_path = file_info.baseName();//绝对路径，包含文件但不包含后缀 basename作用是得到特定的路径中的最后一个'/',后面的内容但不包含后缀
			QString sss = absolute_file_path.right(10);//右侧10个字符
			if (absolute_file_path.right(10) == "CheckClass")
			{
				QString aDebug = file_info.filePath();//完整路径
				QLibrary* dynamicDLL = new QLibrary(file_info.filePath());
				if (dynamicDLL->load())//加载库文件
				{
					QString Desti = GetFileProductName(aDebug);
					dllQMap.insert(Desti, dynamicDLL);//放入dllQMap
				}
				else
				{
					emit ERRORMESSAGE(TYPE_SYSTEM, dynamicDLL->errorString(), e_WARN);
				}
			}
		}
	}
	return true;
}
extern void PLC_PARAMSTATIC(void* context);
bool MultInit_Run::InitPLC()
{
	lib_PLC = new QtPLCControl();//dll
	lib_PLC->setCallBackGetParam(_parent, PLC_PARAMSTATIC);
	lib_PLCThread = new QThread();
	lib_PLC->moveToThread(lib_PLCThread);
	lib_PLCThread->start();
	bool b = connect(lib_PLC, SIGNAL(signal_SUCCESSFULCONNECTED()), (NewUI_Demo*)_parent, SLOT(ForPLCSUCCESS()));
	b = connect(this, SIGNAL(STARTCONNECTPLC()), lib_PLC, SLOT(ConnectPlc()));
	b = connect(lib_PLC, SIGNAL(SOCKETERROR()), (NewUI_Demo*)_parent, SLOT(PlcSocketError()));
	b = connect(lib_PLC, SIGNAL(ERRORMESSAGE(int, QString, int)), m_MessageInit, SLOT(SLOT_MESSAGELOG(int, QString, int)));
	emit STARTCONNECTPLC();
	return false;
}
bool MultInit_Run::InitSummary()
{
	if (nullptr == m_MultSummaryThread)
	{
		m_MultSummaryThread = new MultSummaryThread_Run(nullptr);
		bool b = QObject::connect(m_MultSummaryThread, SIGNAL(ERRORMESSAGE(int, QString, int)), m_MessageInit, SLOT(SLOT_MESSAGELOG(int, QString, int)));
		b = QObject::connect(m_MultSummaryThread, SIGNAL(SUMMARYRESULTINCIRCLE(QStringList)), (NewUI_Demo*)_parent, SLOT(SlotShowResult(QStringList)));
		b = QObject::connect(m_MultSummaryThread, SIGNAL(KICKTOPLCEXE(int, int*, int)), lib_PLC, SLOT(KickResult(int, int*, int)));
		b = QObject::connect(m_MultSummaryThread, SIGNAL(PLCMESSAGE(int, bool)), lib_PLC, SLOT(SetOutPut(int, bool)));
		QTH_MultSummaryThread = new QThread();
		m_MultSummaryThread->moveToThread(QTH_MultSummaryThread);
		QTH_MultSummaryThread->start();
	}
	return false;
}
bool MultInit_Run::ThreadInitCamera()
{
	ThreadInitAlgo();
	InitPLC();
	InitSummary();
	char configpath[200];
	strcpy(configpath, AppPath.toStdString().c_str());
	strcat(configpath, (QString("/DefaultModel") + "\\CameraConfig.ini").toStdString().c_str());
	QVector<std::string> v_line;
	std::ifstream cin(configpath);
	std::string filename;
	std::string line;
	int r_n = 0, ipos = 0;
	if (cin)
	{
		while (getline(cin, line))
		{
			if (line.find("[") != std::string::npos && line.find("]") != std::string::npos)//
			{
				line.erase(line.length() - 1, 1);
				line.erase(0, 1);
				if (line != "DefaultCameraType")
				{
					v_line.push_back(line);
				}
			}
		}
	}
	r_n = v_line.size();
	QSettings configIniRead(configpath, QSettings::IniFormat);
	configIniRead.setIniCodec("UTF-8");

	QtBaseCameraClass* tempcam;
	QThread* tempthd;
	CatchImageFromCam* tempcatch;
	QThread* tempcatchthread;

	char* algo_Name = new char[20];
	MultDecodeThread_Run* tempDecode;
	IMAGE_INGREDIENT* tempInger = nullptr;

	MultShowThread_run* tempShow;

	QLibrary* LocalFileDLL = new QLibrary(AppPath + "/QtCameraClass.dll");
	pExportCamera CreateLocalCamera = (pExportCamera)(LocalFileDLL->resolve("CreateExportCameraObj"));//导出类
	QLibrary* BaslerCameraDLL = new QLibrary(AppPath + "/QtBaslerCameraClass.dll");
	pExportCamera CreateBaslerCamera = (pExportCamera)(BaslerCameraDLL->resolve("CreateExportCameraBasler"));//导出类
	QLibrary* HikCameraDLL = new QLibrary(AppPath + "/QtHikCameraClass.dll");
	pExportCamera CreateHikWebCamera = (pExportCamera)(HikCameraDLL->resolve("CreateExportCameraHikvision"));//导出类
	bool b = false;
	int inde_cam = 0;
	for (int i = 0; i < r_n; i++)
	{
		QString s = v_line[i].c_str();
		CAMERATYPE itpye = (CAMERATYPE)configIniRead.value(s + "/TypeOFCamera", -1).toInt();
		if (-1 == itpye)
		{
			continue;
		}
		switch (itpye)
		{
		case LOCALFILE:
		{
			if (LocalFileDLL)
			{
				tempcam = CreateLocalCamera();
			}
			else
				emit ERRORMESSAGE(TYPE_SYSTEM, "Can't Find LocalFile dll", e_CRITICAL);
			break;
		}
		case CAMERABASLER:
		{
			if (BaslerCameraDLL)
			{
				tempcam = CreateBaslerCamera();
			}
			else
				emit ERRORMESSAGE(TYPE_SYSTEM, "Can't Find BaslerCamera dll", e_CRITICAL);
			break;
		}
		case CAMERAHIKVISION:
		{
			if (HikCameraDLL)
			{
				tempcam = CreateHikWebCamera();
			}
			else
				emit ERRORMESSAGE(TYPE_SYSTEM, "Can't Find BaslerCamera dll", e_CRITICAL);
			break;
		}
		default:
			//暂不支持，继续遍历
			emit ERRORMESSAGE(TYPE_SYSTEM, "Can't support other camera", e_WARN);
			continue;
			break;
		}
		tempcatch = new CatchImageFromCam();
		InitCameraStruct camerastruct;
		strcpy(camerastruct.c_CameraName, v_line[i].c_str());
		strcpy(camerastruct.c_IniFile, configpath);
		tempcatch->SetCameraClass(tempcam);
		tempcatch->SetCatchTreadIndex(inde_cam);
		camerastruct.CallBackToGetImage = &CallbackGetImg;
		camerastruct.parentpointer = tempcatch;
		if (tempcam->SingleInitCamera(-1, camerastruct))
		{
			tempShow = new MultShowThread_run();
			tempShow->SetCamIndex(inde_cam);
			tempShow->SetParentPointer(_parent);
			b = QObject::connect((NewUI_Demo*)_parent, SIGNAL(Signal_STOPCHECK()), tempShow, SLOT(StopThread()), Qt::DirectConnection);
			b = QObject::connect(tempShow, SIGNAL(Signal_ShowImage(int, int)), (NewUI_Demo*)_parent, SLOT(SLOTShowImage(int, int)));
			b = QObject::connect(tempShow, SIGNAL(ERRORMESSAGE(int, QString, int)), m_MessageInit, SLOT(SLOT_MESSAGELOG(int, QString, int)));
			QThread* qth = new QThread();
			tempShow->moveToThread(qth);
			qth->start();
			QTH_MultShowThread.push_back(qth);
			m_MultShowThread.push_back(tempShow);

			m_MultSaveThread = new MultSaveThread_Run(nullptr, inde_cam);
			b = QObject::connect(m_MultSaveThread, SIGNAL(ERRORMESSAGE(int, QString, int)), m_MessageInit, SLOT(SLOT_MESSAGELOG(int, QString, int)));
			QTH_MultSaveThread = new QThread();
			m_MultSaveThread->moveToThread(QTH_MultSaveThread);
			QTH_MultSaveThread->start();



			m_MultGetImageFromCam.push_back(tempcatch);
			QThread* qthre = new QThread();
			tempcatch->moveToThread(qthre);
			qthre->start();
			m_MultGetImageFromCamThread.push_back(qth);
			b = QObject::connect(tempcatch, SIGNAL(SIGNAL_MESSAGELOG(ERRORTYPE, QString, ERRORLEVEL)), m_MessageInit, SLOT(SLOT_MESSAGELOG(ERRORTYPE, QString, ERRORLEVEL)));
			g_MultCamera.push_back(tempcam);
			tempthd = new QThread();
			tempcam->moveToThread(tempthd);
			tempthd->start();
			m_MultGetThread.push_back(tempthd);
			b = QObject::connect((NewUI_Demo*)_parent, SIGNAL(Signal_STARTCHECK()), tempcatch, SLOT(StartCatchImage()));
			b = QObject::connect(tempcatch, SIGNAL(ERRORMESSAGE(int, QString, int)), m_MessageInit, SLOT(SLOT_MESSAGELOG(int, QString, int)));
			tempcam->SingleGetAlgName(*algo_Name);
			pExport CExportDLL = (pExport)((QLibrary*)dllQMap.find(algo_Name).value()->resolve("CreateExportObj"));//导出类
			pDeleteExport m_UnLoadDLL = (pDeleteExport)((QLibrary*)dllQMap.find(algo_Name).value()->resolve("DestroyExportObj"));
			if (CExportDLL != NULL)
			{
				for (int j = 0; j < ALGNUM; j++)
				{
					tempDecode = new MultDecodeThread_Run(nullptr, nullptr);
					tempDecode->_CheckClass = CExportDLL(false);//是否是测试模式，false是正常，true是测试模式
					tempDecode->_CheckClass->SetParam(0, (char*)s.toStdString().c_str());
					tempDecode->destructor = m_UnLoadDLL;
					tempDecode->SetMultIndex(inde_cam, j);
					b = QObject::connect(tempcatch, SIGNAL(GetImageData(int, int)), tempDecode, SLOT(ThreadDecodeImageMat(int, int)));
					b = QObject::connect(tempDecode, SIGNAL(SignaltoShowThread(int, int)), tempShow, SLOT(onSlotToShow(int, int)), Qt::DirectConnection);
					b = QObject::connect(tempDecode, SIGNAL(SAVESIGNAL(int, int)), m_MultSaveThread, SLOT(ThreadSave(int, int)));
					b = QObject::connect(tempDecode, SIGNAL(ERRORMESSAGE(int, QString, int)), m_MessageInit, SLOT(SLOT_MESSAGELOG(int, QString, int)));
					b = QObject::connect(tempDecode, SIGNAL(OUTRESULTSUMMARY(QString, int, int)), m_MultSummaryThread, SLOT(ThreadSummary(QString, int, int)));

					m_MultDecodeThread.push_back(tempDecode);
					QThread* te = new QThread;
					tempDecode->moveToThread(te);
					te->start();
					QTH_MultDecodeThread.push_back(te);
				}
				for (int j = 0; j < CAMREACATCHBUFFER; j++)
				{
					tempInger = new IMAGE_INGREDIENT();
					tempInger->resetStuct();
					tempInger->_bExeResult = true;
					g_GetedImage[inde_cam].push_back(tempInger);
					g_ShowBUfferImage[inde_cam].push_back(new IMAGE_SIMPLEINGREDIENT());
				}
			}
			++inde_cam;
		}
		else
		{
			delete tempcatch;
			tempcatch = nullptr;
		}

	}

	g_iCameraTotal = g_MultCamera.size();
	if (0 == g_iCameraTotal)
	{
		emit ERRORMESSAGE(TYPE_SYSTEM, "init camera Finished, Didn't find any camera", e_ERROR);
	}
	else
	{
		emit ERRORMESSAGE(TYPE_SYSTEM, "init camera successed,Camera count:" + QString::number(g_iCameraTotal), e_DEBUG);
	}

	delete algo_Name;
	algo_Name = nullptr;

	b = QObject::connect(this, SIGNAL(Signal_SetShowDlg()), (NewUI_Demo*)_parent, SLOT(FinishInitOther()));

	emit Signal_SetShowDlg();

	m_MultSummaryThread->SetCamCount(g_iCameraTotal);
	delete LocalFileDLL;
	LocalFileDLL = nullptr;
	delete BaslerCameraDLL;
	BaslerCameraDLL = nullptr;
	delete HikCameraDLL;
	HikCameraDLL = nullptr;
	return true;
}

#pragma endregion
MultSaveThread_Run::MultSaveThread_Run(QObject* parent, int _camcount)
{
	m_icamcount = _camcount;

	//ti = new QTimer();
	//connect(ti, SIGNAL(timeout()), this, SLOT(RealSaveThread()));
	//ti->start(50);
}
MultSaveThread_Run::~MultSaveThread_Run()
{
}
void MultSaveThread_Run::RealSaveThread()
{
}
void MultSaveThread_Run::ThreadSave(int m_iCamIndex, int _icurrentRead)
{
	if (_iSaveLoop == 0)
	{
		return;
	}
	tempingredient = *g_GetedImage[m_iCamIndex][_icurrentRead % CAMREACATCHBUFFER];
	ResultStruct re = tempingredient._result;
	if (_iSaveOKorNG == 0 ||
		_iSaveOKorNG == -1 && !re._bResultNGOK ||
		_iSaveOKorNG == 1 && re._bResultNGOK)
	{
		QString strpa = AppPath.toLocal8Bit() + "/SaveImage/" + datatime + "/" + QString::number(m_iCamIndex) + "/" + (re._bResultNGOK == true ? "OK/" : "NG/");
		strcpy(tempingredient._cSavePath, strpa.toLocal8Bit());
		QDir dir(tempingredient._cSavePath);
		if (!dir.exists())
		{
			bool res = dir.mkpath(tempingredient._cSavePath);
		}
		strcpy(temppath, tempingredient._cSavePath);
		if (_iSaveLoop != 0)
		{
			int  n = _iSaveLoop != -1 ? tempingredient._iIndexofImg % _iSaveLoop : tempingredient._iIndexofImg;
			itoa(n, te, 10);
			strcat(temppath, te);
			strcat(temppath, ".bmp");
			savebitmap(tempingredient._cImageData, tempingredient.w, tempingredient.h, temppath);
			emit ERRORMESSAGE(TYPE_SYSTEM, strpa, e_TRACE);
		}
	}
}
MultShowThread_run::MultShowThread_run()
{
}
bool MultShowThread_run::SetCamIndex(int _index)
{
	m_iCamIndex = _index;
	return true;
};
bool MultShowThread_run::SetParentPointer(void* p)
{
	if (p)
		_parent = p;
	return true;
};
void MultShowThread_run::StopThread()
{
	m_bStop = true;
}
void MultShowThread_run::onSlotToShow(int camIndex, int imgIndex)
{
	int i_captotal = imgIndex;
	//if (i_captotal == 0)
	//{
	//	int w_show, h_show;
	//	if (((NewUI_Demo*)_parent)->GetImageShowRect(m_iCamIndex, m_iCount, w_show, h_show))
	//}
	int delaytostop = 0;
	{
		tempingredient = *g_GetedImage[m_iCamIndex].at(i_captotal % CAMREACATCHBUFFER);
		g_GetedImage[m_iCamIndex].at(i_captotal % CAMREACATCHBUFFER)->resetStuct();

#ifndef _DEBUG
		//if (i_captotal % 5 == 0)
#endif
		{
			//SYS_logger->trace("Thread onSlotToShow UNLOCK,NumCount:{}", i_captotal);
			if (0 == i_captotal)
			{
				w = tempingredient.w;
				h = tempingredient.h;
				c = tempingredient.c;
				m_temp = cv::Mat(h, w, c == 1 ? CV_8UC1 : CV_8UC3);
				//g_mToShow[m_iCamIndex] = cv::Mat(h, c, CV_8UC3);
			}
			memcpy((char*)(m_temp.data), tempingredient._cImageShowData, w * c * h);
			//memcpy(tempingredient._cImageShowData, (char*)(m_tempshow.data), w * c * h);
			g_MutexShowRefresh.lock();
			*g_ShowBUfferImage[m_iCamIndex][i_captotal % CAMREACATCHBUFFER] = tempingredient;
			g_MutexShowRefresh.unlock();
			emit Signal_ShowImage(m_iCamIndex, tempingredient._iIndexofImg);
			emit ERRORMESSAGE(TYPE_SYSTEM,
				QString("onSlotToShow ,Image Index %1, Alg Time %2")\
				.arg(QString::number(tempingredient._iIndexofImg))\
				.arg(QString::number(tempingredient._msspend)), e_TRACE);
		}
	}

	return;
}

MultDeleteTifThread_Run::MultDeleteTifThread_Run(QObject* parent)
{
}
MultDeleteTifThread_Run::~MultDeleteTifThread_Run()
{
}
void MultDeleteTifThread_Run::evethingSearchResults(QString str)
{
}
MultSummaryThread_Run::MultSummaryThread_Run(QObject* parent, int _camcount, int lastpos)
{
	m_icamcount = _camcount;
	m_qslResultEachCamera = new QStringList[_camcount];
	b_eachalreadyfinish = new bool[_camcount];

}
MultSummaryThread_Run::~MultSummaryThread_Run()
{
	if (nullptr != m_qslResultEachCamera)
	{
		delete[] m_qslResultEachCamera;
		m_qslResultEachCamera = nullptr;
	}
	if (b_eachalreadyfinish != nullptr)
	{
		delete b_eachalreadyfinish;
		b_eachalreadyfinish = nullptr;
	}
	if (nullptr != result)
	{
		delete result;
		result = nullptr;
	}
}
void MultSummaryThread_Run::ThreadSummary(QString str, int pos, int timeincircle)
{
	if (1 == m_icamcount)
	{

		//only one camera
		int re = 1;
		if ("Good" != str)
		{
			m_iContinue++;
			if (m_bCAlarm && m_iContinue > m_iCAlarm)
			{
				emit PLCMESSAGE(7, true);
			}
			if (m_bCStop && m_iContinue > m_iCStop)
			{
				emit PLCMESSAGE(8, true);
			}
			emit KICKTOPLCEXE(timeincircle, &re, 1);
			emit ERRORMESSAGE(TYPE_SYSTEM, QString::number(timeincircle) + "-Sent result to PLC :" + "NG", 0);

		}
		else
		{
			m_iContinue = 0;
			re = 0;
			emit KICKTOPLCEXE(timeincircle, &re, 1);
			emit ERRORMESSAGE(TYPE_SYSTEM, QString::number(timeincircle) + "-Sent result to PLC :" + "OK", 0);
		}
		emit SUMMARYRESULTINCIRCLE(QStringList(str));
		return;
	}
	QMap<int, QStringList>::Iterator it = resultMap.find(timeincircle);
	if (resultMap.end() != it)
	{
		//didn't test greater then one camera
		resultMap.find(timeincircle).value().push_back(str);
		if (it.value().size() == m_icamcount)
		{
			for (int i = 0; i < m_icamcount; i++)
			{
				if (it.value()[i] != "Good")
				{
					emit SUMMARYRESULTINCIRCLE(QStringList(it.value()[i]));

					return;
				}
			}
			emit SUMMARYRESULTINCIRCLE(QStringList("Good"));
		}
	}
	else
	{
		if (m_icamcount != 1)
		{
			resultMap.insert(timeincircle, QStringList(str));
		}
		else
		{
			//only one camera
			int re = 1;
			if ("Good" != str)
			{
				m_iContinue++;
				if (m_bCAlarm && m_iContinue > m_iCAlarm)
				{
					emit PLCMESSAGE(7, true);
				}
				if (m_bCStop && m_iContinue > m_iCStop)
				{
					emit PLCMESSAGE(9, true);
				}
				emit KICKTOPLCEXE(timeincircle, &re, 1);
			}
			else
			{
				m_iContinue = 0;
				re = 0;
				emit KICKTOPLCEXE(timeincircle, &re, 1);
			}
			emit SUMMARYRESULTINCIRCLE(QStringList(str));
		}
	}
}

void MultSummaryThread_Run::getPlcLib(void* lib)
{
	//plcLib = lib;
}

void MultSummaryThread_Run::getM_iCount(int index)
{
	m_iCount = index;
	m_iNgNo = 0;//开始时归零
}