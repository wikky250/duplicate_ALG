#pragma once
//#define TESTINCOMPUTER
#include <QVector>
#include <QSettings>
#include <QApplication>
#include <QLabel>
#include <QThread>
#include <QMessageBox>
#include <QGraphicsScene>
typedef void*   UI_MONITOR;
#include "OpencvRelay.h"
#include "QT_FrontCheckClass_global.h"
#define WM_USER_CLOSE			WM_USER+10009	//�ر�


#ifndef BUILD_STATIC
# if defined(QT_FrontCheckClass_LIB)
#  define QT_FrontCheckClass_EXPORT Q_DECL_EXPORT
# else
#  define QT_FrontCheckClass_EXPORT Q_DECL_IMPORT
# endif
#else 
# define QT_FrontCheckClass_EXPORT
#endif

class CBaseCheckAlg:public QObject
{
	Q_OBJECT
signals:
	void SHOWIMGPOSTOTAL(int, Mat, int);
public:
	virtual void Release() = 0;
	virtual char* GetCameraName(void) = 0;
	virtual char* GetAlgName(void) = 0;
	virtual int SetParam(int _typeofcamera, char* _cameraName) = 0;
	virtual int ShowParamDlg(QWidget * parent, bool b_showornot) = 0;
	virtual int ReturnParam(int *_typeofcamera, char& _cameraName) = 0;
	//virtual int ParamDlg(RECT re) = 0;
	virtual int InitWindow(int pos, HANDLE _LEDhandle, void* _auhandle) = 0;
	//virtual int ReturnWindow(CWnd **hwnd, RECT &rect, CWnd **hwnd2, RECT &rect2) = 0;
	virtual int GetCheckPosNo() = 0;
	virtual void StartCheck(QString camerasign,  std::shared_ptr<spd::logger> daily_logger,int w,int h) = 0;
	virtual void StopCheck() = 0;
	virtual QString GetResult() = 0;
	virtual int Check(Mat imgpackage, void *checkparam, QString &str) = 0;
	virtual void ShowResult(QVector<double*> &result) = 0;
	virtual void BeatStart(void) = 0;
	virtual void BeatEnd(void) = 0;
	virtual void* GetEncryptHandle() = 0;
	virtual void EnableShow(bool) = 0;
	virtual void TESTSETSHOW(void*) =0;
	//������Ϣ�ص�����
	typedef void (*CallbackText)(UI_MONITOR ui, char* i_result);
	virtual void SetResultCallBack(UI_MONITOR ui, CallbackText callbackfun) = 0;
	//����ͼ��ص�����
	typedef void(*CallbackImage)(UI_MONITOR ui, int pos, Mat img, int times);
	virtual void SetShowCallBack(UI_MONITOR ui, CallbackImage callbackfun) = 0;
	//���ùرջص�����
	typedef void(*CallbackClose)();
	virtual void SetCloseCallBack(CallbackClose callbackfun) = 0;
};
extern "C" QT_FrontCheckClass_EXPORT CBaseCheckAlg* APIENTRY CreateExportObj(bool b_init);
extern "C" QT_FrontCheckClass_EXPORT void APIENTRY DestroyExportObj(CBaseCheckAlg* pExport);
