#pragma once

#include <QtWidgets/QMainWindow>
#include "windows.h"
#pragma comment(lib, "version")
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QLibrary>
#include "../QT_FrontCheckClass/QT_FrontCheckClass.h"
typedef CBaseCheckAlg* (__stdcall* pExport)(bool b);
typedef void(__stdcall* pDeleteExport)(CBaseCheckAlg*);
#include "ui_OffLine_Test.h"

class OffLine_Test : public QMainWindow
{
    Q_OBJECT

signals:
	void ERRORMESSAGE(int, QString, int);
	void CONTINUESIGNAL();
	void CHECKRESULT(QStringList);
public:
    OffLine_Test(QWidget *parent = Q_NULLPTR);

private:
    Ui::OffLine_TestClass ui;
	QMap<QString, QLibrary*> dllQMap;
	QString AppPath;
	QString m_sImageListPath;
	void initImageLS(QString);
	bool isImage(QFileInfo& info);
	bool containImages(QDir&);
	int m_w = -1, m_h = -1, m_c = -1;
	cv::Mat ImgRead;
	cv::Mat m_matCheck;
	cv::Mat m_tempshow;
	cv::Mat m_tempRGB;
	QImage disImage;
	cv::Mat ReadImage(QString file);
	bool InitAlgo();
	CBaseCheckAlg* _CheckClass = nullptr;
	void ShowMatToLabel(cv::Mat);
	bool m_bContinueCheck = false;
	bool m_bNGSTOP = false;
	bool m_bOKSTOP= false;
	QTimer* ti = nullptr;
public slots:
	void onDoubleImageList(QListWidgetItem* item);
	void onSelectImageList(QListWidgetItem*, QListWidgetItem*);
	void showEvent(QShowEvent* event) 
	{
		resizeEvent(nullptr);
	};
	void resizeEvent(QResizeEvent* event);
	void ontoggledStartBtn(bool);
	void ImgListAutoDown();
	bool SlotShowResult(QStringList);
};
