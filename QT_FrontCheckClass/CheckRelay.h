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


	///////////the kinds of errors////////////////////////////////////////////////
	//the size of band
	int i_Channel_Band = -1;
	int i_MinGray_Band = -1;
	int i_MinArea_Band = -1;
	//defect of band
	int i_PlusLength_Rectangle = -1;
	int i_MinArea_BandDefect = -1;
	//defect of Al	
	int i_Channel_Al = -1;
	int i_MinGray_Al = -1;
	int i_MinArea_Al = -1;
	//number of pill
	int i_Channel1_NumPill = -1;
	int i_Channel2_NumPill = -1;
	int i_Number_NumPill = -1;
	int i_MinGray_NumPill = -1;
	int i_MinArea_NumPill = -1;
	//powder
	int i_Channel_Powder = -1;
	int i_DifMean_Powder = -1;
	//batch 
	int i_PlusLength1_Batch = -1;
	int i_PlusLength2_Batch = -1;
	int i_Channel_Batch = -1;
	int i_MinGray_Batch = -1;
	double d_Opening_Batch = -1;
	double d_Closing_Batch = -1;
	int i_MinArea_Batch = -1;
	//shadow of bubble
	int i_Channel1_Shadow = -1;
	int i_Channel2_Shadow = -1;
	int i_Channel3_Shadow = -1;
	int i_MinGray_Shadow = -1;
	int i_Width_Shadow = -1;
	int i_Length_Shadow = -1;
	int i_Area_Shadow = -1;
	double d_Closing_Shadow = -1;
	double d_Opening_Shadow = -1;
	int i_Radius_Shadow = -1;
	//crack of band
	int i_Channel_Crack = -1;
	int i_Length_Crack = -1;
	double d_Circularity_Crack = -1;
	//dirty of band
	int i_MinArea_BlackSpot = -1;
	double d_Circularity_BlackSpot = -1;
	int i_Channel_Dirty = -1;
	int i_MaxGray_Dirty = -1;
	int i_MinArea_Dirty = -1;
	double d_Circularity_Dirty = -1;
	//another crack of band
	int i_Channel_Fissure = -1;
	int i_MinGray_Fissure = -1;
	int i_MinArea_Fissure = -1;
	double d_Circularity_Fissure = -1;
	//inner defect of pill
	int i_Channel1_Iner = -1;
	int i_Channel2_Iner = -1;
	int i_MaxGray_Iner = -1;
	double d_Closing_Iner = -1;
	double d_Opening_Iner = -1;
	//half pill
	int i_MinArea_Half = -1;
	int i_RectMove1_Half = -1;
	int i_RectMove2_Half = -1;
	//cracked pill
	double d_Erosion_Break = -1;
	int i_Channel_Break = -1;
	int i_Length_Break = -1;
	double d_Circularity_Break = -1;
	//pressedbubble
	int i_Channel1_PressedBubble = -1;
	int i_Channel2_PressedBubble = -1;
	int i_Channel3_PressedBubble = -1;
	double d_Erosion_PressedBubble = -1;
	int i_MaxGray_PressedBubble = -1;
	double d_Opening1_PressedBubble = -1;
	int i_MinArea1_PressedBubble = -1;
	int i_MinGray_PressedBubble = -1;
	double d_Opeing2_PressedBubble = -1;
	double d_Closing_PressedBubble = -1;
	int i_MinArea2_PressedBubble = -1;
	//lack of pill
	int i_RectMove_Lack = -1;
	int i_Channel1_Lack = -1;
	int i_Channel2_Lack = -1;
	int i_MaxGray_Lack = -1;
	int i_MinArea1_Lack = -1;
	int i_RadiusMove_Lack = -1;
	int i_MinArea2_Lack = -1;
	double d_Circularity_Lack = -1;
	//dirty of pill
	int i_MinArea_PillDirty = -1;
	double d_Circularity_PillDirty = -1;
	//defect of shadow
	int i_Channel1_ShadowDefect = -1;
	int i_Channel2_ShadowDefect = -1;
	int i_MaxGray_ShadowDefect = -1;
	int i_MinArea_ShadowDefect = -1;
	//folds of band
	int i_Channel1_Folds = -1;
	int i_Channel2_Folds = -1;
	int i_Length_Folds = -1;
	double d_Circularity_Folds = -1;
	//net unclear
	int i_Channel_Unclear = -1;


	//switch of detect 
	bool b_CheckPill = -1;
	bool b_CheckCapsule = -1;
	bool b_Band = -1;
	bool b_BandDefect = -1;
	bool b_Al = -1;
	bool b_NumPill = -1;
	bool b_Powder = -1;
	bool b_Batch = -1;
	bool b_Shadow = -1;
	bool b_Crack = -1;
	bool b_BlackSpot = -1;
	bool b_Dirty = -1;
	bool b_Fissure = -1;
	bool b_Iner = -1;
	bool b_Half = -1;
	bool b_Break = -1;
	bool b_Lack = -1;
	bool b_PillDirty = -1;
	bool b_ShadowDefect = -1;
	bool b_PressedBubble = -1;
	bool b_Folds = -1;
	bool b_Unclear = -1;


	////the number of Capsules
	//int i_CapsulesChannel = -1;
	//int i_MinGray_Capsules = -1;
	//double d_OR_Capsules = -1;	//OR means opening radius
	//int i_MinArea_Capsules = -1;
	//int i_MaxArea_Capsules = -1;
	//int i_Number_Capsules = -1;
	////Batch
	//int i_BatchChannel = -1;
	//int i_LengthAdd_1 = -1;
	//int i_LengthAdd_2 = -1;
	//double d_ER_Batch = -1;	//ER means erosion radius
	//int i_MaskHeight_Batch = -1;
	//int i_MaskWidth_Batch = -1;
	//int i_MinGray_Batch = -1;
	//double d_OR_Batch = -1;
	//double d_CR_Batch = -1;	//CR means closing radius
	//int i_MinArea_Batch = -1;
	//int i_MaxArea_Batch = -1;
	////int i_MinArea_BatchDefect = -1;
	////int i_MaxArea_BatchDefect = -1;
	////the defect of capsules
	//int i_Channel_CapsulesDefect = -1;
	//double d_ER_RegionCapsules = -1;
	//int i_MaxGray_CapsulesRed = -1;
	//int i_MinArea_CapsulesRed = -1;
	//int i_MaxArea_CapsulesRed = -1;
	//double d_DR_CapsulesRed = -1;//DR means dilation radius
	//double d_DR_CapsulesYellow = -1;
	////the defect of red region
	//int i_Channel_RegionRed = -1;
	//int i_Channel_RedDefect1 = -1;
	//int i_Channel_RedDefect2 = -1;
	//int i_MaskHeight_RegionRed = -1;
	//int i_MaskWidth_RegionRed = -1;
	//int i_MinGray_RegionRed = -1;
	//int i_MaxGray_RegionRed = -1;
	//double d_CR_RegionRed = -1;
	//double d_OR_RegionRed = -1;
	//int i_MinArea_RegionRed = -1;
	//int i_MaxArea_RegionRed = -1;
	//int i_MaskHeight_RedDefect = -1;
	//int i_MaskWidth_RedDefect = -1;
	//int i_MinGray_RedDefect = -1;
	//double d_ER_RedDefect = -1;
	//double d_OR_RedDefect = -1;
	//int i_MinArea_RedDefect = -1;
	//int i_MaxArea_RedDefect = -1;
	////the defect of yellow region 
	////int i_Channel_RegionYellow = -1;
	////int i_Channel_YellowDefect = -1;
	//int i_MaskHeight_RegionYellow = -1;
	//int i_MaskWidth_RegionYellow = -1;
	//int i_MinGray_RegionYellow = -1;
	//int i_MaxGray_RegionYellow = -1;
	//double d_OR_RegionYellow = -1;
	//int i_MinArea_RegionYellow = -1;
	//int i_MaxArea_RegionYellow = -1;
	////the defect of band
	//int i_Channel_BandDefect = -1;
	//double d_DR_BandDefect = -1;
	//double d_ER_BandDefect = -1;
	//int i_MaskHeight_BandDefect = -1;
	//int i_MaskWidth_BandDefect = -1;
	//int i_MinGray_BandDefect = -1;
	//double	d_CR_BandDefect = -1;
	//int i_MinWidth_BandDefect = -1;
	//int i_MaxWidth_BandDefect = -1;
	//int i_MinHeight_BandDefect = -1;
	//int i_MaxHeight_BandDefect = -1;
	//int i_MinArea_BandDefect = -1;
	//int i_MaxArea_BandDefect = -1;
	////2020.10.29 new added params
	//bool b_IsOpen = -1;
	//int i_Channel_InCapsules = -1;
	//double d_ER_InCapsules = -1;//67
	//int i_MaxGray_InCapsules = -1;
	//double d_OR_InCapsules = -1;
	//int i_MinArea_InCapsules = -1;
	//int i_MaxArea_InCapsules = -1;
	//int i_LengthPlus_1 = -1;
	//int i_LengthPlus_2 = -1;

	//int i_Channel_Middle = -1;
	//int i_MaskHeight_Middle = -1;//75
	//int i_MaskWidth_Middle = -1;
	//int i_MinGray_Middle = -1;
	//int i_MinWidth_Middle = -1;
	//int i_MinHeight_Middle = -1;
	//double d_CR_Middle = -1;
	//int i_MinArea_Middle = -1;
	//int i_Channel_Plate = -1;
	//int i_MaxGray_Plate = -1;//83
	//double d_CR_Plate = -1;
	//double d_OR_Plate_1 = -1;
	//int i_MinArea_Plate_1 = -1;
	//double d_OR_Plate_2 = -1;
	//int i_MinArea_Plate_2 = -1;


	//////////////////////////////////////////////////////////////////////

	CHECKPARAM()
	{
	}
	~CHECKPARAM()
	{
	}
};