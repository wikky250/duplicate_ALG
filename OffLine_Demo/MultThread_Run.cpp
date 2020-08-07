#include "MultThread_Run.h"
#include "OffLine_Demo.h"
extern HANDLE g_bShutDown;
extern QVector<CAMERASTRUCT*> g_vectorCamera;
extern QVector<CCycleBuffer*> g_ImgBuffer;
extern QVector<HANDLE> WriteSign;
extern int g_iCameraTotal;
extern std::shared_ptr<spd::logger> daily_logger;
extern QVector<CBaseCheckAlg*> g_CheckClass;
extern QVector<CONTROLPARAM> g_controlparam;
extern HANDLE g_hCom;
extern QString AppPath;
extern int g_PhotoTimes;
extern int MAX_CAPSULECOUNT;
CBaseCheckAlg* EnsureAlg(QString str)
{
	for (QVector<CBaseCheckAlg*>::iterator it = g_CheckClass.begin(); it != g_CheckClass.end(); it++)
	{
		if ((*it)->GetCameraName() == str)
		{
			return *it;
		}
	}
	return NULL;
}
//设置外触发Line1
void SetTriggerLine(int i, int line)
{
#ifdef BASLER
	if (!g_vectorCamera[i]->cb_Camera.IsOpen())
	{
		g_vectorCamera[i]->cb_Camera.Open();
	}
	g_vectorCamera[i]->cb_Camera.TriggerSelector.SetValue(TriggerSelector_FrameStart);
	g_vectorCamera[i]->cb_Camera.TriggerMode.SetValue(TriggerMode_On);
	TriggerModeEnums e = g_vectorCamera[i]->cb_Camera.TriggerMode.GetValue();
	if (line == 0)
	{
		g_vectorCamera[i]->cb_Camera.TriggerSource.SetValue(TriggerSource_Software);
	}
	if (line == 1)
	{
		g_vectorCamera[i]->cb_Camera.TriggerSource.SetValue(TriggerSource_Line1);
	}
	if (line == 2)
	{
		g_vectorCamera[i]->cb_Camera.TriggerSource.SetValue(TriggerSource_Line2);
	}
#endif
#ifdef DAHENG
	CGXFeatureControlPointer m_objFeatureControlPtr = g_vectorCamera[i]->m_objDevicePtr->GetRemoteFeatureControl();
	//将当前功能设置到设备中
	m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
	//将当前功能设置到设备中
	m_objFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");
#endif
}
//设置连续采集
void SetTriggerSoft(int i)
{
#ifdef BASLER
	if (!g_vectorCamera[i]->cb_Camera.IsOpen())
	{
		g_vectorCamera[i]->cb_Camera.Open();
	}
	g_vectorCamera[i]->cb_Camera.TriggerSelector.SetValue(TriggerSelector_FrameStart);
	g_vectorCamera[i]->cb_Camera.TriggerMode.SetValue(TriggerMode_Off);
	g_vectorCamera[i]->cb_Camera.ExposureAuto.SetValue(ExposureAuto_Off);
	g_vectorCamera[i]->cb_Camera.AcquisitionFrameRateEnable.SetValue(true);
	g_vectorCamera[i]->cb_Camera.AcquisitionFrameRate.SetValue(g_vectorCamera[i]->i_LineRateHZ);
	g_vectorCamera[i]->cb_Camera.ExposureTime.SetValue(g_vectorCamera[i]->i_ExpouseTime);
	if (g_vectorCamera[i]->b_InvertX)
	{
		g_vectorCamera[i]->cb_Camera.ReverseX.SetValue(true);
	}
	else
	{
		g_vectorCamera[i]->cb_Camera.ReverseX.SetValue(false);
	}
#endif
#ifdef DAHENG
	CGXFeatureControlPointer m_objFeatureControlPtr = g_vectorCamera[i]->m_objDevicePtr->GetRemoteFeatureControl();
	//将当前功能设置到设备中
	m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionFrameRateMode")->SetValue("On");\
		m_objFeatureControlPtr->GetFloatFeature("AcquisitionFrameRate")->SetValue(g_vectorCamera[i]->i_LineRateHZ);
#endif
}

//////////////////////////////////////////////////////////////////////////
//test time

double dfTim = 0.0;
LARGE_INTEGER litmpst, litmpend, freq;
#pragma region MultGetThread
#ifdef LocalPath
MultGetThread_Run::MultGetThread_Run(QObject *parent)
	: QObject(parent)
{
	QueryPerformanceFrequency(&freq);
	totalcount = 0;
	sizetotal = 0;
	m_LabelShow = nullptr;
	m_bAlRegister = false;
}

MultGetThread_Run::~MultGetThread_Run()
{

}
int MultGetThread_Run::ThreadGetImage(int indexcam = 1, bool = false)//取图函数
{
	if (indexcam != -1)
	{
		if (m_iSelfIndex != indexcam)
			return -1;
	}
	//添加
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//如果是文件夹，则遍历file_path里的.bmp 、.jpg、.tif文件，然后imread
	//如果是目录，则直接imread
	//g_vectorCamera[0]->file_path
	QFileInfo  di(QString::fromLocal8Bit(g_vectorCamera[0]->file_path));
	if (di.isFile())
	{
		m_MatGetOnece = imread(g_vectorCamera[0]->file_path);
		emit GETONEIMAGEMAT(m_MatGetOnece);
	}
	return -1;
}


void MultGetThread_Run::SetMultIndex(int ind)
{
	m_iSelfIndex = ind;
}

void MultGetThread_Run::SetDirectShowDlg(void *dlg)
{
	m_LabelShow = dlg;
}
#endif
#pragma endregion

#pragma region MultDecodeThread

MultDecodeThread_Run::MultDecodeThread_Run(QObject *parent)
	: QObject(parent)
{
	_CheckClass = nullptr;
}

MultDecodeThread_Run::~MultDecodeThread_Run()
{
}

int MultDecodeThread_Run::ThreadDecodeImage(int indexcam = -1, bool b = false)
{
	if (!b)
	{
		_CheckClass->StartCheck(g_vectorCamera[m_iSelfIndex]->c_CameraSign, daily_logger,0,0);
	}
	return -1;
}

int MultDecodeThread_Run::ThreadDecodeImageMat(Mat img)
{
	QString str;
	int results = _CheckClass->Check(img, nullptr, str);
	emit RESULTERRORCOUNT(results);
	emit OUTRESULTSUMMARY(str, index_pos, timecheck);
	timecheck++;
	return 0;
}

void MultDecodeThread_Run::SetMultIndex(int ind)
{
	m_iSelfIndex = ind;
	_CheckClass = g_CheckClass[0];
	daily_logger->info("Thread No.{} EnsureAlg Finish", m_iSelfIndex);

	int pi = 0;
	if (nullptr == _CheckClass)
	{
		return;
	}
	m_iSaveOKorNG = 1;
	i_SaveLoop = -1;
	bool b_doing = false;
	i_captotal = 0;
	timecheck = 0;
	index_pos = 0;
	QDateTime current_time = QDateTime::currentDateTime();

	ngdir_str = AppPath + "/SaveImage/LocalFile/" + QString::number(current_time.date().year()) + "_" + QString::number(current_time.date().month()) + "_" + QString::number(current_time.date().day()) + "_" + QString::number(current_time.time().hour()) + "_" + QString::number(current_time.time().minute()) + "_" + QString::number(current_time.time().second()) + "/NG/";
	okdir_str = AppPath + "/SaveImage/LocalFile/" + QString::number(current_time.date().year()) + "_" + QString::number(current_time.date().month()) + "_" + QString::number(current_time.date().day()) + "_" + QString::number(current_time.time().hour()) + "_" + QString::number(current_time.time().minute()) + "_" + QString::number(current_time.time().second()) + "/OK/";
	QDir dir(ngdir_str);
	if (!dir.exists())
	{
		bool res = dir.mkpath(ngdir_str);
	}
	QStringList filter;
	filter << "*.tif";
	dir.setNameFilters(filter);
	QDir dirs(okdir_str);
	if (!dirs.exists())
	{
		bool res = dirs.mkpath(okdir_str);
	}
	dirs.setNameFilters(filter);
	return;
}


#pragma endregion

#pragma region MultInit_Run
MultInit_Run::MultInit_Run(QObject * parent)
{
	_dammy = new Dammy(parent);//初始化所有的类，下面放入一个线程中
	dammy_th = new QThread();
	QObject::connect(this, SIGNAL(StartsSingle()), _dammy, SLOT(DammyInit()));//????????read222
	_dammy->moveToThread(dammy_th);
	dammy_th->start();
}

MultInit_Run::~MultInit_Run()
{
	if (dammy_th != nullptr)
	{
		dammy_th->quit();
		dammy_th->wait();
		delete dammy_th;//上面三句让所含的多线程对象进程结束，并删除
	}
}

int MultInit_Run::CloseTh()
{
	return 0;//关闭 0
}

int MultInit_Run::ThreadInit()
{
	emit StartsSingle();
	return -1;//发射-1
}

Dammy::Dammy(QObject * parent)
{
	_parent = parent;
}

Dammy::~Dammy()
{
}

bool sortcamera(CAMERASTRUCT *a, CAMERASTRUCT *b)//排序方法
{
	QString x = a->c_CameraSign;
	QString y = b->c_CameraSign;
	return x.mid(3).toInt() < y.mid(3).toInt();
}
void Dammy::DammyInit()//Dammy类唯一槽函数，执行所有初始化操作
{
	QDateTime current_time = QDateTime::currentDateTime();
	QString StrCurrentTime = current_time.toString("hh:mm:ss:zzz");
	InitFunction*Camera_Func = new InitFunction(this);//初始化相机
	Camera_Func->StartModel = true;
	Camera_Func->GetAllCamera();//包含ReadConfig(); read333
	delete Camera_Func;
	Camera_Func = nullptr;
	g_iCameraTotal = g_vectorCamera.size();
	current_time = QDateTime::currentDateTime();
	StrCurrentTime = current_time.toString("hh:mm:ss:zzz");
	QThread::msleep(200);
	((OffLine_Demo*)_parent)->LoadAlgorithmDLL();//加载算法
	daily_logger->info("LoadAlgorithmDLL Finished");
	current_time = QDateTime::currentDateTime();
	StrCurrentTime = current_time.toString("hh:mm:ss:zzz");
	QThread::msleep(200);
	((OffLine_Demo*)_parent)->InitCheckClass();
	daily_logger->info("InitCheckClass Finished");
	g_iCameraTotal = g_CheckClass.size();
	current_time = QDateTime::currentDateTime();
	StrCurrentTime = current_time.toString("hh:mm:ss:zzz");
	QThread::msleep(200);
	((OffLine_Demo*)_parent)->InitPicList();//初始化图像列表
	daily_logger->info("InitPicList Finished");
	current_time = QDateTime::currentDateTime();
	StrCurrentTime = current_time.toString("hh:mm:ss:zzz");
	QThread::msleep(200);
	((OffLine_Demo*)_parent)->FinishInitOther();//其他初始化
	delete this;//删掉以后，所在线程随之销毁，就不在了,也就是说专门开了一个线程用来初始化，初始化完成后该线程伴随对象而销毁
}
#pragma endregion

MultSaveThread_Run::MultSaveThread_Run(QObject * parent, int _camcount)
{
	m_icamcount = _camcount;
	m_qvEachResule = new QVector<int>[m_icamcount];
}

MultSaveThread_Run::~MultSaveThread_Run()
{
}


void MultSaveThread_Run::ThreadSave(Mat img, QString str)
{
	imwrite(std::string(str.toLocal8Bit()), img);
}

MultSummaryThread_Run::MultSummaryThread_Run(QObject * parent, int _camcount)
{
	m_icamcount = _camcount;
	m_iResultAllList = 0;
	for (int i = 0; i < g_vectorCamera.size(); i++)
	{
		m_iResultAllList = g_vectorCamera[i]->i_RealLocatPos > m_iResultAllList ? g_vectorCamera[i]->i_RealLocatPos : m_iResultAllList;
	}
	m_iResultAllList++;
	m_qslResultEachCamera = new QStringList[m_iResultAllList];
	b_eachalreadyfinish = new bool[m_iResultAllList];
	for (int i = 0; i < m_icamcount; i++)
	{
		b_eachalreadyfinish[i] = TRUE;
	}
	for (int i = 0; i < m_iResultAllList; i++)
	{
		for (int z = 0;z < g_vectorCamera.size();z++)
		{
			if (i == g_vectorCamera[z]->i_RealLocatPos)
			{
				b_eachalreadyfinish[i] = FALSE;
				break;
			}
		}
	}
}

MultSummaryThread_Run::~MultSummaryThread_Run()
{
}

void MultSummaryThread_Run::ThreadSummary(QString str, int pos, int timeincircle)
{
	// 	if (pos >= m_icamcount|| str =="")
	// 	{
	// 		return;
	// 	}
		//第一个为赋值
	if (g_PhotoTimes == 1)
	{
		m_qslResultEachCamera[pos] = str.split(",");
		b_eachalreadyfinish[pos] = TRUE;
		unsigned int result[4];
		result[0] = 0; result[1] = 0;
		result[2] = 0; result[3] = 0;

		for (int i = 0; i < m_iResultAllList; i++)
		{
			int z = 0;
			for (; z < m_qslResultEachCamera[i].size(); z++)
			{
				if (m_qslResultEachCamera[i][z] != "Good")
				{
					if (i < 8)
						result[0] |= 1 << i;
					else if (i < 16)
						result[1] |= 1 << (i - 8);
					else if (i < 24)
						result[2] |= 1 << (i - 16);
					else if (i < 32)
						result[3] |= 1 << (i - 24);
				}
				m_qslResultTOTALCamera.push_back(m_qslResultEachCamera[i][z]);
			}
			if (z == 0)
			{
				for (; z < MAX_CAPSULECOUNT; z++)
				{
					int xy = i * 6 + z;
					if (xy < 8)
						result[0] |= 1 << xy;
					else if (xy < 16)
						result[1] |= 1 << (xy - 8);
					else if (xy < 24)
						result[2] |= 1 << (xy - 16);
					else if (xy < 32)
						result[3] |= 1 << (xy - 24);
					m_qslResultTOTALCamera.push_back("NULL");
				}
			}
			m_qslResultEachCamera[i].clear();
		}
		emit SUMMARYRESULTINCIRCLE(m_qslResultTOTALCamera);
#ifdef PLCCONNECT
		g_SocketPLC->SetResult(0, result);
#endif
		m_qslResultTOTALCamera.clear();
		b_eachalreadyfinish[pos] = false;
	}
	else
	{
		if (0 == timeincircle % g_PhotoTimes)
		{
			m_qslResultEachCamera[pos] = str.split(",");
		}
		else
		{
			//若算法检测为error，则覆盖m_qslResultEachCamera对应值
			QStringList tem = str.split(",");
			for (int i = 0; i < tem.size(); i++)
			{
				if (tem[i] != "Good"&&i < m_qslResultEachCamera[pos].size())
				{
					m_qslResultEachCamera[pos][i] = tem[i];
				}
			}
			if (0 == (timeincircle + 1) % g_PhotoTimes)
			{
				b_eachalreadyfinish[pos] = TRUE;
			}
		}
		if (0 == (timeincircle + 1) % g_PhotoTimes)
		{
			bool _fini = TRUE;
			for (int i = 0; i < m_icamcount; i++)
			{
				if (b_eachalreadyfinish[i] == FALSE)
				{
					_fini = FALSE;
				}
			}
			if (!_fini)
			{
				return;
			}
			unsigned int result[4];
			result[0] = 0; result[1] = 0;
			result[2] = 0; result[3] = 0;

			for (int i = 0; i < m_iResultAllList; i++)
			{
				int z = 0;
				for (; z < m_qslResultEachCamera[i].size(); z++)
				{
					if (m_qslResultEachCamera[i][z] != "Good")
					{
						if (i < 8)
							result[0] |= 1 << i;
						else if (i < 16)
							result[1] |= 1 << (i - 8);
						else if (i < 24)
							result[2] |= 1 << (i - 16);
						else if (i < 32)
							result[3] |= 1 << (i - 24);
					}
					m_qslResultTOTALCamera.push_back(m_qslResultEachCamera[i][z]);
				}
				if (z == 0)
				{
					for (; z < MAX_CAPSULECOUNT; z++)
					{
						int xy = i * 6 + z;
						if (xy < 8)
							result[0] |= 1 << xy;
						else if (xy < 16)
							result[1] |= 1 << (xy - 8);
						else if (xy < 24)
							result[2] |= 1 << (xy - 16);
						else if (xy < 32)
							result[3] |= 1 << (xy - 24);
						m_qslResultTOTALCamera.push_back("NULL");
					}
				}
				m_qslResultEachCamera[i].clear();
			}
			emit SUMMARYRESULTINCIRCLE(m_qslResultTOTALCamera);
#ifdef PLCCONNECT
			g_SocketPLC->SetResult(0, result);
#endif
			m_qslResultTOTALCamera.clear();
			b_eachalreadyfinish[pos] = false;
		}
	}
	QueryPerformanceCounter(&litmpend);
	dfTim += (double)(litmpend.QuadPart - litmpst.QuadPart) / freq.QuadPart * 1000;
	avg = dfTim / timeincircle;
}
