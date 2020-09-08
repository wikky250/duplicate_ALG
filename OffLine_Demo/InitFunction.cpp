#include "InitFunction.h"
extern QString AppPath;
extern QVector<CAMERASTRUCT*> g_vectorCamera;
extern QString g_qModelName;//当前应用模板
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
	strcat(configpath, ("\\ModelFile\\" + g_qModelName + "\\CameraConfig.ini").toStdString().c_str());//c_str返回char*
	QVector<std::string> v_line;//存储相机序列号
	std::ifstream cin(configpath);//将文件路径放入标准流
	delete configpath;
	std::string filename;
	std::string line;
	int r_n = 0, ipos = 0;
	if (cin) // 有该文件  
	{//确定ini文件中有多少相机配置文件，并将IP存入v_line
		while (getline(cin, line)) // line中不包括每行的换行符  
		{
			if (line.find("[") != std::string::npos && line.find("]") != std::string::npos)//std::string::npos指不存在
			{
				line.erase(line.length() - 1, 1);//删除]
				line.erase(0, 1);//删除[
				if (line != "DefaultCameraType")
				{
					v_line.push_back(line);//存储相机序列号
				}
			}
		}
	}
	r_n = v_line.size();//当前size==5 可能更多
	QSettings configIniRead(AppPath + "\\ModelFile\\" + g_qModelName + "\\CameraConfig.ini", QSettings::IniFormat);//生成读取ini格式文件的对象
	configIniRead.setIniCodec("UTF-8");
	// 	for (int i = 0; i < 1; i++)//for循环内将所有typeofcamera为0的相机及其参数放入g_vectorCamera
	// 	{
	// 
	QString s = "LocalFile";
	// 		int itpye = configIniRead.value(s + "/TypeOFCamera").toInt();//[]是必要的，序列号表头下的参数
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

		g_vectorCamera.push_back(tempcamera);//有11条内容，其中10条来自ini文件
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
	//获取相机参数
	ReadConfig();// read444
	return TRUE;
}