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
	//相机显示位置，InitCheckClass添加，并加载入g_CheckClass。
	//未设置算法时，只标记在此，方便后续补充加载g_CheckClass；
	//RECT u_Rect;
	int TriggerBy;//4：TriggerBy
	bool b_waitrespond;
	OUTMODEL i_OutModel; //3：OutModel
	//Basler无法读取IP，该参数为IP最后三位
	char c_CameraSign[10];		//7：CameraSign
	int i_RealLocatPos;
	char c_CameraOtherSign[30];		// 8：OtherCameraName
	int i_CheckPosNo;		//程序内部序号 3--4==ipos++
							//标识符
	int TypeOFCamera;		//序号见上 //10：TypeOFCamera
	char c_AlgName[30];                //2：AlgName
	char file_path[256];                //2：AlgName
	int i_ExpouseTime;		//曝光时长 //5：ExpouseTime
	int i_LineRateHZ;		//连续采集帧率 //6：LineRateHZ
	int i_SaveOKorNG;	//存图标识符，-1，NG存图；0，全部保存；1，OK存图
	int i_SaveLoop;		//存图循环标志服，0，全部存图；>0，张数内循环
	int i_OffsetX;		//ROI X 偏移量
	int i_OffsetY;		//ROI Y 偏移量
	int i_FrameWidth;		//图像高度
	int i_FrameHeight;		//图像宽度
	int i_FrameChannel;		//图像通道数
	bool b_Attach = false;
	int u_iTriggerSelect;	//0:外触发Line1,1：软件触发
	bool b_InvertX;				//9：InvertInt
	//
	void* CallBackToGetImage = nullptr;
	void* parentpointer = nullptr;
	char c_CameraName[20];	//相机名称 //1：序列号
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
	//系统时间
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

	virtual bool SingleGetSettableValues(QStringList&) = 0;//获取相机可设置trigger值
	virtual bool SetCameraTrigger(const QString) = 0;
	virtual bool GetCameraTrigger(QString&) = 0;
	virtual bool GetImage(UINT_PTR&) = 0;


	UINT_PTR* ptrbuffer;

	QMutex g_mutex;
public slots:
	virtual bool StartCamera(QString,bool b_continue = false) = 0;
	virtual bool StopCamera(QString) = 0;
};