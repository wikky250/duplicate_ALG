#include "InterCHeck.h"
#include <QMessageBox>
#include "MORClass.h"
bool CInterCHeck::SaveCheckParam()
{
	LoadCheckParam(&m_checkparam);
	m_bchangedparam = true;
	return false;
}
CInterCHeck::CInterCHeck(bool b_test)
{
	m_b_test = b_test;
	m_bchangedparam = false;
	AppPath = qApp->applicationDirPath();
	//test_name = "OCRCheck";
	c_AlgName = "RYcapsule";
	c_AlgVersion = "0.0.1";
	//strcpy(c_AlgName, test_name);
	m_iShowPos = 0;
	USB_Thread = nullptr;
	//QtUsbKey = nullptr;
	m_LEDHANDLE = nullptr;
	//setdlg = nullptr;
	saveImageindex = 0;
	saveImageOKindex = 0;
	//后期移动到其他位置。2018年11月23日13:47:49
//	HException::InstallHHandler(&MyHalconExceptionHandler);
	if (b_test)
	{
	}
}
CInterCHeck::~CInterCHeck()
{
}
void CInterCHeck::EndCheck()
{
	QMessageBox::about(NULL, QString::fromLocal8Bit("严重错误"), QString::fromLocal8Bit("USBkey意外断开！"));
	delete this;
}
void CInterCHeck::Release()
{
	delete this;
}
char* CInterCHeck::GetCameraName(void)
{
	return m_checkparam.c_CameraName;
}
char* CInterCHeck::GetAlgName(void)
{
	return (char*)c_AlgName;
}
char* CInterCHeck::GetAlgVersion(void)
{
	return (char*)c_AlgVersion;
}
int CInterCHeck::ShowParamDlg(QWidget* parent, bool b_showornot)
{
	QtGuiSetting* setdlg = new QtGuiSetting(nullptr, this);
	setdlg->SetParam(m_checkparam.c_CameraName);
	setdlg->SetModelMat(m_LastImage);
	m_bchangedparam = false;
	QObject::connect(setdlg, SIGNAL(ShouldSaveParam(bool)), this, SLOT(SaveCheckParam()));
	setdlg->exec();
	delete setdlg;
	setdlg = nullptr;
	if (m_bchangedparam)
	{
		return 1;
	}
	return -1;
}
int CInterCHeck::SetParam(int _typeofcamera, char* _cameraName)
{
	m_checkparam.i_TypeOfCamera = _typeofcamera;
	strcpy(m_checkparam.c_CameraName, _cameraName);
	return 0;
}
int CInterCHeck::ReturnParam(int* _typeofcamera, char& _cameraName)
{
	return 0;
}
void CInterCHeck::StartCheck(QString camerasign, std::shared_ptr<spd::logger> _daily_logger, int w, int h, int c)
{
	if (_daily_logger != nullptr)
	{
		daily_logger = _daily_logger;
	}
	m_oriImage = cv::Mat(w, h, c == 1 ? CV_8UC1 : CV_8UC3);
	total_check = 0;
	map<string, string> optin;
	optin.insert(pair<string, string>("conf", (AppPath + "/DefaultModel/conf_inspect.yaml").toLocal8Bit()));
	optin.insert(pair<string, string>("parameter", (AppPath + "/DefaultModel/params.yaml").toLocal8Bit()));
	optin.insert(pair<string, string>("detectors", (AppPath + "/DefaultModel/detectors.yaml").toLocal8Bit()));

	int Radius = 11;
	if (nullptr == ColValue)
	{
		ColValue = (int*)malloc((w + Radius + Radius) * (1) * sizeof(int));
	}
	else
	{
		delete ColValue;
		ColValue = (int*)malloc((w + Radius + Radius) * (1) * sizeof(int));
	}
	if (nullptr == ColOffset)
	{
		ColOffset = (int*)malloc((h + Radius + Radius) * sizeof(int));
	}
	else
	{
		delete ColOffset;
		ColOffset = (int*)malloc((h + Radius + Radius) * sizeof(int));
	}
	LoadCheckParam(&m_checkparam);
}
void CInterCHeck::StopCheck()
{
}
QString CInterCHeck::GetResult()
{
	return QString();
}
template<typename T>
inline T norm_L2_Sqr(const T* a, const T* b, int n)
{
	double s = 0.f;
	for (int i = 0; i < n; i++) {
		double v = double(a[i] - b[i]);
		s += v * v;
	}
	return s;
}
int CInterCHeck::ComputerBestLabes(std::vector<float> data, double& dsts)
{
	int k_best = -1;
	const int K = m_CenterModel.size();
	const int dims = m_CenterModel[0].size();
	const std::vector<float> sample = data;
	double min_dist = std::numeric_limits<double>::max(); // DBL_MAX
	for (int k = 0; k < K; ++k) {
		const std::vector<float> center = m_CenterModel[k];
		const double dist = norm_L2_Sqr(sample.data(), center.data(), dims);
		if (min_dist > dist) {
			min_dist = dist;
			k_best = k;
		}
	}
	dsts = sqrt(min_dist) / 2500;
	return k_best;
}
bool compColx(const Rect& a, const Rect& b)
{
	return a.x < b.x;
}
bool compColy(const Rect& a, const Rect& b)
{
	return a.y < b.y;
}
int otsu(Mat src_image)
{//全局大津算法
	double sum = 0.0;
	double w0 = 0.0;
	double w1 = 0.0;
	double u0_temp = 0.0;
	double u1_temp = 0.0;
	double u0 = 0.0;
	double u1 = 0.0;
	double delta_temp = 0.0;
	double delta_max = 0.0;

	//src_image灰度级
	int pixel_count[256] = { 0 };
	float pixel_pro[256] = { 0 };
	int threshold = 0;
	// uchar* data = (uchar*)src_image.imageData;
	//统计每个灰度级中像素的个数
	for (int i = 0; i < src_image.rows; i++)
	{
		for (int j = 0; j < src_image.cols; j++)
		{
			pixel_count[src_image.at<uchar>(i, j)]++;
			// sum += (int)data[i * src_image.cols + j];
		}
	}
	//cout<<"平均灰度："<<sum / ( src_image->height * src_image->width )<<endl;
	//计算每个灰度级的像素数目占整幅图像的比例
	for (int i = 0; i < 256; i++)
	{
		pixel_pro[i] = (float)pixel_count[i] / (src_image.cols * src_image.rows);
	}
	//遍历灰度级[0,255],寻找合适的threshold
	for (int i = 0; i < 256; i++)
	{
		w0 = w1 = u0_temp = u1_temp = u0 = u1 = delta_temp = 0;
		for (int j = 0; j < 256; j++)
		{
			if (j <= i)   //背景部分
			{
				w0 += pixel_pro[j];
				u0_temp += j * pixel_pro[j];
			}
			else   //前景部分
			{
				w1 += pixel_pro[j];
				u1_temp += j * pixel_pro[j];
			}
		}
		u0 = u0_temp / w0;
		u1 = u1_temp / w1;
		delta_temp = (float)(w0 * w1 * pow((u0 - u1), 2));
		if (delta_temp > delta_max)
		{
			delta_max = delta_temp;
			threshold = i;
		}
	}
	return threshold;
}
int CInterCHeck::Check(Mat& imgpackage, void* checkparam, ResultStruct& str)
{
	if (checkparam != nullptr)
	{
		m_tcheckparam = *(CHECKPARAM*)checkparam;
	}
	else
		m_tcheckparam = m_checkparam;
	bool _bresult = TRUE;
	ResultStruct st;
	st._bResultNGOK = true;
	contours.clear();
	contours_Draw.clear();
	try
	{
		if (total_check == 0)
		{
			m_maskBand = cv::Mat(imgpackage.size(), CV_8UC1);
			m_element = getStructuringElement(MORPH_RECT, Size(11, 11)); // 5、高级形态学变化，闭操作
		}
		imgpackage.copyTo(m_LastImage);
		m_maskBand = 0;
		imgpackage.copyTo(m_oriImage);
		imgpackage.copyTo(m_MatToShow);
		cv::split(m_oriImage, m_chanBGR);
		cv::cvtColor(m_oriImage, m_oriImage, CV_BGR2HSV);
		cv::split(m_oriImage, m_chanHSV);
		cv::Mat temp;
		cv::threshold(m_chanBGR[2], temp, m_tcheckparam.i_Gray_Pill, 255, cv::THRESH_BINARY);
		contours.clear();
		cv::findContours(temp.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		int n = -1, n_index = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			int _iarea = cv::contourArea(contours[i]);
			if (_iarea > n)
			{
				n = _iarea;
				n_index = i;
			}
		}
		cv::Rect re_band;
		if (n != -1)
		{
			re_band = cv::boundingRect(contours[n_index]);
			re_band.x += 10;
			re_band.y += 10;
			re_band.height -= 20;
			re_band.width -= 20;
			contours_Draw.push_back(contours[n_index]);
		}
		drawContours(m_MatToShow, contours_Draw, -1, cv::Scalar(255, 255, 0), 10, 8);
		contours_Draw.clear();
		m_MatToShow.copyTo(imgpackage);
		total_check++;
		if(total_check%5==0)
		{
			strcpy(st.error_describe, "Good");
			st._bResultNGOK = true;
		}
		else
		{
			strcpy(st.error_describe, "Error1");
			st._bResultNGOK = false;
		}
	}
	catch (cv::Exception& e)
	{
	}
	catch (...)
	{
	}
	str = st;
	return -1;
}
void CInterCHeck::ShowResult(QVector<double*>& result)
{
}
void CInterCHeck::BeatStart(void)
{
}
void CInterCHeck::BeatEnd(void)
{
}
void* CInterCHeck::GetEncryptHandle()
{
	return nullptr;
}
void CInterCHeck::EnableShow(bool b)
{
}
#include <QDebug>
void CInterCHeck::TESTSETSHOW(void* showlabel)
{
	// 	HTuple hv_WindowID;
	// 	open_window(((QLabel*)showlabel)->y(), ((QLabel*)showlabel)->x(), ((QLabel*)showlabel)->width(), ((QLabel*)showlabel)->height(), (long)((QWidget*)((QLabel*)showlabel)->parent())->winId(), "", "", &hv_WindowID);
	// 	//set_check("father");
	// 	m_ShowLabel.push_back(hv_WindowID);
}

int CInterCHeck::IM_BoxBlur_C(unsigned char* Src, unsigned char* Dest, int Width, int Height, int Stride, int Radius)
{
	int Channel = Stride / Width;
	if ((Src == NULL) || (Dest == NULL))                    return -1;
	if ((Width <= 0) || (Height <= 0) || (Radius <= 0))        return -1;
	if (Radius < 1)                                            return -1;
	if ((Channel != 1) && (Channel != 3) && (Channel != 4))    return -1;

	Radius = IM_Min(IM_Min(Radius, Width - 1), Height - 1);        //    由于镜像的需求，要求半径不能大于宽度或高度-1的数据

	int SampleAmount = (2 * Radius + 1) * (2 * Radius + 1);
	float Inv = 1.0 / SampleAmount;
	if ((ColValue == NULL) && (ColOffset == NULL))
	{
		ColValue = (int*)malloc((Width + Radius + Radius) * (Channel == 1 ? Channel : 4) * sizeof(int));
		ColOffset = (int*)malloc((Height + Radius + Radius) * sizeof(int));
	}
	if ((ColValue == NULL) || (ColOffset == NULL))
	{
		if (ColValue != NULL)    free(ColValue);
		if (ColOffset != NULL)    free(ColOffset);
		return -1;
	}
	for (int Y = 0; Y < Height + Radius + Radius; Y++)
		ColOffset[Y] = IM_GetMirrorPos(Height, Y - Radius);

	if (Channel == 1)
	{
		for (int Y = 0; Y < Height; Y++)
		{
			unsigned char* LinePD = Dest + Y * Stride;
			if (Y == 0)
			{
				memset(ColValue + Radius, 0, Width * sizeof(int));
				for (int Z = -Radius; Z <= Radius; Z++)
				{
					unsigned char* LinePS = Src + ColOffset[Z + Radius] * Stride;
					int BlockSize = 8, Block = Width / BlockSize;
					for (int X = 0; X < Block * BlockSize; X += BlockSize)
					{
						int* DestP = ColValue + X + Radius;
						__m128i Sample = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(LinePS + X)));
						_mm_storeu_si128((__m128i*)DestP, _mm_add_epi32(_mm_loadu_si128((__m128i*)DestP), _mm_cvtepi16_epi32(Sample)));
						_mm_storeu_si128((__m128i*)(DestP + 4), _mm_add_epi32(_mm_loadu_si128((__m128i*)(DestP + 4)), _mm_unpackhi_epi16(Sample, _mm_setzero_si128())));
					}
				}
			}
			else
			{
				unsigned char* RowMoveOut = Src + ColOffset[Y - 1] * Stride;                //    即将减去的那一行的首地址
				unsigned char* RowMoveIn = Src + ColOffset[Y + Radius + Radius] * Stride;    //    即将加上的那一行的首地址
				int BlockSize = 8, Block = Width / BlockSize;
				__m128i Zero = _mm_setzero_si128();
				for (int X = 0; X < Block * BlockSize; X += BlockSize)
				{
					int* DestP = ColValue + X + Radius;
					__m128i MoveOut = _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(RowMoveOut + X)), Zero);
					__m128i MoveIn = _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(RowMoveIn + X)), Zero);
					__m128i Diff = _mm_sub_epi16(MoveIn, MoveOut);                        //    注意这个有负数也有正数的，有负数时转换32位是不能用_mm_unpackxx_epi16体系的函数
					_mm_storeu_si128((__m128i*)DestP, _mm_add_epi32(_mm_loadu_si128((__m128i*)DestP), _mm_cvtepi16_epi32(Diff)));
					_mm_storeu_si128((__m128i*)(DestP + 4), _mm_add_epi32(_mm_loadu_si128((__m128i*)(DestP + 4)), _mm_cvtepi16_epi32(_mm_srli_si128(Diff, 8))));
				}
			}
			FillLeftAndRight_Mirror_C(ColValue, Width, Radius);                //    镜像填充左右数据
			int LastSum = SumofArray_C(ColValue, Radius * 2 + 1);                //    处理每行第一个数据
			//int LastSum1 = SumofArray_SSE(ColValue, Radius * 2 + 1);                //    处理每行第一个数据
			LinePD[0] = IM_ClampToByte(LastSum * Inv);
			int BlockSize = 4, Block = (Width - 1) / BlockSize;
			__m128i OldSum = _mm_set1_epi32(LastSum);
			__m128 Inv128 = _mm_set1_ps(Inv);
			for (int X = 1; X < Block * BlockSize + 1; X += BlockSize)
			{
				__m128i ColValueOut = _mm_loadu_si128((__m128i*)(ColValue + X - 1));
				__m128i ColValueIn = _mm_loadu_si128((__m128i*)(ColValue + X + Radius + Radius));
				__m128i ColValueDiff = _mm_sub_epi32(ColValueIn, ColValueOut);                            //    P3 P2 P1 P0
				__m128i Value_Temp = _mm_add_epi32(ColValueDiff, _mm_slli_si128(ColValueDiff, 4));        //    P3+P2 P2+P1 P1+P0 P0
				__m128i Value = _mm_add_epi32(Value_Temp, _mm_slli_si128(Value_Temp, 8));                 //    P3+P2+P1+P0 P2+P1+P0 P1+P0 P0
				__m128i NewSum = _mm_add_epi32(OldSum, Value);
				OldSum = _mm_shuffle_epi32(NewSum, _MM_SHUFFLE(3, 3, 3, 3));                              //    重新赋值为最新值
				//mean 模糊
				__m128 Mean = _mm_mul_ps(_mm_cvtepi32_ps(NewSum), Inv128);
				//_mm_storesi128_4char(_mm_cvtps_epi32(Mean), LinePD + X);
				//膨胀
				//__m128i Flag = _mm_cmpgt_epi32(NewSum, _mm_setzero_si128());
				//Flag = _mm_packs_epi32(Flag, Flag);
				//*((int *)(LinePD + X)) = _mm_cvtsi128_si32(_mm_packs_epi16(Flag, Flag));
				//腐蚀
				__m128i Flag = _mm_cmpgt_epi32(NewSum, _mm_setr_epi32(254.9 / Inv, 254.9 / Inv, 254.9 / Inv, 254.9 / Inv));
				Flag = _mm_packs_epi32(Flag, Flag);
				__m128i s = _mm_packs_epi16(Flag, Flag);
				int xx = _mm_cvtsi128_si32(s);
				*((int*)(LinePD + X)) = xx;
			}
		}
	}
	else if (Channel == 3)
	{
	}
	else if (Channel == 4)
	{
	}
	return 0;
}