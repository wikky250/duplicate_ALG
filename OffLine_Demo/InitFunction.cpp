#include "InitFunction.h"
extern QString AppPath;
extern QVector<CAMERASTRUCT*> g_vectorCamera;
extern QString g_qModelName;//��ǰӦ��ģ��
InitFunction::InitFunction(QObject *parent)
	: QObject(parent)
{
	TypeOFCamera = 1;
	StartModel = false;
}

InitFunction::~InitFunction()
{
}

int InitFunction::ReadConfig()
{
	CAMERASTRUCT* tempcamera = new CAMERASTRUCT();
	strcpy_s(tempcamera->c_CameraName, "LocalFile");
	strcpy(tempcamera->file_path, (char*)(AppPath.toStdString().c_str()));
	g_vectorCamera.push_back(tempcamera);
	return 1;
}

bool InitFunction::GetAllCamera()
{

	//��ȡ�������
	ReadConfig();// read444
	return TRUE;
}