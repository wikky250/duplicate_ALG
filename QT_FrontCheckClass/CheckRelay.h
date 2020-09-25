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
	CHECKPARAM()
	{
	}
	~CHECKPARAM()
	{
	}
};