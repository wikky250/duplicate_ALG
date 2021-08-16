#pragma once

#define DLLINITSUCCESSED	1000
#define	DLLLOADERROR		1001
#define CAMERAINITSUCCESSED 20000
#define CAMERAINITFINDOTHER 20001
#define CAMERASETERROR		21000
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <intsafe.h>
#include <QSettings>
#include "opencv.hpp"
#include "highgui.hpp"
#ifdef _DEBUG
#pragma comment(lib,"opencv_core3410d.lib")
#pragma comment(lib,"opencv_highgui3410d.lib")
#pragma comment(lib,"opencv_imgcodecs3410d.lib")
#pragma comment(lib,"opencv_imgproc3410d.lib")
#else
#pragma comment(lib,"opencv_core3410.lib")
#pragma comment(lib,"opencv_highgui3410.lib")
#pragma comment(lib,"opencv_imgcodecs3410.lib")
#pragma comment(lib,"opencv_imgproc3410.lib")
#endif
#define BUFFERSIZE 5
#define TOTALCAMERACOUTN 10

typedef void(*CallbackImage) (void*);
enum OUTMODEL
{
	OUTMODEL_STRING = 0,
	OUTMODEL_INT,
	OUTMODEL_RISING,
	OUTMODEL_Falling,
};

enum CAMERATYPE
{
	LOCALFILE = 0,
	CAMERABASLER,
	CAMERAHIKVISION
};

typedef struct InitCameraStruct
{
	//////////////////////////////////////////////////////////////////////////
	//�����ʾλ�ã�InitCheckClass��ӣ���������g_CheckClass��
	//δ�����㷨ʱ��ֻ����ڴˣ���������������g_CheckClass��
	//RECT u_Rect;
	int TriggerBy;//4��TriggerBy
	bool b_waitrespond;
	OUTMODEL i_OutModel; //3��OutModel
	//Basler�޷���ȡIP���ò���ΪIP�����λ
	char c_CameraSign[10];		//7��CameraSign
	int i_RealLocatPos;
	char c_CameraOtherSign[30];		// 8��OtherCameraName
	int i_CheckPosNo;		//�����ڲ���� 3--4==ipos++
							//��ʶ��
	int TypeOFCamera;		//��ż��� //10��TypeOFCamera
	char c_AlgName[30];                //2��AlgName
	char file_path[256];                //2��AlgName
	int i_ExpouseTime;		//�ع�ʱ�� //5��ExpouseTime
	int i_LineRateHZ;		//�����ɼ�֡�� //6��LineRateHZ
	int i_SaveOKorNG;	//��ͼ��ʶ����-1��NG��ͼ��0��ȫ�����棻1��OK��ͼ
	int i_SaveLoop;		//��ͼѭ����־����0��ȫ����ͼ��>0��������ѭ��
	int i_OffsetX;		//ROI X ƫ����
	int i_OffsetY;		//ROI Y ƫ����
	int i_FrameWidth;		//ͼ��߶�
	int i_FrameHeight;		//ͼ����
	int i_FrameChannel;		//ͼ��ͨ����
	bool b_Attach = false;
	int u_iTriggerSelect;	//0:�ⴥ��Line1,1���������
	bool b_InvertX;				//9��InvertInt
	//
	void* CallBackToGetImage = nullptr;
	void* parentpointer = nullptr;
	char c_CameraName[20];	//������� //1�����к�
	char c_IniFile[200];
	InitCameraStruct()
	{
	}
	~InitCameraStruct()
	{
	}
	bool operator == (const InitCameraStruct& caminf)
	{
		if (this->i_ExpouseTime != caminf.i_ExpouseTime) return false;
		if (this->i_LineRateHZ != caminf.i_LineRateHZ) return false;
		if (this->i_FrameWidth != caminf.i_FrameWidth) return false;
		if (this->i_FrameHeight != caminf.i_FrameHeight) return false;
		if (this->i_OffsetX != caminf.i_OffsetX) return false;
		if (this->i_OffsetY != caminf.i_OffsetY) return false;
		if (this->TriggerBy != caminf.TriggerBy) return false;
		return true;
	}
	bool operator != (const InitCameraStruct& caminf)
	{
		this->i_ExpouseTime = caminf.i_ExpouseTime;
		this->i_LineRateHZ = caminf.i_LineRateHZ;
		this->i_FrameWidth = caminf.i_FrameWidth;
		this->i_FrameHeight = caminf.i_FrameHeight;
		this->i_OffsetX = caminf.i_OffsetX;
		this->i_OffsetY = caminf.i_OffsetY;
		this->TriggerBy = caminf.TriggerBy;
		this->i_SaveOKorNG = caminf.i_SaveOKorNG;
		this->i_SaveLoop = caminf.i_SaveLoop;
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
}CAMERAINFO_F;

enum CameraInt
{
	WIDTH,
	HEIGHT,
	IMAGECHANNEL,
	OFFSETX,
	OFFSETY,
	FRAMES,
	EXPOUSETIME,
	CAMIMGCOUNT
};
enum CameraTrigger
{
	NONE,
	SOFTTRIGGER,
	LINE0,
	LINE1,
	LINE2
};

class QtBaseCameraClass : public QObject
{
	Q_OBJECT
signals:
	void StartSingle();
	void RESTARTSIGNAL();
	void GETONEIMAGEMAT(uchar*, int);
	void signal_SHOWDIRECTIMG(cv::Mat);
	void signal_SendImageOut(char*, const cv::Mat, int);
	void ERRORMESSAGE(int, QString);
public:
	CAMERAINFO_F m_info;
private:
	CAMERAINFO_F Old_m_info;
	//ϵͳʱ��
	QDateTime current_time;
	int m_iTypeOFCamera = 0;
	int m_iSelfIndex;
	int totalcount;
	int sizetotal;
	int t = 0;
	bool m_bAlRegister = false;
	bool m_bDirectshow = false;
	bool m_bSave = false;
	QString m_AppPath;
	QString m_strFile;
	QVector<char*> m_QvData;
public:
	virtual void SetMultIndex(int) = 0;
	virtual void SetDirectShowDlg(bool) = 0;
	void* m_pUser = nullptr;
	CallbackImage imageCallBack = nullptr;

	virtual bool SingleInitCamera(const int, InitCameraStruct) = 0;

	virtual bool SingleOpenCamera(const char*) = 0;
	virtual bool SingleCloseCamera(const char*) = 0;

	virtual bool SingleGetSaveSignal(int&, int&) = 0;
	virtual bool SingleSetSaveSignal(const int, const int) = 0;

	virtual bool SingleGetAlgName(char&) = 0;
	virtual bool SingleSetAlgName(const char) = 0;

	virtual bool SingleSetDefault(const int) = 0;
	virtual bool SingleLoadDefault(const int) = 0;


	virtual bool SetCameraInt(CameraInt, int) = 0;
	virtual bool GetCameraInt(CameraInt, int&) = 0;

	virtual bool Set_GetBalanceWhite(bool) = 0;

	virtual bool SaveParam(bool) = 0;

	virtual bool SingleGetSettableValues(QStringList&) = 0;//��ȡ���������triggerֵ
	virtual bool SetCameraTrigger(const QString) = 0;
	virtual bool GetCameraTrigger(QString&) = 0;
	virtual bool GetImage(UINT_PTR&) = 0;


	UINT_PTR* ptrbuffer;

	QMutex g_mutex;
public slots:
	virtual bool StartCamera(QString,bool b_continue = false) = 0;
	virtual bool StopCamera(QString) = 0;
};