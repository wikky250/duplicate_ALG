#pragma once

#include "AllRelayHead.h"
#include <QtWidgets/QMainWindow>
#include "ui_OffLine_Demo.h"
//�����ʼ����֮������ģ��
#include "InitFunction.h"
//�������ģ��
#include "MultThread_Run.h"
//�������ģ��
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
	//DLL��ʾ�����߳���ʾͼ��
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
	//������г�ʼ������
	bool FinishInitOther();
	//�����㷨��̬��
	int LoadAlgorithmDLL();
	//����ÿ��������㷨
	bool InitCheckClass();
	//��ʼ��������
	bool InitPicList();
	//�����������
	bool OpenConnect();
	char m_Result[30];
	int showMsgBox(QMessageBox::Icon icon,const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str);//ȫ������
	void OffLine_Demo::createFile(QString filePath, QString fileName);//Ϊ����txt�ļ���������ģ����
	QString OffLine_Demo::txtFilenameInDir(QString path);//��ȡΨһ��.txt�ļ���

	bool isImage(QFileInfo& info);
	bool containImages(QDir&);
	void initImageLS(QString);
private:
	Ui::OffLine_DemoClass ui;
	//�������ģ��
	InitFunction *Camera_Func;
	//�㷨���߳�ģ��
	QVector<MultDecodeThread_Run*> m_MultDecodeThread;
	QVector<QThread*> QTH_MultDecodeThread;
	//����������߳�
	MultSaveThread_Run* m_MultSaveThread;
	QThread* QTH_MultSaveThread;
	//�����ͳ���߳�
	MultSummaryThread_Run* m_MultSummaryThread;
	QThread* QTH_MultSummaryThread;
	//�Ƿ����ڼ���ʶ��
	bool m_bStarting;
	//״̬����ʾ
	QLabel *normalshow;
	//ϵͳʱ��
	QDateTime current_time;
	//��ʼ���߳�
	MultInit_Run *m_MultInit;
	QThread *QTH_Init;
	//��ʼ����ɱ�ʶ��
	bool m_bAllInited;
	//����Ƿ��ڼ��
	QString m_SLabelStatue;

	QString m_IniResultPath;//ini result ·�������¿�ʼ��ʱ��
	QTimer *times_listImg;
public slots:
	void onTest();
	void onStartCheck(bool);
	void onStopCheck();
	void closeEvent(QCloseEvent *eve);
	void onTestSignal();
	void onCameraSet();
	//DLL��ʾ�����߳���ʾͼ��
	void SLOTShowImage(int pos, Mat img, int checktimes);
	//��ʾͳ�ƽ��
	bool SlotShowResult(QStringList);
	void onSelectImageList(QListWidgetItem *, QListWidgetItem *);
	void onDoubleImageList(QListWidgetItem *);
	void ImgAutoDown();
	void SetEvertDlg();
};
