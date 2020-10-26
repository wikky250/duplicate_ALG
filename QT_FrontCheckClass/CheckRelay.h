#pragma once
//#define PYTHONAPPLY
//#define TESTINCOMPUTER
#pragma region QT
#include<QMessageBox>
#include<QFileInfo>
#include<QFileDialog>
#include<QThread>
#include<QLabel>
#include <QVector>
#include <QDateTime>
#include <QDate>
#pragma endregion QT
#ifndef __APPLE__
#include "cpp/HalconCpp.h"
#pragma comment(lib,"halconcpp10.lib")
using namespace Halcon;
#endif
struct ListStruct
{
	double iresult;
	double iminx, iminy, imaxx, imaxy;
	double score;
};
bool sortResult(ListStruct a, ListStruct b);
bool sortScore(ListStruct a, ListStruct b);
#pragma region TimeCount
#include <time.h>
#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#else
#  include <sys/time.h>
#endif
#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp);
#endif
#pragma endregion TimeCount
using namespace std;


struct CHECKPARAM
{
	int i_TypeOfCamera;
	char c_CameraName[20];
	int i_CheckPosNo;
	char c_OperateCore[20];
	//////////////////////////////////////////////////////////////////////////
	int i_BandArea = -1;
	int i_BandException1_Area = -1;
	int i_BandException2_Value = -1;
	int i_BandException3_Area = -1;
	int i_InterException_Area = -1;
	double d_InterException_Open = -1;
	int i_PillRadius = -1;
	int i_BandException2_Radius = -1;
	//int i_BandChannel = -1;
	//int i_BandThread = -1;
	//int i_PillChannel1 = -1;
	//int i_PillThread1 = -1;
	//////////////////////////////////////////////////////////////////////////


	///////////Capsules Detect////////////////////////////////////////////////
	//the size of band
	int i_BandChannel = -1;
	int i_MinGray_Band = -1;
	int i_MinArea_Band = -1;
	//the number of Capsules
	int i_CapsulesChannel = -1;
	int i_MinGray_Capsules = -1;
	double d_OR_Capsules = -1;	//OR means opening radius
	int i_MinArea_Capsules = -1;
	int i_MaxArea_Capsules = -1;
	int i_Number_Capsules = -1;
	//Batch
	int i_BatchChannel = -1;
	int i_LengthAdd_1 = -1;
	int i_LengthAdd_2 = -1;
	double d_ER_Batch = -1;	//ER means erosion radius
	int i_MaskHeight_Batch = -1;
	int i_MaskWidth_Batch = -1;
	int i_MinGray_Batch = -1;
	double d_OR_Batch = -1;
	double d_CR_Batch = -1;	//CR means closing radius
	int i_MinArea_Batch = -1;
	int i_MaxArea_Batch = -1;
	//int i_MinArea_BatchDefect = -1;
	//int i_MaxArea_BatchDefect = -1;
	//the defect of capsules
	int i_Channel_CapsulesDefect = -1;
	double d_ER_RegionCapsules = -1;
	int i_MaxGray_CapsulesRed = -1;
	int i_MinArea_CapsulesRed = -1;
	int i_MaxArea_CapsulesRed = -1;
	double d_DR_CapsulesRed = -1;//DR means dilation radius
	double d_DR_CapsulesYellow = -1;
	//the defect of red region
	int i_Channel_RegionRed = -1;
	int i_Channel_RedDefect1 = -1;
	int i_Channel_RedDefect2 = -1;
	int i_MaskHeight_RegionRed = -1;
	int i_MaskWidth_RegionRed = -1;
	int i_MinGray_RegionRed = -1;
	int i_MaxGray_RegionRed = -1;
	double d_CR_RegionRed = -1;
	double d_OR_RegionRed = -1;
	int i_MinArea_RegionRed = -1;
	int i_MaxArea_RegionRed = -1;
	int i_MaskHeight_RedDefect = -1;
	int i_MaskWidth_RedDefect = -1;
	int i_MinGray_RedDefect = -1;
	double d_ER_RedDefect = -1;
	double d_OR_RedDefect = -1;
	int i_MinArea_RedDefect = -1;
	int i_MaxArea_RedDefect = -1;
	//the defect of yellow region 
	//int i_Channel_RegionYellow = -1;
	//int i_Channel_YellowDefect = -1;
	int i_MaskHeight_RegionYellow = -1;
	int i_MaskWidth_RegionYellow = -1;
	int i_MinGray_RegionYellow = -1;
	int i_MaxGray_RegionYellow = -1;
	double d_OR_RegionYellow = -1;
	int i_MinArea_RegionYellow = -1;
	int i_MaxArea_RegionYellow = -1;
	//the defect of band
	int i_Channel_BandDefect = -1;
	double d_DR_BandDefect = -1;
	double d_ER_BandDefect = -1;
	int i_MaskHeight_BandDefect = -1;
	int i_MaskWidth_BandDefect = -1;
	int i_MinGray_BandDefect = -1;
	double	d_CR_BandDefect = -1;
	int i_MinWidth_BandDefect = -1;
	int i_MaxWidth_BandDefect = -1;
	int i_MinHeight_BandDefect = -1;
	int i_MaxHeight_BandDefect = -1;
	int i_MinArea_BandDefect = -1;
	int i_MaxArea_BandDefect = -1;
	//////////////////////////////////////////////////////////////////////

	CHECKPARAM()
	{
	}
	~CHECKPARAM()
	{
	}
};