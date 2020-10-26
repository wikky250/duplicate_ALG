#pragma once
#include "QT_FrontCheckClass.h"
#include "CheckRelay.h"
#include "OpencvRelay.h"
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "ui_QtGuiSetting.h"
class QtGuiSetting : public QDialog
{
	Q_OBJECT
signals:
	void StartGetsss(QString str);
	void StartGetSample(QLabel *lab, QString str);
	void UpdatetoTest(CHECKPARAM*);
	void StartSave(QString *str);
	void ShouldSaveParam(CHECKPARAM*);
	void CloseGetSample();
	void SignShowImage(int, Mat, int);
	
private:
	uchar* dataR = NULL;
	uchar* dataG = NULL;
	uchar* dataB = NULL;
	Mat imgchannel[3];
	Hobject Mat2Hobject(Mat & image);
	Mat Hobject2Mat(Hobject Hobj);
	int m_WND =-1;
	QDialog *dlg_keyboard = nullptr;
public:
	QtGuiSetting(QWidget *parent = Q_NULLPTR,void* AlgPointer = nullptr);
	int QtGuiSetting::showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str);//ȫ������
	~QtGuiSetting();
	//////////////////////////////////////////////////////////////////////////
		//�㷨���
public:
	void SetParam(CHECKPARAM param);
	void SetModelMat(Mat img);
private:
	bool b_GetAuthority;
	//�㷨�ָุ��
	void* p_Parent;
	//�㷨�ָุ��
	QWidget* pp;
	//ѡ����Ƶ·��
	QString m_sSelectFile;
	//������
	CHECKPARAM _checkparam;
	//ģ����Ʒ����λ��
	QString m_strSaveFile;
	//ģ����Ʒ����λ��
	QString m_strTrainFile;
	//����������
	bool m_bButton;
	QPoint m_LabelOriginal;
	QPoint m_PointOriginal;
	QPoint m_PointFinale;
	QRect m_RectToDraw;
	cv::Rect m_RectOpencv;
	//��ʾͼ��
	cv::Mat m_MOriginal;
	QPixmap pixmap;
	QImage disImage;
	timeval tv_begin, tv_end;
	//�������޸ķ���
	bool m_bChanged;
	bool m_bRectChanged;
	void m_TestApplyParam();
	//////////////////////////////////////////////////////////////////////////
private:
	Ui::QtGuiSetting ui;
	QString m_SAppPath;
	QString m_sImageListPath;
	QString m_currentImagePath; //the current Item of the current Image checking
	QTimer *m_timerReadList;
	QTimer *clickedtimer;
	bool isImage(QFileInfo& info);
	bool containImages(QDir&);
	void initImageLS(QString str);
public slots:
	//void onBandChanged(int);
	//void onCapsulesChanged(int);
	//void onBatchChanged(int);
	//void onCapDefectChanged(int);
	//void onRedChanged(int);
	//void onYellowChanged(int);
	//void onBandDefectChanged(int);
	void closeEvent(QCloseEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent * p);
	void mouseMoveEvent(QMouseEvent * p);
	bool eventFilter(QObject *watched, QEvent *event);
	void resizeEvent(QResizeEvent *event);
	void onBtnGetImage();
	void onBtnGetSample();
	void onShowImage(Mat *img);
	void onShowFps(int i);
	void onCellChanged(int, int);
	void onSelectTrainFile();
	void onClickedImage(QListWidgetItem *);
	void onSingleClicked();
	void onSelectImageList(QListWidgetItem *);
	void SLOTShowImage(int, Mat img, int);
	//
	void onContinueTest(bool);
	void onTimerReadList();
	//
	void onSaveParam();
	//�Զ�����ֵ
	void onAutoDetest();
	void onSelectImageChannel(int);

	void onPopKeyboard(int, int);
	void hideKeyBoard();
};
