#pragma once
#include "QT_FRONTCheckClass.h"
#include "CheckRelay.h"
#include "QtGuiSetting.h"
//#include "InspectProcessor.h"

#ifndef __APPLE__
#include "cpp/HalconCpp.h"
#pragma comment(lib,"halconcpp10.lib")
using namespace Halcon;
#endif
class CInterCHeck :public QObject,
	public CBaseCheckAlg
{
	Q_OBJECT

signals :
	void StartUSBKEY(void* handle);
private:
	int saveImageindex;
	int saveImageOKindex;
	QString ngdir_str;
	QString okdir_str;
	const char* test_name;
	char* c_AlgName;
	//显示窗口，InitWindow赋值
	int  m_iShowPos;
	QImage disImage;
	//USBkey，InitWindow赋值
	HANDLE m_LEDHANDLE;
	//父窗口发送关闭指令
	void* m_parent;
	//USBKey监控线程
	QThread* USB_Thread;
	//QTLiveUSBkey *QtUsbKey;
	//检测参数
	CHECKPARAM m_checkparam;
	bool m_bchangedparam;
	//日志logger
	std::shared_ptr<spd::logger> daily_logger;


private:
#pragma region 算法相关参数
private:
	bool m_b_test;
	int sizeofimage;
	int circle_times = 0;
	int total_check;
	int total_NGSave;
	int total_OKSave;

	Mat MatToShow, LastImage;
	Mat grey,temp;

	//H9::InspectProcessor proc;
	Mat m_SaveImg;
	vector<vector<float>> m_CenterModel;

	vector<HTuple> m_ShowLabel;
	Hobject m_hoLiveImage;
	int ComputerBestLabes(std::vector<float> data, double &dsts);
#pragma endregion 算法相关参数



public:
	CInterCHeck(bool b_test);
	~CInterCHeck();
	UI_MONITOR ui;
	CallbackText textCallback;
	CallbackImage imageCallBack;
	CallbackClose CloseCallBack;
public:
	virtual void Release();
	virtual char* GetCameraName(void);
	virtual char* GetAlgName(void);
	virtual int ShowParamDlg(QWidget *parent, bool b_showornot);
	virtual int SetParam(int _typeofcamera, char* _cameraName);
	virtual int ReturnParam(int *_typeofcamera, char& _cameraName);
	//virtual int ParamDlg(RECT re);
	virtual int InitWindow(int pos, HANDLE _LEDhandle, void* _auhandle);
	//virtual int ReturnWindow(CWnd **hwnd, RECT &rect, CWnd **hwnd2, RECT &rect2);
	virtual int GetCheckPosNo();
	virtual void StartCheck(QString camerasign, std::shared_ptr<spd::logger> _daily_logger);
	virtual void StopCheck();
	virtual QString GetResult();
	virtual bool Check(Mat imgpackage, CHECKPARAM *checkparam, QString &str);
	virtual void ShowResult(QVector<double*> &result);
	virtual void BeatStart(void);
	virtual void BeatEnd(void);
	virtual void* GetEncryptHandle();
	virtual void EnableShow(bool);
	virtual void TESTSETSHOW(void*);
	typedef void(*CallbackText)(UI_MONITOR ui, char* i_result);
	virtual void SetResultCallBack(UI_MONITOR ui, CallbackText callbackfun);
	typedef void(*CallbackImage)(UI_MONITOR ui, int pos, Mat img, int times);
	virtual void SetShowCallBack(UI_MONITOR ui, CallbackImage callbackfun);
	typedef void(*CallbackClose)();
	virtual void SetCloseCallBack(CallbackClose callbackfun);
	bool OtherBeforeCheck(Mat);
	bool OtherAfterCheck();
	bool RealCheck(QString&);
	public slots:
	void EndCheck();
	bool LoadCheckParam(CHECKPARAM *checkparam);
	bool SaveCheckParam(CHECKPARAM *checkparam);
};

