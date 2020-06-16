#pragma once
#include "QT_FRONTCheckClass.h"
#include "InterCHeck.h"
class COutShowCheck :public QObject,
	public CBaseCheckAlg
{
	Q_OBJECT
private:
	CInterCHeck* SecretBase;
public:
	COutShowCheck(bool b_init);
	~COutShowCheck();
public:
	virtual void Release();
	virtual char* GetCameraName(void);
	virtual char* GetAlgName(void);
	virtual int ShowParamDlg(QWidget * parent, bool b_showornot);
	virtual int SetParam(int _typeofcamera, char* _cameraName);
	virtual int ReturnParam(int *_typeofcamera, char& _cameraName);
	//virtual int ParamDlg(RECT re);
	virtual int InitWindow(int pos, HANDLE _LEDhandle, void* _auhandle);
	//virtual int ReturnWindow(CWnd **hwnd, RECT &rect, CWnd **hwnd2, RECT &rect2);
	virtual int GetCheckPosNo();
	virtual void StartCheck(QString camerasign, std::shared_ptr<spd::logger> daily_logger);
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
	virtual void SetShowCallBack(UI_MONITOR ui,CallbackImage callbackfun);
	typedef void(*CallbackClose)();
	virtual void SetCloseCallBack(CallbackClose callbackfun);

};

