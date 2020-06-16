#pragma once
#include "AllRelayHead.h"
#include <QObject>
class Dammy : public QWidget
{
	Q_OBJECT
		signals :
	void InitSingle(QString str);//初始化信号
private:
	QObject* _parent;
public:
	Dammy(QObject *parent);
	~Dammy();
	public slots:
	void DammyInit();//公共槽函数
};
class MultInit_Run : public QObject
{
	Q_OBJECT
		signals:
	void InitSingle(QString str);//初始化信号，由下面的槽函数发送👇👇👇
	void StartsSingle();//开始信号
	void PlaySoundPath(QString str);//声音路径信号
private:
	QObject* _parent;//一个对象
	Dammy* _dammy;//初始化所有东西的类Dammy
	QThread *dammy_th;//一个多线程
public:
	MultInit_Run(QObject *parent);
	~MultInit_Run();
	public slots:
	int ThreadInit();//线程初始化，用于发送👆👆👆初始化信号给Dammy类对象，return -1；
	int CloseTh();//return 0；
};
#ifdef LocalPath
class MultGetThread_Run : public QObject
{
	Q_OBJECT
signals:
	void StartSingle();
	void RESTARTSIGNAL();
	void GETONEIMAGEMAT(Mat);
public:
private:
	int m_iSelfIndex;
	int totalcount;
	int sizetotal;
	bool m_bAlRegister;
	void * m_LabelShow;
	Mat m_MatGetOnece;
public:
	MultGetThread_Run(QObject *parent);
	~MultGetThread_Run();
public slots:
	int ThreadGetImage(int);
public:
	void SetMultIndex(int);
	void SetDirectShowDlg(void*);

};
#endif

class MultDecodeThread_Run : public QObject
{
	Q_OBJECT
signals:
	void SAVESIGNAL(Mat, QString);
	void OUTRESULTSUMMARY(QString, int, int);
private:
	int m_iSelfIndex;
	uint m_iResult;	//单次检测结果，最大不超过八个
	int i_captotal = 0;
	int m_iSaveOKorNG;
	int i_SaveLoop;
	long timecheck;
	int index_pos;
	QString okdir_str;
	QString ngdir_str;
public:
	MultDecodeThread_Run(QObject *parent);
	~MultDecodeThread_Run();
	CBaseCheckAlg *_CheckClass;

public slots:
	int ThreadDecodeImage(int);
	int ThreadDecodeImageMat(Mat);
public:
	void SetMultIndex(int);

};

class MultSaveThread_Run : public QObject
{
	Q_OBJECT
private:
	int m_iSelfIndex;
	int m_icamcount;
	QVector<int>* m_qvEachResule;
public:
	MultSaveThread_Run(QObject *parent, int camcount);
	~MultSaveThread_Run();
public slots:
	void ThreadSave(Mat, QString);
};

class MultSummaryThread_Run : public QObject
{
	Q_OBJECT
signals:
	void SUMMARYRESULTINCIRCLE(QStringList);
private:
	int m_iSelfIndex;
	int m_icamcount;
	int m_iResultAllList;
	double avg;
	bool *b_eachalreadyfinish;
	QStringList *m_qslResultEachCamera;
	QStringList m_qslResultTOTALCamera;
public:
	MultSummaryThread_Run(QObject *parent,int camcount);
	~MultSummaryThread_Run();
public slots:
	void ThreadSummary(QString,int,int);
};