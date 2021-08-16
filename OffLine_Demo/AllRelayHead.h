#pragma once
//#define TEST

#define MODEL_BASLER		0
#define MODEL_DAHENG		1
#define CAMREACATCHBUFFER		20
#define ALGNUM		5
#define MAX_CAMERACOUNT 4
#define REFRESHPLCSPACE 10


#pragma region QT
#include <QSettings>
#include <QTcpSocket>
#include <QtNetwork>
#include <QThread>
#include <QMessageBox>
#include <QMessageBox>
#include <QDir>
#include <QLibrary>
#include <QVector>
#include <QGraphicsScene>
#include <QCloseEvent>
#include <QTimerEvent>
#include <QtMultimedia/QSound>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#pragma endregion

#ifdef PLCCONNECT
#include "comm.h"
#include "Socket_CPP.h"
#endif // PLCCONNECT

#include "RingBuffer.h"
#include "opencv.hpp"
#include "highgui.hpp"
#include <io.h>  
#include <direct.h>  
#include <vector>
#include <fstream>
#include <iostream> 
#include <math.h>
#include "windows.h"
#include "shellapi.h"
#pragma region log
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
namespace spd = spdlog;
#pragma endregion

static const size_t c_maxCamerasToUse = 32;
