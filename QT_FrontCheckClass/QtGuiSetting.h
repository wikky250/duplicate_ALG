#pragma once
#include "QT_FrontCheckClass.h"
#include "CheckRelay.h"
#include "OpencvRelay.h"
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "ui_QtGuiSetting.h"
extern QString AppPath;
bool LoadCheckParam(CHECKPARAM* checkparam, QString* str = nullptr);
class QtGuiSetting : public QDialog
{
	Q_OBJECT
signals:
	void StartGetsss(QString str);
	void StartGetSample(QLabel* lab, QString str);
	void UpdatetoTest(CHECKPARAM*);
	void StartSave(QString* str);
	void ShouldSaveParam();
	void CloseGetSample();
	void SignShowImage(int, Mat, int);

private:
	uchar* dataR = NULL;
	uchar* dataG = NULL;
	uchar* dataB = NULL;
	Mat imgchannel[3];
	//Hobject Mat2Hobject(Mat & image);
	//Mat Hobject2Mat(Hobject Hobj);
	int m_WND = -1;
public:
	QtGuiSetting(QDialog* parent = Q_NULLPTR, void* AlgPointer = nullptr);
	int QtGuiSetting::showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str);//全是中文
	~QtGuiSetting();
	//////////////////////////////////////////////////////////////////////////
		//算法相关
public:
	void SetParam(char*);
	void SetModelMat(Mat img);
private:
	bool b_GetAuthority;
	//算法类父指针
	void* p_Parent;
	//算法类父指针
	QWidget* pp;
	//选择视频路径
	QString m_sSelectFile;
	//检测参数
	YAML::Node m_node;
	char _CameraName[20];
	//模板样品保存位置
	QString m_strSaveFile;
	//模板样品保存位置
	QString m_strTrainFile;
	//鼠标左键坐标
	bool m_bButton;
	QPoint m_LabelOriginal;
	QPoint m_PointOriginal;
	QPoint m_PointFinale;
	QRect m_RectToDraw;
	cv::Rect m_RectOpencv;
	//显示图像
	cv::Mat m_MOriginal;
	QPixmap pixmap;
	QImage disImage;
	timeval tv_begin, tv_end;
	//参数被修改符号
	bool m_bChanged;
	bool m_bRectChanged;
	void m_TestApplyParam();
	//////////////////////////////////////////////////////////////////////////
private:
	Ui::QtGuiSetting ui;
	QString m_SAppPath;
	QString m_sImageListPath;
	QString m_currentImagePath; //the current Item of the current Image checking
	QTimer* m_timerReadList;
	QTimer* clickedtimer;
	QDialog* dlg_keyboard = nullptr;
	QTimer* m_timerChanger = nullptr;
	int m_itimechange = 0;
	int m_itimestep = 5;
	int m_ichagetimes = 0;
	bool m_bm1 = false, m_bm05 = false, m_ba1 = false, m_ba05 = false;
	bool isImage(QFileInfo& info);
	bool containImages(QDir&);
	void initImageLS(QString str);
	void ShowImagetoLabel(Mat img);
public slots:
	void closeEvent(QCloseEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent* p);
	void mouseMoveEvent(QMouseEvent* p);
	bool eventFilter(QObject* watched, QEvent* event);
	void resizeEvent(QResizeEvent* event);
	void onBtnGetImage();
	void onBtnGetSample();
	void onShowFps(int i);
	void onCellChanged(int, int);
	void onSelectTrainFile();
	void onClickedImage(QListWidgetItem*);
	void onSingleClicked();
	void onSelectImageList(QListWidgetItem*);
	void SLOTShowImage(int, Mat img, int);
	//
	void onContinueTest(bool);
	void onTimerReadList();
	//
	void onSaveParam();
	//自动测量值
	void onAutoDetest();
	void onSelectImageChannel(int);

	void onPopKeyboard(int, int);
	void onbtnpressed();
	void onbtnreleased();
	void ontimechanger();
	void hideKeyBoard();
};
