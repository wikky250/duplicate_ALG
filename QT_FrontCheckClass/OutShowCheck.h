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
	virtual int ShowParamDlg(QWidget* parent, bool b_showornot);
	virtual int SetParam(int _typeofcamera, char* _cameraName);
	virtual int ReturnParam(int* _typeofcamera, char& _cameraName);
	virtual void StartCheck(QString camerasign, std::shared_ptr<spd::logger> daily_logger, int w, int h, int c);
	virtual void StopCheck();
	virtual QString GetResult();
	virtual int Check(Mat& imgpackage, void* checkparam, ResultStruct& str);
	virtual void ShowResult(QVector<double*>& result);
	virtual void BeatStart(void);
	virtual void BeatEnd(void);
	virtual void* GetEncryptHandle();
	virtual void EnableShow(bool);
	virtual void TESTSETSHOW(void*);
};
