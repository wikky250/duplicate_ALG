#pragma once
#include "QT_FRONTCheckClass.h"
#include "QtGuiSetting.h"
//#include "InspectProcessor.h"
#include "yaml-cpp/yaml.h"
#if DEBUG
#pragma comment(lib,"yaml-cppd.lib")
#else
#pragma comment(lib,"yaml-cpp.lib")

#endif
class CInterCHeck :public QObject,
	public CBaseCheckAlg
{
	Q_OBJECT
signals:
	void StartUSBKEY(void* handle);
private:
	int saveImageindex;
	int saveImageOKindex;
	QString ngdir_str;
	QString okdir_str;
	const char* test_name;
	const char* c_AlgName;
	const char* c_AlgVersion;
	//显示窗口，InitWindow赋值
	int  m_iShowPos;
	QImage disImage;
	//USBkey，InitWindow赋值
	HANDLE m_LEDHANDLE;
	//父窗口发送关闭指令
	void* m_parent;
	//USBKey监控线程
	QThread* USB_Thread;
	//QTLiveUSBkey *QtUsbKey;
	//检测参数
	CHECKPARAM m_checkparam;
	bool m_bchangedparam;
	//日志logger
	std::shared_ptr<spd::logger> daily_logger;
	Mat imgchannel;
private:
#pragma region 算法相关参数
private:
	bool m_b_test;
	int sizeofimage;
	int circle_times = 0;
	int total_check;
	int total_NGSave;
	int total_OKSave;
	Mat m_oriImage, m_LastImage, m_MatToShow;
	Mat grey, temp;
	Mat m_chanBGR[3];
	Mat m_chanHSV[3];
	Mat m_SaveImg;
	cv::Mat m_maskBand;
	cv::Mat m_tempCap1st, m_tempCap2nd;
	Mat m_element;

	vector<vector<float>> m_CenterModel;
	vector<Vec4i>hierarchy;
	vector<vector<Point>>contours;
	vector<vector<Point>>contours_Draw;
	int ComputerBestLabes(std::vector<float> data, double& dsts);
	int* ColValue = nullptr;
	int* ColOffset = nullptr;
	int IM_BoxBlur_C(unsigned char* Src, unsigned char* Dest, int Width, int Height, int Stride, int Radius);
#pragma endregion 算法相关参数
public:
	CInterCHeck(bool b_test);
	~CInterCHeck();
	UI_MONITOR ui;
public:
	virtual void Release();
	virtual char* GetCameraName(void);
	virtual char* GetAlgName(void);
	virtual char* GetAlgVersion(void);
	virtual int ShowParamDlg(QWidget* parent, bool b_showornot);
	virtual int SetParam(int _typeofcamera, char* _cameraName);
	virtual int ReturnParam(int* _typeofcamera, char& _cameraName);
	virtual void StartCheck(QString camerasign, std::shared_ptr<spd::logger> _daily_logger, int w, int h, int c);
	virtual void StopCheck();
	virtual QString GetResult();
	virtual int Check(Mat& imgpackage, void* checkparam, ResultStruct& str);
	virtual void ShowResult(QVector<double*>& result);
	virtual void BeatStart(void);
	virtual void BeatEnd(void);
	virtual void* GetEncryptHandle();
	virtual void EnableShow(bool);
	virtual void TESTSETSHOW(void*);
public slots:
	void EndCheck();
	bool SaveCheckParam();
};
