#pragma once

#include "AllRelayHead.h"
#include <QtWidgets/QMainWindow>
#include "ui_OffLine_Demo.h"
//相机初始化和之后配置模块
#include "InitFunction.h"
//相机运行模块
#include "MultThread_Run.h"
//相机设置模块
#include "QtCameraSet.h"
typedef CBaseCheckAlg* (__stdcall *pExport)();
typedef void(__stdcall *pDeleteExport)(CBaseCheckAlg*);

struct LOADDLLANDRELEASE
{
	char *dllName;
	pExport LoadDLL;
	pDeleteExport UnLoadDLL;
	LOADDLLANDRELEASE()
	{
		dllName = new char[20];
	}
	~LOADDLLANDRELEASE()
	{
		delete dllName;
		dllName = NULL;
	}
 };

class OffLine_Demo : public QMainWindow
{
	Q_OBJECT
signals:
	void STARTCHECK(int);
 	void STOPCHECK();
 	void GetResult(QString letter, QString _CameraName);
 	void SendImage(Mat, QString);
 	void StartInitSingle();
 	void EveryResult2Summary(QString str);
 	void tempyellow();
	void QTCLOSEALLLED();
	//DLL显示在主线程显示图像
	void SignShowImage(int pos, Mat img, int checktimes);
protected:
	QTimer *timer;
	QTimer *timerResize = nullptr;
	QTimer *timerreboot;
	QTimer *timerStart;
	QTimer *timerdoubleclick;
	QString m_sImageListPath;
public:
	pDeleteExport m_UnLoadDLL;
	OffLine_Demo(QWidget *parent = Q_NULLPTR);
	~OffLine_Demo();

	void mousePressEvent(QMouseEvent *p);
	void mouseDoubleClickEvent(QMouseEvent *q);
	void showEvent(QShowEvent *p);
	bool LoadImportantValue();
	//完成所有初始化工作
	bool FinishInitOther();
	//加载算法动态库
	int LoadAlgorithmDLL();
	//分配每个相机的算法
	bool InitCheckClass();
	//初始化检测队列
	bool InitPicList();
	//连接弱电控制
	bool OpenConnect();
	char m_Result[30];
	int showMsgBox(QMessageBox::Icon icon,const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str);//全是中文
	void OffLine_Demo::createFile(QString filePath, QString fileName);//为创建txt文件，名字是模板名
	QString OffLine_Demo::txtFilenameInDir(QString path);//截取唯一的.txt文件名

	bool isImage(QFileInfo& info);
	bool containImages(QDir&);
	void initImageLS(QString);
private:
	Ui::OffLine_DemoClass ui;
	//相机控制模块
	InitFunction *Camera_Func;
	//算法多线程模块
	QVector<MultDecodeThread_Run*> m_MultDecodeThread;
	QVector<QThread*> QTH_MultDecodeThread;
	//检测结果发送线程
	MultSaveThread_Run* m_MultSaveThread;
	QThread* QTH_MultSaveThread;
	//检测结果统计线程
	MultSummaryThread_Run* m_MultSummaryThread;
	QThread* QTH_MultSummaryThread;
	//是否正在检测标识符
	bool m_bStarting;
	//状态栏显示
	QLabel *normalshow;
	//系统时间
	QDateTime current_time;
	//初始化线程
	MultInit_Run *m_MultInit;
	QThread *QTH_Init;
	//初始化完成标识符
	bool m_bAllInited;
	//标记是否在检测
	QString m_SLabelStatue;

	QString m_IniResultPath;//ini result 路径，按下开始的时候
	QTimer *times_listImg;
public slots:
	void onTest();
	void onStartCheck(bool);
	void onStopCheck();
	void closeEvent(QCloseEvent *eve);
	void onTestSignal();
	void onCameraSet();
	//DLL显示在主线程显示图像
	void SLOTShowImage(int pos, Mat img, int checktimes);
	//显示统计结果
	bool SlotShowResult(QStringList);
	void onSelectImageList(QListWidgetItem *, QListWidgetItem *);
	void onDoubleImageList(QListWidgetItem *);
	void ImgAutoDown();
	void SetEvertDlg();
};
