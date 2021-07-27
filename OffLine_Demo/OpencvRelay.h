#pragma once

#pragma region MyRegion

#include "opencv.hpp"
#include "highgui.hpp"
#include <memory.h>
#include "spdlog/spdlog.h"
//#include "QTLiveUSBkey.h"
namespace spd = spdlog;
#ifdef DEBUG
#pragma comment(lib,"opencv_core3410d.lib")
#pragma comment(lib,"opencv_highgui3410d.lib")
#pragma comment(lib,"opencv_imgcodecs3410d.lib")
#pragma comment(lib,"opencv_imgproc3410d.lib")
#else
#pragma comment(lib,"opencv_core3410.lib")
#pragma comment(lib,"opencv_highgui3410.lib")
#pragma comment(lib,"opencv_imgcodecs3410.lib")
#pragma comment(lib,"opencv_imgproc3410.lib")
#endif
using namespace cv;
#pragma endregion