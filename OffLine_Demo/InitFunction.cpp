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
	char * configpath = new char[MAX_PATH];
	strcpy(configpath, AppPath.toStdString().c_str());
	strcat(configpath, ("\\ModelFile\\" + g_qModelName + "\\CameraConfig.ini").toStdString().c_str());//c_str����char*
	QVector<std::string> v_line;//�洢������к�
	std::ifstream cin(configpath);//���ļ�·�������׼��
	delete configpath;
	std::string filename;
	std::string line;
	int r_n = 0, ipos = 0;
	if (cin) // �и��ļ�  
	{//ȷ��ini�ļ����ж�����������ļ�������IP����v_line
		while (getline(cin, line)) // line�в�����ÿ�еĻ��з�  
		{
			if (line.find("[") != std::string::npos && line.find("]") != std::string::npos)//std::string::nposָ������
			{
				line.erase(line.length() - 1, 1);//ɾ��]
				line.erase(0, 1);//ɾ��[
				if (line != "DefaultCameraType")
				{
					v_line.push_back(line);//�洢������к�
				}
			}
		}
	}
	r_n = v_line.size();//��ǰsize==5 ���ܸ���
	QSettings configIniRead(AppPath + "\\ModelFile\\" + g_qModelName + "\\CameraConfig.ini", QSettings::IniFormat);//���ɶ�ȡini��ʽ�ļ��Ķ���
	configIniRead.setIniCodec("UTF-8");
	// 	for (int i = 0; i < 1; i++)//forѭ���ڽ�����typeofcameraΪ0����������������g_vectorCamera
	// 	{
	// 
	QString s = "LocalFile";
	// 		int itpye = configIniRead.value(s + "/TypeOFCamera").toInt();//[]�Ǳ�Ҫ�ģ����кű�ͷ�µĲ���
	//		if (itpye == TypeOFCamera) //KE0180100154 typeofcamera 1,	23175533 typeofcamera 0
	{
		CAMERASTRUCT* tempcamera = new CAMERASTRUCT();
		strcpy_s(tempcamera->c_CameraName, "LocalFile");
		strcpy_s(tempcamera->c_AlgName, configIniRead.value(s + "/AlgName").toString().toStdString().c_str());
		tempcamera->i_OutModel = (OUTMODEL)configIniRead.value(s + "/OutModel").toInt();
		tempcamera->i_CheckPosNo = ipos++;
		tempcamera->TriggerBy = configIniRead.value(s + "/TriggerBy").toInt();
		tempcamera->i_ExpouseTime = configIniRead.value(s + "/ExpouseTime").toInt();
		tempcamera->i_LineRateHZ = configIniRead.value(s + "/LineRateHZ").toInt();
		tempcamera->i_imgWidth = configIniRead.value(s + "/imgWidth").toInt();
		tempcamera->i_imgHeight = configIniRead.value(s + "/imgHeight").toInt();
		tempcamera->i_OffsetX = configIniRead.value(s + "/OffsetX").toInt();
		tempcamera->i_OffsetY = configIniRead.value(s + "/OffsetY").toInt();
		tempcamera->i_SaveOKorNG = configIniRead.value(s + "/SaveOKorNG").toInt();
		tempcamera->i_SaveLoop = configIniRead.value(s + "/SaveLoop").toInt();
		strcpy(tempcamera->c_CameraSign, configIniRead.value(s + "/CameraSign").toString().toStdString().c_str());
		tempcamera->i_RealLocatPos = QString(tempcamera->c_CameraSign).mid(3).toInt();
		strcpy(tempcamera->c_CameraOtherSign, configIniRead.value(s + "/OtherCameraName").toString().toLocal8Bit());
		tempcamera->TypeOFCamera = TypeOFCamera;

		g_vectorCamera.push_back(tempcamera);//��11�����ݣ�����10������ini�ļ�
	}
	// 	}
	v_line.clear();
	return r_n;
}

bool InitFunction::GetAllCamera()
{

#ifdef BASLER
	TypeOFCamera = MODEL_BASLER;//0
#endif
#ifdef DAHENG
	TypeOFCamera = MODEL_DAHENG;//1
#endif
	//��ȡ�������
	ReadConfig();// read444
	return TRUE;
}