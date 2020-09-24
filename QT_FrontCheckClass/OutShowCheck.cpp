#include "OutShowCheck.h"
COutShowCheck::COutShowCheck(bool b_init)
{
	SecretBase = new CInterCHeck(b_init);
	//bool b = QObject::connect(SecretBase, SIGNAL(SHOWIMGPOSTOTALS(int, Mat, int)), this, SLOT(sendmessagetoOUT(int, Mat, int)));
}
COutShowCheck::~COutShowCheck()
{
}
void COutShowCheck::Release()
{
	SecretBase->Release();
	delete this;
}
char * COutShowCheck::GetCameraName(void)
{
	return SecretBase->GetCameraName();
}
char * COutShowCheck::GetAlgName(void)
{
	return SecretBase->GetAlgName();
}
int COutShowCheck::ShowParamDlg(QWidget * parent, bool b_showornot)
{
	return SecretBase->ShowParamDlg(parent,b_showornot);
}
int COutShowCheck::SetParam(int _typeofcamera, char * _cameraName)
{
	return SecretBase->SetParam(_typeofcamera, _cameraName);
}
int COutShowCheck::ReturnParam(int * _typeofcamera, char & _cameraName)
{
	return SecretBase->ReturnParam(_typeofcamera, _cameraName);
}
int COutShowCheck::InitWindow(int pos, HANDLE _LEDhandle, void* _auhandle)
{
	return SecretBase->InitWindow(pos, _LEDhandle, _auhandle);
}
int COutShowCheck::GetCheckPosNo()
{
	return SecretBase->GetCheckPosNo();
}
void COutShowCheck::StartCheck(QString camerasign, std::shared_ptr<spd::logger> daily_logger,int w,int h)
{
	SecretBase->StartCheck(camerasign, daily_logger, w ,h);
}
void COutShowCheck::StopCheck()
{
	SecretBase->StopCheck();
}
QString COutShowCheck::GetResult()
{
	return SecretBase->GetResult();
}
int COutShowCheck::Check(Mat imgpackage, void *checkparam, QString &str)
{
	SecretBase->Check(imgpackage, checkparam, str);
	SecretBase->OtherAfterCheck();
	int b = SecretBase->RealCheck(str, (CHECKPARAM*)checkparam,-1);
	SecretBase->OtherBeforeCheck(imgpackage);
	return b;
}
void COutShowCheck::ShowResult(QVector<double*>& result)
{
	SecretBase->ShowResult(result);
}
void COutShowCheck::BeatStart(void)
{
	SecretBase->BeatStart();
}
void COutShowCheck::BeatEnd(void)
{
	SecretBase->BeatEnd();
}
void * COutShowCheck::GetEncryptHandle()
{
	return SecretBase->GetEncryptHandle();
}
void COutShowCheck::EnableShow(bool s)
{
	SecretBase->EnableShow(s);
}
void COutShowCheck::TESTSETSHOW(void * pt)
{
	SecretBase->TESTSETSHOW(pt);
}
void COutShowCheck::SetResultCallBack(UI_MONITOR ui, CallbackText callbackfun)
{
	SecretBase->SetResultCallBack(ui,callbackfun);
}
void COutShowCheck::SetShowCallBack(UI_MONITOR ui, CallbackImage callbackfun)
{
	SecretBase->SetShowCallBack(ui, callbackfun);
}
void COutShowCheck::SetCloseCallBack(CallbackClose callbackfun)
{
	SecretBase->SetCloseCallBack(callbackfun);
}
