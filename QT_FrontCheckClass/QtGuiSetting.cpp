#include "QtGuiSetting.h"
#include "InterCHeck.h"
#include <QtWidgets/QComboBox>
#include <QDesktopWidget>
#include <math.h>
#include<QMessageBox>
#include<QCheckBox>
#include<QMyTreeWidget.h>;
typedef void*   UI_MONITOR;
QString AppPath;
void ShowCallBack(UI_MONITOR ui, int pos, Mat img, int times)
{
	((QtGuiSetting*)ui)->emit SignShowImage(pos, img, times);
}
bool NodeToParam(CHECKPARAM &checkparam, YAML::Node &_param)
{
	checkparam.i_Channel_Band = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_泡罩板缺陷").toStdString()][QString::fromLocal8Bit("泡罩板通道").toStdString()]["value"].as<double>();
	checkparam.i_MinGray_Band = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_泡罩板缺陷").toStdString()][QString::fromLocal8Bit("泡罩板最小灰度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_Band = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_泡罩板缺陷").toStdString()][QString::fromLocal8Bit("泡罩板最小面积").toStdString()]["value"].as<double>();
	checkparam.i_PlusLength_Rectangle = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_泡罩板缺陷").toStdString()][QString::fromLocal8Bit("最小外接矩形增量").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_BandDefect = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_泡罩板缺陷").toStdString()][QString::fromLocal8Bit("泡罩板缺陷最小面积").toStdString()]["value"].as<double>();
	checkparam.i_Channel_Al = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_铝模缺陷").toStdString()][QString::fromLocal8Bit("铝模通道").toStdString()]["value"].as<double>();
	checkparam.i_MinGray_Al = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_铝模缺陷").toStdString()][QString::fromLocal8Bit("铝模最小灰度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_Al = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_铝模缺陷").toStdString()][QString::fromLocal8Bit("铝模缺陷最小面积").toStdString()]["value"].as<double>();
	checkparam.i_Channel1_NumPill = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂数量").toStdString()][QString::fromLocal8Bit("片剂数量通道1").toStdString()]["value"].as<double>();
	checkparam.i_Channel2_NumPill = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂数量").toStdString()][QString::fromLocal8Bit("片剂数量通道2").toStdString()]["value"].as<double>();
	checkparam.i_MinGray_NumPill = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂数量").toStdString()][QString::fromLocal8Bit("最小灰度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_NumPill = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂数量").toStdString()][QString::fromLocal8Bit("最小面积").toStdString()]["value"].as<double>();
	checkparam.i_Number_NumPill = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂数量").toStdString()][QString::fromLocal8Bit("片剂个数").toStdString()]["value"].as<double>();
	checkparam.i_Channel_Powder = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_夹粉").toStdString()][QString::fromLocal8Bit("颜色通道").toStdString()]["value"].as<double>();
	checkparam.i_DifMean_Powder = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_夹粉").toStdString()][QString::fromLocal8Bit("灰度均值差值").toStdString()]["value"].as<double>();
	checkparam.i_PlusLength1_Batch = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_批号").toStdString()][QString::fromLocal8Bit("最小外接矩形增量1").toStdString()]["value"].as<double>();
	checkparam.i_PlusLength2_Batch = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_批号").toStdString()][QString::fromLocal8Bit("最小外接矩形增量2").toStdString()]["value"].as<double>();
	checkparam.i_Channel_Batch = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_批号").toStdString()][QString::fromLocal8Bit("批号通道").toStdString()]["value"].as<double>();
	checkparam.i_MinGray_Batch = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_批号").toStdString()][QString::fromLocal8Bit("最小灰度").toStdString()]["value"].as<double>();
	checkparam.d_Opening_Batch = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_批号").toStdString()][QString::fromLocal8Bit("开运算参数").toStdString()]["value"].as<double>();
	checkparam.d_Closing_Batch = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_批号").toStdString()][QString::fromLocal8Bit("闭运算参数").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_Batch = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_批号").toStdString()][QString::fromLocal8Bit("最小面积").toStdString()]["value"].as<double>();
	checkparam.i_Channel1_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("阴影通道1").toStdString()]["value"].as<double>();
	checkparam.i_Channel2_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("阴影通道2").toStdString()]["value"].as<double>();
	checkparam.i_Channel3_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("阴影通道3").toStdString()]["value"].as<double>();
	checkparam.i_MinGray_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("最小灰度").toStdString()]["value"].as<double>();
	checkparam.i_Width_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("筛选最小宽度").toStdString()]["value"].as<double>();
	checkparam.i_Length_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("筛选最小长度").toStdString()]["value"].as<double>();
	checkparam.i_Area_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("筛选最小面积").toStdString()]["value"].as<double>();
	checkparam.d_Closing_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("闭运算参数").toStdString()]["value"].as<double>();
	checkparam.d_Opening_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("开运算参数").toStdString()]["value"].as<double>();
	checkparam.i_Radius_Shadow = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域").toStdString()][QString::fromLocal8Bit("最小内接圆半径").toStdString()]["value"].as<double>();
	checkparam.i_Channel_Crack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板裂纹/毛发").toStdString()][QString::fromLocal8Bit("颜色通道").toStdString()]["value"].as<double>();
	checkparam.i_Length_Crack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板裂纹/毛发").toStdString()][QString::fromLocal8Bit("筛选最小长度").toStdString()]["value"].as<double>();
	checkparam.d_Circularity_Crack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板裂纹/毛发").toStdString()][QString::fromLocal8Bit("筛选最大圆度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_BlackSpot = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板脏污/黑点").toStdString()][QString::fromLocal8Bit("筛选黑点最小面积").toStdString()]["value"].as<double>();
	checkparam.d_Circularity_BlackSpot = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板脏污/黑点").toStdString()][QString::fromLocal8Bit("筛选黑点最小圆度").toStdString()]["value"].as<double>();
	checkparam.i_Channel_Dirty = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板脏污/黑点").toStdString()][QString::fromLocal8Bit("颜色通道").toStdString()]["value"].as<double>();
	checkparam.i_MaxGray_Dirty = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板脏污/黑点").toStdString()][QString::fromLocal8Bit("最大灰度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_Dirty = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板脏污/黑点").toStdString()][QString::fromLocal8Bit("筛选脏污最小面积").toStdString()]["value"].as<double>();
	checkparam.d_Circularity_Dirty = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_药板脏污/黑点").toStdString()][QString::fromLocal8Bit("筛选脏污最小圆度").toStdString()]["value"].as<double>();
	checkparam.i_Channel_Fissure = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_裂纹2").toStdString()][QString::fromLocal8Bit("颜色通道").toStdString()]["value"].as<double>();
	checkparam.i_MinGray_Fissure = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_裂纹2").toStdString()][QString::fromLocal8Bit("最小灰度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_Fissure = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_裂纹2").toStdString()][QString::fromLocal8Bit("筛选最小面积").toStdString()]["value"].as<double>();
	checkparam.d_Circularity_Fissure = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_裂纹2").toStdString()][QString::fromLocal8Bit("筛选最大圆度").toStdString()]["value"].as<double>();
	checkparam.i_Channel1_Iner = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂内部缺陷").toStdString()][QString::fromLocal8Bit("颜色通道1").toStdString()]["value"].as<double>();
	checkparam.i_Channel2_Iner = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂内部缺陷").toStdString()][QString::fromLocal8Bit("颜色通道2").toStdString()]["value"].as<double>();
	checkparam.i_MinGray_Iner = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂内部缺陷").toStdString()][QString::fromLocal8Bit("最小灰度").toStdString()]["value"].as<double>();
	checkparam.d_Closing_Iner = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂内部缺陷").toStdString()][QString::fromLocal8Bit("闭运算参数").toStdString()]["value"].as<double>();
	checkparam.d_Opening_Iner = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂内部缺陷").toStdString()][QString::fromLocal8Bit("开运算参数").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_Half = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_半片").toStdString()][QString::fromLocal8Bit("最小面积").toStdString()]["value"].as<double>();
	checkparam.i_RectMove1_Half = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_半片").toStdString()][QString::fromLocal8Bit("仿射矩形增量1").toStdString()]["value"].as<double>();
	checkparam.i_RectMove2_Half = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_半片").toStdString()][QString::fromLocal8Bit("仿射矩形增量2").toStdString()]["value"].as<double>();
	checkparam.d_Erosion_Break = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_裂片").toStdString()][QString::fromLocal8Bit("圆腐蚀参数").toStdString()]["value"].as<double>();
	checkparam.i_Channel_Break = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_裂片").toStdString()][QString::fromLocal8Bit("颜色通道").toStdString()]["value"].as<double>();
	checkparam.i_Length_Break = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_裂片").toStdString()][QString::fromLocal8Bit("筛选裂片最小长度").toStdString()]["value"].as<double>();
	checkparam.d_Circularity_Break = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_裂片").toStdString()][QString::fromLocal8Bit("筛选裂片最大圆度").toStdString()]["value"].as<double>();
	checkparam.i_RectMove_Lack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂缺损").toStdString()][QString::fromLocal8Bit("仿射矩形增量").toStdString()]["value"].as<double>();
	checkparam.i_Channel1_Lack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂缺损").toStdString()][QString::fromLocal8Bit("颜色通道1").toStdString()]["value"].as<double>();
	checkparam.i_Channel2_Lack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂缺损").toStdString()][QString::fromLocal8Bit("颜色通道2").toStdString()]["value"].as<double>();
	checkparam.i_MaxGray_Lack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂缺损").toStdString()][QString::fromLocal8Bit("最大灰度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea1_Lack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂缺损").toStdString()][QString::fromLocal8Bit("筛选圆最小面积").toStdString()]["value"].as<double>();

	checkparam.i_RadiusMove_Lack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂缺损").toStdString()][QString::fromLocal8Bit("最小内接圆半径增量").toStdString()]["value"].as<double>();
	checkparam.i_MinArea2_Lack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂缺损").toStdString()][QString::fromLocal8Bit("筛选缺陷最小面积").toStdString()]["value"].as<double>();
	checkparam.d_Circularity_Lack = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂缺损").toStdString()][QString::fromLocal8Bit("筛选缺陷最小圆度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_PillDirty = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂脏污/黑点").toStdString()][QString::fromLocal8Bit("最小面积").toStdString()]["value"].as<double>();
	checkparam.d_Circularity_PillDirty = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_片剂脏污/黑点").toStdString()][QString::fromLocal8Bit("最小圆度").toStdString()]["value"].as<double>();
	checkparam.i_Channel1_ShadowDefect = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域缺陷").toStdString()][QString::fromLocal8Bit("颜色通道1").toStdString()]["value"].as<double>();
	checkparam.i_Channel2_ShadowDefect = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域缺陷").toStdString()][QString::fromLocal8Bit("颜色通道2").toStdString()]["value"].as<double>();
	checkparam.i_MaxGray_ShadowDefect = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域缺陷").toStdString()][QString::fromLocal8Bit("最大灰度").toStdString()]["value"].as<double>();
	checkparam.i_MinArea_ShadowDefect = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_阴影区域缺陷").toStdString()][QString::fromLocal8Bit("最小面积").toStdString()]["value"].as<double>();
	checkparam.i_Channel1_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("颜色通道1").toStdString()]["value"].as<double>();
	checkparam.i_Channel2_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("颜色通道2").toStdString()]["value"].as<double>();
	checkparam.i_Channel3_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("颜色通道3").toStdString()]["value"].as<double>();
	checkparam.d_Erosion_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("圆腐蚀参数").toStdString()]["value"].as<double>();
	checkparam.i_MaxGray_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("最大灰度").toStdString()]["value"].as<double>();
	checkparam.d_Opening1_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("开运算参数1").toStdString()]["value"].as<double>();
	checkparam.i_MinArea1_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("最小面积1").toStdString()]["value"].as<double>();
	checkparam.i_MinGray_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("最小灰度").toStdString()]["value"].as<double>();
	checkparam.d_Opeing2_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("开运算参数2").toStdString()]["value"].as<double>();
	checkparam.d_Closing_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("闭运算参数").toStdString()]["value"].as<double>();
	checkparam.i_MinArea2_PressedBubble = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_压泡").toStdString()][QString::fromLocal8Bit("最小面积2").toStdString()]["value"].as<double>();
	checkparam.i_Channel1_Folds = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_皱板").toStdString()][QString::fromLocal8Bit("颜色通道1").toStdString()]["value"].as<double>();
	checkparam.i_Channel2_Folds = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_皱板").toStdString()][QString::fromLocal8Bit("颜色通道2").toStdString()]["value"].as<double>();
	checkparam.i_Length_Folds = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_皱板").toStdString()][QString::fromLocal8Bit("最小长度").toStdString()]["value"].as<double>();
	checkparam.d_Circularity_Folds = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_皱板").toStdString()][QString::fromLocal8Bit("最大圆度").toStdString()]["value"].as<double>();
	checkparam.i_Channel_Unclear = _param[checkparam.c_CameraName][QString::fromLocal8Bit("Error_网纹不清").toStdString()][QString::fromLocal8Bit("颜色通道").toStdString()]["value"].as<double>();
	return true;
}
bool LoadCheckParam(CHECKPARAM *checkparam, QString* str)
{
	YAML::Node _param;
	if (nullptr == str)
	{
		_param = YAML::LoadFile(QString(AppPath + "/DefaultModel/CheckParam.yaml").toStdString());
		QSettings configIniRead(AppPath + "\\DefaultModel\\CheckParam.ini", QSettings::IniFormat);
		configIniRead.setIniCodec("UTF-8");
		QString cameraname = checkparam->c_CameraName;
	}
	else
	{
		_param = YAML::LoadFile(str->toStdString().c_str());
	}

	if (!_param[checkparam->c_CameraName].IsDefined())
	{
		return false;
	}
	NodeToParam(*checkparam, _param);



	// 	//读取检测参数配置
	// 	//////////////////////////////////////////////////////////////////////////
	// 	//checkparam->i_BandArea = configIniRead.value("/" + cameraname + "/BandArea", "865000").toInt();
	// 	//checkparam->i_BandException1_Area = configIniRead.value("/" + cameraname + "/BandException1_Area", "300").toInt();
	// 	//checkparam->i_BandException2_Value = configIniRead.value("/" + cameraname + "/BandException2_Value", "110").toInt();
	// 	//checkparam->i_BandException3_Area = configIniRead.value("/" + cameraname + "/BandException3_Area", "50").toInt();
	// 	//checkparam->i_InterException_Area = configIniRead.value("/" + cameraname + "/InterException_Area", "50").toInt();
	// 	//checkparam->d_InterException_Open = configIniRead.value("/" + cameraname + "/InterException_Open", "1.5").toDouble();
	// 	//checkparam->i_PillRadius = configIniRead.value("/" + cameraname + "/PillRadius", "85").toInt();
	// 	//checkparam->i_BandException2_Radius = configIniRead.value("/" + cameraname + "/BandException2_Radius", "0").toInt();
	// 	
	// 	checkparam->i_BandChannel = configIniRead.value("/" + cameraname + "/BandChannel", "0").toInt();
	// 	checkparam->i_MinGray_Band = configIniRead.value("/" + cameraname + "/MinGray_Band", "100").toInt();
	// 	checkparam->i_MinArea_Band = configIniRead.value("/" + cameraname + "/MinArea_Band", "680000").toInt();
	// 	checkparam->i_CapsulesChannel = configIniRead.value("/" + cameraname + "/CapsulesChannel", "4").toInt();
	// 	checkparam->i_MinGray_Capsules = configIniRead.value("/" + cameraname + "/MinGray_Capsules", "70").toInt();
	// 	checkparam->d_OR_Capsules = configIniRead.value("/" + cameraname + "/OR_Capsules", "2.5").toDouble();
	// 	checkparam->i_MinArea_Capsules = configIniRead.value("/" + cameraname + "/MinArea_Capsules", "10000").toInt();
	// 	checkparam->i_MaxArea_Capsules = configIniRead.value("/" + cameraname + "/MaxArea_Capsules", "99999").toInt();
	// 	checkparam->i_Number_Capsules = configIniRead.value("/" + cameraname + "/Number_Capsules", "10").toInt();
	// 	checkparam->i_BatchChannel = configIniRead.value("/" + cameraname + "/BatchChannel", "0").toInt();
	// 	checkparam->i_LengthAdd_1 = configIniRead.value("/" + cameraname + "/LengthAdd_1", "30").toInt();
	// 	checkparam->i_LengthAdd_2 = configIniRead.value("/" + cameraname + "/LengthAdd_2", "1000").toInt();
	// 	checkparam->d_ER_Batch = configIniRead.value("/" + cameraname + "/ER_Batch", "10").toDouble();
	// 	checkparam->i_MaskHeight_Batch = configIniRead.value("/" + cameraname + "/MaksHeight_Batch", "11").toInt();
	// 	checkparam->i_MaskWidth_Batch = configIniRead.value("/" + cameraname + "/MaksWidth_Batch", "11").toInt();
	// 	checkparam->i_MinGray_Batch = configIniRead.value("/" + cameraname + "/MinGray_Batch", "15").toInt();
	// 	checkparam->d_OR_Batch = configIniRead.value("/" + cameraname + "/OR_Batch", "1").toDouble();
	// 	checkparam->d_CR_Batch = configIniRead.value("/" + cameraname + "/CR_Batch", "5").toDouble();
	// 	checkparam->i_MinArea_Batch = configIniRead.value("/" + cameraname + "/MinArea_Batch", "150").toInt();
	// 	checkparam->i_MaxArea_Batch = configIniRead.value("/" + cameraname + "/MaxArea_Batch", "99999").toInt();
	// 	//checkparam->i_MinArea_BatchDefect = configIniRead.value("/" + cameraname + "/MinArea_BatchDefect", "50").toInt();
	// 	//checkparam->i_MaxArea_BatchDefect = configIniRead.value("/" + cameraname + "/MaxArea_BatchDefect", "99999").toInt();
	// 	checkparam->i_Channel_CapsulesDefect = configIniRead.value("/" + cameraname + "/Channel_CapsulesDefect", "1").toInt();
	// 	checkparam->d_ER_RegionCapsules = configIniRead.value("/" + cameraname + "/ER_RegionCapsules", "12.5").toDouble();
	// 	checkparam->i_MaxGray_CapsulesRed = configIniRead.value("/" + cameraname + "/MaxGray_CapsulesRed", "128").toInt();
	// 	checkparam->i_MinArea_CapsulesRed = configIniRead.value("/" + cameraname + "/MinArea_CapsulesRed", "1500").toInt();
	// 	checkparam->i_MaxArea_CapsulesRed = configIniRead.value("/" + cameraname + "/MaxArea_CapsulesRed", "99999").toInt();
	// 	checkparam->d_DR_CapsulesRed = configIniRead.value("/" + cameraname + "/DR_CapsulesRed", "3").toDouble();
	// 	checkparam->d_DR_CapsulesYellow = configIniRead.value("/" + cameraname + "/DR_CapsulesYellow", "3").toDouble();
	// 	checkparam->i_Channel_RegionRed = configIniRead.value("/" + cameraname + "/Channel_RegionRed", "4").toInt();
	// 	checkparam->i_MaskHeight_RegionRed = configIniRead.value("/" + cameraname + "/MaksHeight_RegionRed", "11").toInt();
	// 	checkparam->i_MaskWidth_RegionRed = configIniRead.value("/" + cameraname + "/MaksWidth_RegionRed", "11").toInt();
	// 	checkparam->i_MinGray_RegionRed = configIniRead.value("/" + cameraname + "/MinGray_RegionRed", "60").toInt();
	// 	checkparam->i_MaxGray_RegionRed = configIniRead.value("/" + cameraname + "/MaxGray_RegionRed", "100").toInt();
	// 	checkparam->d_CR_RegionRed = configIniRead.value("/" + cameraname + "/CR_RegionRed", "1").toDouble();
	// 	checkparam->d_OR_RegionRed = configIniRead.value("/" + cameraname + "/OR_RegionRed", "1").toDouble();
	// 	checkparam->i_MinArea_RegionRed = configIniRead.value("/" + cameraname + "/MinArea_RegionRed", "20").toInt();
	// 	checkparam->i_MaxArea_RegionRed = configIniRead.value("/" + cameraname + "/MaxArea_RegionRed", "2000").toInt();
	// 	checkparam->i_Channel_RedDefect1 = configIniRead.value("/" + cameraname + "/Channel_RedDefect1", "5").toInt();
	// 	checkparam->i_Channel_RedDefect2 = configIniRead.value("/" + cameraname + "/Channel_RedDefect2", "4").toInt();
	// 	checkparam->i_MaskHeight_RedDefect = configIniRead.value("/" + cameraname + "/MaksHeight_RedDefect", "5").toInt();
	// 	checkparam->i_MaskWidth_RedDefect = configIniRead.value("/" + cameraname + "/MaksWidth_RedDefect", "5").toInt();
	// 	checkparam->i_MinGray_RedDefect = configIniRead.value("/" + cameraname + "/MinGray_RedDefect", "35").toInt();
	// 	checkparam->d_ER_RedDefect = configIniRead.value("/" + cameraname + "/ER_RedDefect", "3.5").toDouble();
	// 	checkparam->d_OR_RedDefect = configIniRead.value("/" + cameraname + "/OR_RedDefect", "1.5").toDouble();
	// 	checkparam->i_MinArea_RedDefect = configIniRead.value("/" + cameraname + "/MinArea_RedDefect", "20").toInt();
	// 	checkparam->i_MaxArea_RedDefect = configIniRead.value("/" + cameraname + "/MaxArea_RedDefect", "99999").toInt();
	// 	checkparam->i_MaskHeight_RegionYellow = configIniRead.value("/" + cameraname + "/MaskHeight_RegionYellow", "5").toInt();
	// 	checkparam->i_MaskWidth_RegionYellow = configIniRead.value("/" + cameraname + "/MaskWidth_RegionYellow", "5").toInt();
	// 	checkparam->i_MinGray_RegionYellow = configIniRead.value("/" + cameraname + "/MinGray_RegionYellow", "20").toInt();
	// 	checkparam->i_MaxGray_RegionYellow = configIniRead.value("/" + cameraname + "/MaxGray_RegionYellow", "185").toInt();
	// 	checkparam->d_OR_RegionYellow = configIniRead.value("/" + cameraname + "/OR_RegionYellow", "1.5").toDouble();
	// 	checkparam->i_MinArea_RegionYellow = configIniRead.value("/" + cameraname + "/MinArea_RegionYellow", "20").toInt();
	// 	checkparam->i_MaxArea_RegionYellow = configIniRead.value("/" + cameraname + "/MaxArea_RegionYellow", "99999").toInt();
	// 	checkparam->i_Channel_BandDefect = configIniRead.value("/" + cameraname + "/Channel_BandDefect", "2").toInt();
	// 	checkparam->d_DR_BandDefect = configIniRead.value("/" + cameraname + "/DR_BandDefect", "3").toDouble();
	// 	checkparam->d_ER_BandDefect = configIniRead.value("/" + cameraname + "/ER_BandDefect", "5").toDouble();
	// 	checkparam->i_MaskHeight_BandDefect = configIniRead.value("/" + cameraname + "/MaskHeight_BandDefect", "3").toInt();
	// 	checkparam->i_MaskWidth_BandDefect = configIniRead.value("/" + cameraname + "/MaskWidth_BandDefect", "3").toInt();
	// 	checkparam->i_MinGray_BandDefect = configIniRead.value("/" + cameraname + "/MinGray_BandDefect", "45").toInt();
	// 	checkparam->d_CR_BandDefect = configIniRead.value("/" + cameraname + "/CR_BandDefect", "1.5").toDouble();
	// 	checkparam->i_MinWidth_BandDefect = configIniRead.value("/" + cameraname + "/MinWidth_BandDefect", "1").toInt();
	// 	checkparam->i_MaxWidth_BandDefect = configIniRead.value("/" + cameraname + "/MaxWidth_BandDefect", "100").toInt();
	// 	checkparam->i_MinHeight_BandDefect = configIniRead.value("/" + cameraname + "/MinHeight_BandDefect", "3").toInt();
	// 	checkparam->i_MaxHeight_BandDefect = configIniRead.value("/" + cameraname + "/MaxHeight_BandDefect", "100").toInt();
	// 	checkparam->i_MinArea_BandDefect = configIniRead.value("/" + cameraname + "/MinArea_BandDefect", "10").toInt();
	// 	checkparam->i_MaxArea_BandDefect = configIniRead.value("/" + cameraname + "/MaxArea_BandDefect", "9999").toInt();
	// 	checkparam->b_IsOpen = configIniRead.value("/" + cameraname + "/IsOpen", "false").toBool();
	// 	checkparam->i_Channel_InCapsules = configIniRead.value("/" + cameraname + "/Channel_InCapsules", "2").toInt();
	// 	checkparam->d_ER_InCapsules = configIniRead.value("/" + cameraname + "/ER_InCapsules", "7.5").toDouble();
	// 	checkparam->i_MaxGray_InCapsules = configIniRead.value("/" + cameraname + "/MaxGray_InCapsules", "120").toInt();
	// 	checkparam->d_OR_InCapsules = configIniRead.value("/" + cameraname + "/OR_InCapsules", ".5").toDouble();
	// 	checkparam->i_MinArea_InCapsules = configIniRead.value("/" + cameraname + "/MinArea_InCapsules", "2000").toInt();
	// 	checkparam->i_MaxArea_InCapsules = configIniRead.value("/" + cameraname + "/MaxArea_InCapsules", "99999").toInt();
	// 	checkparam->i_LengthPlus_1 = configIniRead.value("/" + cameraname + "/LengthPlus_1", "70").toInt();
	// 	checkparam->i_LengthPlus_2 = configIniRead.value("/" + cameraname + "/LengthPlus_1", "70").toInt();
	// 	checkparam->i_Channel_Middle = configIniRead.value("/" + cameraname + "/Channel_Middle", "0").toInt();
	// 	checkparam->i_MaskHeight_Middle = configIniRead.value("/" + cameraname + "/MaskHeight_Middle", "7").toInt();
	// 	checkparam->i_MaskWidth_Middle = configIniRead.value("/" + cameraname + "/MaskWidth_Middle", "7").toInt();
	// 	checkparam->i_MinGray_Middle = configIniRead.value("/" + cameraname + "/MinGray_Middle", "45").toInt();
	// 	checkparam->i_MinWidth_Middle = configIniRead.value("/" + cameraname + "/MinWidth_Middle", "5").toInt();
	// 	checkparam->i_MinHeight_Middle = configIniRead.value("/" + cameraname + "/MinHeight_Middle", "3").toInt();
	// 	checkparam->d_CR_Middle = configIniRead.value("/" + cameraname + "/CR_Middle", "2.5").toDouble();
	// 	checkparam->i_MinArea_Middle = configIniRead.value("/" + cameraname + "/MinArea_Middle", "30").toInt();
	// 	checkparam->i_Channel_Plate = configIniRead.value("/" + cameraname + "/Channel_Plate", "0").toInt();
	// 	checkparam->i_MaxGray_Plate = configIniRead.value("/" + cameraname + "/MaxGray_Plate", "190").toInt();
	// 	checkparam->d_CR_Plate = configIniRead.value("/" + cameraname + "/CR_Plate", "1.5").toDouble();
	// 	checkparam->d_OR_Plate_1 = configIniRead.value("/" + cameraname + "/OR_Plate_1", "3.5").toDouble();
	// 	checkparam->i_MinArea_Plate_1 = configIniRead.value("/" + cameraname + "/MinArea_Middle_1", "1500").toInt();
	// 	checkparam->d_OR_Plate_2 = configIniRead.value("/" + cameraname + "/OR_Plate_2", "1.5").toDouble();
	// 	checkparam->i_MinArea_Plate_2 = configIniRead.value("/" + cameraname + "/MinArea_Middle_2", "30").toInt();






	return false;
}
QtGuiSetting::QtGuiSetting(QWidget *parent, void* AlgPointer)
	: QDialog(parent)
{
	//QString str = QString::number(0.5);
	//double c = str.toDouble();
	//double a = pow(10, 0 / 55);
	//c = c - a;
	//str = QString::number(c);
	b_GetAuthority = false;
	m_sSelectFile = "";
	m_strSaveFile = "";
	m_strTrainFile = "";
	m_bChanged = false;
	m_bRectChanged = false;
	m_timerReadList = nullptr;
	clickedtimer = nullptr;
	ui.setupUi(this);
// 	ui.tableWidget->setColumnCount(2);
// 	QStringList LIST;
// 	LIST << QString::fromLocal8Bit("项目") << QString::fromLocal8Bit("参数");
// 	ui.tableWidget->setHorizontalHeaderLabels(LIST);
// 	ui.tableWidget->setColumnWidth(1, 40);
// 	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
// 	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
// 	ui.tableWidget->viewport()->installEventFilter(this);
// 	ui.tableWidget->verticalHeader()->setDefaultSectionSize(35);//默认行高20
	pp = parent;
	HTuple hv_WindowID;
	open_window(ui.ShowLabel->y(), ui.ShowLabel->x(), ui.ShowLabel->width(), ui.ShowLabel->height(), (long)((QWidget*)ui.ShowLabel->parent())->winId(), "", "", &hv_WindowID);
	m_WND = hv_WindowID[0];
	ui.ShowLabel->setMouseTracking(true);
//	ui.tableWidget->setMouseTracking(true);
	ui.lw_ImageList->viewport()->installEventFilter(this);

	bool b = ui.lw_ImageList->hasMouseTracking();
	setMouseTracking(true);
	m_bButton = false;
	m_SAppPath = qApp->applicationDirPath();//exe文件夹
	m_sImageListPath = m_SAppPath;
	initImageLS(m_sImageListPath);
	connect(ui.lw_ImageList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onClickedImage(QListWidgetItem *)));
	//connect(ui.lw_ImageList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onSelectImageList(QListWidgetItem *)));
	connect(ui.pB_ContinueTest, SIGNAL(toggled(bool)), this, SLOT(onContinueTest(bool)));
	connect(ui.pB_Save, SIGNAL(clicked()), this, SLOT(onSaveParam()));
	connect(ui.pB_Exit, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "退出确认", "确认退出算法设置页面吗？", "确认", "取消"))
		{
			close();
		}});
	//设置check parent 算法显示位置
	connect(this, SIGNAL(SignShowImage(int, Mat, int)), this, SLOT(SLOTShowImage(int, Mat, int)));
	connect(ui.pB_Calibration, SIGNAL(clicked()), this, SLOT(onAutoDetest()));
	p_Parent = AlgPointer;
	((CInterCHeck*)p_Parent)->StartCheck("", nullptr, m_MOriginal.cols, m_MOriginal.rows);
	((CInterCHeck*)p_Parent)->SetShowCallBack(this, ShowCallBack);
//	QObject::connect(ui.tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(onCellChanged(int, int)));
	QDesktopWidget *dwsktopwidget = QApplication::desktop();
	QRect deskrect = dwsktopwidget->availableGeometry();
	QRect screenrect = dwsktopwidget->screenGeometry();
	if (deskrect.height() < 1100)
	{
		this->resize(768 * 2 + 5, 1366 / 2);
		ui.groupBox->move(ui.ShowLabel->pos().x() + ui.ShowLabel->width() + 2, ui.ShowLabel->pos().y());
		//ui.lw_ImageList->move(ui.tableWidget->pos().x() + ui.tableWidget->width() + 2, ui.tableWidget->pos().y());
	}
//	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onPopKeyboard(int, int)));
	connect(ui.treeWidget, &QMyTreeWidget::TempSave, [=](QString str)
	{
		ui.treeWidget->SaveYAMLFile(AppPath + "/Temp.yaml");

		CHECKPARAM _checkparam;
		strcpy(_checkparam.c_CameraName, _CameraName);
		NodeToParam(_checkparam, ui.treeWidget->_mparam);
		bool results = ((CInterCHeck*)p_Parent)->Check(m_MOriginal, nullptr, str);
		bool b = ((CInterCHeck*)p_Parent)->RealCheck(str, &_checkparam, m_WND,&str);
		m_bChanged = true;
	});
}
void QtGuiSetting::mouseReleaseEvent(QMouseEvent *p)
{
	setCursor(Qt::ArrowCursor);
	m_bButton = false;
	hideKeyBoard();


	return;
}
bool QtGuiSetting::eventFilter(QObject * watched, QEvent * event)
{
	if (watched == ui.lw_ImageList->viewport())
	{
		if (event->type() == QEvent::MouseButtonRelease)
		{
			hideKeyBoard();
		}
	}
	if (watched->metaObject()->className() == QStringLiteral("QComboBox"))
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			hideKeyBoard();
		}
	}
	return QWidget::eventFilter(watched, event);
}
void QtGuiSetting::resizeEvent(QResizeEvent * event)
{
	try
	{
		// 		Mat tempgray;
		// 		if (m_MOriginal.rows == 0 || m_MOriginal.cols == 0)
		// 		{
		// 			return;
		// 		}
		// 		cvtColor(m_MOriginal, tempgray, COLOR_BGR2RGB);
		// 		onShowImage(&tempgray);
		if (m_MOriginal.empty())
		{
			return;
		}
		Hobject ho_Image = Mat2Hobject(m_MOriginal);
		HTuple hv_Width, hv_Height;
		get_image_size(ho_Image, &hv_Width, &hv_Height);
		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		disp_obj(ho_Image, m_WND);
	}
	catch (...)
	{
		return;
	}
	return;
}
bool QtGuiSetting::isImage(QFileInfo& info)
{
	QString filename = info.fileName();
	if (filename == "." || filename == "..")
		return false;
	if (filename.contains("def") || filename.contains(".csv"))
		return false;
	return filename.contains(".bmp") || filename.contains(".BMP") ||
		filename.contains(".jpg") || filename.contains(".JPG") ||
		filename.contains(".jpeg") || filename.contains(".JPEG") ||
		filename.contains(".png") || filename.contains(".PNG") ||
		filename.contains(".tif") || filename.contains(".TIF");
}
bool QtGuiSetting::containImages(QDir& dir)
{
	foreach(QFileInfo entry, dir.entryInfoList(QDir::Dirs | QDir::Files)) {
		QString filename = entry.fileName();
		if (filename == "." || filename == "..")
			continue;
		if (entry.isDir() && containImages(QDir(entry.absoluteFilePath())))
			return true;
		if (entry.isFile() && isImage(entry))
			return true;
	}
	return false;
}
void QtGuiSetting::initImageLS(QString str)
{
	QDir dir(str);
	ui.lw_ImageList->clear();
	ui.lw_ImageList->addItem(".");
	ui.lw_ImageList->addItem("..");
	foreach(QFileInfo mfi, dir.entryInfoList(QDir::Dirs | QDir::Files, QDir::Time))
	{
		if (mfi.fileName() == "." || mfi.fileName() == "..")
			continue;
		if (mfi.isDir()) {
			if (containImages(QDir(mfi.absoluteFilePath())))
				ui.lw_ImageList->addItem(mfi.fileName());
			continue;
		}
		if (mfi.isFile() && isImage(mfi))
			ui.lw_ImageList->addItem(mfi.fileName());
	}
}
void QtGuiSetting::mousePressEvent(QMouseEvent *p)
{
	if (ui.ShowLabel->geometry().contains(this->mapFromGlobal(QCursor::pos())) && p->button() == Qt::LeftButton)
	{
		hideKeyBoard();
		HTuple hv_Width, hv_Height;
		if (m_MOriginal.empty())
		{
			return;
		}
		Hobject ho_Image = Mat2Hobject(m_MOriginal);
		get_image_size(ho_Image, &hv_Width, &hv_Height);
		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		disp_obj(ho_Image, m_WND);
		return;
	}
	return;
}
void QtGuiSetting::mouseMoveEvent(QMouseEvent *p)
{
	// 	QPoint poi = QCursor::pos();
	// 	if (ui.ShowLabel->geometry().contains(this->mapFromGlobal(poi)) && b_GetAuthority)
	// 	{
	// 		setCursor(Qt::CrossCursor);
	// 		m_PointFinale = p->pos();
	// 		if (m_bButton)
	// 		{
	// 			QRect rec = ui.ShowLabel->geometry();
	// 			QPoint pt = p->pos();
	// 			pt.setX(pt.x() - rec.x() - ui.ShowLabel->frameWidth());
	// 			pt.setY(pt.y() - rec.y() - ui.ShowLabel->frameWidth());
	// 			double scale_x = m_MOriginal.cols*1.0 / (rec.width() - ui.ShowLabel->frameWidth() * 2);
	// 			double scale_y = m_MOriginal.rows*1.0 / (rec.height() - ui.ShowLabel->frameWidth() * 2);
	// 			if (pt.x() > m_PointOriginal.x())
	// 			{
	// 				m_RectToDraw.setLeft(m_PointOriginal.x());
	// 				m_RectToDraw.setRight(pt.x());
	// 			}
	// 			else
	// 			{
	// 				m_RectToDraw.setRight(m_PointOriginal.x());
	// 				m_RectToDraw.setLeft(pt.x());
	// 			}
	// 			if (pt.y() > m_PointOriginal.y())
	// 			{
	// 				m_RectToDraw.setTop(m_PointOriginal.y());
	// 				m_RectToDraw.setBottom(pt.y());
	// 			}
	// 			else
	// 			{
	// 				m_RectToDraw.setBottom(m_PointOriginal.y());
	// 				m_RectToDraw.setTop(pt.y());
	// 			}
	// 			cv::Mat img2show;
	// 			cvtColor(m_MOriginal, img2show, COLOR_BGR2RGB);
	// 			m_RectOpencv = cv::Rect(m_RectToDraw.x()*scale_x, m_RectToDraw.y()*scale_y, m_RectToDraw.width()*scale_x, m_RectToDraw.height()*scale_y);
	// 			cv::rectangle(img2show, m_RectOpencv, cv::Scalar(255, 0, 0), 5, 8, 0);
	// 			onShowImage(&img2show);
	// 			m_bChanged = true;
	// 		}
	// 	}
	// 	else
	// 		setCursor(Qt::ArrowCursor);
	return;
}
QtGuiSetting::~QtGuiSetting()
{
}
void QtGuiSetting::SLOTShowImage(int pos, Mat img, int times)
{
	int zz = ui.ShowLabel->frameWidth();
	QSize ss = ui.ShowLabel->size();
	ss.setWidth(ss.width() - zz * 2);
	ss.setHeight(ss.height() - zz * 2);
	Mat imgsend;
	if (img.channels() == 1)	{		cv::cvtColor(img, imgsend, COLOR_GRAY2BGR);
	}
	else if (img.channels() == 3)
	{
		cv::cvtColor(img, imgsend, COLOR_BGR2RGB);
	}
	cv::resize(imgsend, imgsend, Size(ss.width(), ss.height()));
	QImage disImage = QImage((const unsigned char*)(imgsend.data), imgsend.cols, imgsend.rows, imgsend.step, QImage::Format_RGB888);
	ui.ShowLabel->setPixmap(QPixmap::fromImage(disImage));
}
void QtGuiSetting::onContinueTest(bool b)
{
	if (b)
	{
		if (nullptr == m_timerReadList)
		{
			m_timerReadList = new QTimer();
			connect(m_timerReadList, SIGNAL(timeout()), this, SLOT(onTimerReadList()));
			m_timerReadList->start(1000);
		}
	}
	else
	{
		if (nullptr != m_timerReadList)
		{
			m_timerReadList->stop();
			delete m_timerReadList;
			m_timerReadList = nullptr;
		}
	}
}
void QtGuiSetting::onTimerReadList()
{
	if (ui.lw_ImageList->count() < 3)
	{
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("连续测试错误"), QString::fromLocal8Bit("未定位到图像文件"));
		return;
	}
	int index_list = ui.lw_ImageList->currentRow();
	if (index_list == ui.lw_ImageList->count() - 1)
	{
		m_timerReadList->stop();
		delete m_timerReadList;
		m_timerReadList = nullptr;
		ui.pB_ContinueTest->setChecked(false);
		return;
	}
	if (index_list < 2)
	{
		ui.lw_ImageList->setCurrentRow(2);
		onSelectImageList(ui.lw_ImageList->currentItem());
	}
	else
	{
		ui.lw_ImageList->setCurrentRow(index_list + 1);
		onSelectImageList(ui.lw_ImageList->currentItem());
	}
}
void QtGuiSetting::onSaveParam()
{
	if (m_bChanged)
	{
		if (QMessageBox::Save == QMessageBox::question(this, QString::fromLocal8Bit("参数已修改"), QString::fromLocal8Bit("是否保存？"), QMessageBox::Save, QMessageBox::No))
		{
			QFile::remove(QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml"));
			QFile::copy(QString(qApp->applicationDirPath() + "/Temp.yaml"), QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml"));
			emit ShouldSaveParam();
			m_bChanged = false;
			ui.pB_Save->setEnabled(false);
		}
	}
}
Hobject QtGuiSetting::Mat2Hobject(Mat& image)
{
	Hobject Hobj = Hobject();
	int hgt = image.rows;
	int wid = image.cols;
	int i;
	//	CV_8UC3
	if (image.empty())
		return Hobject();
	if (image.type() == CV_8UC3)
	{
		split(image, imgchannel);
		Mat imgB = imgchannel[0];
		Mat imgG = imgchannel[1];
		Mat imgR = imgchannel[2];
		if (dataR == NULL)
		{
			dataR = new uchar[hgt*wid];
		}
		if (dataG == NULL)
		{
			dataG = new uchar[hgt*wid];
		}
		if (dataB == NULL)
		{
			dataB = new uchar[hgt*wid];
		}
		for (i = 0; i < hgt; i++)
		{
			memcpy(dataR + wid * i, imgR.data + imgR.step*i, wid);
			memcpy(dataG + wid * i, imgG.data + imgG.step*i, wid);
			memcpy(dataB + wid * i, imgB.data + imgB.step*i, wid);
		}
		gen_image3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
	}
	//	CV_8UCU1
	else if (image.type() == CV_8UC1)
	{
		uchar* data = new uchar[hgt*wid];
		for (i = 0; i < hgt; i++)
			memcpy(data + wid * i, image.data + image.step*i, wid);
		gen_image1(&Hobj, "byte", wid, hgt, (Hlong)data);
		delete[] data;
		data = NULL;
	}
	return Hobj;
}
Mat QtGuiSetting::Hobject2Mat(Hobject Hobj)
{
	HTuple htCh = HTuple();
	HTuple cType;
	Mat Image;
	convert_image_type(Hobj, &Hobj, "byte");
	count_channels(Hobj, &htCh);
	HTuple wid;
	HTuple hgt;
	int W, H;
	if (htCh[0].I() == 1)
	{
		HTuple ptr;
		get_image_pointer1(Hobj, &ptr, &cType, &wid, &hgt);
		W = (Hlong)wid[0].I();
		H = (Hlong)hgt[0].I();
		Image.create(H, W, CV_8UC1);
		uchar* pdata = (uchar*)ptr[0].I();
		memcpy(Image.data, pdata, W*H);
	}
	else if (htCh[0].I() == 3)
	{
		HTuple ptrR, ptrG, ptrB;
		get_image_pointer3(Hobj, &ptrR, &ptrG, &ptrB, &cType, &wid, &hgt);
		W = (Hlong)wid[0].I();
		H = (Hlong)hgt[0].I();
		Image.create(H, W, CV_8UC3);
		vector<Mat> vecM(3);
		vecM[2].create(H, W, CV_8UC1);
		vecM[1].create(H, W, CV_8UC1);
		vecM[0].create(H, W, CV_8UC1);
		uchar* pr = (uchar*)ptrR[0].I();
		uchar* pg = (uchar*)ptrG[0].I();
		uchar* pb = (uchar*)ptrB[0].I();
		memcpy(vecM[2].data, pr, W*H);
		memcpy(vecM[1].data, pg, W*H);
		memcpy(vecM[0].data, pb, W*H);
		merge(vecM, Image);
	}
	return Image;
}
void QtGuiSetting::onAutoDetest()
{
}

void QtGuiSetting::onSelectImageChannel(int channels)
{
	QComboBox* obj = qobject_cast<QComboBox*>(sender());//判断是哪个按钮触发了槽函数
	// Local iconic variables 
	Hobject  ho_Image1, ho_Image2, ho_Image3;
	Hobject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
	Hobject  ho_ImageSub2, ho_EmptyRegion_Out, ho_EmptyRegion_OCR;
	Hobject  ho_EmptyRegion_Top, ho_EmptyRegion_Inner, ho_EmptyRegion_TopUP;
	Hobject  ho_EmptyRegion_Intensity, ho_ImageMax2, ho_Regions;
	Hobject  ho_Regions_Leak, ho_Region, ho_RegionOpening2, ho_RegionOpening;
	Hobject  ho_ConnectedRegions2, ho_SelectedRegions1, ho_RegionOpening_CAPOOT;
	Hobject  ho_ImageReduced, ho_ImageMax, ho_ImageSub, ho_Region1;
	Hobject  ho_RegionClosing, ho_Regions_Convex, ho_Regionx;
	Hobject  ho_Region_1st, ho_Regionsx, ho_Regions_2nd, ho_Region_OCR;
	Hobject  ho_Rectangle, ho_RegionIntersection5, ho_ConnectedRegions;
	Hobject  ho_SelectedRegions, ho_RegionOpening8, ho_RegionTrans2;
	Hobject  ho_RegionIntersection, ho_RegionErosion, ho_RegionIntersection4;
	Hobject  ho_Regions_LeakTOPError, ho_RegionErosion3, ho_RegionIntersection3;
	Hobject  ho_RegionOpening5, ho_Rectangle2, ho_RegionIntersection7;
	Hobject  ho_ConnectedRegions4, ho_SelectedRegions5, ho_RegionOpening9;
	Hobject  ho_RegionTrans3, ho_Rectangle1, ho_RegionIntersection8;
	Hobject  ho_RegionErosion1, ho_RegionIntersection2, ho_Regions_LeakTOPError2;
	Hobject  ho_RegionErosion6, ho_RegionIntersection9, ho_RegionOpening10;
	// Local control variables 
	HTuple  hv_ImageFiles, hv_Index, hv_Pointer, hv_Type;
	HTuple  hv_Width, hv_Height, hv_Tuple_Error, hv_Co_Index;
	HTuple  hv_Indices, hv_C_s, hv_R_s, hv_R_sEnd, hv_Area9;
	HTuple  hv_Row13, hv_Column13, hv_Row1, hv_Column1, hv_Row2;
	HTuple  hv_Column2, hv_Height_CAP, hv_Row14, hv_Column14;
	HTuple  hv_Row23, hv_Column23, hv_H1, hv_Row15, hv_Column15;
	HTuple  hv_Row24, hv_Column24, hv_H2, hv_h, hv_Area8, hv_Row10;
	HTuple  hv_Column10, hv_Row11, hv_Column11, hv_Row21, hv_Column21;
	HTuple  hv_Mean, hv_Deviation, hv_Area5, hv_Row7, hv_Column7;
	HTuple  hv_Row12, hv_Column12, hv_Row22, hv_Column22, hv_Mean2;
	HTuple  hv_Deviation2, hv_Area7, hv_Row9, hv_Column9, hv_Area2;
	HTuple  hv_Row4, hv_Column4, hv_Area3, hv_Row5, hv_Column5;
	HTuple  hv_Area, hv_Row, hv_Column, hv_Max1, hv_Max;
	if (m_MOriginal.empty())
	{
		return;
	}
	Hobject ho_Image = Mat2Hobject(m_MOriginal);
	decompose3(ho_Image, &ho_Image1, &ho_Image2, &ho_Image3);
	trans_from_rgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2, &ho_ImageResult3, "hsv");
	get_image_size(ho_Image, &hv_Width, &hv_Height);
	set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
	if (obj->objectName() == "BandChannel")
	{
		//_checkparam.i_BandChannel = channels;
	}
	// 	if (obj->objectName() == "1stChannel")
	// 	{
	// 		_checkparam.i_1stChannel = channels;
	// 	}
	// 	if (obj->objectName() == "2ndChannel")
	// 	{
	// 		_checkparam.i_2ndChannel = channels;
	// 	}
	switch (channels)
	{
	case 0:
		disp_obj(ho_Image1, m_WND);
		break;
	case 1:
		disp_obj(ho_Image2, m_WND);
		break;
	case 2:
		disp_obj(ho_Image3, m_WND);
		break;
	case 3:
		disp_obj(ho_ImageResult1, m_WND);
		break;
	case 4:
		disp_obj(ho_ImageResult2, m_WND);
		break;
	case 5:
		disp_obj(ho_ImageResult3, m_WND);
		break;
	default:
		break;
	}
	m_bChanged = true;
}
void QtGuiSetting::onPopKeyboard(int, int)
{
// 	if (nullptr == dlg_keyboard)
// 	{
// 		//QMessageBox::warning(nullptr, "", "123");
// 		dlg_keyboard = new QDialog(this);
// 		dlg_keyboard->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
// 		dlg_keyboard->resize(QSize(100, 50));
// 		QPushButton* pb = nullptr;
// 		int w = 50;
// 		for (int i = 0; i < 2; i++)
// 		{
// 			pb = new QPushButton(dlg_keyboard);
// 			if (i == 0) { pb->setObjectName("-1"); pb->setText("-1"); };
// 	/*		if (i == 1) { pb->setObjectName("-0.5"); pb->setText("-0.5"); };
// 			if (i == 2) { pb->setObjectName("0.5");  pb->setText("+0.5"); };*/
// 			if (i == 1) { pb->setObjectName("1");  pb->setText("+1"); };
// 			pb->resize(50, 50);
// 			pb->move(50 * i, 0);
// 			pb->setCheckable(true);
// 			//connect(pb, SIGNAL(toggled(bool)), this, SLOT(onbtnchange(bool)));
// 			connect(pb, SIGNAL(pressed()), this, SLOT(onbtnpressed()));
// 			connect(pb, SIGNAL(released()), this, SLOT(onbtnreleased()));
// 
// 		}
// 	}
// 	QPoint p = this->cursor().pos();
// 
// //	QPoint q = ui.tableWidget->mapToGlobal(QPoint(0, 0));
// 	dlg_keyboard->show();
// 	//QMessageBox::warning(nullptr, "", QString::number(q.x() + ui.tableWidget->width()) +","+ QString::number(p.y()));
// //	dlg_keyboard->move(QPoint(q.x() + ui.tableWidget->width(), p.y()));
// 
// 	//QPoint p = this->cursor().pos();
// 	//QPoint q = (ui.tableWidget->pos());
// 	//dlg_keyboard->show();
// 	//dlg_keyboard->move(p.x(), p.y());
// 	//dlg_keyboard->move(mapToParent( QPoint(q.x() + /*3 * */ui.tableWidget->width(), p.y())));
// 	//dlg->move(QPoint(100, 100));
}
void QtGuiSetting::onbtnpressed()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	double d = btn->objectName().toDouble();
	bool b_checked = btn->isChecked();
	if (nullptr==m_timerChanger)
	{
		m_timerChanger = new QTimer();
		connect(m_timerChanger, SIGNAL(timeout()), this, SLOT(ontimechanger()));
		m_timerChanger->start(100);
	}
	if (1 == d)
	{
		/*QTableWidgetItem *item = ui.tableWidget->currentItem();
		item->setText(QString::number(item->text().toInt() + 1));*/
		m_ba1 = true;

	}
	if (0.5 == d)
	{
	/*	QTableWidgetItem *item = ui.tableWidget->currentItem();
		item->setText(QString::number(item->text().toDouble() + 0.5));*/
		//m_ba05 = true;
	}
	if (-0.5 == d)
	{
		/*QTableWidgetItem *item = ui.tableWidget->currentItem();
		item->setText(QString::number(item->text().toDouble() - 0.5));*/
		//m_bm05 = true;
	}
	if (-1 == d)
	{
		/*	QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toInt() - 1));*/
		m_bm1 = true;
	}
	
	//if (b_checked)
	//{
	//	btn->setChecked(false);
		
	//}
}
void QtGuiSetting::onbtnreleased()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	double d = btn->objectName().toDouble();
	bool b_checked = btn->isChecked();
	{
		btn->setChecked(false);
	if (b_checked)
	 
		if (nullptr!=m_timerChanger)
		{
			m_timerChanger->stop();
			delete m_timerChanger;
			m_timerChanger = nullptr;
		}
		m_itimechange = 0;
		m_itimestep = 5;
		m_bm1 = false;
		m_bm05 = false;
		m_ba05 = false;
		m_ba1 = false;
	
		/*if (1 == d)
		{
			QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toInt() + 1));

		}
		if (0.5 == d)
		{
			QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toDouble() + 0.5));
		}
		if (-0.5 == d)
		{
			QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toDouble() - 0.5));
		}
		if (-1 == d)
		{
			QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toInt() - 1));
		}*/
	}
}

void QtGuiSetting::ontimechanger()
{
// 	try
// 	{
// 		if (m_itimechange%m_itimestep == 0)
// 		{
// 			if (m_ba1)
// 			{
// 				QTableWidgetItem *item = ui.tableWidget->currentItem();
// 				if (0>item->text().toDouble())
// 				{
// 					item->setText("0");
// 				}
// 				if (m_itimechange < 24)
// 				{
// 					/*item->setText(QString::number(item->text().toDouble() + pow(10, m_itimechange++%m_itimestep-1)));*/
// 				}
// 				else
// 				{
// 					m_itimestep = 2;
// 					//ui.tableWidget->blockSignals(true);
// 				}
// 				item->setText(QString::number(item->text().toDouble() + pow(10, m_itimechange / 55)));
// 
// 				//else if (m_itimechange < 100)
// 				//{
// 				//	//m_itimestep = 2;
// 				//	item->setText(QString::number(item->text().toDouble() + 10));
// 				//}
// 			}
// 			if (m_ba05)
// 			{
// 				QTableWidgetItem *item = ui.tableWidget->currentItem();
// 				item->setText(QString::number(item->text().toDouble() + 0.5));
// 			}
// 			if (m_bm05)
// 			{
// 				QTableWidgetItem *item = ui.tableWidget->currentItem();
// 				item->setText(QString::number(item->text().toDouble() - 0.5));
// 			}
// 			if (m_bm1)
// 			{
// 				QTableWidgetItem *item = ui.tableWidget->currentItem();
// 				if (m_itimechange < 24)
// 				{
// 					/*item->setText(QString::number(item->text().toDouble() + pow(10, m_itimechange++%m_itimestep-1)));*/
// 				}
// 				else
// 				{
// 					m_itimestep = 2;
// 					//ui.tableWidget->blockSignals(true);
// 
// 				}
// 				item->setText(QString::number(item->text().toDouble() - pow(10, m_itimechange / 55)));
// 			
// 			}
// 		}
// 	}
// 	catch (Exception e)
// 	{
// 		QMessageBox::warning(nullptr, "", e.what());
// 	}
// 		
// 
// 	
// 	m_itimechange++;
// 	ui.tableWidget->blockSignals(false);
}

void QtGuiSetting::hideKeyBoard()
{
	if (nullptr != dlg_keyboard)
	{
		if (!dlg_keyboard->isHidden())
		{
			dlg_keyboard->hide();
		}
	}
}
void QtGuiSetting::SetParam(char* c_CameraName)
{
	m_node = YAML::LoadFile(QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml").toStdString());
	strcpy(_CameraName, c_CameraName);
	ui.treeWidget->ReadYAMLFile(m_node,c_CameraName);
}
void QtGuiSetting::onCellChanged(int r, int c)
{
}
void QtGuiSetting::SetModelMat(Mat tempgray)
{
	if (tempgray.empty())
	{
		m_MOriginal=imread((qApp->applicationDirPath() + "/DefaultModel/StandTemp.bmp").toStdString());
	}
	else
	{
		tempgray.copyTo(m_MOriginal);
	}
}
void QtGuiSetting::closeEvent(QCloseEvent *)
{
	close_window(HTuple(m_WND));
	onSaveParam();
}
void QtGuiSetting::onBtnGetImage()
{
	return;
}
void QtGuiSetting::m_TestApplyParam()
{
	return;
}
void QtGuiSetting::onBtnGetSample()
{
	return;
}
void QtGuiSetting::onShowImage(Mat* tempgray)
{
	try
	{
		Mat temp;
		tempgray->copyTo(temp);
		int zz = ui.ShowLabel->frameWidth();
		QSize ss = ui.ShowLabel->size();
		ss.setWidth(ss.width() - zz * 2);
		ss.setHeight(ss.height() - zz * 2);
		cv::resize(temp, temp, Size(ss.width(), ss.height()));
		disImage = QImage((const unsigned char*)(temp.data), temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
		ui.ShowLabel->setPixmap(QPixmap::fromImage(disImage));
	}
	catch (cv::Exception e)
	{
		return;
	}
	return;
}
void QtGuiSetting::onShowFps(int i)
{
}
extern bool compColx(const Rect &a, const Rect &b);
extern bool compColy(const Rect &a, const Rect &b);
void QtGuiSetting::onSelectTrainFile()
{
}
void QtGuiSetting::onClickedImage(QListWidgetItem *item)
{
	if (nullptr == clickedtimer)
	{
		clickedtimer = new QTimer();
		clickedtimer->setSingleShot(true);
		bool b = connect(clickedtimer, SIGNAL(timeout()), this, SLOT(onSingleClicked()));
		clickedtimer->start(500);
		return;
	}
	else
	{
		clickedtimer->stop();
		delete clickedtimer;
		clickedtimer = nullptr;
	}
	onSelectImageList(item);
}
void QtGuiSetting::onSingleClicked()
{
	if (nullptr != clickedtimer)
	{
		clickedtimer->stop();
		delete clickedtimer;
		clickedtimer = nullptr;
	}
	QString pathselect;
	QListWidgetItem *item = ui.lw_ImageList->currentItem();
	QString sSelectItem = item->text();
	pathselect = m_sImageListPath + "/" + sSelectItem;
	m_currentImagePath = pathselect;
	QFileInfo file(pathselect);
	if (!file.isFile() && m_timerReadList == nullptr)
	{
	}
	else
	{
		m_MOriginal = imread(m_currentImagePath.toStdString().c_str());
		if (m_MOriginal.rows > 0)
		{
			HTuple hv_W, hv_H;
			Hobject ho_Image = Mat2Hobject(m_MOriginal);
			get_image_size(ho_Image, &hv_W, &hv_H);
			set_part(m_WND, 0, 0, hv_H - 1, hv_W - 1);
			disp_obj(ho_Image, m_WND);
		}
	}
}
void QtGuiSetting::onSelectImageList(QListWidgetItem *item)
{
	//&&m_timerReadList==nullptr 防止进入深一层目录
	QString sSelectItem = item->text();
	if (sSelectItem == "."&&m_timerReadList == nullptr)
	{
		QString newPath = m_sImageListPath.left(m_sImageListPath.indexOf("/") + 1);
		m_sImageListPath = newPath;
		initImageLS(m_sImageListPath);
		return;
	}
	if (sSelectItem == ".."&&m_timerReadList == nullptr)
	{
		QString newPath = m_sImageListPath.left(m_sImageListPath.lastIndexOf("/"));
		//exe所在的根目录
		if (newPath.length() > 2)
		{
			// only allow user to access the data in dataPath
			m_sImageListPath = newPath;
		}
		if (newPath.length() == 2)
		{
			// only allow user to access the data in dataPath
			m_sImageListPath = newPath + "/";
		}
		initImageLS(m_sImageListPath);
		return;
	}
	QString pathselect;
	pathselect = m_sImageListPath + "/" + sSelectItem;
	m_currentImagePath = pathselect;
	QFileInfo file(pathselect);
	if (!file.isFile() && m_timerReadList == nullptr)
	{
		m_sImageListPath = pathselect;
		initImageLS(m_sImageListPath);
	}
	else
	{
		m_MOriginal = imread(std::string(m_currentImagePath.toLocal8Bit()));
		if (m_MOriginal.rows > 0)
		{
			uint m_iResult;	//单次检测结果，最大不超过八个 
			int i_captotal = 0;
			QString str;
			CHECKPARAM _checkparam;

			strcpy(_checkparam.c_CameraName, _CameraName);
			NodeToParam(_checkparam, ui.treeWidget->_mparam);
			bool results = ((CInterCHeck*)p_Parent)->Check(m_MOriginal, &_checkparam, str);
			bool b = ((CInterCHeck*)p_Parent)->RealCheck(str, &_checkparam, m_WND);
		}
	}
}
int QtGuiSetting::showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str)//全是中文
{
	if (QString::fromLocal8Bit(button2Str) == "")
	{
		QMessageBox msg(icon, QString::fromLocal8Bit(titleStr), QString::fromLocal8Bit(contentStr), QMessageBox::Ok);
		msg.setButtonText(QMessageBox::Ok, QString::fromLocal8Bit(button1Str));
		msg.setWindowIcon(QIcon("./dr.ico"));
		return msg.exec();
	}
	else
	{
		QMessageBox msg(icon, QString::fromLocal8Bit(titleStr), QString::fromLocal8Bit(contentStr), QMessageBox::Yes | QMessageBox::No);
		msg.setButtonText(QMessageBox::Yes, QString::fromLocal8Bit(button1Str));
		msg.setButtonText(QMessageBox::No, QString::fromLocal8Bit(button2Str));
		msg.setWindowIcon(QIcon("./dr.ico"));
		return msg.exec();
	}
}
