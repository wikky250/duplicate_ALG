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