#pragma once
#include "AllRelayHead.h"
#include "../QT_FrontCheckClass/QT_FrontCheckClass.h"
#define  QT_FrontCheckClass_LIB
#include <QObject>
#include <QWaitCondition>
#include <QSemaphore>
typedef CBaseCheckAlg* (__stdcall* pExport)(bool b);
typedef void(__stdcall* pDeleteExport)(CBaseCheckAlg*);
extern QMutex mutexDecode;


#define LOCALCAMERA
#include "../QBaseCameraClass.h"
typedef QtBaseCameraClass* (__stdcall* pExportCamera)();
extern QVector<QtBaseCameraClass*> g_MultCamera;
extern QVector<QThread*> m_MultGetThread;

//map which to load from disk temporary
extern QMap<QString, QLibrary*> dllQMap;

struct IMAGE_INGREDIENT
{
	char* _cImageData = nullptr;
	char* _cImageShowData = nullptr;
	int w = -1, h = -1, c = -1;
	int _iIndexofImg = -1;
	int _iIndexofCam = -1;
	bool _bGetIMG = false;
	bool _bDecoding = false;
	bool _bDecodeResult = false;
	char _cSavePath[256];
	ResultStruct _result;
	int  _iSaveOKorNG = -1;
	bool _bSave = false;
	bool _bShowImg = false;
	bool _bFinished = false;
	bool _bKickExecute = false;
	LARGE_INTEGER nBeginTime;
	bool _bExeResult = false;

	double _msspend = -1.0;
	bool operator = (IMAGE_INGREDIENT _in)
	{
		if (nullptr != _cImageData && w != _in.w || h != _in.h || c != _in.c)
		{
			delete[] _cImageData;
			_cImageData = new char[_in.w * _in.h * _in.c];
		}
		if (nullptr != _cImageShowData && w != _in.w || h != _in.h || c != _in.c)
		{
			delete[] _cImageShowData;
			_cImageShowData = new char[_in.w * _in.h * _in.c];
		}
		w = _in.w;
		h = _in.h;
		c = _in.c;
		_bGetIMG = _in._bGetIMG;
		_iIndexofImg = _in._iIndexofImg;
		_iIndexofCam = _in._iIndexofCam;
		_bDecoding = _in._bDecoding;
		_bDecodeResult = _in._bDecodeResult;
		_result = _in._result;
		memcpy(_cSavePath, _in._cSavePath, 256);
		_iSaveOKorNG = _in._iSaveOKorNG;
		_bSave = _in._bSave;
		_bShowImg = _in._bShowImg;
		_bFinished = _in._bFinished;
		_bKickExecute = _in._bKickExecute;
		nBeginTime = _in.nBeginTime;
		_bExeResult = _in._bExeResult;
		_msspend = _in._msspend;
		if (nullptr != _in._cImageData && w != -1 && h != -1 && c != -1)
		{
			if (nullptr == _cImageData)
			{
				_cImageData = new char[w * h * c];
			}
			memcpy(_cImageData, _in._cImageData, w * h * c);
			if (nullptr == _cImageShowData)
			{
				_cImageShowData = new char[w * h * c];
			}
			memcpy(_cImageShowData, _in._cImageShowData, w * h * c);
		}
		return true;
	};
	void resetStuct()
	{
		_bGetIMG = false;
		_iIndexofImg = -1;
		_iIndexofCam = -1;
		_bDecoding = false;
		_bDecodeResult = false;
		_iSaveOKorNG = -1;
		memset(&_result, 0, sizeof(ResultStruct));
		strset(_cSavePath, '0');
		_bSave = false;
		_bShowImg = false;
		_bFinished = true;
		_bKickExecute = false;
		nBeginTime;
		_bExeResult = false;
		_msspend = -1;
	}
};
struct IMAGE_SIMPLEINGREDIENT
{
	char* _cImageData = nullptr;
	char* _cImageShowData = nullptr;
	int w = -1, h = -1, c = -1;
	int _iIndexofImg = -1;
	int _iIndexofCam = -1;
	char _cSavePath[256];
	int  _iSaveOKorNG = -1;
	bool _bSave = false;
	bool _bShowImg = false;
	bool _bFinished = false;
	bool _bKickExecute = false;
	LARGE_INTEGER nBeginTime;
	bool _bExeResult = false;

	double _msspend = -1.0;
	bool operator = (IMAGE_INGREDIENT _in)
	{
		if (nullptr != _cImageData && w != _in.w || h != _in.h || c != _in.c)
		{
			delete[] _cImageData;
			_cImageData = new char[_in.w * _in.h * _in.c];
		}
		if (nullptr != _cImageShowData && w != _in.w || h != _in.h || c != _in.c)
		{
			delete[] _cImageShowData;
			_cImageShowData = new char[_in.w * _in.h * _in.c];
		}
		w = _in.w;
		h = _in.h;
		c = _in.c;
		_iIndexofImg = _in._iIndexofImg;
		_iIndexofCam = _in._iIndexofCam;
		memcpy(_cSavePath, _in._cSavePath, 256);
		_iSaveOKorNG = _in._iSaveOKorNG;
		_bSave = _in._bSave;
		_bShowImg = _in._bShowImg;
		_bFinished = _in._bFinished;
		_msspend = _in._msspend;
		if (nullptr != _in._cImageData && w != -1 && h != -1 && c != -1)
		{
			if (nullptr == _cImageData)
			{
				_cImageData = new char[w * h * c];
			}
			memcpy(_cImageData, _in._cImageData, w * h * c);
			if (nullptr == _cImageShowData)
			{
				_cImageShowData = new char[w * h * c];
			}
			memcpy(_cImageShowData, _in._cImageShowData, w * h * c);
		}
		return true;
	};
	bool operator = (IMAGE_SIMPLEINGREDIENT _in)
	{
		if (nullptr != _cImageData && w != _in.w || h != _in.h || c != _in.c)
		{
			delete[] _cImageData;
			_cImageData = new char[_in.w * _in.h * _in.c];
		}
		if (nullptr != _cImageShowData && w != _in.w || h != _in.h || c != _in.c)
		{
			delete[] _cImageShowData;
			_cImageShowData = new char[_in.w * _in.h * _in.c];
		}
		w = _in.w;
		h = _in.h;
		c = _in.c;
		_iIndexofImg = _in._iIndexofImg;
		_iIndexofCam = _in._iIndexofCam;
		memcpy(_cSavePath, _in._cSavePath, 256);
		_iSaveOKorNG = _in._iSaveOKorNG;
		_bSave = _in._bSave;
		_bShowImg = _in._bShowImg;
		_bFinished = _in._bFinished;
		if (nullptr != _in._cImageData && w != -1 && h != -1 && c != -1)
		{
			if (nullptr == _cImageData)
			{
				_cImageData = new char[w * h * c];
			}
			memcpy(_cImageData, _in._cImageData, w * h * c);
			if (nullptr == _cImageShowData)
			{
				_cImageShowData = new char[w * h * c];
			}
			memcpy(_cImageShowData, _in._cImageShowData, w * h * c);
		}
		return true;
	};
};

enum ERRORTYPE
{
	TYPE_SYSTEM = 0,
	TYPE_OPERATE,
	TYPE_ALARM
};
enum ERRORLEVEL
{
	e_TRACE = 0,
	e_DEBUG,
	e_INFO,
	e_WARN,
	e_ERROR,
	e_CRITICAL,
	e_OFF,
	e_levels
};
enum CURRENTINDEX
{
	READINDEX = 0,
	DECODEDRESULT,
	SAVEFINISHED,
	KICKEXECUTE,
	EXERESULT
};
extern int g_iGetFromCam[];

extern int g_iDecodeFineshed[];

extern QVector<IMAGE_INGREDIENT*>g_GetedImage[];
extern QVector<IMAGE_SIMPLEINGREDIENT*>g_ShowBUfferImage[];

//日志工具

class CatchImageFromCam :public QObject
{
	Q_OBJECT
signals:
	void GetImageData(int campos, int imgindex);
	void ERRORMESSAGE(int, QString, int);
private:
	int m_iCamIndex = 0;
	int i_captotal = -1;
	int w = -1, h = -1, c = -1;
	int m_iWriteImg = -1;
	QtBaseCameraClass* _toGetImg;
	IMAGE_INGREDIENT m_currentWrite;
public:
	CatchImageFromCam(QObject* parent = nullptr)
	{
		m_iCamIndex = 0;
	};
	~CatchImageFromCam()
	{
	};
	bool SetCameraClass(void* pCamera)
	{
		_toGetImg = (QtBaseCameraClass*)pCamera;
		return true;
	};
	bool SetCatchTreadIndex(int i)
	{
		m_iCamIndex = i;
		return true;
	};
	bool GetCurrentIndex(int& index2return)
	{
		index2return = i_captotal;
		return true;
	};
	void GetImageFromCam()
	{
		UINT_PTR p;
		int _iIndexofCam;
		bool b = _toGetImg->GetCameraInt(CAMIMGCOUNT, _iIndexofCam);
		b = _toGetImg->GetImage(p);
		if (b)
		{
			if (0 == i_captotal)
			{
				_toGetImg->GetCameraInt(WIDTH, w);
				_toGetImg->GetCameraInt(HEIGHT, h);
				_toGetImg->GetCameraInt(IMAGECHANNEL, c);
				if (nullptr == m_currentWrite._cImageData)
					m_currentWrite._cImageData = new char[w * h * c];
				else
				{
					delete[] m_currentWrite._cImageData;
					m_currentWrite._cImageData = new char[w * h * c];
				}
				if (nullptr == m_currentWrite._cImageShowData)
					m_currentWrite._cImageShowData = new char[w * h * c];
				else
				{
					delete[] m_currentWrite._cImageShowData;
					m_currentWrite._cImageShowData = new char[w * h * c];
				}
				m_currentWrite.w = w;
				m_currentWrite.h = h;
				m_currentWrite.c = c;
				g_iGetFromCam[m_iCamIndex] = 0;
			}
			memcpy(m_currentWrite._cImageData, (char*)p, w * h * c);
			m_currentWrite._bGetIMG = true;
			m_currentWrite._iIndexofImg = i_captotal;
			m_currentWrite._iSaveOKorNG = m_iWriteImg;
			QueryPerformanceCounter(&(m_currentWrite.nBeginTime));
			if (!g_GetedImage[m_iCamIndex][g_iGetFromCam[m_iCamIndex] % CAMREACATCHBUFFER]->_bFinished)
			{
				QString str = QString("GetImageFromCam _bFinished ERROR Cam %1,System Count %2").arg(_iIndexofCam).arg(i_captotal);
				emit ERRORMESSAGE(TYPE_SYSTEM, str, e_CRITICAL);
			}
			*g_GetedImage[m_iCamIndex][g_iGetFromCam[m_iCamIndex] % CAMREACATCHBUFFER] = m_currentWrite;
			g_iGetFromCam[m_iCamIndex]++;
			emit GetImageData(m_iCamIndex, i_captotal);
			QString str = QString("GetImageFromCam LOCK Cam %1,System Count %2").arg(_iIndexofCam).arg(i_captotal);
			emit ERRORMESSAGE(TYPE_SYSTEM, str, e_TRACE);
			i_captotal++;
		}
	};
public slots:
	void StartCatchImage()
	{
		i_captotal = 0;
		m_iWriteImg = g_MultCamera[m_iCamIndex]->m_info.i_SaveOKorNG;
	};
};
extern QVector<CatchImageFromCam*>m_MultGetImageFromCam;
extern QVector<QThread*>m_MultGetImageFromCamThread;

class ErrorMessageStatistic : public QObject
{
	Q_OBJECT
signals:
	void SIGNAL_SHOWONMAINDLG(QString, int);
private:
	std::shared_ptr<spd::logger> m_SYS_logger;//系统
	std::shared_ptr<spd::logger> m_ALM_logger;//报警
	std::shared_ptr<spd::logger> m_OPS_logger;//操作
	void SYSTEMLOG(QString messageStr, ERRORLEVEL messagelevel)
	{
		switch (messagelevel)
		{
		case e_TRACE:
			m_SYS_logger->trace(messageStr.toStdString());
			break;
		case e_DEBUG:
			m_SYS_logger->debug(messageStr.toStdString());
			break;
		case e_INFO:
			m_SYS_logger->info(messageStr.toStdString());
			break;
		case e_WARN:
			m_SYS_logger->warn(messageStr.toStdString());
			break;
		case e_ERROR:
			m_SYS_logger->error(messageStr.toStdString());
			break;
		case e_CRITICAL:
			m_SYS_logger->critical(messageStr.toStdString());
			break;
		default:
			break;
		}
	};
	void ALARMLOG(QString messageStr, ERRORLEVEL messagelevel)
	{
		switch (messagelevel)
		{
		case e_TRACE:
			m_ALM_logger->trace(messageStr.toStdString());
			break;
		case e_DEBUG:
			m_ALM_logger->debug(messageStr.toStdString());
			break;
		case e_INFO:
			m_ALM_logger->info(messageStr.toStdString());
			break;
		case e_WARN:
			m_ALM_logger->warn(messageStr.toStdString());
			break;
		case e_ERROR:
			m_ALM_logger->error(messageStr.toStdString());
			break;
		case e_CRITICAL:
			m_ALM_logger->critical(messageStr.toStdString());
			break;
		default:
			break;
		}
	};
	void OPERATELOG(QString messageStr, ERRORLEVEL messagelevel)
	{
		switch (messagelevel)
		{
		case e_TRACE:
			m_OPS_logger->trace(messageStr.toStdString());
			break;
		case e_DEBUG:
			m_OPS_logger->debug(messageStr.toStdString());
			break;
		case e_INFO:
			m_OPS_logger->info(messageStr.toStdString());
			break;
		case e_WARN:
			m_OPS_logger->warn(messageStr.toStdString());
			break;
		case e_ERROR:
			m_OPS_logger->error(messageStr.toStdString());
			break;
		case e_CRITICAL:
			m_OPS_logger->critical(messageStr.toStdString());
			break;
		default:
			break;
		}
	};
	QString GetDataY_M_D()
	{
		QDateTime _current_time = QDateTime::currentDateTime();
		return QString("%1_%2_%3_%4_%5_%6").arg(_current_time.date().year(), 4, 10, QChar('0')).arg(_current_time.date().month(), 2, 10, QChar('0')).arg(_current_time.date().day(), 2, 10, QChar('0')).arg(_current_time.time().hour(), 2, 10, QChar('0')).arg(_current_time.time().minute(), 2, 10, QChar('0')).arg(_current_time.time().second(), 2, 10, QChar('0'));
	}
	QString m_strName = "SigmaSquare_Default";
public:
	ErrorMessageStatistic(QObject* parent = nullptr)
	{
	};
	void SetOperaterName(QString _name)
	{
		m_strName = _name;
	}
	void setLevel(int FLUSHON, int LEVEL)
	{
		QString AppPath = qApp->applicationDirPath();
		AppPath.replace("/", "\\");
		QString logpath = AppPath + "/logs/" + m_strName.mid(5) + "/System/System_daily_"
			+ GetDataY_M_D() + ".txt";
		m_SYS_logger = spd::basic_logger_mt("M_SYS_logger", logpath.toStdString());
		m_SYS_logger->flush_on((spd::level::level_enum)FLUSHON);
		m_SYS_logger->set_level((spd::level::level_enum)LEVEL);
		logpath = AppPath + "/logs/" + m_strName.mid(5) + "/Alarm/Alarm_daily_"
			+ GetDataY_M_D() + ".txt";
		m_ALM_logger = spd::basic_logger_mt("m_ALM_logger", logpath.toStdString());
		m_ALM_logger->flush_on((spd::level::level_enum)FLUSHON);
		m_ALM_logger->set_level((spd::level::level_enum)LEVEL);
		logpath = AppPath + "/logs/" + m_strName.mid(5) + "/Operation/Operation_daily_"
			+ GetDataY_M_D() + ".txt";
		m_OPS_logger = spd::basic_logger_mt("m_OPS_logger", logpath.toStdString());
		m_OPS_logger->flush_on((spd::level::level_enum)FLUSHON);
		m_OPS_logger->set_level((spd::level::level_enum)LEVEL);

	}
public slots:
	void SLOT_MESSAGELOG(int messageType, QString messageStr, int messagelevel)
	{
		switch (messageType)
		{
		case TYPE_SYSTEM:
			SYSTEMLOG(messageStr, (ERRORLEVEL)messagelevel);
			break;
		case TYPE_OPERATE:
			OPERATELOG(messageStr, (ERRORLEVEL)messagelevel);
			break;
		case TYPE_ALARM:
			ALARMLOG(messageStr, (ERRORLEVEL)messagelevel);
			break;
		default:
			break;
		}
		emit SIGNAL_SHOWONMAINDLG(messageStr, (ERRORLEVEL)messagelevel);
	};
};
extern ErrorMessageStatistic* m_MessageInit;

class MultInit_Run : public QWidget
{
	Q_OBJECT
signals:
	void FINISHEDCAMERA(int);
	void ERRORMESSAGE(int, QString, int);
	void STARTCONNECTPLC();
	void Signal_SetShowDlg();
private:
	void* _parent;
	bool ThreadInitAlgo();
	bool InitPLC();
	bool InitSummary();
public:
	MultInit_Run(QWidget* parent, void* user);
	~MultInit_Run();
public slots:
	bool ThreadInitCamera();
};
class MultDecodeThread_Run : public QObject
{
	Q_OBJECT
signals:
	void SAVESIGNAL(int, int);
	void OUTRESULTSUMMARY(QString, int, int);
	void SignaltoShowThread(int, int);
	void ERRORMESSAGE(int, QString, int);
private:
	int m_iCamIndex;
	int m_iAlgIndex;
	uint m_iResult;	//单次检测结果，最大不超过八个
	int i_captotal = 0;
	int w, h, c;
	QString m_sCameraSign;
	int index_pos;
	LARGE_INTEGER litmpst, litmpend, freq;
	double dfTim = 0.0;
	int n_index = 0;
	QString okdir_str;
	QString ngdir_str;
	bool m_bStop = false;
	IMAGE_INGREDIENT tempingredient;
	QString str;
	cv::Mat m;
	cv::Mat temp;
	QImage Qtemp;
	bool _bread = false;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	LARGE_INTEGER nFreq;
	char te[7];
	char te_time[7];
	char savePath[256];
public:
	MultDecodeThread_Run(QObject* parent, void*);
	~MultDecodeThread_Run();
	CBaseCheckAlg* _CheckClass = nullptr;
	pDeleteExport destructor = nullptr;
public slots:
	int ThreadDecodeImageMat(int camIndex, int _index);
	int FinishedCheck();
	void StopThread() {
		m_bStop = true;
	};
public:
	void SetTimeCheck();
	void SetMultIndex(int, int);
	void SetCameraInfo(CAMERAINFO_F&);
	int ResetDecodeImage(int indexcam = -1, bool b = false);
	bool unloadDll();
};
//decode which is algo run in
extern QVector<MultDecodeThread_Run*> m_MultDecodeThread;
extern QVector<QThread*> QTH_MultDecodeThread;
class MultSaveThread_Run : public QObject
{
	Q_OBJECT
signals:
	void ERRORMESSAGE(int, QString, int);
private:
	int m_iSelfIndex;
	int m_icamcount;
	int m_iwriteIndex = 0;
	int m_ireadIndex = 0;
	int _iSaveOKorNG = -2;
	int _iSaveLoop = -2;
	QTimer* ti = nullptr;
	QMutex mutx;
	IMAGE_INGREDIENT tempingredient;
	BOOL savebitmap(const char* data, int nImgW, int nImgH, const char* filename)
	{
		BITMAPINFOHEADER bmiHdr; //定义信息头
		bmiHdr.biSize = sizeof(BITMAPINFOHEADER);
		bmiHdr.biWidth = nImgW;
		bmiHdr.biHeight = nImgH;
		bmiHdr.biPlanes = 1;
		bmiHdr.biBitCount = 24;
		bmiHdr.biCompression = BI_RGB;
		bmiHdr.biSizeImage = nImgW * nImgH * 3;
		bmiHdr.biXPelsPerMeter = 0;
		bmiHdr.biYPelsPerMeter = 0;
		bmiHdr.biClrUsed = 0;
		bmiHdr.biClrImportant = 0;
		FILE* fp = fopen(filename, "wb");
		if (fp)
		{
			BITMAPFILEHEADER fheader = { 0 };
			fheader.bfType = 'M' << 8 | 'B';
			fheader.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + bmiHdr.biSizeImage;
			fheader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
			fwrite(&fheader, 1, sizeof(fheader), fp);
			fwrite(&bmiHdr, 1, sizeof(BITMAPINFOHEADER), fp);
			fwrite(data, 1, bmiHdr.biSizeImage, fp);
			fclose(fp);
			return TRUE;
		}
		else
			return FALSE;
	}
	char temppath[256];
	char te[10];
	QString datatime;
public:
	MultSaveThread_Run(QObject* parent, int camcount);
	~MultSaveThread_Run();
	void SetSaveSignal(int i, int n, QDateTime g_current_time) {
		_iSaveOKorNG = i;
		_iSaveLoop = n;

		datatime = QString("%1_%2_%3_%4_%5_%6").arg(g_current_time.date().year(), 4, 10, QChar('0')).arg(g_current_time.date().month(), 2, 10, QChar('0')).arg(g_current_time.date().day(), 2, 10, QChar('0')).arg(g_current_time.time().hour(), 2, 10, QChar('0')).arg(g_current_time.time().second(), 2, 10, QChar('0')).arg(g_current_time.time().second(), 2, 10, QChar('0'));
	};
signals:
	void DELETETIF(QString str_tif);
	void SaveIndex(int);
public slots:
	void ThreadSave(int, int);
	void RealSaveThread();
	//void QMutexUnLock();
};
//Thread to save
extern MultSaveThread_Run* m_MultSaveThread;
extern QThread* QTH_MultSaveThread;

extern QMutex g_MutexShowRefresh;
//extern cv::Mat g_mToShow[];
class MultShowThread_run : public QObject
{
	Q_OBJECT
signals:
	void ERRORMESSAGE(int, QString, int);
	void Signal_ShowImage(int, int);
private:
	bool m_bStop = false;
	int m_iCamIndex = -1;
	int m_iCount = -1;
	void* _parent = nullptr;
	int w = -1, h = -1, c = -1;
	cv::Mat m_temp;
	cv::Mat m_tempshow;
	IMAGE_INGREDIENT tempingredient;
public:
	MultShowThread_run();
public:
	bool SetCamIndex(int _index);
	bool SetParentPointer(void* p);
public slots:
	void StopThread();
	void onSlotToShow(int camIndex, int imgIndex);
};
//THread to Show
extern QVector<MultShowThread_run*> m_MultShowThread;
extern QVector<QThread*> QTH_MultShowThread;

class MultDeleteTifThread_Run : public QObject
{
	Q_OBJECT
signals:
	void UNLOCKSIGNAL();
private:
	int m_iSelfIndex;
	int m_icamcount;
public:
	MultDeleteTifThread_Run(QObject* parent);
	~MultDeleteTifThread_Run();
public slots:
	//everything
	void evethingSearchResults(QString str);
};
class MultSummaryThread_Run : public QObject
{
	Q_OBJECT
signals:
	void summaryNgNo(int);
	void SUMMARYRESULTINCIRCLE(QStringList);
	void ERRORMESSAGE(int, QString, int);
	void KICKTOPLCEXE(int, int*, int);
	void PLCMESSAGE(int, bool);
private:
	int m_iNgNo = 0;//连剔除数
	int m_iSelfIndex;
	int m_icamcount;
	double avg;
	bool* b_eachalreadyfinish = nullptr;

	unsigned int* result = nullptr;
	QStringList* m_qslResultEachCamera = nullptr;
	QStringList m_qslResultTOTALCamera;
	QMap<int, QStringList>resultMap;

	int m_iCount = 0;
	int m_iContinue = 0;
	bool m_bCAlarm = false;
	bool m_bCStop = false;
	int m_iCAlarm = 0;
	int m_iCStop = 0;
public:
	void getPlcLib(void*);
	void getM_iCount(int index);
	void SetCamCount(int index) { m_icamcount = index; };
	void SetKickCount(bool b_alarm, int _continueKickAlaram, bool b_stop, int _continueKickStop)
	{
		m_bCAlarm = b_alarm;
		m_iCAlarm = _continueKickAlaram;
		m_bCStop = b_stop;
		m_iCStop = _continueKickStop;
	};
	MultSummaryThread_Run(QObject* parent = nullptr, int camcount = 0, int lastpos = 0);
	~MultSummaryThread_Run();
public slots:
	void ThreadSummary(QString, int, int);
};

//THread to Summary
extern MultSummaryThread_Run* m_MultSummaryThread;
extern QThread* QTH_MultSummaryThread;