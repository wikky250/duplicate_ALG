#include "QtGuiSetting.h"
#include "InterCHeck.h"
#include <QtWidgets/QComboBox>
#include <QDesktopWidget>
#include <math.h>
#include<QMessageBox>
#include<QCheckBox>
#include<QMyTreeWidget.h>;
typedef void*   UI_MONITOR;
void ShowCallBack(UI_MONITOR ui, int pos, Mat img, int times)
{
	((QtGuiSetting*)ui)->emit SignShowImage(pos, img, times);
}

QtGuiSetting::QtGuiSetting(QWidget *parent, void* AlgPointer)
	: QDialog(parent)
{
	//QString str = QString::number(0.5);
	//double c = str.toDouble();
	//double a = pow(10, 0 / 55);
	//c = c - a;
	//str = QString::number(c);
	b_GetAuthority = false;
	m_sSelectFile = "";
	m_strSaveFile = "";
	m_strTrainFile = "";
	m_bChanged = false;
	m_bRectChanged = false;
	m_timerReadList = nullptr;
	clickedtimer = nullptr;
	ui.setupUi(this);
<<<<<<< HEAD
	ui.tableWidget->setColumnCount(2);
	//QMyTreeWidget *qmt;
	//qmt->ReadYAMLFile("D:/work/Qt_Test/CheckParam.yaml");
	QStringList LIST;
	LIST << QString::fromLocal8Bit("��Ŀ") << QString::fromLocal8Bit("����");
	ui.tableWidget->setHorizontalHeaderLabels(LIST);
	ui.tableWidget->setColumnWidth(1, 40);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->viewport()->installEventFilter(this);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(35);//Ĭ���и�20
=======
// 	ui.tableWidget->setColumnCount(2);
// 	QStringList LIST;
// 	LIST << QString::fromLocal8Bit("��Ŀ") << QString::fromLocal8Bit("����");
// 	ui.tableWidget->setHorizontalHeaderLabels(LIST);
// 	ui.tableWidget->setColumnWidth(1, 40);
// 	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
// 	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
// 	ui.tableWidget->viewport()->installEventFilter(this);
// 	ui.tableWidget->verticalHeader()->setDefaultSectionSize(35);//Ĭ���и�20
>>>>>>> ff61316fb6997015f5341c9e555eab343b892193
	pp = parent;
	HTuple hv_WindowID;
	open_window(ui.ShowLabel->y(), ui.ShowLabel->x(), ui.ShowLabel->width(), ui.ShowLabel->height(), (long)((QWidget*)ui.ShowLabel->parent())->winId(), "", "", &hv_WindowID);
	m_WND = hv_WindowID[0];
	ui.ShowLabel->setMouseTracking(true);
//	ui.tableWidget->setMouseTracking(true);
	ui.lw_ImageList->viewport()->installEventFilter(this);

	bool b = ui.lw_ImageList->hasMouseTracking();
	setMouseTracking(true);
	m_bButton = false;
	m_SAppPath = qApp->applicationDirPath();//exe�ļ���
	m_sImageListPath = m_SAppPath;
	initImageLS(m_sImageListPath);
	connect(ui.lw_ImageList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onClickedImage(QListWidgetItem *)));
	//connect(ui.lw_ImageList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onSelectImageList(QListWidgetItem *)));
	connect(ui.pB_ContinueTest, SIGNAL(toggled(bool)), this, SLOT(onContinueTest(bool)));
	connect(ui.pB_Save, SIGNAL(clicked()), this, SLOT(onSaveParam()));
	connect(ui.pB_Exit, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "�˳�ȷ��", "ȷ���˳��㷨����ҳ����", "ȷ��", "ȡ��"))
		{
			close();
		}});
	//����check parent �㷨��ʾλ��
	connect(this, SIGNAL(SignShowImage(int, Mat, int)), this, SLOT(SLOTShowImage(int, Mat, int)));
	connect(ui.pB_Calibration, SIGNAL(clicked()), this, SLOT(onAutoDetest()));
	p_Parent = AlgPointer;
	((CInterCHeck*)p_Parent)->StartCheck("", nullptr, m_MOriginal.cols, m_MOriginal.rows);
	((CInterCHeck*)p_Parent)->SetShowCallBack(this, ShowCallBack);
//	QObject::connect(ui.tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(onCellChanged(int, int)));
	QDesktopWidget *dwsktopwidget = QApplication::desktop();
	QRect deskrect = dwsktopwidget->availableGeometry();
	QRect screenrect = dwsktopwidget->screenGeometry();
	if (deskrect.height() < 1100)
	{
		this->resize(768 * 2 + 5, 1366 / 2);
		ui.groupBox->move(ui.ShowLabel->pos().x() + ui.ShowLabel->width() + 2, ui.ShowLabel->pos().y());
		//ui.lw_ImageList->move(ui.tableWidget->pos().x() + ui.tableWidget->width() + 2, ui.tableWidget->pos().y());
	}
//	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onPopKeyboard(int, int)));
	connect(ui.treeWidget, &QMyTreeWidget::TempSave, [=]
	{
		ui.treeWidget->SaveYAMLFile(qApp->applicationDirPath()+"/Temp.yaml");
		((CInterCHeck*)p_Parent)->LoadCheckParam(&_checkparam,&QString(qApp->applicationDirPath()+"/Temp.yaml"));
		m_bChanged = true;
	});
}
void QtGuiSetting::mouseReleaseEvent(QMouseEvent *p)
{
	setCursor(Qt::ArrowCursor);
	m_bButton = false;
	hideKeyBoard();


	return;
}
bool QtGuiSetting::eventFilter(QObject * watched, QEvent * event)
{
	if (watched == ui.lw_ImageList->viewport())
	{
		if (event->type() == QEvent::MouseButtonRelease)
		{
			hideKeyBoard();
		}
	}
	if (watched->metaObject()->className() == QStringLiteral("QComboBox"))
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			hideKeyBoard();
		}
	}
	return QWidget::eventFilter(watched, event);
}
void QtGuiSetting::resizeEvent(QResizeEvent * event)
{
	try
	{
		// 		Mat tempgray;
		// 		if (m_MOriginal.rows == 0 || m_MOriginal.cols == 0)
		// 		{
		// 			return;
		// 		}
		// 		cvtColor(m_MOriginal, tempgray, COLOR_BGR2RGB);
		// 		onShowImage(&tempgray);
		if (m_MOriginal.empty())
		{
			return;
		}
		Hobject ho_Image = Mat2Hobject(m_MOriginal);
		HTuple hv_Width, hv_Height;
		get_image_size(ho_Image, &hv_Width, &hv_Height);
		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		disp_obj(ho_Image, m_WND);
	}
	catch (...)
	{
		return;
	}
	return;
}
void QtGuiSetting::mousePressEvent(QMouseEvent *p)
{
	if (ui.ShowLabel->geometry().contains(this->mapFromGlobal(QCursor::pos())) && p->button() == Qt::LeftButton)
	{
		hideKeyBoard();
		HTuple hv_Width, hv_Height;
		if (m_MOriginal.empty())
		{
			return;
		}
		Hobject ho_Image = Mat2Hobject(m_MOriginal);
		get_image_size(ho_Image, &hv_Width, &hv_Height);
		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		disp_obj(ho_Image, m_WND);
		return;
	}
	return;
}
void QtGuiSetting::mouseMoveEvent(QMouseEvent *p)
{
	// 	QPoint poi = QCursor::pos();
	// 	if (ui.ShowLabel->geometry().contains(this->mapFromGlobal(poi)) && b_GetAuthority)
	// 	{
	// 		setCursor(Qt::CrossCursor);
	// 		m_PointFinale = p->pos();
	// 		if (m_bButton)
	// 		{
	// 			QRect rec = ui.ShowLabel->geometry();
	// 			QPoint pt = p->pos();
	// 			pt.setX(pt.x() - rec.x() - ui.ShowLabel->frameWidth());
	// 			pt.setY(pt.y() - rec.y() - ui.ShowLabel->frameWidth());
	// 			double scale_x = m_MOriginal.cols*1.0 / (rec.width() - ui.ShowLabel->frameWidth() * 2);
	// 			double scale_y = m_MOriginal.rows*1.0 / (rec.height() - ui.ShowLabel->frameWidth() * 2);
	// 			if (pt.x() > m_PointOriginal.x())
	// 			{
	// 				m_RectToDraw.setLeft(m_PointOriginal.x());
	// 				m_RectToDraw.setRight(pt.x());
	// 			}
	// 			else
	// 			{
	// 				m_RectToDraw.setRight(m_PointOriginal.x());
	// 				m_RectToDraw.setLeft(pt.x());
	// 			}
	// 			if (pt.y() > m_PointOriginal.y())
	// 			{
	// 				m_RectToDraw.setTop(m_PointOriginal.y());
	// 				m_RectToDraw.setBottom(pt.y());
	// 			}
	// 			else
	// 			{
	// 				m_RectToDraw.setBottom(m_PointOriginal.y());
	// 				m_RectToDraw.setTop(pt.y());
	// 			}
	// 			cv::Mat img2show;
	// 			cvtColor(m_MOriginal, img2show, COLOR_BGR2RGB);
	// 			m_RectOpencv = cv::Rect(m_RectToDraw.x()*scale_x, m_RectToDraw.y()*scale_y, m_RectToDraw.width()*scale_x, m_RectToDraw.height()*scale_y);
	// 			cv::rectangle(img2show, m_RectOpencv, cv::Scalar(255, 0, 0), 5, 8, 0);
	// 			onShowImage(&img2show);
	// 			m_bChanged = true;
	// 		}
	// 	}
	// 	else
	// 		setCursor(Qt::ArrowCursor);
	return;
}
QtGuiSetting::~QtGuiSetting()
{
}
void QtGuiSetting::SLOTShowImage(int pos, Mat img, int times)
{
	int zz = ui.ShowLabel->frameWidth();
	QSize ss = ui.ShowLabel->size();
	ss.setWidth(ss.width() - zz * 2);
	ss.setHeight(ss.height() - zz * 2);
	Mat imgsend;
	if (img.channels() == 1)
	{
		cv::cvtColor(img, imgsend, COLOR_GRAY2BGR);
	}
	else if (img.channels() == 3)
	{
		cv::cvtColor(img, imgsend, COLOR_BGR2RGB);
	}
	cv::resize(imgsend, imgsend, Size(ss.width(), ss.height()));
	QImage disImage = QImage((const unsigned char*)(imgsend.data), imgsend.cols, imgsend.rows, imgsend.step, QImage::Format_RGB888);
	ui.ShowLabel->setPixmap(QPixmap::fromImage(disImage));
}
void QtGuiSetting::onContinueTest(bool b)
{
	if (b)
	{
		if (nullptr == m_timerReadList)
		{
			m_timerReadList = new QTimer();
			connect(m_timerReadList, SIGNAL(timeout()), this, SLOT(onTimerReadList()));
			m_timerReadList->start(1000);
		}
	}
	else
	{
		if (nullptr != m_timerReadList)
		{
			m_timerReadList->stop();
			delete m_timerReadList;
			m_timerReadList = nullptr;
		}
	}
}
void QtGuiSetting::onTimerReadList()
{
	if (ui.lw_ImageList->count() < 3)
	{
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("�������Դ���"), QString::fromLocal8Bit("δ��λ��ͼ���ļ�"));
		return;
	}
	int index_list = ui.lw_ImageList->currentRow();
	if (index_list == ui.lw_ImageList->count() - 1)
	{
		m_timerReadList->stop();
		delete m_timerReadList;
		m_timerReadList = nullptr;
		ui.pB_ContinueTest->setChecked(false);
		return;
	}
	if (index_list < 2)
	{
		ui.lw_ImageList->setCurrentRow(2);
		onSelectImageList(ui.lw_ImageList->currentItem());
	}
	else
	{
		ui.lw_ImageList->setCurrentRow(index_list + 1);
		onSelectImageList(ui.lw_ImageList->currentItem());
	}
}
void QtGuiSetting::onSaveParam()
{
	if (m_bChanged)
	{
		if (QMessageBox::Save == QMessageBox::question(this, QString::fromLocal8Bit("�������޸�"), QString::fromLocal8Bit("�Ƿ񱣴棿"), QMessageBox::Save, QMessageBox::No))
		{
			std::ofstream fout(QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml").toStdString());

			fout << m_node;

			fout.close();

			try
			{
			}
			catch (YAML::ParserException e)
			{
				QMessageBox::warning(nullptr, "", e.what());
			}
			catch (YAML::RepresentationException e)
			{
				QMessageBox::warning(nullptr, "", e.what());
			}
			catch (YAML::Exception e)
			{
				QMessageBox::warning(nullptr, "", e.what());
			}
			m_bChanged = false;
			ui.pB_Save->setEnabled(false);
		}
	}
}
Hobject QtGuiSetting::Mat2Hobject(Mat& image)
{
	Hobject Hobj = Hobject();
	int hgt = image.rows;
	int wid = image.cols;
	int i;
	//	CV_8UC3
	if (image.empty())
		return Hobject();
	if (image.type() == CV_8UC3)
	{
		split(image, imgchannel);
		Mat imgB = imgchannel[0];
		Mat imgG = imgchannel[1];
		Mat imgR = imgchannel[2];
		if (dataR == NULL)
		{
			dataR = new uchar[hgt*wid];
		}
		if (dataG == NULL)
		{
			dataG = new uchar[hgt*wid];
		}
		if (dataB == NULL)
		{
			dataB = new uchar[hgt*wid];
		}
		for (i = 0; i < hgt; i++)
		{
			memcpy(dataR + wid * i, imgR.data + imgR.step*i, wid);
			memcpy(dataG + wid * i, imgG.data + imgG.step*i, wid);
			memcpy(dataB + wid * i, imgB.data + imgB.step*i, wid);
		}
		gen_image3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
	}
	//	CV_8UCU1
	else if (image.type() == CV_8UC1)
	{
		uchar* data = new uchar[hgt*wid];
		for (i = 0; i < hgt; i++)
			memcpy(data + wid * i, image.data + image.step*i, wid);
		gen_image1(&Hobj, "byte", wid, hgt, (Hlong)data);
		delete[] data;
		data = NULL;
	}
	return Hobj;
}
Mat QtGuiSetting::Hobject2Mat(Hobject Hobj)
{
	HTuple htCh = HTuple();
	HTuple cType;
	Mat Image;
	convert_image_type(Hobj, &Hobj, "byte");
	count_channels(Hobj, &htCh);
	HTuple wid;
	HTuple hgt;
	int W, H;
	if (htCh[0].I() == 1)
	{
		HTuple ptr;
		get_image_pointer1(Hobj, &ptr, &cType, &wid, &hgt);
		W = (Hlong)wid[0].I();
		H = (Hlong)hgt[0].I();
		Image.create(H, W, CV_8UC1);
		uchar* pdata = (uchar*)ptr[0].I();
		memcpy(Image.data, pdata, W*H);
	}
	else if (htCh[0].I() == 3)
	{
		HTuple ptrR, ptrG, ptrB;
		get_image_pointer3(Hobj, &ptrR, &ptrG, &ptrB, &cType, &wid, &hgt);
		W = (Hlong)wid[0].I();
		H = (Hlong)hgt[0].I();
		Image.create(H, W, CV_8UC3);
		vector<Mat> vecM(3);
		vecM[2].create(H, W, CV_8UC1);
		vecM[1].create(H, W, CV_8UC1);
		vecM[0].create(H, W, CV_8UC1);
		uchar* pr = (uchar*)ptrR[0].I();
		uchar* pg = (uchar*)ptrG[0].I();
		uchar* pb = (uchar*)ptrB[0].I();
		memcpy(vecM[2].data, pr, W*H);
		memcpy(vecM[1].data, pg, W*H);
		memcpy(vecM[0].data, pb, W*H);
		merge(vecM, Image);
	}
	return Image;
}
void QtGuiSetting::onAutoDetest()
{
}

void QtGuiSetting::onSelectImageChannel(int channels)
{
	QComboBox* obj = qobject_cast<QComboBox*>(sender());//�ж����ĸ���ť�����˲ۺ���
	// Local iconic variables 
	Hobject  ho_Image1, ho_Image2, ho_Image3;
	Hobject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
	Hobject  ho_ImageSub2, ho_EmptyRegion_Out, ho_EmptyRegion_OCR;
	Hobject  ho_EmptyRegion_Top, ho_EmptyRegion_Inner, ho_EmptyRegion_TopUP;
	Hobject  ho_EmptyRegion_Intensity, ho_ImageMax2, ho_Regions;
	Hobject  ho_Regions_Leak, ho_Region, ho_RegionOpening2, ho_RegionOpening;
	Hobject  ho_ConnectedRegions2, ho_SelectedRegions1, ho_RegionOpening_CAPOOT;
	Hobject  ho_ImageReduced, ho_ImageMax, ho_ImageSub, ho_Region1;
	Hobject  ho_RegionClosing, ho_Regions_Convex, ho_Regionx;
	Hobject  ho_Region_1st, ho_Regionsx, ho_Regions_2nd, ho_Region_OCR;
	Hobject  ho_Rectangle, ho_RegionIntersection5, ho_ConnectedRegions;
	Hobject  ho_SelectedRegions, ho_RegionOpening8, ho_RegionTrans2;
	Hobject  ho_RegionIntersection, ho_RegionErosion, ho_RegionIntersection4;
	Hobject  ho_Regions_LeakTOPError, ho_RegionErosion3, ho_RegionIntersection3;
	Hobject  ho_RegionOpening5, ho_Rectangle2, ho_RegionIntersection7;
	Hobject  ho_ConnectedRegions4, ho_SelectedRegions5, ho_RegionOpening9;
	Hobject  ho_RegionTrans3, ho_Rectangle1, ho_RegionIntersection8;
	Hobject  ho_RegionErosion1, ho_RegionIntersection2, ho_Regions_LeakTOPError2;
	Hobject  ho_RegionErosion6, ho_RegionIntersection9, ho_RegionOpening10;
	// Local control variables 
	HTuple  hv_ImageFiles, hv_Index, hv_Pointer, hv_Type;
	HTuple  hv_Width, hv_Height, hv_Tuple_Error, hv_Co_Index;
	HTuple  hv_Indices, hv_C_s, hv_R_s, hv_R_sEnd, hv_Area9;
	HTuple  hv_Row13, hv_Column13, hv_Row1, hv_Column1, hv_Row2;
	HTuple  hv_Column2, hv_Height_CAP, hv_Row14, hv_Column14;
	HTuple  hv_Row23, hv_Column23, hv_H1, hv_Row15, hv_Column15;
	HTuple  hv_Row24, hv_Column24, hv_H2, hv_h, hv_Area8, hv_Row10;
	HTuple  hv_Column10, hv_Row11, hv_Column11, hv_Row21, hv_Column21;
	HTuple  hv_Mean, hv_Deviation, hv_Area5, hv_Row7, hv_Column7;
	HTuple  hv_Row12, hv_Column12, hv_Row22, hv_Column22, hv_Mean2;
	HTuple  hv_Deviation2, hv_Area7, hv_Row9, hv_Column9, hv_Area2;
	HTuple  hv_Row4, hv_Column4, hv_Area3, hv_Row5, hv_Column5;
	HTuple  hv_Area, hv_Row, hv_Column, hv_Max1, hv_Max;
	if (m_MOriginal.empty())
	{
		return;
	}
	Hobject ho_Image = Mat2Hobject(m_MOriginal);
	decompose3(ho_Image, &ho_Image1, &ho_Image2, &ho_Image3);
	trans_from_rgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2, &ho_ImageResult3, "hsv");
	get_image_size(ho_Image, &hv_Width, &hv_Height);
	set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
	if (obj->objectName() == "BandChannel")
	{
		//_checkparam.i_BandChannel = channels;
	}
	// 	if (obj->objectName() == "1stChannel")
	// 	{
	// 		_checkparam.i_1stChannel = channels;
	// 	}
	// 	if (obj->objectName() == "2ndChannel")
	// 	{
	// 		_checkparam.i_2ndChannel = channels;
	// 	}
	switch (channels)
	{
	case 0:
		disp_obj(ho_Image1, m_WND);
		break;
	case 1:
		disp_obj(ho_Image2, m_WND);
		break;
	case 2:
		disp_obj(ho_Image3, m_WND);
		break;
	case 3:
		disp_obj(ho_ImageResult1, m_WND);
		break;
	case 4:
		disp_obj(ho_ImageResult2, m_WND);
		break;
	case 5:
		disp_obj(ho_ImageResult3, m_WND);
		break;
	default:
		break;
	}
	m_bChanged = true;
}
void QtGuiSetting::onPopKeyboard(int, int)
{
// 	if (nullptr == dlg_keyboard)
// 	{
// 		//QMessageBox::warning(nullptr, "", "123");
// 		dlg_keyboard = new QDialog(this);
// 		dlg_keyboard->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
// 		dlg_keyboard->resize(QSize(100, 50));
// 		QPushButton* pb = nullptr;
// 		int w = 50;
// 		for (int i = 0; i < 2; i++)
// 		{
// 			pb = new QPushButton(dlg_keyboard);
// 			if (i == 0) { pb->setObjectName("-1"); pb->setText("-1"); };
// 	/*		if (i == 1) { pb->setObjectName("-0.5"); pb->setText("-0.5"); };
// 			if (i == 2) { pb->setObjectName("0.5");  pb->setText("+0.5"); };*/
// 			if (i == 1) { pb->setObjectName("1");  pb->setText("+1"); };
// 			pb->resize(50, 50);
// 			pb->move(50 * i, 0);
// 			pb->setCheckable(true);
// 			//connect(pb, SIGNAL(toggled(bool)), this, SLOT(onbtnchange(bool)));
// 			connect(pb, SIGNAL(pressed()), this, SLOT(onbtnpressed()));
// 			connect(pb, SIGNAL(released()), this, SLOT(onbtnreleased()));
// 
// 		}
// 	}
// 	QPoint p = this->cursor().pos();
// 
// //	QPoint q = ui.tableWidget->mapToGlobal(QPoint(0, 0));
// 	dlg_keyboard->show();
// 	//QMessageBox::warning(nullptr, "", QString::number(q.x() + ui.tableWidget->width()) +","+ QString::number(p.y()));
// //	dlg_keyboard->move(QPoint(q.x() + ui.tableWidget->width(), p.y()));
// 
// 	//QPoint p = this->cursor().pos();
// 	//QPoint q = (ui.tableWidget->pos());
// 	//dlg_keyboard->show();
// 	//dlg_keyboard->move(p.x(), p.y());
// 	//dlg_keyboard->move(mapToParent( QPoint(q.x() + /*3 * */ui.tableWidget->width(), p.y())));
// 	//dlg->move(QPoint(100, 100));
}
void QtGuiSetting::onbtnpressed()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	double d = btn->objectName().toDouble();
	bool b_checked = btn->isChecked();
	if (nullptr==m_timerChanger)
	{
		m_timerChanger = new QTimer();
		connect(m_timerChanger, SIGNAL(timeout()), this, SLOT(ontimechanger()));
		m_timerChanger->start(100);
	}
	if (1 == d)
	{
		/*QTableWidgetItem *item = ui.tableWidget->currentItem();
		item->setText(QString::number(item->text().toInt() + 1));*/
		m_ba1 = true;

	}
	if (0.5 == d)
	{
	/*	QTableWidgetItem *item = ui.tableWidget->currentItem();
		item->setText(QString::number(item->text().toDouble() + 0.5));*/
		//m_ba05 = true;
	}
	if (-0.5 == d)
	{
		/*QTableWidgetItem *item = ui.tableWidget->currentItem();
		item->setText(QString::number(item->text().toDouble() - 0.5));*/
		//m_bm05 = true;
	}
	if (-1 == d)
	{
		/*	QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toInt() - 1));*/
		m_bm1 = true;
	}
	
	//if (b_checked)
	//{
	//	btn->setChecked(false);
		
	//}
}
void QtGuiSetting::onbtnreleased()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	double d = btn->objectName().toDouble();
	bool b_checked = btn->isChecked();
	{
		btn->setChecked(false);
	if (b_checked)
	 
		if (nullptr!=m_timerChanger)
		{
			m_timerChanger->stop();
			delete m_timerChanger;
			m_timerChanger = nullptr;
		}
		m_itimechange = 0;
		m_itimestep = 5;
		m_bm1 = false;
		m_bm05 = false;
		m_ba05 = false;
		m_ba1 = false;
	
		/*if (1 == d)
		{
			QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toInt() + 1));

		}
		if (0.5 == d)
		{
			QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toDouble() + 0.5));
		}
		if (-0.5 == d)
		{
			QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toDouble() - 0.5));
		}
		if (-1 == d)
		{
			QTableWidgetItem *item = ui.tableWidget->currentItem();
			item->setText(QString::number(item->text().toInt() - 1));
		}*/
	}
}

void QtGuiSetting::ontimechanger()
{
// 	try
// 	{
// 		if (m_itimechange%m_itimestep == 0)
// 		{
// 			if (m_ba1)
// 			{
// 				QTableWidgetItem *item = ui.tableWidget->currentItem();
// 				if (0>item->text().toDouble())
// 				{
// 					item->setText("0");
// 				}
// 				if (m_itimechange < 24)
// 				{
// 					/*item->setText(QString::number(item->text().toDouble() + pow(10, m_itimechange++%m_itimestep-1)));*/
// 				}
// 				else
// 				{
// 					m_itimestep = 2;
// 					//ui.tableWidget->blockSignals(true);
// 				}
// 				item->setText(QString::number(item->text().toDouble() + pow(10, m_itimechange / 55)));
// 
// 				//else if (m_itimechange < 100)
// 				//{
// 				//	//m_itimestep = 2;
// 				//	item->setText(QString::number(item->text().toDouble() + 10));
// 				//}
// 			}
// 			if (m_ba05)
// 			{
// 				QTableWidgetItem *item = ui.tableWidget->currentItem();
// 				item->setText(QString::number(item->text().toDouble() + 0.5));
// 			}
// 			if (m_bm05)
// 			{
// 				QTableWidgetItem *item = ui.tableWidget->currentItem();
// 				item->setText(QString::number(item->text().toDouble() - 0.5));
// 			}
// 			if (m_bm1)
// 			{
// 				QTableWidgetItem *item = ui.tableWidget->currentItem();
// 				if (m_itimechange < 24)
// 				{
// 					/*item->setText(QString::number(item->text().toDouble() + pow(10, m_itimechange++%m_itimestep-1)));*/
// 				}
// 				else
// 				{
// 					m_itimestep = 2;
// 					//ui.tableWidget->blockSignals(true);
// 
// 				}
// 				item->setText(QString::number(item->text().toDouble() - pow(10, m_itimechange / 55)));
// 			
// 			}
// 		}
// 	}
// 	catch (Exception e)
// 	{
// 		QMessageBox::warning(nullptr, "", e.what());
// 	}
// 		
// 
// 	
// 	m_itimechange++;
// 	ui.tableWidget->blockSignals(false);
}

void QtGuiSetting::hideKeyBoard()
{
	if (nullptr != dlg_keyboard)
	{
		if (!dlg_keyboard->isHidden())
		{
			dlg_keyboard->hide();
		}
	}
}
void QtGuiSetting::SetParam(CHECKPARAM _param)
{
	m_node = YAML::LoadFile(QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml").toStdString());
	_checkparam = _param;
	ui.treeWidget->ReadYAMLFile(m_node,_checkparam.c_CameraName);
}
void QtGuiSetting::onCellChanged(int r, int c)
{
// 	try
// 	{
// 		c = 1;//��ֹ����table
// 		Mat MatToShow(m_MOriginal);
// 		//Local iconic variables 
// 		Hobject  ho_R, ho_G, ho_B, ho_H, ho_S;
// 		Hobject  ho_V, ho_Region, ho_RegionFillUp2, ho_ConnectedRegions;
// 		Hobject  ho_RegionBand, ho_ImageReduced, ho_Regions, ho_RegionOpening;
// 		Hobject  ho_RegionFillUp, ho_ConnectedRegions1, ho_SelectedRegions;
// 		Hobject  ho_RegionPill, ho_EmptyPill, ho_RegionUnion5, ho_RegionTrans3;
// 		Hobject  ho_RegionDifference3, ho_TopPills, ho_BottomPills;
// 		Hobject  ho_SortedRegions, ho_ObjectSelected, ho_ObjectSelected1;
// 		Hobject  ho_RegionDilation1, ho_ConnectedRegions9, ho_Rectangle2;
// 		Hobject  ho_RegionDifference2, ho_RegionTrans2, ho_RegionTrans;
// 		Hobject  ho_RegionUnion, ho_Rectangle, ho_RegionDifference;
// 		Hobject  ho_RegionErosion, ho_ConnectedRegions16, ho_SelectedRegions10;
// 		Hobject  ho_ImageMax, ho_ImageSub1, ho_Region4, ho_RegionOpening1;
// 		Hobject  ho_RegionIntersection, ho_RegionClosing, ho_ConnectedRegions3;
// 		Hobject  ho_SelectedRegions2, ho_RegionTrans1, ho_RegionUnion1;
// 		Hobject  ho_Rectangle1, ho_RegionIntersection9, ho_ImageReduced4;
// 		Hobject  ho_Region11, ho_RegionOpening6, ho_ConnectedRegions8;
// 		Hobject  ho_SelectedRegions5, ho_Circle3, ho_RegionArc, ho_RegionErosion6;
// 		Hobject  ho_ImageReduced12, ho_Region18, ho_RegionFillUp6;
// 		Hobject  ho_RegionOpening15, ho_ConnectedRegions22, ho_SelectedRegions17;
// 		Hobject  ho_RegionTrans7, ho_Rectangle4, ho_Rectangle7, ho_RegionDifference15;
// 		Hobject  ho_RegionErosion2, ho_ImageReduced1, ho_Region1;
// 		Hobject  ho_ConnectedRegions4, ho_SelectedRegions4, ho_RegionUnion3;
// 		Hobject  ho_RegionRed, ho_ImageReduced9, ho_Region15, ho_RegionIntersection6;
// 		Hobject  ho_RegionFillUp4, ho_RegionOpening9, ho_RegionErosion4;
// 		Hobject  ho_Region3, ho_RegionYellow, ho_RegionDilation2;
// 		Hobject  ho_RegionDilation3, ho_RegionCross, ho_ImageReduced2;
// 		Hobject  ho_ImageMax1, ho_ImageSub3, ho_Regions2, ho_Region2;
// 		Hobject  ho_RegionIntersection1, ho_RegionClosing1, ho_RegionOpening5;
// 		Hobject  ho_ConnectedRegions2, ho_SelectedRegions1, ho_ImageReduced5;
// 		Hobject  ho_ImageMax5, ho_ImageSub7, ho_Region6, ho_RegionErosion3;
// 		Hobject  ho_RegionDifference8, ho_RegionDifference9, ho_RegionOpening8;
// 		Hobject  ho_ConnectedRegions15, ho_SelectedRegions9, ho_RegionUnion2;
// 		Hobject  ho_RegionDifference19, ho_ConnectedRegions7, ho_Circle1;
// 		Hobject  ho_ImageReduced3, ho_ImageMax2, ho_ImageSub, ho_Region5;
// 		Hobject  ho_Region8, ho_RegionIntersection3, ho_RegionDifference7;
// 		Hobject  ho_ConnectedRegions14, ho_RegionOpening3, ho_SelectedRegions11;
// 		Hobject  ho_RegionDifference18, ho_ConnectedRegions10, ho_Circle2;
// 		Hobject  ho_ImageMax3, ho_ImageSub4, ho_Region9, ho_RegionIntersection13;
// 		Hobject  ho_ConnectedRegions24, ho_SelectedRegions19, ho_RegionUnion7;
// 		Hobject  ho_RegionClosing7, ho_ConnectedRegions23, ho_SelectedRegions18;
// 		Hobject  ho_ImageReduced13, ho_Regions1, ho_RegionClosing8;
// 		Hobject  ho_RegionFillUp7, ho_RegionOpening17, ho_ConnectedRegions25;
// 		Hobject  ho_SelectedRegions20, ho_RegionTrans8, ho_RegionUnion8;
// 		Hobject  ho_RegionDifference17, ho_RegionIntersection14;
// 		Hobject  ho_RegionFillUp8, ho_RegionOpening18, ho_ConnectedRegions26;
// 		Hobject  ho_SelectedRegions21, ho_RegionUnion9, ho_ConnectedRegions27;
// 		Hobject  ho_Circle;
// 
// 		// Local control variables 
// 		HTuple  hv_ImageFiles, hv_Index, hv_BaseName;
// 		HTuple  hv_Extension, hv_Directory, hv_AreaBand, hv_ExpDefaultCtrlDummyVar;
// 		HTuple  hv_NumberPill, hv_Row11, hv_Column11, hv_Row21;
// 		HTuple  hv_Column21, hv_Area1, hv_Row7, hv_Column7, hv_Mean;
// 		HTuple  hv_Number1, hv_Index1, hv_Number5, hv_Index2, hv_Number6;
// 		HTuple  hv_Row6, hv_Column6, hv_Phi2, hv_Length12, hv_Length22;
// 		HTuple  hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2;
// 		HTuple  hv_Number3, hv_Row1, hv_Column1, hv_Phi1, hv_Length11;
// 		HTuple  hv_Length21, hv_Value, hv_Value1, hv_ratio, hv_Number4;
// 		HTuple  hv_Row5, hv_Column5, hv_Radius3, hv_Number, hv_Row3;
// 		HTuple  hv_Column3, hv_Radius1, hv_Area, hv_Row4, hv_Column4;
// 		HTuple  hv_Radius2, hv_Row8, hv_Column8, hv_Phi3, hv_Length13;
// 		HTuple  hv_Length23, hv_Number8, hv_Row2, hv_Column2, hv_Radius;
// 		HTuple  hv_UsedThreshold, hv_Width, hv_Height;
// 
// 		Hobject ho_ImageChannel[6];
// #pragma region setvalue
// 
// 		_checkparam.i_BandChannel = ((QComboBox*)(ui.tableWidget->cellWidget(0, 1)))->currentIndex();
// 		//_checkparam.i_BandChannel = ui.tableWidget->item(0, c)->text().toInt();
// 		//_checkparam.i_PillChannel1 = ((QComboBox*)(ui.tableWidget->cellWidget(3, 1)))->currentIndex();
// 		_checkparam.i_MinGray_Band = ui.tableWidget->item(1, c)->text().toInt();
// 		if (_checkparam.i_MinGray_Band <= 0 || _checkparam.i_MinGray_Band>=255)
// 		{
// 			if (nullptr!=m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 
// 		}
// 		_checkparam.i_MinArea_Band = ui.tableWidget->item(2, c)->text().toInt();
// 		if (_checkparam.i_MinArea_Band<=0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 
// 		}
// 		_checkparam.i_CapsulesChannel = ((QComboBox*)(ui.tableWidget->cellWidget(3, 1)))->currentIndex();
// 		_checkparam.i_MinGray_Capsules = ui.tableWidget->item(4, c)->text().toInt();
// 		if (_checkparam.i_MinGray_Capsules <= 0 || _checkparam.i_MinGray_Capsules >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_OR_Capsules = ui.tableWidget->item(5, c)->text().toDouble();
// 		if (_checkparam.d_OR_Capsules <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_Capsules = ui.tableWidget->item(6, c)->text().toInt();
// 		if (_checkparam.i_MinArea_Capsules <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxArea_Capsules = ui.tableWidget->item(7, c)->text().toInt();
// 		if (_checkparam.i_MaxArea_Capsules <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_Number_Capsules = ui.tableWidget->item(8, c)->text().toInt();
// 		if (_checkparam.i_Number_Capsules <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_BatchChannel = ((QComboBox*)(ui.tableWidget->cellWidget(9, 1)))->currentIndex();
// 		_checkparam.i_LengthAdd_1 = ui.tableWidget->item(10, c)->text().toInt();
// 		if (_checkparam.i_LengthAdd_1 <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_LengthAdd_2 = ui.tableWidget->item(11, c)->text().toInt();
// 		if (_checkparam.i_LengthAdd_2 <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_ER_Batch = ui.tableWidget->item(12, c)->text().toDouble();
// 		if (_checkparam.d_ER_Batch <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskHeight_Batch = ui.tableWidget->item(13, c)->text().toInt();
// 		if (_checkparam.i_MaskHeight_Batch <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskWidth_Batch = ui.tableWidget->item(14, c)->text().toInt();
// 		if (_checkparam.i_MaskWidth_Batch <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinGray_Batch = ui.tableWidget->item(15, c)->text().toInt();
// 		if (_checkparam.i_MinGray_Batch <= 0 || _checkparam.i_MinGray_Batch >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_OR_Batch = ui.tableWidget->item(16, c)->text().toDouble();
// 		if (_checkparam.d_OR_Batch <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_CR_Batch = ui.tableWidget->item(17, c)->text().toDouble();
// 		if (_checkparam.d_CR_Batch <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_Batch = ui.tableWidget->item(18, c)->text().toInt();
// 		if (_checkparam.i_MinArea_Batch <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxArea_Batch = ui.tableWidget->item(19, c)->text().toInt();
// 		if (_checkparam.i_MaxArea_Batch <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		//_checkparam.i_MinArea_BatchDefect = ui.tableWidget->item(20, c)->text().toInt();
// 		//_checkparam.i_MaxArea_BatchDefect = ui.tableWidget->item(21, c)->text().toInt();
// 		_checkparam.i_Channel_CapsulesDefect = ((QComboBox*)(ui.tableWidget->cellWidget(20, 1)))->currentIndex();
// 		_checkparam.d_ER_RegionCapsules = ui.tableWidget->item(21, c)->text().toDouble();
// 		if (_checkparam.d_ER_RegionCapsules <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxGray_CapsulesRed = ui.tableWidget->item(22, c)->text().toInt();
// 		if (_checkparam.i_MaxGray_CapsulesRed <= 0 || _checkparam.i_MaxGray_CapsulesRed >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_CapsulesRed = ui.tableWidget->item(23, c)->text().toInt();
// 		if (_checkparam.i_MinArea_CapsulesRed <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxArea_CapsulesRed = ui.tableWidget->item(24, c)->text().toInt();
// 		if (_checkparam.i_MaxArea_CapsulesRed <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_DR_CapsulesRed = ui.tableWidget->item(25, c)->text().toDouble();
// 		if (_checkparam.d_DR_CapsulesRed <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_DR_CapsulesYellow = ui.tableWidget->item(26, c)->text().toDouble();
// 		if (_checkparam.d_DR_CapsulesYellow <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_Channel_RegionRed = ((QComboBox*)(ui.tableWidget->cellWidget(27, 1)))->currentIndex();
// 		_checkparam.i_Channel_RedDefect1 = ((QComboBox*)(ui.tableWidget->cellWidget(28, 1)))->currentIndex();
// 		_checkparam.i_Channel_RedDefect2 = ((QComboBox*)(ui.tableWidget->cellWidget(29, 1)))->currentIndex();
// 		_checkparam.i_MaskHeight_RegionRed = ui.tableWidget->item(30, c)->text().toInt();
// 		if (_checkparam.i_MaskHeight_RegionRed <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskWidth_RegionRed = ui.tableWidget->item(31, c)->text().toInt();
// 		if (_checkparam.i_MaskWidth_RegionRed <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinGray_RegionRed = ui.tableWidget->item(32, c)->text().toInt();
// 		if (_checkparam.i_MinGray_RegionRed <= 0 || _checkparam.i_MinGray_RegionRed >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxGray_RegionRed = ui.tableWidget->item(33, c)->text().toInt();
// 		if (_checkparam.i_MaxGray_RegionRed <= 0 || _checkparam.i_MaxGray_RegionRed >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_CR_RegionRed = ui.tableWidget->item(34, c)->text().toDouble();
// 		if (_checkparam.d_CR_RegionRed <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_OR_RegionRed = ui.tableWidget->item(35, c)->text().toDouble();
// 		if (_checkparam.d_OR_RegionRed <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_RegionRed = ui.tableWidget->item(36, c)->text().toInt();
// 		if (_checkparam.i_MinArea_RegionRed <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxArea_RegionRed = ui.tableWidget->item(37, c)->text().toInt();
// 		if (_checkparam.i_MaxArea_RegionRed <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskHeight_RedDefect = ui.tableWidget->item(38, c)->text().toInt();
// 		if (_checkparam.i_MaskHeight_RedDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskWidth_RedDefect = ui.tableWidget->item(39, c)->text().toInt();
// 		if (_checkparam.i_MaskWidth_RedDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinGray_RedDefect = ui.tableWidget->item(40, c)->text().toInt();
// 		if (_checkparam.i_MinGray_RedDefect <= 0 || _checkparam.i_MinGray_RedDefect >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_ER_RedDefect = ui.tableWidget->item(41, c)->text().toDouble();
// 		if (_checkparam.d_ER_RedDefect <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_OR_RedDefect = ui.tableWidget->item(42, c)->text().toDouble();
// 		if (_checkparam.d_OR_RedDefect <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_RedDefect = ui.tableWidget->item(43, c)->text().toInt();
// 		if (_checkparam.i_MinArea_RedDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxArea_RedDefect = ui.tableWidget->item(44, c)->text().toInt();
// 		if (_checkparam.i_MaxArea_RedDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskHeight_RegionYellow = ui.tableWidget->item(45, c)->text().toInt();
// 		if (_checkparam.i_MaskHeight_RegionYellow <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskWidth_RegionYellow = ui.tableWidget->item(46, c)->text().toInt();
// 		if (_checkparam.i_MaskWidth_RegionYellow <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinGray_RegionYellow = ui.tableWidget->item(47, c)->text().toInt();
// 		if (_checkparam.i_MinGray_RegionYellow <= 0 || _checkparam.i_MinGray_RegionYellow >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxGray_RegionYellow = ui.tableWidget->item(48, c)->text().toInt();
// 		if (_checkparam.i_MaxGray_RegionYellow <= 0 || _checkparam.i_MaxGray_RegionYellow >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_OR_RegionYellow = ui.tableWidget->item(49, c)->text().toDouble();
// 		if (_checkparam.d_OR_RegionYellow <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_RegionYellow = ui.tableWidget->item(50, c)->text().toInt();
// 		if (_checkparam.i_MinArea_RegionYellow <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxArea_RegionYellow = ui.tableWidget->item(51, c)->text().toInt();
// 		if (_checkparam.i_MaxArea_RegionYellow <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		/*_checkparam.i_Channel_BandDefect = ((QComboBox*)(ui.tableWidget->cellWidget(52, 1)))->currentIndex();
// 		_checkparam.d_DR_BandDefect = ui.tableWidget->item(53, c)->text().toDouble();
// 		if (_checkparam.d_DR_BandDefect <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_ER_BandDefect = ui.tableWidget->item(54, c)->text().toDouble();
// 		if (_checkparam.d_ER_BandDefect <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskHeight_BandDefect = ui.tableWidget->item(55, c)->text().toInt();
// 		if (_checkparam.i_MaskHeight_BandDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskWidth_BandDefect = ui.tableWidget->item(56, c)->text().toInt();
// 		if (_checkparam.i_MaskWidth_BandDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinGray_BandDefect = ui.tableWidget->item(57, c)->text().toInt();
// 		if (_checkparam.i_MinGray_BandDefect <= 0 || _checkparam.i_MinGray_BandDefect >= 255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_CR_BandDefect = ui.tableWidget->item(58, c)->text().toDouble();
// 		if (_checkparam.d_CR_BandDefect <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinWidth_BandDefect = ui.tableWidget->item(59, c)->text().toInt();
// 		if (_checkparam.i_MinWidth_BandDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxWidth_BandDefect = ui.tableWidget->item(60, c)->text().toInt();
// 		if (_checkparam.i_MaxWidth_BandDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinHeight_BandDefect = ui.tableWidget->item(61, c)->text().toInt();
// 		if (_checkparam.i_MinHeight_BandDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxHeight_BandDefect = ui.tableWidget->item(62, c)->text().toInt();
// 		if (_checkparam.i_MaxHeight_BandDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_BandDefect = ui.tableWidget->item(63, c)->text().toInt();
// 		if (_checkparam.i_MinArea_BandDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxArea_BandDefect = ui.tableWidget->item(64, c)->text().toInt();
// 		if (_checkparam.i_MaxArea_BandDefect <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}*/
// 		_checkparam.i_Channel_InCapsules = ((QComboBox*)(ui.tableWidget->cellWidget(53, 1)))->currentIndex();
// 		_checkparam.d_ER_InCapsules = ui.tableWidget->item(54, c)->text().toDouble();
// 		if (_checkparam.d_ER_InCapsules <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaxGray_InCapsules = ui.tableWidget->item(55, c)->text().toInt();
// 		if (_checkparam.i_MaxGray_InCapsules <= 0 || _checkparam.i_MaxGray_InCapsules>=255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_OR_InCapsules = ui.tableWidget->item(56, c)->text().toDouble();
// 		if (_checkparam.d_OR_InCapsules <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_InCapsules = ui.tableWidget->item(57, c)->text().toInt();
// 		if (_checkparam.i_MinArea_InCapsules <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 
// 		_checkparam.i_MaxArea_InCapsules = ui.tableWidget->item(58, c)->text().toInt();
// 		if (_checkparam.i_MaxArea_InCapsules <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_LengthPlus_1 = ui.tableWidget->item(59, c)->text().toInt();
// 		if (_checkparam.i_LengthPlus_1 <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_LengthPlus_2 = ui.tableWidget->item(60, c)->text().toInt();
// 		if (_checkparam.i_LengthPlus_2 <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_Channel_Middle = ((QComboBox*)(ui.tableWidget->cellWidget(61, 1)))->currentIndex();
// 		_checkparam.i_MaskHeight_Middle = ui.tableWidget->item(62, c)->text().toInt();
// 		if (_checkparam.i_MaskHeight_Middle <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MaskWidth_Middle = ui.tableWidget->item(63, c)->text().toInt();
// 		if (_checkparam.i_MaskWidth_Middle <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinGray_Middle = ui.tableWidget->item(64, c)->text().toInt();
// 		if (_checkparam.i_MinGray_Middle <= 0||_checkparam.i_MinGray_Middle>=255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinWidth_Middle = ui.tableWidget->item(65, c)->text().toInt();
// 		if (_checkparam.i_MinWidth_Middle <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinHeight_Middle = ui.tableWidget->item(66, c)->text().toInt();
// 		if (_checkparam.i_MinHeight_Middle <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_CR_Middle = ui.tableWidget->item(67, c)->text().toDouble();
// 		if (_checkparam.d_CR_Middle <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_Middle = ui.tableWidget->item(68, c)->text().toInt();
// 		if (_checkparam.i_MinArea_Middle <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_Channel_Plate = ((QComboBox*)(ui.tableWidget->cellWidget(69, 1)))->currentIndex();
// 		_checkparam.i_MaxGray_Plate = ui.tableWidget->item(70, c)->text().toInt();
// 		if (_checkparam.i_MaxGray_Plate <= 0 || _checkparam.i_MaxGray_Plate>=255)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_CR_Plate = ui.tableWidget->item(71, c)->text().toDouble();
// 		if (_checkparam.d_CR_Plate <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_OR_Plate_1 = ui.tableWidget->item(72, c)->text().toDouble();
// 		if (_checkparam.d_OR_Plate_1 <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_Plate_1 = ui.tableWidget->item(73,c)->text().toInt();
// 		if (_checkparam.i_MinArea_Plate_1 <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.d_OR_Plate_2 = ui.tableWidget->item(74, c)->text().toDouble();
// 		if (_checkparam.d_OR_Plate_2 <= 0.5)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 		_checkparam.i_MinArea_Plate_2 = ui.tableWidget->item(75, c)->text().toInt();
// 		if (_checkparam.i_MinArea_Plate_2 <= 0)
// 		{
// 			if (nullptr != m_timerChanger)
// 			{
// 				m_timerChanger->stop();
// 				delete m_timerChanger;
// 				m_timerChanger = nullptr;
// 			}
// 		}
// 
// 
// 		m_bChanged = true;
// 		ui.pB_Save->setEnabled(true);
// 		//((CInterCHeck*)p_Parent)->RealCheck(m_MOriginal, _checkparam, m_WND);
// 
// #pragma endregion setvalue
// 		if (m_MOriginal.empty())
// 		{
// 			return;
// 		}
// 		
// 		Hobject ho_Image = Mat2Hobject(m_MOriginal);
// 		decompose3(ho_Image, &ho_ImageChannel[0], &ho_ImageChannel[1], &ho_ImageChannel[2]);
// 		trans_from_rgb(ho_ImageChannel[0], ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageChannel[3], &ho_ImageChannel[4], &ho_ImageChannel[5], "hsv");
// 		get_image_size(ho_Image, &hv_Width, &hv_Height);
// 		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
// 
// 		//***ҩ��ߴ�
// 		threshold(ho_ImageChannel[_checkparam.i_BandChannel], &ho_Region, _checkparam.i_MinGray_Band, 255);
// 		if (1==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region, m_WND);
// 			return;
// 		}
// 		fill_up(ho_Region, &ho_RegionFillUp2);
// 		connection(ho_RegionFillUp2, &ho_ConnectedRegions);
// 		select_shape_std(ho_ConnectedRegions, &ho_RegionBand, "max_area", 70);
// 		area_center(ho_RegionBand, &hv_AreaBand, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
// 		if (2==r)
// 		{
// 			if (hv_AreaBand < _checkparam.i_MinArea_Band)
// 			{
// 				disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
// 				set_draw(m_WND, "margin");
// 				set_color(m_WND, "red");
// 				set_line_width(m_WND, 3);
// 				disp_obj(ho_RegionBand, m_WND);
// 				set_tposition(m_WND, 10, 10);
// 				write_string(m_WND, "���ְ������" + hv_AreaBand);
// 			}
// 			else
// 			{
// 				disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
// 				set_draw(m_WND, "margin");
// 				set_color(m_WND, "green");
// 				set_line_width(m_WND, 3);
// 				disp_obj(ho_RegionBand, m_WND);
// 				set_tposition(m_WND, 10, 10);
// 				write_string(m_WND, "���ְ������" + hv_AreaBand);
// 			}
// 			return;
// 		}
// 		//if (0 != (hv_AreaBand < 740000))
// 		//{
// 		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//	disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
// 		//	result = QString::fromLocal8Bit("����ߴ����");
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "����ߴ����");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 80, 10);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰҩ�����Ϊ��" + hv_AreaBand);
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//	return 1;
// 		//	// stop(); only in hdevelop
// 		//}
// 
// 
// 		//**��������
// 		reduce_domain(ho_ImageChannel[_checkparam.i_CapsulesChannel], ho_RegionBand, &ho_ImageReduced);
// 		threshold(ho_ImageReduced, &ho_Regions, _checkparam.i_MinGray_Capsules, 255);
// 		if (4==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Regions, m_WND);
// 			return;
// 		}
// 		opening_circle(ho_Regions, &ho_RegionOpening, _checkparam.d_OR_Capsules);
// 		if (5==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionOpening, m_WND);
// 			return;
// 		}
// 		fill_up(ho_RegionOpening, &ho_RegionFillUp);
// 		connection(ho_RegionFillUp, &ho_ConnectedRegions1);
// 		select_shape(ho_ConnectedRegions1, &ho_SelectedRegions, "area", "and", _checkparam.i_MinArea_Capsules,
// 			_checkparam.i_MaxArea_Capsules);
// 		if (6==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions, m_WND);
// 			return;
// 		}
// 		if (7==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions, m_WND);
// 			return;
// 		}
// 		shape_trans(ho_SelectedRegions, &ho_RegionPill, "convex");
// 		count_obj(ho_RegionPill, &hv_NumberPill);
// 
// 		if (8==r)
// 		{
// 			if (hv_NumberPill == _checkparam.i_Number_Capsules)
// 			{
// 				disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 				set_draw(m_WND, "margin");
// 				set_color(m_WND, "green");
// 				set_line_width(m_WND, 3);
// 				disp_obj(ho_RegionPill, m_WND);
// 			}
// 			else
// 			{
// 				disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 				set_draw(m_WND, "margin");
// 				set_color(m_WND, "red");
// 				set_line_width(m_WND, 3);
// 				disp_obj(ho_RegionPill, m_WND);
// 			}
// 			return;
// 		}
// 	
// 		//if (0 != (HTuple(hv_NumberPill != 10).And(hv_NumberPill != 0)))
// 		//{
// 		//	gen_empty_obj(&ho_EmptyPill);
// 		//	if (0 != (hv_NumberPill == 5))
// 		//	{
// 		//		union1(ho_RegionPill, &ho_RegionUnion5);
// 		//		shape_trans(ho_RegionUnion5, &ho_RegionTrans3, "rectangle2");
// 		//		difference(ho_RegionBand, ho_RegionTrans3, &ho_RegionDifference3);
// 		//		inner_rectangle1(ho_RegionDifference3, &hv_Row11, &hv_Column11, &hv_Row21,
// 		//			&hv_Column21);
// 		//		gen_rectangle1(&ho_EmptyPill, hv_Row11, hv_Column11, hv_Row21, hv_Column21);
// 		//	}
// 		//	else
// 		//	{
// 		//		gen_empty_obj(&ho_TopPills);
// 		//		gen_empty_obj(&ho_BottomPills);
// 		//		sort_region(ho_RegionPill, &ho_SortedRegions, "first_point", "true", "row");
// 		//		area_center(ho_SortedRegions, &hv_Area1, &hv_Row7, &hv_Column7);
// 		//		tuple_mean(hv_Row7, &hv_Mean);
// 		//		count_obj(ho_SortedRegions, &hv_Number1);
// 		//		{
// 		//			HTuple end_val56 = hv_Number1;
// 		//			HTuple step_val56 = 1;
// 		//			for (hv_Index1 = 1; hv_Index1.Continue(end_val56, step_val56); hv_Index1 += step_val56)
// 		//			{
// 		//				select_obj(ho_SortedRegions, &ho_ObjectSelected, hv_Index1);
// 		//				if (0 != ((hv_Row7.Select(hv_Index1 - 1)) < hv_Mean))
// 		//				{
// 		//					concat_obj(ho_ObjectSelected, ho_TopPills, &ho_TopPills);
// 		//				}
// 		//				else
// 		//				{
// 		//					concat_obj(ho_ObjectSelected, ho_BottomPills, &ho_BottomPills);
// 		//				}
// 		//			}
// 		//		}
// 		//		union1(ho_TopPills, &ho_TopPills);
// 		//		union1(ho_BottomPills, &ho_BottomPills);
// 		//		concat_obj(ho_TopPills, ho_BottomPills, &ho_RegionPill);
// 		//		count_obj(ho_RegionPill, &hv_Number5);
// 		//		{
// 		//			HTuple end_val68 = hv_Number5;
// 		//			HTuple step_val68 = 1;
// 		//			for (hv_Index2 = 1; hv_Index2.Continue(end_val68, step_val68); hv_Index2 += step_val68)
// 		//			{
// 		//				select_obj(ho_RegionPill, &ho_ObjectSelected1, hv_Index2);
// 		//				dilation_circle(ho_ObjectSelected1, &ho_RegionDilation1, 15);
// 		//				connection(ho_RegionDilation1, &ho_ConnectedRegions9);
// 		//				count_obj(ho_ConnectedRegions9, &hv_Number6);
// 		//				if (0 != (hv_Number6 != 5))
// 		//				{
// 		//					smallest_rectangle2(ho_ObjectSelected1, &hv_Row6, &hv_Column6, &hv_Phi2,
// 		//						&hv_Length12, &hv_Length22);
// 		//					gen_rectangle2(&ho_Rectangle2, hv_Row6, hv_Column6, hv_Phi2, hv_Length12 + 20,
// 		//						hv_Length22 + 20);
// 		//					difference(ho_Rectangle2, ho_RegionDilation1, &ho_RegionDifference2);
// 		//					shape_trans(ho_RegionDifference2, &ho_RegionTrans2, "inner_rectangle1");
// 		//					concat_obj(ho_EmptyPill, ho_RegionTrans2, &ho_EmptyPill);
// 		//				}
// 		//			}
// 		//		}
// 		//		union1(ho_EmptyPill, &ho_EmptyPill);
// 		//	}
// 		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//	disp_obj(ho_EmptyPill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
// 		//	result = QString::fromLocal8Bit("���Ҹ�������");
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "���Ҹ�������");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//	return 1;
// 
// 		//	// stop(); only in hdevelop
// 		//}
// 		//else if (0 != (hv_NumberPill == 0))
// 		//{
// 		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
// 		//	result = QString::fromLocal8Bit("�޽���");
// 		//	disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "�޽���");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//	return 1;
// 		//	// stop(); only in hdevelop
// 		//}
// 		//**�������
// 		shape_trans(ho_RegionPill, &ho_RegionTrans, "convex");
// 		union1(ho_RegionTrans, &ho_RegionUnion);
// 		smallest_rectangle2(ho_RegionUnion, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
// 			&hv_Length2);
// 		gen_rectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1 + _checkparam.i_LengthAdd_1, hv_Length2 + _checkparam.i_LengthAdd_2);
// 		if (10==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "fill");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Rectangle, m_WND);
// 			return;
// 		}
// 		if (11==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "fill");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Rectangle, m_WND);
// 			return;
// 		}
// 		difference(ho_RegionBand, ho_Rectangle, &ho_RegionDifference);
// 		erosion_circle(ho_RegionDifference, &ho_RegionErosion, _checkparam.d_ER_Batch);
// 		if (12==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionErosion, m_WND);
// 			return;
// 		}
// 		connection(ho_RegionErosion, &ho_ConnectedRegions16);
// 		select_shape_std(ho_ConnectedRegions16, &ho_SelectedRegions10, "max_area", 70);
// 		gray_dilation_rect(ho_ImageChannel[_checkparam.i_BatchChannel], &ho_ImageMax, _checkparam.i_MaskHeight_Batch, _checkparam.i_MaskWidth_Batch);
// 		if (13==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax, m_WND);
// 			return;
// 		}
// 		if (14==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax, m_WND);
// 			return;
// 		}
// 		sub_image(ho_ImageMax, ho_ImageChannel[_checkparam.i_BatchChannel], &ho_ImageSub1, 1, 0);
// 		threshold(ho_ImageSub1, &ho_Region4, _checkparam.i_MinGray_Batch, 255);
// 		if (15==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region4, m_WND);
// 			return;
// 		}
// 		//dyn_threshold (ImageMax, G, RegionDynThresh, 10, 'light')
// 		opening_circle(ho_Region4, &ho_RegionOpening1, _checkparam.d_OR_Batch);
// 		if (16==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionOpening1, m_WND);
// 			return;
// 		}
// 		intersection(ho_RegionOpening1, ho_SelectedRegions10, &ho_RegionIntersection);
// 		closing_circle(ho_RegionIntersection, &ho_RegionClosing, _checkparam.d_CR_Batch);
// 		if (17==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionClosing, m_WND);
// 			return;
// 		}
// 		connection(ho_RegionClosing, &ho_ConnectedRegions3);
// 		select_shape(ho_ConnectedRegions3, &ho_SelectedRegions2, "area", "and", _checkparam.i_MinArea_Batch, _checkparam.i_MaxArea_Batch);
// 		if (18==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions2, m_WND);
// 			return;
// 		}
// 		if (19==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions2, m_WND);
// 			return;
// 		}
// 		shape_trans(ho_SelectedRegions2, &ho_RegionTrans1, "convex");
// 		union1(ho_RegionTrans1, &ho_RegionUnion1);
// 		count_obj(ho_RegionUnion1, &hv_Number3);
// 		//if (0 != (hv_Number3 == 0))
// 		//{
// 		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
// 		//	result = QString::fromLocal8Bit("������");
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "������");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//	// stop(); only in hdevelop
// 		//	return 1;
// 		//}
// 		//else
// 		//{
// 		//	smallest_rectangle2(ho_RegionUnion1, &hv_Row1, &hv_Column1, &hv_Phi1, &hv_Length11,
// 		//		&hv_Length21);
// 		//	gen_rectangle2(&ho_Rectangle1, hv_Row1, hv_Column1, hv_Phi1, hv_Length11, hv_Length21);
// 		//	region_features(ho_Rectangle1, "ra", &hv_Value);
// 		//	region_features(ho_RegionBand, "rb", &hv_Value1);
// 		//	hv_ratio = hv_Value / hv_Value1;
// 		//	if (0 != (hv_ratio < 0.3))
// 		//	{
// 		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//		//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
// 		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//		disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
// 		//		result = QString::fromLocal8Bit("����ӡˢ����");
// 		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "����ӡˢ������");
// 		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//		return 1;
// 		//		// stop(); only in hdevelop
// 		//	}
// 		//	intersection(ho_Rectangle1, ho_RegionBand, &ho_RegionIntersection9);
// 		//	reduce_domain(ho_ImageSub1, ho_RegionIntersection9, &ho_ImageReduced4);
// 		//	threshold(ho_ImageReduced4, &ho_Region11, 50, 255);
// 		//	opening_circle(ho_Region11, &ho_RegionOpening6, 1.5);
// 		//	connection(ho_RegionOpening6, &ho_ConnectedRegions8);
// 		//	select_shape(ho_ConnectedRegions8, &ho_SelectedRegions5, "area", "and", 50,
// 		//		99999);
// 		//	count_obj(ho_SelectedRegions5, &hv_Number4);
// 		//	if (0 != (hv_Number4 != 0))
// 		//	{
// 		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//		//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
// 		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//		smallest_circle(ho_SelectedRegions5, &hv_Row5, &hv_Column5, &hv_Radius3);
// 		//		gen_circle(&ho_Circle3, hv_Row5, hv_Column5, hv_Radius3);
// 		//		disp_obj(ho_Circle3, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
// 		//		result = QString::fromLocal8Bit("����ȱ��");
// 		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "����ȱ��");
// 		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//		return 1;
// 		//		// stop(); only in hdevelop
// 		//	}
// 		//	//else
// 		//	//{
// 		//	//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
// 		//	//	disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	//}
// 
// 		//}
// 
// 
// 		gen_empty_obj(&ho_RegionArc);
// 		erosion_circle(ho_RegionBand, &ho_RegionErosion6, _checkparam.d_ER_InCapsules);
// 		if (54==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionErosion6, m_WND);
// 			return;
// 		}
// 		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_InCapsules], ho_RegionErosion6, &ho_ImageReduced12);
// 		threshold(ho_ImageReduced12, &ho_Region18, 0, _checkparam.i_MaxGray_InCapsules);
// 		if (55 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region18, m_WND);
// 			return;
// 		}
// 		fill_up(ho_Region18, &ho_RegionFillUp6);
// 		opening_circle(ho_RegionFillUp6, &ho_RegionOpening15, _checkparam.d_OR_InCapsules);
// 		if (56 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionOpening15, m_WND);
// 			return;
// 		}
// 		connection(ho_RegionOpening15, &ho_ConnectedRegions22);
// 		select_shape(ho_ConnectedRegions22, &ho_SelectedRegions17, "area", "and", _checkparam.i_MinArea_InCapsules,
// 			_checkparam.i_MaxArea_InCapsules);
// 		if (57 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions17, m_WND);
// 			return;
// 		}
// 		if (58 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions17, m_WND);
// 			return;
// 		}
// 		shape_trans(ho_SelectedRegions17, &ho_RegionTrans7, "convex");
// 		//inner_rectangle1 (RegionTrans7, Row13, Column13, Row23, Column23)
// 		//gen_rectangle1 (Rectangle6, Row13, Column13, Row23, Column23)
// 		smallest_rectangle2(ho_RegionTrans7, &hv_Row8, &hv_Column8, &hv_Phi3, &hv_Length13,
// 			&hv_Length23);
// 		gen_rectangle2(&ho_Rectangle4, hv_Row8, hv_Column8, hv_Phi3, (hv_Length13 / hv_Length13) * _checkparam.i_LengthPlus_1,
// 			(hv_Length23 / hv_Length23) * _checkparam.i_LengthPlus_2);
// 		if (59 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Rectangle4, m_WND);
// 			return;
// 		}
// 		if (60 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Rectangle4, m_WND);
// 			return;
// 		}
// 		gen_rectangle2(&ho_Rectangle7, hv_Row8, hv_Column8, hv_Phi3, hv_Length13, hv_Length23);
// 		difference(ho_Rectangle7, ho_Rectangle4, &ho_RegionDifference15);
// 
// 
// 
// 		//***�����ڲ�ȱ��
// 		union1(ho_RegionPill, &ho_RegionPill);
// 		erosion_circle(ho_RegionPill, &ho_RegionErosion2, _checkparam.d_ER_RegionCapsules);
// 		if (21==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionErosion2, m_WND);
// 			return;
// 		}
// 		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], ho_RegionErosion2, &ho_ImageReduced1);
// 		//****��ɫ���ֽ���
// 		threshold(ho_ImageReduced1, &ho_Region1, 0, _checkparam.i_MaxGray_CapsulesRed);
// 		if (22==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region1, m_WND);
// 			return;
// 		}
// 		connection(ho_Region1, &ho_ConnectedRegions4);
// 		select_shape(ho_ConnectedRegions4, &ho_SelectedRegions4, "area", "and", _checkparam.i_MinArea_CapsulesRed,
// 			_checkparam.i_MaxArea_CapsulesRed);
// 		if (23==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions4, m_WND);
// 			return;
// 		}
// 		if (24 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions4, m_WND);
// 			return;
// 		}
// 		union1(ho_SelectedRegions4, &ho_RegionUnion3);
// 		fill_up(ho_RegionUnion3, &ho_RegionRed);
// 
// 		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_RegionRed], ho_RegionPill, &ho_ImageReduced9);
// 		threshold(ho_ImageReduced9, &ho_Region15, 0, 160);
// 		intersection(ho_Region15, ho_RegionRed, &ho_RegionIntersection6);
// 		fill_up(ho_RegionIntersection6, &ho_RegionFillUp4);
// 		opening_circle(ho_RegionFillUp4, &ho_RegionOpening9, 5);
// 		erosion_circle(ho_RegionOpening9, &ho_RegionErosion4, 3.5);
// 		//****��ɫ���ֽ���
// 		threshold(ho_ImageReduced1, &ho_Region3, 0, 255);
// 		difference(ho_Region3, ho_RegionRed, &ho_RegionYellow);
// 
// 		//***��Ʒֽ�����
// 		dilation_circle(ho_RegionRed, &ho_RegionDilation2, _checkparam.d_DR_CapsulesRed);
// 		if (25==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionDilation2, m_WND);
// 			return;
// 		}
// 		dilation_circle(ho_RegionYellow, &ho_RegionDilation3, _checkparam.d_DR_CapsulesYellow);
// 		if (26==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionDilation3, m_WND);
// 			return;
// 		}
// 		intersection(ho_RegionDilation2, ho_RegionDilation3, &ho_RegionCross);
// 
// 		//****��ɫ����ȱ��
// 		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], ho_RegionErosion4, &ho_ImageReduced2);
// 		gray_dilation_rect(ho_ImageReduced2, &ho_ImageMax1, _checkparam.i_MaskHeight_RegionRed, _checkparam.i_MaskWidth_RegionRed);
// 		if (30==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax1, m_WND);
// 			return;
// 		}
// 		if (31 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax1, m_WND);
// 			return;
// 		}
// 		sub_image(ho_ImageMax1, ho_ImageReduced2, &ho_ImageSub3, 1, 0);
// 		threshold(ho_ImageSub3, &ho_Regions2, _checkparam.i_MinGray_RegionRed, 255);
// 		if (32==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Regions2, m_WND);
// 			return;
// 		}
// 		threshold(ho_ImageReduced2, &ho_Region2, 0, _checkparam.i_MaxGray_RegionRed);
// 		if (33==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region2, m_WND);
// 			return;
// 		}
// 		intersection(ho_Regions2, ho_Region2, &ho_RegionIntersection1);
// 		closing_circle(ho_RegionIntersection1, &ho_RegionClosing1, _checkparam.d_CR_RegionRed);
// 		if (34==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionClosing1, m_WND);
// 			return;
// 		}
// 		opening_circle(ho_RegionClosing1, &ho_RegionOpening5, _checkparam.d_OR_RegionRed);
// 		if (35==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionOpening5, m_WND);
// 			return;
// 		}
// 		connection(ho_RegionOpening5, &ho_ConnectedRegions2);
// 		select_shape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", _checkparam.i_MinArea_RegionRed, _checkparam.i_MaxArea_RegionRed);
// 		if (36==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions1, m_WND);
// 			return;
// 		}
// 		if (37 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions1, m_WND);
// 			return;
// 		}
// 
// 		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], ho_RegionErosion4, &ho_ImageReduced5);
// 		gray_dilation_rect(ho_ImageReduced5, &ho_ImageMax5, _checkparam.i_MaskHeight_RedDefect, _checkparam.i_MaskWidth_RegionRed);
// 		if (38==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax5, m_WND);
// 			return;
// 		}
// 		if (39 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax5, m_WND);
// 			return;
// 		}
// 		sub_image(ho_ImageMax5, ho_ImageReduced5, &ho_ImageSub7, 1, 0);
// 		//gray_erosion_rect (ImageReduced5, ImageMin, 5, 5)
// 		//sub_image (ImageReduced5, ImageMin, ImageSub2, 1, 0)
// 		threshold(ho_ImageSub7, &ho_Region6, _checkparam.i_MinGray_RedDefect, 255);
// 		if (40==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region6, m_WND);
// 			return;
// 		}
// 		erosion_circle(ho_RegionErosion4, &ho_RegionErosion3, _checkparam.d_ER_RedDefect);
// 		if (41==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionErosion3, m_WND);
// 			return;
// 		}
// 		difference(ho_RegionOpening9, ho_RegionErosion3, &ho_RegionDifference8);
// 		difference(ho_Region6, ho_RegionDifference8, &ho_RegionDifference9);
// 		opening_circle(ho_RegionDifference9, &ho_RegionOpening8, _checkparam.d_OR_RedDefect);
// 		if (42==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionOpening8, m_WND);
// 			return;
// 		}
// 		connection(ho_RegionOpening8, &ho_ConnectedRegions15);
// 		select_shape(ho_ConnectedRegions15, &ho_SelectedRegions9, "area", "and", _checkparam.i_MinArea_RedDefect,
// 			_checkparam.i_MaxArea_RedDefect);
// 		if (43==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions9, m_WND);
// 			return;
// 		}
// 		if (44 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions9, m_WND);
// 			return;
// 		}
// 		//threshold (ImageReduced5, Region7, 120, 255)
// 		//intersection (Region6, Region7, RegionIntersection2)
// 		//opening_circle (RegionIntersection2, RegionOpening4, 1)
// 		//connection (RegionOpening4, ConnectedRegions5)
// 		//select_shape (ConnectedRegions5, SelectedRegions6, 'area', 'and', 20, 2000)
// 
// 		union2(ho_SelectedRegions1, ho_SelectedRegions9, &ho_RegionUnion2);
// 		if (_checkparam.b_IsOpen)
// 		{
// 			difference(ho_RegionUnion2, ho_RegionDifference15, &ho_RegionUnion2);
// 		}
// 		count_obj(ho_RegionUnion2, &hv_Number);
// 		//if (0 != (hv_Number != 0))
// 		//{
// 		//	connection(ho_RegionUnion2, &ho_ConnectedRegions7);
// 		//	smallest_circle(ho_ConnectedRegions7, &hv_Row3, &hv_Column3, &hv_Radius1);
// 		//	gen_circle(&ho_Circle1, hv_Row3, hv_Column3, hv_Radius1);
// 		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//	disp_obj(ho_Circle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
// 		//	result = QString::fromLocal8Bit("��ɫ����ȱ��");
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ɫ����ȱ��");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//	return 1;
// 		//	// stop(); only in hdevelop
// 		//}
// 		//***��ɫ����
// 		reduce_domain(ho_ImageReduced1, ho_RegionYellow, &ho_ImageReduced3);
// 		gray_dilation_rect(ho_ImageReduced3, &ho_ImageMax2, _checkparam.i_MaskHeight_RegionYellow, _checkparam.i_MaskWidth_RegionYellow);
// 		if (45==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax2, m_WND);
// 			return;
// 		}
// 		if (46 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax2, m_WND);
// 			return;
// 		}
// 		sub_image(ho_ImageMax2, ho_ImageReduced3, &ho_ImageSub, 1, 0);
// 		threshold(ho_ImageSub, &ho_Region5, _checkparam.i_MinGray_RegionYellow, 255);
// 		if (47==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region5, m_WND);
// 			return;
// 		}
// 		threshold(ho_ImageReduced3, &ho_Region8, 0, _checkparam.i_MaxGray_RegionYellow);
// 		if (48==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region8, m_WND);
// 			return;
// 		}
// 		intersection(ho_Region5, ho_Region8, &ho_RegionIntersection3);
// 		difference(ho_RegionIntersection3, ho_RegionCross, &ho_RegionDifference7);
// 		connection(ho_RegionDifference7, &ho_ConnectedRegions14);
// 		opening_circle(ho_RegionDifference7, &ho_RegionOpening3, _checkparam.d_OR_RegionYellow);
// 		if (49==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionOpening3, m_WND);
// 			return;
// 		}
// 		
// 		select_shape(ho_RegionOpening3, &ho_SelectedRegions11, "area", "and", _checkparam.i_MinArea_RegionYellow, _checkparam.i_MaxArea_RegionYellow);
// 		if (_checkparam.b_IsOpen)
// 		{
// 			difference(ho_SelectedRegions11, ho_RegionDifference15, &ho_SelectedRegions11);
// 		}
// 		if (50==r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions11, m_WND);
// 			return;
// 		}
// 		if (51 == r)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions11, m_WND);
// 			return;
// 		}
// 		area_center(ho_SelectedRegions11, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
// 		//if (0 != (hv_Area.Num()))
// 		//{
// 		//	connection(ho_RegionOpening3, &ho_ConnectedRegions10);
// 		//	smallest_circle(ho_ConnectedRegions10, &hv_Row4, &hv_Column4, &hv_Radius2);
// 		//	gen_circle(&ho_Circle2, hv_Row4, hv_Column4, hv_Radius2);
// 		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//	disp_obj(ho_Circle2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
// 		//	result = QString::fromLocal8Bit("��ɫ����ȱ��");
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ɫ����ȱ��");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//	// stop(); only in hdevelop
// 		//	return 1;
// 		//}
// 		//***ҩ��ȱ��
// 		gray_dilation_rect(ho_ImageChannel[_checkparam.i_Channel_Middle], &ho_ImageMax3, _checkparam.i_MaskHeight_Middle, _checkparam.i_MaskWidth_Middle);
// 		if (r==62)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax3, m_WND);
// 			return;
// 		}
// 		if (r == 63)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_ImageMax3, m_WND);
// 			return;
// 		}
// 		sub_image(ho_ImageMax3, ho_ImageChannel[_checkparam.i_Channel_Middle], &ho_ImageSub4, 1, 0);
// 		threshold(ho_ImageSub4, &ho_Region9, _checkparam.i_MinGray_Middle, 255);
// 		if (r == 64)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Region9, m_WND);
// 			return;
// 		}
// 		intersection(ho_Region9, ho_Rectangle4, &ho_RegionIntersection13);
// 		connection(ho_RegionIntersection13, &ho_ConnectedRegions24);
// 		select_shape(ho_ConnectedRegions24, &ho_SelectedRegions19, (HTuple("width").Append("height")),
// 			"and", (HTuple(_checkparam.i_MinWidth_Middle).Append(_checkparam.i_MinHeight_Middle)), (HTuple(99999).Append(9999)));
// 		if (r == 65)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions19, m_WND);
// 			return;
// 		}
// 		if (r == 66)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions19, m_WND);
// 			return;
// 		}
// 		union1(ho_SelectedRegions19, &ho_RegionUnion7);
// 		closing_circle(ho_RegionUnion7, &ho_RegionClosing7, _checkparam.d_CR_Middle);
// 		if (r == 67)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionClosing7, m_WND);
// 			return;
// 		}
// 		connection(ho_RegionClosing7, &ho_ConnectedRegions23);
// 		select_shape(ho_ConnectedRegions23, &ho_SelectedRegions18, "area", "and", _checkparam.i_MinArea_Middle,
// 			99999);
// 		if (r == 68)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions18, m_WND);
// 			return;
// 		}
// 
// 
// 		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_Plate], ho_RegionErosion6, &ho_ImageReduced13);
// 		threshold(ho_ImageReduced13, &ho_Regions1, 0, _checkparam.i_MaxGray_Plate);
// 		if (r == 70)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_Regions1, m_WND);
// 			return;
// 		}
// 		closing_circle(ho_Regions1, &ho_RegionClosing8, _checkparam.d_CR_Plate);
// 		if (r == 71)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionClosing8, m_WND);
// 			return;
// 		}
// 		fill_up(ho_RegionClosing8, &ho_RegionFillUp7);
// 		opening_circle(ho_RegionFillUp7, &ho_RegionOpening17, _checkparam.d_OR_Plate_1);
// 		if (r == 72)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionOpening17, m_WND);
// 			return;
// 		}
// 		connection(ho_RegionOpening17, &ho_ConnectedRegions25);
// 		select_shape(ho_ConnectedRegions25, &ho_SelectedRegions20, "area", "and", _checkparam.i_MinArea_Plate_1,
// 			99999);
// 		if (r == 73)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions20, m_WND);
// 			return;
// 		}
// 		shape_trans(ho_SelectedRegions20, &ho_RegionTrans8, "convex");
// 		union2(ho_RegionTrans8, ho_Rectangle1, &ho_RegionUnion8);
// 		difference(ho_RegionErosion6, ho_RegionUnion8, &ho_RegionDifference17);
// 		intersection(ho_RegionDifference17, ho_Region9, &ho_RegionIntersection14);
// 		fill_up(ho_RegionIntersection14, &ho_RegionFillUp8);
// 		opening_circle(ho_RegionFillUp8, &ho_RegionOpening18, _checkparam.d_OR_Plate_2);
// 		if (r == 74)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_RegionOpening18, m_WND);
// 			return;
// 		}
// 		connection(ho_RegionOpening18, &ho_ConnectedRegions26);
// 		select_shape(ho_ConnectedRegions26, &ho_SelectedRegions21, "area", "and", _checkparam.i_MinArea_Plate_2,
// 			99999);
// 		if (r == 75)
// 		{
// 			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
// 			set_draw(m_WND, "margin");
// 			set_color(m_WND, "red");
// 			set_line_width(m_WND, 3);
// 			disp_obj(ho_SelectedRegions21, m_WND);
// 			return;
// 		}
// 		union2(ho_SelectedRegions18, ho_SelectedRegions21, &ho_RegionUnion9);
// 		connection(ho_RegionUnion9, &ho_ConnectedRegions27);
// 		//if (0 != (hv_Number2 != 0))
// 		//{
// 		//	smallest_circle(ho_SelectedRegions8, &hv_Row2, &hv_Column2, &hv_Radius);
// 		//	gen_circle(&ho_Circle, hv_Row2, hv_Column2, hv_Radius);
// 		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
// 		//	disp_obj(ho_Circle, Wnd == -1 ? m_ShowLabel[0] : Wnd);
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
// 		//	result = QString::fromLocal8Bit("ҩ��ȱ��");
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ҩ��ȱ��");
// 		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
// 		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
// 		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
// 		//	// stop(); only in hdevelop
// 		//	return 1;
// 		//}
// 
// 
// 		//switch (r)
// 		//{
// 
// 		//case 1:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Region, m_WND);
// 		//	//set_tposition(m_WND, 10,10);
// 		//	//write_string(m_WND, "ȱ�������" + hv_Area7);
// 		//	break;
// 		//}
// 		//case 2:
// 		//{
// 		//	if (hv_AreaBand < _checkparam.i_MinArea_Band)
// 		//	{
// 		//		disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
// 		//		set_draw(m_WND, "margin");
// 		//		set_color(m_WND, "red");
// 		//		set_line_width(m_WND, 3);
// 		//		disp_obj(ho_RegionBand, m_WND);
// 		//		set_tposition(m_WND, 10, 10);
// 		//		write_string(m_WND, "���ְ������" + hv_AreaBand);
// 		//	}
// 		//	else
// 		//	{
// 		//		disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
// 		//		set_draw(m_WND, "margin");
// 		//		set_color(m_WND, "green");
// 		//		set_line_width(m_WND, 3);
// 		//		disp_obj(ho_RegionBand, m_WND);
// 		//		set_tposition(m_WND, 10, 10);
// 		//		write_string(m_WND, "���ְ������" + hv_AreaBand);
// 		//	}
// 
// 		//	//set_tposition(m_WND, 10, 10);
// 		//	//write_string(m_WND, "�����ֵ��" + HTuple(_checkparam.i_BandException2_Value));
// 		//	break;
// 		//}
// 		//case 4:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Regions, m_WND);
// 
// 		//	break;
// 		//}
// 		//case 5:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionOpening, m_WND);
// 		//	break;
// 		//}
// 		//case 6:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions, m_WND);
// 		//	break;
// 		//}
// 		//case 7:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions, m_WND);
// 		//	break;
// 		//}
// 		//case 8:
// 		//{
// 		//	if (hv_NumberPill == _checkparam.i_Number_Capsules)
// 		//	{
// 		//		disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 		//		set_draw(m_WND, "margin");
// 		//		set_color(m_WND, "green");
// 		//		set_line_width(m_WND, 3);
// 		//		disp_obj(ho_RegionPill, m_WND);
// 		//	}
// 		//	else
// 		//	{
// 		//		disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
// 		//		set_draw(m_WND, "margin");
// 		//		set_color(m_WND, "red");
// 		//		set_line_width(m_WND, 3);
// 		//		disp_obj(ho_RegionPill, m_WND);
// 		//	}
// 
// 		//	break;
// 		//}
// 
// 		//case 10:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "fill");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Rectangle, m_WND);
// 		//	break;
// 		//}
// 		//case 11:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "fill");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Rectangle, m_WND);
// 		//	break;
// 		//}
// 		//case 12:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionErosion, m_WND);
// 		//	break;
// 		//}
// 		//case 13:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax, m_WND);
// 		//	break;
// 		//}
// 		//case 14:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax, m_WND);
// 		//	break;
// 		//}
// 		//case 15:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Region4, m_WND);
// 		//	break;
// 		//}
// 		//case 16:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionOpening1, m_WND);
// 		//	break;
// 		//}
// 		//case 17:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionClosing, m_WND);
// 		//	break;
// 		//}
// 		//case 18:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions2, m_WND);
// 		//	break;
// 		//}
// 		//case 19:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions2, m_WND);
// 		//	break;
// 		//}
// 		//case 21:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionErosion2, m_WND);
// 		//	break;
// 		//}
// 		//case 22:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Region1, m_WND);
// 		//	break;
// 		//}
// 		//case 23:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions4, m_WND);
// 		//	break;
// 		//}
// 		//case 24:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions4, m_WND);
// 		//	break;
// 		//}
// 		//case 25:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionDilation2, m_WND);
// 		//	break;
// 		//}
// 		//case 26:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionDilation3, m_WND);
// 		//	break;
// 		//}
// 		//case 30:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax1, m_WND);
// 		//	break;
// 		//}
// 		//case 31:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax1, m_WND);
// 
// 		//	break;
// 		//}
// 		//case 32:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Regions2, m_WND);
// 		//	break;
// 		//}
// 		//case 33:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Region2, m_WND);
// 		//	break;
// 		//}
// 		//case 34:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionClosing1, m_WND);
// 		//	break;
// 		//}
// 		//case 35:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionOpening5, m_WND);
// 		//	break;
// 		//}
// 		//case 36:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions1, m_WND);
// 		//	break;
// 		//}
// 		//case 37:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions1, m_WND);
// 		//	break;
// 		//}
// 		//case 38:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax5, m_WND);
// 		//	break;
// 		//}
// 		//case 39:
// 		//{
// 
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax5, m_WND);
// 		//	break;
// 		//}
// 		//case 40:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Region6, m_WND);
// 		//	break;
// 		//}
// 		//case 41:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionErosion3, m_WND);
// 		//	break;
// 		//}
// 		//case 42:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionOpening8, m_WND);
// 		//	break;
// 		//}
// 		//case 43:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions9, m_WND);
// 		//	break;
// 		//}
// 		//case 44:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions9, m_WND);
// 		//	break;
// 		//}
// 		//case 45:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax2, m_WND);
// 		//	break;
// 		//}
// 		//case 46:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax2, m_WND);
// 		//	break;
// 		//}
// 		//case 47:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Region5, m_WND);
// 		//	break;
// 		//}
// 		//case 48:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Region8, m_WND);
// 		//	break;
// 		//}
// 		//case 49:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionOpening3, m_WND);
// 		//	break;
// 		//}
// 		//case 50:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions11, m_WND);
// 		//	break;
// 		//}
// 		//case 51:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions11, m_WND);
// 		//	break;
// 		//}
// 
// 		//case 53:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionDilation, m_WND);
// 		//	break;
// 		//}
// 		//case 54:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionErosion1, m_WND);
// 		//	break;
// 		//}
// 		//case 55:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax3, m_WND);
// 		//	break;
// 		//}
// 		//case 56:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_ImageMax3, m_WND);
// 		//	break;
// 		//}
// 		//case 57:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_Region9, m_WND);
// 		//	break;
// 		//}
// 		//case 58:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_RegionClosing3, m_WND);
// 		//	break;
// 		//}
// 		//case 59:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions8, m_WND);
// 		//	break;
// 		//}
// 		//case 60:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions8, m_WND);
// 		//	break;
// 		//}
// 		//case 61:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions8, m_WND);
// 		//	break;
// 		//}
// 		//case 62:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions8, m_WND);
// 		//	break;
// 		//}
// 		//case 63:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions8, m_WND);
// 		//	break;
// 		//}
// 		//case 64:
// 		//{
// 		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
// 		//	set_draw(m_WND, "margin");
// 		//	set_color(m_WND, "red");
// 		//	set_line_width(m_WND, 3);
// 		//	disp_obj(ho_SelectedRegions8, m_WND);
// 		//	break;
// 		//}
// 		//}
// 	}
// 	catch (HException &e)
// 	{
// 		if (nullptr!=m_timerChanger)
// 		{
// 			m_timerChanger->stop();
// 			delete m_timerChanger;
// 			m_timerChanger = nullptr;
// 		}
// 
// 		//QMessageBox::warning(nullptr, "", e.message);
// 	}

}
void QtGuiSetting::SetModelMat(Mat tempgray)
{
<<<<<<< HEAD
	_checkparam = param;
	this->setWindowFlags(Qt::FramelessWindowHint);
	ui.tableWidget->blockSignals(true);
	ui.tableWidget->setSortingEnabled(false);
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);
	int rowindex = ui.tableWidget->rowCount();
	QTableWidgetItem* item;
	QStringList stlist;
	QComboBox *box;
	QSlider* hsd;
	QCheckBox *qck;

	if (0 == rowindex)
	{
		

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("BandChannel");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("BandChannel");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");
		box->setCurrentIndex(_checkparam.i_BandChannel);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		box->installEventFilter(this);
		ui.tableWidget->setCellWidget(rowindex, 1, box);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinGray_Band"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinGray_Band));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);





		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_Band"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_Band));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("CapsulesChannel");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("CapsulesChannel");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");
		box->setCurrentIndex(_checkparam.i_CapsulesChannel);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinGray_Capsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinGray_Capsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("OR_Capsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_OR_Capsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_Capsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_Capsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxArea_Capsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxArea_Capsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("Number_Capsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_Number_Capsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("BatchChannel");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("BatchChannel");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");

		box->setCurrentIndex(_checkparam.i_BatchChannel);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("LengthAdd_1"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_LengthAdd_1));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("LengthAdd_2"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_LengthAdd_2));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("ER_Batch"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_ER_Batch));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskHeight_Batch"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskHeight_Batch));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskWidth_Batch"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskWidth_Batch));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinGray_Batch"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinGray_Batch));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("OR_Batch"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_OR_Batch));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("CR_Batch"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_CR_Batch));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_Batch"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_Batch));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxArea_Batch"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxArea_Batch));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Channel_CapsulesDefect");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("Channel_CapsulesDefect");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");

		box->setCurrentIndex(_checkparam.i_Channel_CapsulesDefect);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("ER_RegionCapsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_ER_RegionCapsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxGray_CapsulesRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxGray_CapsulesRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_CapsulesRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_CapsulesRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxArea_CapsulesRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxArea_CapsulesRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("DR_CapsulesRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_DR_CapsulesRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("DR_CapsulesYellow"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_DR_CapsulesYellow));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Channel_RegionRed");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("Channel_RegionRed");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");

		box->setCurrentIndex(_checkparam.i_Channel_RegionRed);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Channel_RedDefect1");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("Channel_RedDefect1");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");

		box->setCurrentIndex(_checkparam.i_Channel_RedDefect1);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Channel_RedDefect2");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("Channel_RedDefect2");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");

		box->setCurrentIndex(_checkparam.i_Channel_RedDefect2);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskHeight_RegionRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskHeight_RegionRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskWidth_RegionRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskWidth_RegionRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinGray_RegionRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinGray_RegionRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxGray_RegionRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxGray_RegionRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("CR_RegionRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_CR_RegionRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("OR_RegionRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_OR_RegionRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_RegionRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_RegionRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxArea_RegionRed"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxArea_RegionRed));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskHeight_RedDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskHeight_RedDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskWidth_RedDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskWidth_RedDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinGray_RedDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinGray_RedDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);





		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("ER_RedDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_ER_RedDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("OR_RedDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_OR_RedDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_RedDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_RedDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxArea_RedDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxArea_RedDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskHeight_RegionYellow"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskHeight_RegionYellow));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskWidth_RegionYellow"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskWidth_RegionYellow));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinGray_RegionYellow"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinGray_RegionYellow));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxGray_RegionYellow"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxGray_RegionYellow));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("OR_RegionYellow"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_OR_RegionYellow));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_RegionYellow"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_RegionYellow));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxArea_RegionYellow"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxArea_RegionYellow));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		/*rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Channel_BandDefect");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("Channel_BandDefect");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");
		box->setCurrentIndex(_checkparam.i_Channel_BandDefect);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("DR_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_DR_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("ER_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_ER_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskHeight_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskHeight_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskWidth_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskWidth_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinGray_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinGray_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("CR_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_CR_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinWidth_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinWidth_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);





		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxWidth_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxWidth_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinHeight_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinHeight_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxHeight_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxHeight_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxArea_BandDefect"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxArea_BandDefect));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);
*/

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Blisters");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		qck = new QCheckBox();
		qck->setChecked(_checkparam.b_IsOpen);
		qck->setText("Enable");
		ui.tableWidget->setCellWidget(rowindex, 1, qck);
		connect(qck, &QCheckBox::stateChanged, [=](bool b) 
		{
		_checkparam.b_IsOpen = b; m_bChanged = true;
		ui.pB_Save->setEnabled(true); 
		});

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Channel_InCapsules");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("Channel_InCapsules");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");
		box->setCurrentIndex(_checkparam.i_Channel_InCapsules);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("ER_InCapsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_ER_InCapsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxGray_InCapsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxGray_InCapsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("OR_InCapsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_OR_InCapsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_InCapsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_InCapsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxArea_InCapsules"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxArea_InCapsules));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);





		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("LengthPlus_1"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_LengthPlus_1));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("LengthPlus_2"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_LengthPlus_2));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);





		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Channel_Middle");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("Channel_Middle");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");
		box->setCurrentIndex(_checkparam.i_Channel_Middle);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskHeight_Middle"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskHeight_Middle));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);





		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaskWidth_Middle"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaskWidth_Middle));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);





		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinGray_Middle"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinGray_Middle));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinWidth_Middle"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinWidth_Middle));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinHeight_Middle"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinHeight_Middle));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("CR_Middle"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_CR_Middle));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_Middle"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_Middle));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Channel_Plate");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		stlist.clear();
		stlist << "R" << "G" << "B" << "H" << "S" << "V";
		box = new QComboBox();
		box->setObjectName("Channel_Plate");
		box->addItems(stlist);
		box->setView(new QListView());
		box->setStyleSheet("font-size:20pt");
		box->setCurrentIndex(_checkparam.i_Channel_Plate);
		connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		ui.tableWidget->setCellWidget(rowindex, 1, box);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MaxGray_Plate"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MaxGray_Plate));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("CR_Plate"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_CR_Plate));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("OR_Plate_1"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_OR_Plate_1));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_Plate_1"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_Plate_1));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("OR_Plate_2"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_OR_Plate_2));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);





		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("MinArea_Plate_2"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MinArea_Plate_2));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Pill");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		qck = new QCheckBox();
		qck->setChecked(_checkparam.b_CheckPill);
		qck->setText("Enable");
		ui.tableWidget->setCellWidget(rowindex, 1, qck);
		connect(qck, &QCheckBox::stateChanged, [=](bool b)
		{
			_checkparam.b_CheckPill = b; m_bChanged = true;
			ui.pB_Save->setEnabled(true);
		});




		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText("Capsule");
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		qck = new QCheckBox();
		qck->setChecked(_checkparam.b_CheckCapsule);
		qck->setText("Enable");
		ui.tableWidget->setCellWidget(rowindex, 1, qck);
		connect(qck, &QCheckBox::stateChanged, [=](bool b)
		{
			_checkparam.b_CheckCapsule = b; m_bChanged = true;
			ui.pB_Save->setEnabled(true);
		});






		/*rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		box = new QComboBox();
		box->setMaximumWidth(185);
		stlist.clear();
		stlist << QString::fromLocal8Bit("ҩ��ߴ�") << "BandChannel" << "MinGray_Band" << "MaxArea_Band";
		box->addItems(stlist);
		box->setObjectName(QString("Band_") + QString::number(rowindex));
		ui.tableWidget->setCellWidget(rowindex, 0, box);
		connect(box, SIGNAL(activated(int)), this, SLOT(onBandChanged(int)));


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		box = new QComboBox();
		box->setMaximumWidth(185);
		stlist.clear();
		stlist << QString::fromLocal8Bit("��������") << "CapsulesChannel" << "MinGray_Capsules" << "OR_Capsules"
			<< "MinArea_Capsules" << "MaxArea_Capsules" << "Number_Capsules";
		box->addItems(stlist);
		box->setObjectName(QString("Capsules_") + QString::number(rowindex));
		ui.tableWidget->setCellWidget(rowindex, 0, box);
		connect(box, SIGNAL(activated(int)), this, SLOT(onCapsulesChanged(int)));


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		box = new QComboBox();
		box->setMaximumWidth(185);
		stlist.clear();
		stlist << QString::fromLocal8Bit("����ȱ��") << "BatchChannel" << "LengthAdd_1" << "LengthAdd_2"
			<< "ER_Batch" << "MaskHeight_Batch" << "MaskWidth_Batch"<<"MinGray_Batch" << "OR_Batch" << "CR_Batch" << "MinArea_Batch"
			<< "MaxArea_Batch" << "MinArea_BatchDefect" << "MaxArea_BatchDefect" ;
		box->addItems(stlist);
		box->setObjectName(QString("Batch_") + QString::number(rowindex));
		ui.tableWidget->setCellWidget(rowindex, 0, box);
		connect(box, SIGNAL(activated(int)), this, SLOT(onBatchChanged(int)));



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		box = new QComboBox();
		box->setMaximumWidth(185);
		stlist.clear();
		stlist << QString::fromLocal8Bit("����ȱ��") << "Channel_CapsulesDefect" << "ER_RegionCapsules" << "MaxGray_CapsulesRed"
			<< "MinArea_CapsulesRed" << "MaxArea_CapsulesRed" << "DR_CapsulesRed" << "DR_CapsulesYellow";
		box->addItems(stlist);
		box->setObjectName(QString("CapsulesDefect_") + QString::number(rowindex));
		ui.tableWidget->setCellWidget(rowindex, 0, box);
		connect(box, SIGNAL(activated(int)), this, SLOT(onCapDefectChanged(int)));


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		box = new QComboBox();
		box->setMaximumWidth(185);
		stlist.clear();
		stlist << QString::fromLocal8Bit("��ɫ����") << "Channel_RegionRed" << "Channel_RedDefect1" << "Channel_RedDefect2"
			<< "MaskHeight_RegionRed" << "MaskWidth_RegionRed" << "MinGray_RegionRed" << "MaxGray_RegionRed" << "CR_RegionRed"
			 << "OR_RegionRed" << "MinArea_RegionRed"<< "MaxArea_RegionRed" << "MaskHeight_RedDefect" <<"MaskWidth_RedDefect"
			<<"MinGray_RedDefect" << "ER_RedDefect" << "OR_RedDefect" << "MinArea_RedDefect" << "MaxArea_RedDefect";
		box->addItems(stlist);
		box->setObjectName(QString("RedDefect_") + QString::number(rowindex));
		ui.tableWidget->setCellWidget(rowindex, 0, box);
		connect(box, SIGNAL(activated(int)), this, SLOT(onRedChanged(int)));



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		box = new QComboBox();
		box->setMaximumWidth(185);
		stlist.clear();
		stlist << QString::fromLocal8Bit("��ɫ����") << "MaskHeight_RegionYellow" << "MaskWidth_RegionYellow"
			 << "MinGray_RegionYellow" << "MaxGray_RegionYellow" << "OR_RegionYellow"<<"MinArea_RegionYellow"<<"MaxArea_RegionYellow";
		box->addItems(stlist);
		box->setObjectName(QString("YellowDefect_") + QString::number(rowindex));
		ui.tableWidget->setCellWidget(rowindex, 0, box);
		connect(box, SIGNAL(activated(int)), this, SLOT(onYellowChanged(int)));


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		box = new QComboBox();
		box->setMaximumWidth(185);
		stlist.clear();
		stlist << QString::fromLocal8Bit("ҩ��ȱ��") << "Channel_BandDefect" << "DR_BandDefect"<<"ER_BandDefect"
			<< "MaskHeight_BandDefect" << "MaskWidth_BandDefect" << "MinGray_BandDefect" << "CR_BandDefect"
			<< "MinWidth_BandDefect"<<"MaxWidth_BandDefect" << "MinHeight_BandDefect" << "MaxHeight_BandDefect"
			<< "MinArea_BandDefect" << "MaxArea_BandDefect";
		box->addItems(stlist);
		box->setObjectName(QString("BandDefect_") + QString::number(rowindex));
		ui.tableWidget->setCellWidget(rowindex, 0, box);
		connect(box, SIGNAL(activated(int)), this, SLOT(onBandDefectChanged(int)));*/

	}
	ui.tableWidget->blockSignals(false);
}
void QtGuiSetting::onCellChanged(int r, int c)
{
	try
	{
		c = 1;//��ֹ����table
		Mat MatToShow(m_MOriginal);
		//Local iconic variables 
		Hobject  ho_R, ho_G, ho_B, ho_H, ho_S;
		Hobject  ho_V, ho_Region, ho_RegionFillUp2, ho_ConnectedRegions;
		Hobject  ho_RegionBand, ho_ImageReduced, ho_Regions, ho_RegionOpening;
		Hobject  ho_RegionFillUp, ho_ConnectedRegions1, ho_SelectedRegions;
		Hobject  ho_RegionPill, ho_EmptyPill, ho_RegionUnion5, ho_RegionTrans3;
		Hobject  ho_RegionDifference3, ho_TopPills, ho_BottomPills;
		Hobject  ho_SortedRegions, ho_ObjectSelected, ho_ObjectSelected1;
		Hobject  ho_RegionDilation1, ho_ConnectedRegions9, ho_Rectangle2;
		Hobject  ho_RegionDifference2, ho_RegionTrans2, ho_RegionTrans;
		Hobject  ho_RegionUnion, ho_Rectangle, ho_RegionDifference;
		Hobject  ho_RegionErosion, ho_ConnectedRegions16, ho_SelectedRegions10;
		Hobject  ho_ImageMax, ho_ImageSub1, ho_Region4, ho_RegionOpening1;
		Hobject  ho_RegionIntersection, ho_RegionClosing, ho_ConnectedRegions3;
		Hobject  ho_SelectedRegions2, ho_RegionTrans1, ho_RegionUnion1;
		Hobject  ho_Rectangle1, ho_RegionIntersection9, ho_ImageReduced4;
		Hobject  ho_Region11, ho_RegionOpening6, ho_ConnectedRegions8;
		Hobject  ho_SelectedRegions5, ho_Circle3, ho_RegionArc, ho_RegionErosion6;
		Hobject  ho_ImageReduced12, ho_Region18, ho_RegionFillUp6;
		Hobject  ho_RegionOpening15, ho_ConnectedRegions22, ho_SelectedRegions17;
		Hobject  ho_RegionTrans7, ho_Rectangle4, ho_Rectangle7, ho_RegionDifference15;
		Hobject  ho_RegionErosion2, ho_ImageReduced1, ho_Region1;
		Hobject  ho_ConnectedRegions4, ho_SelectedRegions4, ho_RegionUnion3;
		Hobject  ho_RegionRed, ho_ImageReduced9, ho_Region15, ho_RegionIntersection6;
		Hobject  ho_RegionFillUp4, ho_RegionOpening9, ho_RegionErosion4;
		Hobject  ho_Region3, ho_RegionYellow, ho_RegionDilation2;
		Hobject  ho_RegionDilation3, ho_RegionCross, ho_ImageReduced2;
		Hobject  ho_ImageMax1, ho_ImageSub3, ho_Regions2, ho_Region2;
		Hobject  ho_RegionIntersection1, ho_RegionClosing1, ho_RegionOpening5;
		Hobject  ho_ConnectedRegions2, ho_SelectedRegions1, ho_ImageReduced5;
		Hobject  ho_ImageMax5, ho_ImageSub7, ho_Region6, ho_RegionErosion3;
		Hobject  ho_RegionDifference8, ho_RegionDifference9, ho_RegionOpening8;
		Hobject  ho_ConnectedRegions15, ho_SelectedRegions9, ho_RegionUnion2;
		Hobject  ho_RegionDifference19, ho_ConnectedRegions7, ho_Circle1;
		Hobject  ho_ImageReduced3, ho_ImageMax2, ho_ImageSub, ho_Region5;
		Hobject  ho_Region8, ho_RegionIntersection3, ho_RegionDifference7;
		Hobject  ho_ConnectedRegions14, ho_RegionOpening3, ho_SelectedRegions11;
		Hobject  ho_RegionDifference18, ho_ConnectedRegions10, ho_Circle2;
		Hobject  ho_ImageMax3, ho_ImageSub4, ho_Region9, ho_RegionIntersection13;
		Hobject  ho_ConnectedRegions24, ho_SelectedRegions19, ho_RegionUnion7;
		Hobject  ho_RegionClosing7, ho_ConnectedRegions23, ho_SelectedRegions18;
		Hobject  ho_ImageReduced13, ho_Regions1, ho_RegionClosing8;
		Hobject  ho_RegionFillUp7, ho_RegionOpening17, ho_ConnectedRegions25;
		Hobject  ho_SelectedRegions20, ho_RegionTrans8, ho_RegionUnion8;
		Hobject  ho_RegionDifference17, ho_RegionIntersection14;
		Hobject  ho_RegionFillUp8, ho_RegionOpening18, ho_ConnectedRegions26;
		Hobject  ho_SelectedRegions21, ho_RegionUnion9, ho_ConnectedRegions27;
		Hobject  ho_Circle;

		// Local control variables 
		HTuple  hv_ImageFiles, hv_Index, hv_BaseName;
		HTuple  hv_Extension, hv_Directory, hv_AreaBand, hv_ExpDefaultCtrlDummyVar;
		HTuple  hv_NumberPill, hv_Row11, hv_Column11, hv_Row21;
		HTuple  hv_Column21, hv_Area1, hv_Row7, hv_Column7, hv_Mean;
		HTuple  hv_Number1, hv_Index1, hv_Number5, hv_Index2, hv_Number6;
		HTuple  hv_Row6, hv_Column6, hv_Phi2, hv_Length12, hv_Length22;
		HTuple  hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2;
		HTuple  hv_Number3, hv_Row1, hv_Column1, hv_Phi1, hv_Length11;
		HTuple  hv_Length21, hv_Value, hv_Value1, hv_ratio, hv_Number4;
		HTuple  hv_Row5, hv_Column5, hv_Radius3, hv_Number, hv_Row3;
		HTuple  hv_Column3, hv_Radius1, hv_Area, hv_Row4, hv_Column4;
		HTuple  hv_Radius2, hv_Row8, hv_Column8, hv_Phi3, hv_Length13;
		HTuple  hv_Length23, hv_Number8, hv_Row2, hv_Column2, hv_Radius;
		HTuple  hv_UsedThreshold, hv_Width, hv_Height;

		Hobject ho_ImageChannel[6];
#pragma region setvalue

		_checkparam.i_BandChannel = ((QComboBox*)(ui.tableWidget->cellWidget(0, 1)))->currentIndex();
		//_checkparam.i_BandChannel = ui.tableWidget->item(0, c)->text().toInt();
		//_checkparam.i_PillChannel1 = ((QComboBox*)(ui.tableWidget->cellWidget(3, 1)))->currentIndex();
		_checkparam.i_MinGray_Band = ui.tableWidget->item(1, c)->text().toInt();
		if (_checkparam.i_MinGray_Band <= 0 || _checkparam.i_MinGray_Band>=255)
		{
			if (nullptr!=m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}

		}
		_checkparam.i_MinArea_Band = ui.tableWidget->item(2, c)->text().toInt();
		if (_checkparam.i_MinArea_Band<=0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}

		}
		_checkparam.i_CapsulesChannel = ((QComboBox*)(ui.tableWidget->cellWidget(3, 1)))->currentIndex();
		_checkparam.i_MinGray_Capsules = ui.tableWidget->item(4, c)->text().toInt();
		if (_checkparam.i_MinGray_Capsules <= 0 || _checkparam.i_MinGray_Capsules >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_OR_Capsules = ui.tableWidget->item(5, c)->text().toDouble();
		if (_checkparam.d_OR_Capsules <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_Capsules = ui.tableWidget->item(6, c)->text().toInt();
		if (_checkparam.i_MinArea_Capsules <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxArea_Capsules = ui.tableWidget->item(7, c)->text().toInt();
		if (_checkparam.i_MaxArea_Capsules <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_Number_Capsules = ui.tableWidget->item(8, c)->text().toInt();
		if (_checkparam.i_Number_Capsules <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_BatchChannel = ((QComboBox*)(ui.tableWidget->cellWidget(9, 1)))->currentIndex();
		_checkparam.i_LengthAdd_1 = ui.tableWidget->item(10, c)->text().toInt();
		if (_checkparam.i_LengthAdd_1 <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_LengthAdd_2 = ui.tableWidget->item(11, c)->text().toInt();
		if (_checkparam.i_LengthAdd_2 <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_ER_Batch = ui.tableWidget->item(12, c)->text().toDouble();
		if (_checkparam.d_ER_Batch <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskHeight_Batch = ui.tableWidget->item(13, c)->text().toInt();
		if (_checkparam.i_MaskHeight_Batch <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskWidth_Batch = ui.tableWidget->item(14, c)->text().toInt();
		if (_checkparam.i_MaskWidth_Batch <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinGray_Batch = ui.tableWidget->item(15, c)->text().toInt();
		if (_checkparam.i_MinGray_Batch <= 0 || _checkparam.i_MinGray_Batch >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_OR_Batch = ui.tableWidget->item(16, c)->text().toDouble();
		if (_checkparam.d_OR_Batch <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_CR_Batch = ui.tableWidget->item(17, c)->text().toDouble();
		if (_checkparam.d_CR_Batch <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_Batch = ui.tableWidget->item(18, c)->text().toInt();
		if (_checkparam.i_MinArea_Batch <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxArea_Batch = ui.tableWidget->item(19, c)->text().toInt();
		if (_checkparam.i_MaxArea_Batch <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		//_checkparam.i_MinArea_BatchDefect = ui.tableWidget->item(20, c)->text().toInt();
		//_checkparam.i_MaxArea_BatchDefect = ui.tableWidget->item(21, c)->text().toInt();
		_checkparam.i_Channel_CapsulesDefect = ((QComboBox*)(ui.tableWidget->cellWidget(20, 1)))->currentIndex();
		_checkparam.d_ER_RegionCapsules = ui.tableWidget->item(21, c)->text().toDouble();
		if (_checkparam.d_ER_RegionCapsules <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxGray_CapsulesRed = ui.tableWidget->item(22, c)->text().toInt();
		if (_checkparam.i_MaxGray_CapsulesRed <= 0 || _checkparam.i_MaxGray_CapsulesRed >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_CapsulesRed = ui.tableWidget->item(23, c)->text().toInt();
		if (_checkparam.i_MinArea_CapsulesRed <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxArea_CapsulesRed = ui.tableWidget->item(24, c)->text().toInt();
		if (_checkparam.i_MaxArea_CapsulesRed <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_DR_CapsulesRed = ui.tableWidget->item(25, c)->text().toDouble();
		if (_checkparam.d_DR_CapsulesRed <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_DR_CapsulesYellow = ui.tableWidget->item(26, c)->text().toDouble();
		if (_checkparam.d_DR_CapsulesYellow <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_Channel_RegionRed = ((QComboBox*)(ui.tableWidget->cellWidget(27, 1)))->currentIndex();
		_checkparam.i_Channel_RedDefect1 = ((QComboBox*)(ui.tableWidget->cellWidget(28, 1)))->currentIndex();
		_checkparam.i_Channel_RedDefect2 = ((QComboBox*)(ui.tableWidget->cellWidget(29, 1)))->currentIndex();
		_checkparam.i_MaskHeight_RegionRed = ui.tableWidget->item(30, c)->text().toInt();
		if (_checkparam.i_MaskHeight_RegionRed <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskWidth_RegionRed = ui.tableWidget->item(31, c)->text().toInt();
		if (_checkparam.i_MaskWidth_RegionRed <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinGray_RegionRed = ui.tableWidget->item(32, c)->text().toInt();
		if (_checkparam.i_MinGray_RegionRed <= 0 || _checkparam.i_MinGray_RegionRed >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxGray_RegionRed = ui.tableWidget->item(33, c)->text().toInt();
		if (_checkparam.i_MaxGray_RegionRed <= 0 || _checkparam.i_MaxGray_RegionRed >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_CR_RegionRed = ui.tableWidget->item(34, c)->text().toDouble();
		if (_checkparam.d_CR_RegionRed <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_OR_RegionRed = ui.tableWidget->item(35, c)->text().toDouble();
		if (_checkparam.d_OR_RegionRed <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_RegionRed = ui.tableWidget->item(36, c)->text().toInt();
		if (_checkparam.i_MinArea_RegionRed <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxArea_RegionRed = ui.tableWidget->item(37, c)->text().toInt();
		if (_checkparam.i_MaxArea_RegionRed <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskHeight_RedDefect = ui.tableWidget->item(38, c)->text().toInt();
		if (_checkparam.i_MaskHeight_RedDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskWidth_RedDefect = ui.tableWidget->item(39, c)->text().toInt();
		if (_checkparam.i_MaskWidth_RedDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinGray_RedDefect = ui.tableWidget->item(40, c)->text().toInt();
		if (_checkparam.i_MinGray_RedDefect <= 0 || _checkparam.i_MinGray_RedDefect >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_ER_RedDefect = ui.tableWidget->item(41, c)->text().toDouble();
		if (_checkparam.d_ER_RedDefect <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_OR_RedDefect = ui.tableWidget->item(42, c)->text().toDouble();
		if (_checkparam.d_OR_RedDefect <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_RedDefect = ui.tableWidget->item(43, c)->text().toInt();
		if (_checkparam.i_MinArea_RedDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxArea_RedDefect = ui.tableWidget->item(44, c)->text().toInt();
		if (_checkparam.i_MaxArea_RedDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskHeight_RegionYellow = ui.tableWidget->item(45, c)->text().toInt();
		if (_checkparam.i_MaskHeight_RegionYellow <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskWidth_RegionYellow = ui.tableWidget->item(46, c)->text().toInt();
		if (_checkparam.i_MaskWidth_RegionYellow <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinGray_RegionYellow = ui.tableWidget->item(47, c)->text().toInt();
		if (_checkparam.i_MinGray_RegionYellow <= 0 || _checkparam.i_MinGray_RegionYellow >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxGray_RegionYellow = ui.tableWidget->item(48, c)->text().toInt();
		if (_checkparam.i_MaxGray_RegionYellow <= 0 || _checkparam.i_MaxGray_RegionYellow >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_OR_RegionYellow = ui.tableWidget->item(49, c)->text().toDouble();
		if (_checkparam.d_OR_RegionYellow <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_RegionYellow = ui.tableWidget->item(50, c)->text().toInt();
		if (_checkparam.i_MinArea_RegionYellow <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxArea_RegionYellow = ui.tableWidget->item(51, c)->text().toInt();
		if (_checkparam.i_MaxArea_RegionYellow <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		/*_checkparam.i_Channel_BandDefect = ((QComboBox*)(ui.tableWidget->cellWidget(52, 1)))->currentIndex();
		_checkparam.d_DR_BandDefect = ui.tableWidget->item(53, c)->text().toDouble();
		if (_checkparam.d_DR_BandDefect <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_ER_BandDefect = ui.tableWidget->item(54, c)->text().toDouble();
		if (_checkparam.d_ER_BandDefect <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskHeight_BandDefect = ui.tableWidget->item(55, c)->text().toInt();
		if (_checkparam.i_MaskHeight_BandDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskWidth_BandDefect = ui.tableWidget->item(56, c)->text().toInt();
		if (_checkparam.i_MaskWidth_BandDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinGray_BandDefect = ui.tableWidget->item(57, c)->text().toInt();
		if (_checkparam.i_MinGray_BandDefect <= 0 || _checkparam.i_MinGray_BandDefect >= 255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_CR_BandDefect = ui.tableWidget->item(58, c)->text().toDouble();
		if (_checkparam.d_CR_BandDefect <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinWidth_BandDefect = ui.tableWidget->item(59, c)->text().toInt();
		if (_checkparam.i_MinWidth_BandDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxWidth_BandDefect = ui.tableWidget->item(60, c)->text().toInt();
		if (_checkparam.i_MaxWidth_BandDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinHeight_BandDefect = ui.tableWidget->item(61, c)->text().toInt();
		if (_checkparam.i_MinHeight_BandDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxHeight_BandDefect = ui.tableWidget->item(62, c)->text().toInt();
		if (_checkparam.i_MaxHeight_BandDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_BandDefect = ui.tableWidget->item(63, c)->text().toInt();
		if (_checkparam.i_MinArea_BandDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxArea_BandDefect = ui.tableWidget->item(64, c)->text().toInt();
		if (_checkparam.i_MaxArea_BandDefect <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}*/
		_checkparam.i_Channel_InCapsules = ((QComboBox*)(ui.tableWidget->cellWidget(53, 1)))->currentIndex();
		_checkparam.d_ER_InCapsules = ui.tableWidget->item(54, c)->text().toDouble();
		if (_checkparam.d_ER_InCapsules <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaxGray_InCapsules = ui.tableWidget->item(55, c)->text().toInt();
		if (_checkparam.i_MaxGray_InCapsules <= 0 || _checkparam.i_MaxGray_InCapsules>=255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_OR_InCapsules = ui.tableWidget->item(56, c)->text().toDouble();
		if (_checkparam.d_OR_InCapsules <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_InCapsules = ui.tableWidget->item(57, c)->text().toInt();
		if (_checkparam.i_MinArea_InCapsules <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}

		_checkparam.i_MaxArea_InCapsules = ui.tableWidget->item(58, c)->text().toInt();
		if (_checkparam.i_MaxArea_InCapsules <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_LengthPlus_1 = ui.tableWidget->item(59, c)->text().toInt();
		if (_checkparam.i_LengthPlus_1 <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_LengthPlus_2 = ui.tableWidget->item(60, c)->text().toInt();
		if (_checkparam.i_LengthPlus_2 <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_Channel_Middle = ((QComboBox*)(ui.tableWidget->cellWidget(61, 1)))->currentIndex();
		_checkparam.i_MaskHeight_Middle = ui.tableWidget->item(62, c)->text().toInt();
		if (_checkparam.i_MaskHeight_Middle <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MaskWidth_Middle = ui.tableWidget->item(63, c)->text().toInt();
		if (_checkparam.i_MaskWidth_Middle <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinGray_Middle = ui.tableWidget->item(64, c)->text().toInt();
		if (_checkparam.i_MinGray_Middle <= 0||_checkparam.i_MinGray_Middle>=255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinWidth_Middle = ui.tableWidget->item(65, c)->text().toInt();
		if (_checkparam.i_MinWidth_Middle <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinHeight_Middle = ui.tableWidget->item(66, c)->text().toInt();
		if (_checkparam.i_MinHeight_Middle <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_CR_Middle = ui.tableWidget->item(67, c)->text().toDouble();
		if (_checkparam.d_CR_Middle <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_Middle = ui.tableWidget->item(68, c)->text().toInt();
		if (_checkparam.i_MinArea_Middle <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_Channel_Plate = ((QComboBox*)(ui.tableWidget->cellWidget(69, 1)))->currentIndex();
		_checkparam.i_MaxGray_Plate = ui.tableWidget->item(70, c)->text().toInt();
		if (_checkparam.i_MaxGray_Plate <= 0 || _checkparam.i_MaxGray_Plate>=255)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_CR_Plate = ui.tableWidget->item(71, c)->text().toDouble();
		if (_checkparam.d_CR_Plate <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_OR_Plate_1 = ui.tableWidget->item(72, c)->text().toDouble();
		if (_checkparam.d_OR_Plate_1 <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_Plate_1 = ui.tableWidget->item(73,c)->text().toInt();
		if (_checkparam.i_MinArea_Plate_1 <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.d_OR_Plate_2 = ui.tableWidget->item(74, c)->text().toDouble();
		if (_checkparam.d_OR_Plate_2 <= 0.5)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}
		_checkparam.i_MinArea_Plate_2 = ui.tableWidget->item(75, c)->text().toInt();
		if (_checkparam.i_MinArea_Plate_2 <= 0)
		{
			if (nullptr != m_timerChanger)
			{
				m_timerChanger->stop();
				delete m_timerChanger;
				m_timerChanger = nullptr;
			}
		}


		m_bChanged = true;
		ui.pB_Save->setEnabled(true);
		//((CInterCHeck*)p_Parent)->RealCheck(m_MOriginal, _checkparam, m_WND);

#pragma endregion setvalue
		if (m_MOriginal.empty())
		{
			return;
		}
		
		Hobject ho_Image = Mat2Hobject(m_MOriginal);
		decompose3(ho_Image, &ho_ImageChannel[0], &ho_ImageChannel[1], &ho_ImageChannel[2]);
		trans_from_rgb(ho_ImageChannel[0], ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageChannel[3], &ho_ImageChannel[4], &ho_ImageChannel[5], "hsv");
		get_image_size(ho_Image, &hv_Width, &hv_Height);
		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);

		//***ҩ��ߴ�
		threshold(ho_ImageChannel[_checkparam.i_BandChannel], &ho_Region, _checkparam.i_MinGray_Band, 255);
		if (1==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region, m_WND);
			return;
		}
		fill_up(ho_Region, &ho_RegionFillUp2);
		connection(ho_RegionFillUp2, &ho_ConnectedRegions);
		select_shape_std(ho_ConnectedRegions, &ho_RegionBand, "max_area", 70);
		area_center(ho_RegionBand, &hv_AreaBand, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		if (2==r)
		{
			if (hv_AreaBand < _checkparam.i_MinArea_Band)
			{
				disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
				set_draw(m_WND, "margin");
				set_color(m_WND, "red");
				set_line_width(m_WND, 3);
				disp_obj(ho_RegionBand, m_WND);
				set_tposition(m_WND, 10, 10);
				write_string(m_WND, "���ְ������" + hv_AreaBand);
			}
			else
			{
				disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
				set_draw(m_WND, "margin");
				set_color(m_WND, "green");
				set_line_width(m_WND, 3);
				disp_obj(ho_RegionBand, m_WND);
				set_tposition(m_WND, 10, 10);
				write_string(m_WND, "���ְ������" + hv_AreaBand);
			}
			return;
		}
		//if (0 != (hv_AreaBand < 740000))
		//{
		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//	disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 10, 10);
		//	result = QString::fromLocal8Bit("����ߴ����");
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "����ߴ����");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 80, 10);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰҩ�����Ϊ��" + hv_AreaBand);
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//	return 1;
		//	// stop(); only in hdevelop
		//}


		//**��������
		reduce_domain(ho_ImageChannel[_checkparam.i_CapsulesChannel], ho_RegionBand, &ho_ImageReduced);
		threshold(ho_ImageReduced, &ho_Regions, _checkparam.i_MinGray_Capsules, 255);
		if (4==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Regions, m_WND);
			return;
		}
		opening_circle(ho_Regions, &ho_RegionOpening, _checkparam.d_OR_Capsules);
		if (5==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening, m_WND);
			return;
		}
		fill_up(ho_RegionOpening, &ho_RegionFillUp);
		connection(ho_RegionFillUp, &ho_ConnectedRegions1);
		select_shape(ho_ConnectedRegions1, &ho_SelectedRegions, "area", "and", _checkparam.i_MinArea_Capsules,
			_checkparam.i_MaxArea_Capsules);
		if (6==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions, m_WND);
			return;
		}
		if (7==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions, m_WND);
			return;
		}
		shape_trans(ho_SelectedRegions, &ho_RegionPill, "convex");
		count_obj(ho_RegionPill, &hv_NumberPill);

		if (8==r)
		{
			if (hv_NumberPill == _checkparam.i_Number_Capsules)
			{
				disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
				set_draw(m_WND, "margin");
				set_color(m_WND, "green");
				set_line_width(m_WND, 3);
				disp_obj(ho_RegionPill, m_WND);
			}
			else
			{
				disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
				set_draw(m_WND, "margin");
				set_color(m_WND, "red");
				set_line_width(m_WND, 3);
				disp_obj(ho_RegionPill, m_WND);
			}
			return;
		}
	
		//if (0 != (HTuple(hv_NumberPill != 10).And(hv_NumberPill != 0)))
		//{
		//	gen_empty_obj(&ho_EmptyPill);
		//	if (0 != (hv_NumberPill == 5))
		//	{
		//		union1(ho_RegionPill, &ho_RegionUnion5);
		//		shape_trans(ho_RegionUnion5, &ho_RegionTrans3, "rectangle2");
		//		difference(ho_RegionBand, ho_RegionTrans3, &ho_RegionDifference3);
		//		inner_rectangle1(ho_RegionDifference3, &hv_Row11, &hv_Column11, &hv_Row21,
		//			&hv_Column21);
		//		gen_rectangle1(&ho_EmptyPill, hv_Row11, hv_Column11, hv_Row21, hv_Column21);
		//	}
		//	else
		//	{
		//		gen_empty_obj(&ho_TopPills);
		//		gen_empty_obj(&ho_BottomPills);
		//		sort_region(ho_RegionPill, &ho_SortedRegions, "first_point", "true", "row");
		//		area_center(ho_SortedRegions, &hv_Area1, &hv_Row7, &hv_Column7);
		//		tuple_mean(hv_Row7, &hv_Mean);
		//		count_obj(ho_SortedRegions, &hv_Number1);
		//		{
		//			HTuple end_val56 = hv_Number1;
		//			HTuple step_val56 = 1;
		//			for (hv_Index1 = 1; hv_Index1.Continue(end_val56, step_val56); hv_Index1 += step_val56)
		//			{
		//				select_obj(ho_SortedRegions, &ho_ObjectSelected, hv_Index1);
		//				if (0 != ((hv_Row7.Select(hv_Index1 - 1)) < hv_Mean))
		//				{
		//					concat_obj(ho_ObjectSelected, ho_TopPills, &ho_TopPills);
		//				}
		//				else
		//				{
		//					concat_obj(ho_ObjectSelected, ho_BottomPills, &ho_BottomPills);
		//				}
		//			}
		//		}
		//		union1(ho_TopPills, &ho_TopPills);
		//		union1(ho_BottomPills, &ho_BottomPills);
		//		concat_obj(ho_TopPills, ho_BottomPills, &ho_RegionPill);
		//		count_obj(ho_RegionPill, &hv_Number5);
		//		{
		//			HTuple end_val68 = hv_Number5;
		//			HTuple step_val68 = 1;
		//			for (hv_Index2 = 1; hv_Index2.Continue(end_val68, step_val68); hv_Index2 += step_val68)
		//			{
		//				select_obj(ho_RegionPill, &ho_ObjectSelected1, hv_Index2);
		//				dilation_circle(ho_ObjectSelected1, &ho_RegionDilation1, 15);
		//				connection(ho_RegionDilation1, &ho_ConnectedRegions9);
		//				count_obj(ho_ConnectedRegions9, &hv_Number6);
		//				if (0 != (hv_Number6 != 5))
		//				{
		//					smallest_rectangle2(ho_ObjectSelected1, &hv_Row6, &hv_Column6, &hv_Phi2,
		//						&hv_Length12, &hv_Length22);
		//					gen_rectangle2(&ho_Rectangle2, hv_Row6, hv_Column6, hv_Phi2, hv_Length12 + 20,
		//						hv_Length22 + 20);
		//					difference(ho_Rectangle2, ho_RegionDilation1, &ho_RegionDifference2);
		//					shape_trans(ho_RegionDifference2, &ho_RegionTrans2, "inner_rectangle1");
		//					concat_obj(ho_EmptyPill, ho_RegionTrans2, &ho_EmptyPill);
		//				}
		//			}
		//		}
		//		union1(ho_EmptyPill, &ho_EmptyPill);
		//	}
		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//	disp_obj(ho_EmptyPill, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//	result = QString::fromLocal8Bit("���Ҹ�������");
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "���Ҹ�������");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//	return 1;

		//	// stop(); only in hdevelop
		//}
		//else if (0 != (hv_NumberPill == 0))
		//{
		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//	result = QString::fromLocal8Bit("�޽���");
		//	disp_obj(ho_RegionBand, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "�޽���");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//	return 1;
		//	// stop(); only in hdevelop
		//}
		//**�������
		shape_trans(ho_RegionPill, &ho_RegionTrans, "convex");
		union1(ho_RegionTrans, &ho_RegionUnion);
		smallest_rectangle2(ho_RegionUnion, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
			&hv_Length2);
		gen_rectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1 + _checkparam.i_LengthAdd_1, hv_Length2 + _checkparam.i_LengthAdd_2);
		if (10==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Rectangle, m_WND);
			return;
		}
		if (11==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Rectangle, m_WND);
			return;
		}
		difference(ho_RegionBand, ho_Rectangle, &ho_RegionDifference);
		erosion_circle(ho_RegionDifference, &ho_RegionErosion, _checkparam.d_ER_Batch);
		if (12==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionErosion, m_WND);
			return;
		}
		connection(ho_RegionErosion, &ho_ConnectedRegions16);
		select_shape_std(ho_ConnectedRegions16, &ho_SelectedRegions10, "max_area", 70);
		gray_dilation_rect(ho_ImageChannel[_checkparam.i_BatchChannel], &ho_ImageMax, _checkparam.i_MaskHeight_Batch, _checkparam.i_MaskWidth_Batch);
		if (13==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax, m_WND);
			return;
		}
		if (14==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax, m_WND);
			return;
		}
		sub_image(ho_ImageMax, ho_ImageChannel[_checkparam.i_BatchChannel], &ho_ImageSub1, 1, 0);
		threshold(ho_ImageSub1, &ho_Region4, _checkparam.i_MinGray_Batch, 255);
		if (15==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region4, m_WND);
			return;
		}
		//dyn_threshold (ImageMax, G, RegionDynThresh, 10, 'light')
		opening_circle(ho_Region4, &ho_RegionOpening1, _checkparam.d_OR_Batch);
		if (16==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening1, m_WND);
			return;
		}
		intersection(ho_RegionOpening1, ho_SelectedRegions10, &ho_RegionIntersection);
		closing_circle(ho_RegionIntersection, &ho_RegionClosing, _checkparam.d_CR_Batch);
		if (17==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionClosing, m_WND);
			return;
		}
		connection(ho_RegionClosing, &ho_ConnectedRegions3);
		select_shape(ho_ConnectedRegions3, &ho_SelectedRegions2, "area", "and", _checkparam.i_MinArea_Batch, _checkparam.i_MaxArea_Batch);
		if (18==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions2, m_WND);
			return;
		}
		if (19==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions2, m_WND);
			return;
		}
		shape_trans(ho_SelectedRegions2, &ho_RegionTrans1, "convex");
		union1(ho_RegionTrans1, &ho_RegionUnion1);
		count_obj(ho_RegionUnion1, &hv_Number3);
		//if (0 != (hv_Number3 == 0))
		//{
		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//	result = QString::fromLocal8Bit("������");
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "������");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//	// stop(); only in hdevelop
		//	return 1;
		//}
		//else
		//{
		//	smallest_rectangle2(ho_RegionUnion1, &hv_Row1, &hv_Column1, &hv_Phi1, &hv_Length11,
		//		&hv_Length21);
		//	gen_rectangle2(&ho_Rectangle1, hv_Row1, hv_Column1, hv_Phi1, hv_Length11, hv_Length21);
		//	region_features(ho_Rectangle1, "ra", &hv_Value);
		//	region_features(ho_RegionBand, "rb", &hv_Value1);
		//	hv_ratio = hv_Value / hv_Value1;
		//	if (0 != (hv_ratio < 0.3))
		//	{
		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//		disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//		result = QString::fromLocal8Bit("����ӡˢ����");
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "����ӡˢ������");
		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//		return 1;
		//		// stop(); only in hdevelop
		//	}
		//	intersection(ho_Rectangle1, ho_RegionBand, &ho_RegionIntersection9);
		//	reduce_domain(ho_ImageSub1, ho_RegionIntersection9, &ho_ImageReduced4);
		//	threshold(ho_ImageReduced4, &ho_Region11, 50, 255);
		//	opening_circle(ho_Region11, &ho_RegionOpening6, 1.5);
		//	connection(ho_RegionOpening6, &ho_ConnectedRegions8);
		//	select_shape(ho_ConnectedRegions8, &ho_SelectedRegions5, "area", "and", 50,
		//		99999);
		//	count_obj(ho_SelectedRegions5, &hv_Number4);
		//	if (0 != (hv_Number4 != 0))
		//	{
		//		disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//		smallest_circle(ho_SelectedRegions5, &hv_Row5, &hv_Column5, &hv_Radius3);
		//		gen_circle(&ho_Circle3, hv_Row5, hv_Column5, hv_Radius3);
		//		disp_obj(ho_Circle3, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//		result = QString::fromLocal8Bit("����ȱ��");
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "����ȱ��");
		//		set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//		set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//		write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//		return 1;
		//		// stop(); only in hdevelop
		//	}
		//	//else
		//	//{
		//	//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	//	//set_draw(Wnd == -1 ? m_ShowLabel[0] : Wnd, "margin");
		//	//	disp_obj(ho_Rectangle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	//}

		//}


		gen_empty_obj(&ho_RegionArc);
		erosion_circle(ho_RegionBand, &ho_RegionErosion6, _checkparam.d_ER_InCapsules);
		if (54==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionErosion6, m_WND);
			return;
		}
		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_InCapsules], ho_RegionErosion6, &ho_ImageReduced12);
		threshold(ho_ImageReduced12, &ho_Region18, 0, _checkparam.i_MaxGray_InCapsules);
		if (55 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region18, m_WND);
			return;
		}
		fill_up(ho_Region18, &ho_RegionFillUp6);
		opening_circle(ho_RegionFillUp6, &ho_RegionOpening15, _checkparam.d_OR_InCapsules);
		if (56 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening15, m_WND);
			return;
		}
		connection(ho_RegionOpening15, &ho_ConnectedRegions22);
		select_shape(ho_ConnectedRegions22, &ho_SelectedRegions17, "area", "and", _checkparam.i_MinArea_InCapsules,
			_checkparam.i_MaxArea_InCapsules);
		if (57 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions17, m_WND);
			return;
		}
		if (58 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions17, m_WND);
			return;
		}
		shape_trans(ho_SelectedRegions17, &ho_RegionTrans7, "convex");
		//inner_rectangle1 (RegionTrans7, Row13, Column13, Row23, Column23)
		//gen_rectangle1 (Rectangle6, Row13, Column13, Row23, Column23)
		smallest_rectangle2(ho_RegionTrans7, &hv_Row8, &hv_Column8, &hv_Phi3, &hv_Length13,
			&hv_Length23);
		gen_rectangle2(&ho_Rectangle4, hv_Row8, hv_Column8, hv_Phi3, (hv_Length13 / hv_Length13) * _checkparam.i_LengthPlus_1,
			(hv_Length23 / hv_Length23) * _checkparam.i_LengthPlus_2);
		if (59 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Rectangle4, m_WND);
			return;
		}
		if (60 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_InCapsules], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Rectangle4, m_WND);
			return;
		}
		gen_rectangle2(&ho_Rectangle7, hv_Row8, hv_Column8, hv_Phi3, hv_Length13, hv_Length23);
		difference(ho_Rectangle7, ho_Rectangle4, &ho_RegionDifference15);



		//***�����ڲ�ȱ��
		union1(ho_RegionPill, &ho_RegionPill);
		erosion_circle(ho_RegionPill, &ho_RegionErosion2, _checkparam.d_ER_RegionCapsules);
		if (21==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionErosion2, m_WND);
			return;
		}
		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], ho_RegionErosion2, &ho_ImageReduced1);
		//****��ɫ���ֽ���
		threshold(ho_ImageReduced1, &ho_Region1, 0, _checkparam.i_MaxGray_CapsulesRed);
		if (22==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region1, m_WND);
			return;
		}
		connection(ho_Region1, &ho_ConnectedRegions4);
		select_shape(ho_ConnectedRegions4, &ho_SelectedRegions4, "area", "and", _checkparam.i_MinArea_CapsulesRed,
			_checkparam.i_MaxArea_CapsulesRed);
		if (23==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions4, m_WND);
			return;
		}
		if (24 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions4, m_WND);
			return;
		}
		union1(ho_SelectedRegions4, &ho_RegionUnion3);
		fill_up(ho_RegionUnion3, &ho_RegionRed);

		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_RegionRed], ho_RegionPill, &ho_ImageReduced9);
		threshold(ho_ImageReduced9, &ho_Region15, 0, 160);
		intersection(ho_Region15, ho_RegionRed, &ho_RegionIntersection6);
		fill_up(ho_RegionIntersection6, &ho_RegionFillUp4);
		opening_circle(ho_RegionFillUp4, &ho_RegionOpening9, 5);
		erosion_circle(ho_RegionOpening9, &ho_RegionErosion4, 3.5);
		//****��ɫ���ֽ���
		threshold(ho_ImageReduced1, &ho_Region3, 0, 255);
		difference(ho_Region3, ho_RegionRed, &ho_RegionYellow);

		//***��Ʒֽ�����
		dilation_circle(ho_RegionRed, &ho_RegionDilation2, _checkparam.d_DR_CapsulesRed);
		if (25==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionDilation2, m_WND);
			return;
		}
		dilation_circle(ho_RegionYellow, &ho_RegionDilation3, _checkparam.d_DR_CapsulesYellow);
		if (26==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionDilation3, m_WND);
			return;
		}
		intersection(ho_RegionDilation2, ho_RegionDilation3, &ho_RegionCross);

		//****��ɫ����ȱ��
		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], ho_RegionErosion4, &ho_ImageReduced2);
		gray_dilation_rect(ho_ImageReduced2, &ho_ImageMax1, _checkparam.i_MaskHeight_RegionRed, _checkparam.i_MaskWidth_RegionRed);
		if (30==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax1, m_WND);
			return;
		}
		if (31 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax1, m_WND);
			return;
		}
		sub_image(ho_ImageMax1, ho_ImageReduced2, &ho_ImageSub3, 1, 0);
		threshold(ho_ImageSub3, &ho_Regions2, _checkparam.i_MinGray_RegionRed, 255);
		if (32==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Regions2, m_WND);
			return;
		}
		threshold(ho_ImageReduced2, &ho_Region2, 0, _checkparam.i_MaxGray_RegionRed);
		if (33==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region2, m_WND);
			return;
		}
		intersection(ho_Regions2, ho_Region2, &ho_RegionIntersection1);
		closing_circle(ho_RegionIntersection1, &ho_RegionClosing1, _checkparam.d_CR_RegionRed);
		if (34==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionClosing1, m_WND);
			return;
		}
		opening_circle(ho_RegionClosing1, &ho_RegionOpening5, _checkparam.d_OR_RegionRed);
		if (35==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening5, m_WND);
			return;
		}
		connection(ho_RegionOpening5, &ho_ConnectedRegions2);
		select_shape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", _checkparam.i_MinArea_RegionRed, _checkparam.i_MaxArea_RegionRed);
		if (36==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions1, m_WND);
			return;
		}
		if (37 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions1, m_WND);
			return;
		}

		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], ho_RegionErosion4, &ho_ImageReduced5);
		gray_dilation_rect(ho_ImageReduced5, &ho_ImageMax5, _checkparam.i_MaskHeight_RedDefect, _checkparam.i_MaskWidth_RegionRed);
		if (38==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax5, m_WND);
			return;
		}
		if (39 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax5, m_WND);
			return;
		}
		sub_image(ho_ImageMax5, ho_ImageReduced5, &ho_ImageSub7, 1, 0);
		//gray_erosion_rect (ImageReduced5, ImageMin, 5, 5)
		//sub_image (ImageReduced5, ImageMin, ImageSub2, 1, 0)
		threshold(ho_ImageSub7, &ho_Region6, _checkparam.i_MinGray_RedDefect, 255);
		if (40==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region6, m_WND);
			return;
		}
		erosion_circle(ho_RegionErosion4, &ho_RegionErosion3, _checkparam.d_ER_RedDefect);
		if (41==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionErosion3, m_WND);
			return;
		}
		difference(ho_RegionOpening9, ho_RegionErosion3, &ho_RegionDifference8);
		difference(ho_Region6, ho_RegionDifference8, &ho_RegionDifference9);
		opening_circle(ho_RegionDifference9, &ho_RegionOpening8, _checkparam.d_OR_RedDefect);
		if (42==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening8, m_WND);
			return;
		}
		connection(ho_RegionOpening8, &ho_ConnectedRegions15);
		select_shape(ho_ConnectedRegions15, &ho_SelectedRegions9, "area", "and", _checkparam.i_MinArea_RedDefect,
			_checkparam.i_MaxArea_RedDefect);
		if (43==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions9, m_WND);
			return;
		}
		if (44 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions9, m_WND);
			return;
		}
		//threshold (ImageReduced5, Region7, 120, 255)
		//intersection (Region6, Region7, RegionIntersection2)
		//opening_circle (RegionIntersection2, RegionOpening4, 1)
		//connection (RegionOpening4, ConnectedRegions5)
		//select_shape (ConnectedRegions5, SelectedRegions6, 'area', 'and', 20, 2000)

		union2(ho_SelectedRegions1, ho_SelectedRegions9, &ho_RegionUnion2);
		if (_checkparam.b_IsOpen)
		{
			difference(ho_RegionUnion2, ho_RegionDifference15, &ho_RegionUnion2);
		}
		count_obj(ho_RegionUnion2, &hv_Number);
		//if (0 != (hv_Number != 0))
		//{
		//	connection(ho_RegionUnion2, &ho_ConnectedRegions7);
		//	smallest_circle(ho_ConnectedRegions7, &hv_Row3, &hv_Column3, &hv_Radius1);
		//	gen_circle(&ho_Circle1, hv_Row3, hv_Column3, hv_Radius1);
		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//	disp_obj(ho_Circle1, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//	result = QString::fromLocal8Bit("��ɫ����ȱ��");
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ɫ����ȱ��");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//	return 1;
		//	// stop(); only in hdevelop
		//}
		//***��ɫ����
		reduce_domain(ho_ImageReduced1, ho_RegionYellow, &ho_ImageReduced3);
		gray_dilation_rect(ho_ImageReduced3, &ho_ImageMax2, _checkparam.i_MaskHeight_RegionYellow, _checkparam.i_MaskWidth_RegionYellow);
		if (45==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax2, m_WND);
			return;
		}
		if (46 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax2, m_WND);
			return;
		}
		sub_image(ho_ImageMax2, ho_ImageReduced3, &ho_ImageSub, 1, 0);
		threshold(ho_ImageSub, &ho_Region5, _checkparam.i_MinGray_RegionYellow, 255);
		if (47==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region5, m_WND);
			return;
		}
		threshold(ho_ImageReduced3, &ho_Region8, 0, _checkparam.i_MaxGray_RegionYellow);
		if (48==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region8, m_WND);
			return;
		}
		intersection(ho_Region5, ho_Region8, &ho_RegionIntersection3);
		difference(ho_RegionIntersection3, ho_RegionCross, &ho_RegionDifference7);
		connection(ho_RegionDifference7, &ho_ConnectedRegions14);
		opening_circle(ho_RegionDifference7, &ho_RegionOpening3, _checkparam.d_OR_RegionYellow);
		if (49==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening3, m_WND);
			return;
		}
		
		select_shape(ho_RegionOpening3, &ho_SelectedRegions11, "area", "and", _checkparam.i_MinArea_RegionYellow, _checkparam.i_MaxArea_RegionYellow);
		if (_checkparam.b_IsOpen)
		{
			difference(ho_SelectedRegions11, ho_RegionDifference15, &ho_SelectedRegions11);
		}
		if (50==r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions11, m_WND);
			return;
		}
		if (51 == r)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions11, m_WND);
			return;
		}
		area_center(ho_SelectedRegions11, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		//if (0 != (hv_Area.Num()))
		//{
		//	connection(ho_RegionOpening3, &ho_ConnectedRegions10);
		//	smallest_circle(ho_ConnectedRegions10, &hv_Row4, &hv_Column4, &hv_Radius2);
		//	gen_circle(&ho_Circle2, hv_Row4, hv_Column4, hv_Radius2);
		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//	disp_obj(ho_Circle2, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//	result = QString::fromLocal8Bit("��ɫ����ȱ��");
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ɫ����ȱ��");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//	// stop(); only in hdevelop
		//	return 1;
		//}
		//***ҩ��ȱ��
		gray_dilation_rect(ho_ImageChannel[_checkparam.i_Channel_Middle], &ho_ImageMax3, _checkparam.i_MaskHeight_Middle, _checkparam.i_MaskWidth_Middle);
		if (r==62)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax3, m_WND);
			return;
		}
		if (r == 63)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_ImageMax3, m_WND);
			return;
		}
		sub_image(ho_ImageMax3, ho_ImageChannel[_checkparam.i_Channel_Middle], &ho_ImageSub4, 1, 0);
		threshold(ho_ImageSub4, &ho_Region9, _checkparam.i_MinGray_Middle, 255);
		if (r == 64)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region9, m_WND);
			return;
		}
		intersection(ho_Region9, ho_Rectangle4, &ho_RegionIntersection13);
		connection(ho_RegionIntersection13, &ho_ConnectedRegions24);
		select_shape(ho_ConnectedRegions24, &ho_SelectedRegions19, (HTuple("width").Append("height")),
			"and", (HTuple(_checkparam.i_MinWidth_Middle).Append(_checkparam.i_MinHeight_Middle)), (HTuple(99999).Append(9999)));
		if (r == 65)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions19, m_WND);
			return;
		}
		if (r == 66)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions19, m_WND);
			return;
		}
		union1(ho_SelectedRegions19, &ho_RegionUnion7);
		closing_circle(ho_RegionUnion7, &ho_RegionClosing7, _checkparam.d_CR_Middle);
		if (r == 67)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionClosing7, m_WND);
			return;
		}
		connection(ho_RegionClosing7, &ho_ConnectedRegions23);
		select_shape(ho_ConnectedRegions23, &ho_SelectedRegions18, "area", "and", _checkparam.i_MinArea_Middle,
			99999);
		if (r == 68)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Middle], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions18, m_WND);
			return;
		}


		reduce_domain(ho_ImageChannel[_checkparam.i_Channel_Plate], ho_RegionErosion6, &ho_ImageReduced13);
		threshold(ho_ImageReduced13, &ho_Regions1, 0, _checkparam.i_MaxGray_Plate);
		if (r == 70)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Regions1, m_WND);
			return;
		}
		closing_circle(ho_Regions1, &ho_RegionClosing8, _checkparam.d_CR_Plate);
		if (r == 71)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionClosing8, m_WND);
			return;
		}
		fill_up(ho_RegionClosing8, &ho_RegionFillUp7);
		opening_circle(ho_RegionFillUp7, &ho_RegionOpening17, _checkparam.d_OR_Plate_1);
		if (r == 72)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening17, m_WND);
			return;
		}
		connection(ho_RegionOpening17, &ho_ConnectedRegions25);
		select_shape(ho_ConnectedRegions25, &ho_SelectedRegions20, "area", "and", _checkparam.i_MinArea_Plate_1,
			99999);
		if (r == 73)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions20, m_WND);
			return;
		}
		shape_trans(ho_SelectedRegions20, &ho_RegionTrans8, "convex");
		union2(ho_RegionTrans8, ho_Rectangle1, &ho_RegionUnion8);
		difference(ho_RegionErosion6, ho_RegionUnion8, &ho_RegionDifference17);
		intersection(ho_RegionDifference17, ho_Region9, &ho_RegionIntersection14);
		fill_up(ho_RegionIntersection14, &ho_RegionFillUp8);
		opening_circle(ho_RegionFillUp8, &ho_RegionOpening18, _checkparam.d_OR_Plate_2);
		if (r == 74)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening18, m_WND);
			return;
		}
		connection(ho_RegionOpening18, &ho_ConnectedRegions26);
		select_shape(ho_ConnectedRegions26, &ho_SelectedRegions21, "area", "and", _checkparam.i_MinArea_Plate_2,
			99999);
		if (r == 75)
		{
			disp_obj(ho_ImageChannel[_checkparam.i_Channel_Plate], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions21, m_WND);
			return;
		}
		union2(ho_SelectedRegions18, ho_SelectedRegions21, &ho_RegionUnion9);
		connection(ho_RegionUnion9, &ho_ConnectedRegions27);
		//if (0 != (hv_Number2 != 0))
		//{
		//	smallest_circle(ho_SelectedRegions8, &hv_Row2, &hv_Column2, &hv_Radius);
		//	gen_circle(&ho_Circle, hv_Row2, hv_Column2, hv_Radius);
		//	disp_obj(m_hoLiveImage, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "red");
		//	disp_obj(ho_Circle, Wnd == -1 ? m_ShowLabel[0] : Wnd);
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, 30, 30);
		//	result = QString::fromLocal8Bit("ҩ��ȱ��");
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "ҩ��ȱ��");
		//	set_color(Wnd == -1 ? m_ShowLabel[0] : Wnd, "green");
		//	set_tposition(Wnd == -1 ? m_ShowLabel[0] : Wnd, hv_Height - 50, 0);
		//	write_string(Wnd == -1 ? m_ShowLabel[0] : Wnd, "��ǰ�㷨�汾Ϊ��" + hv_AlgVersion);
		//	// stop(); only in hdevelop
		//	return 1;
		//}


		//switch (r)
		//{

		//case 1:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Region, m_WND);
		//	//set_tposition(m_WND, 10,10);
		//	//write_string(m_WND, "ȱ�������" + hv_Area7);
		//	break;
		//}
		//case 2:
		//{
		//	if (hv_AreaBand < _checkparam.i_MinArea_Band)
		//	{
		//		disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
		//		set_draw(m_WND, "margin");
		//		set_color(m_WND, "red");
		//		set_line_width(m_WND, 3);
		//		disp_obj(ho_RegionBand, m_WND);
		//		set_tposition(m_WND, 10, 10);
		//		write_string(m_WND, "���ְ������" + hv_AreaBand);
		//	}
		//	else
		//	{
		//		disp_obj(ho_ImageChannel[_checkparam.i_BandChannel], m_WND);
		//		set_draw(m_WND, "margin");
		//		set_color(m_WND, "green");
		//		set_line_width(m_WND, 3);
		//		disp_obj(ho_RegionBand, m_WND);
		//		set_tposition(m_WND, 10, 10);
		//		write_string(m_WND, "���ְ������" + hv_AreaBand);
		//	}

		//	//set_tposition(m_WND, 10, 10);
		//	//write_string(m_WND, "�����ֵ��" + HTuple(_checkparam.i_BandException2_Value));
		//	break;
		//}
		//case 4:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Regions, m_WND);

		//	break;
		//}
		//case 5:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionOpening, m_WND);
		//	break;
		//}
		//case 6:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions, m_WND);
		//	break;
		//}
		//case 7:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions, m_WND);
		//	break;
		//}
		//case 8:
		//{
		//	if (hv_NumberPill == _checkparam.i_Number_Capsules)
		//	{
		//		disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
		//		set_draw(m_WND, "margin");
		//		set_color(m_WND, "green");
		//		set_line_width(m_WND, 3);
		//		disp_obj(ho_RegionPill, m_WND);
		//	}
		//	else
		//	{
		//		disp_obj(ho_ImageChannel[_checkparam.i_CapsulesChannel], m_WND);
		//		set_draw(m_WND, "margin");
		//		set_color(m_WND, "red");
		//		set_line_width(m_WND, 3);
		//		disp_obj(ho_RegionPill, m_WND);
		//	}

		//	break;
		//}

		//case 10:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "fill");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Rectangle, m_WND);
		//	break;
		//}
		//case 11:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "fill");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Rectangle, m_WND);
		//	break;
		//}
		//case 12:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionErosion, m_WND);
		//	break;
		//}
		//case 13:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax, m_WND);
		//	break;
		//}
		//case 14:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax, m_WND);
		//	break;
		//}
		//case 15:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Region4, m_WND);
		//	break;
		//}
		//case 16:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionOpening1, m_WND);
		//	break;
		//}
		//case 17:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionClosing, m_WND);
		//	break;
		//}
		//case 18:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions2, m_WND);
		//	break;
		//}
		//case 19:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_BatchChannel], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions2, m_WND);
		//	break;
		//}
		//case 21:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionErosion2, m_WND);
		//	break;
		//}
		//case 22:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Region1, m_WND);
		//	break;
		//}
		//case 23:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions4, m_WND);
		//	break;
		//}
		//case 24:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions4, m_WND);
		//	break;
		//}
		//case 25:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionDilation2, m_WND);
		//	break;
		//}
		//case 26:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_CapsulesDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionDilation3, m_WND);
		//	break;
		//}
		//case 30:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax1, m_WND);
		//	break;
		//}
		//case 31:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax1, m_WND);

		//	break;
		//}
		//case 32:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Regions2, m_WND);
		//	break;
		//}
		//case 33:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Region2, m_WND);
		//	break;
		//}
		//case 34:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionClosing1, m_WND);
		//	break;
		//}
		//case 35:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionOpening5, m_WND);
		//	break;
		//}
		//case 36:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions1, m_WND);
		//	break;
		//}
		//case 37:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect1], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions1, m_WND);
		//	break;
		//}
		//case 38:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax5, m_WND);
		//	break;
		//}
		//case 39:
		//{

		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax5, m_WND);
		//	break;
		//}
		//case 40:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Region6, m_WND);
		//	break;
		//}
		//case 41:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionErosion3, m_WND);
		//	break;
		//}
		//case 42:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionOpening8, m_WND);
		//	break;
		//}
		//case 43:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions9, m_WND);
		//	break;
		//}
		//case 44:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_RedDefect2], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions9, m_WND);
		//	break;
		//}
		//case 45:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax2, m_WND);
		//	break;
		//}
		//case 46:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax2, m_WND);
		//	break;
		//}
		//case 47:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Region5, m_WND);
		//	break;
		//}
		//case 48:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Region8, m_WND);
		//	break;
		//}
		//case 49:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionOpening3, m_WND);
		//	break;
		//}
		//case 50:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions11, m_WND);
		//	break;
		//}
		//case 51:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions11, m_WND);
		//	break;
		//}

		//case 53:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionDilation, m_WND);
		//	break;
		//}
		//case 54:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionErosion1, m_WND);
		//	break;
		//}
		//case 55:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax3, m_WND);
		//	break;
		//}
		//case 56:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_ImageMax3, m_WND);
		//	break;
		//}
		//case 57:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_Region9, m_WND);
		//	break;
		//}
		//case 58:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_RegionClosing3, m_WND);
		//	break;
		//}
		//case 59:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions8, m_WND);
		//	break;
		//}
		//case 60:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions8, m_WND);
		//	break;
		//}
		//case 61:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions8, m_WND);
		//	break;
		//}
		//case 62:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions8, m_WND);
		//	break;
		//}
		//case 63:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions8, m_WND);
		//	break;
		//}
		//case 64:
		//{
		//	disp_obj(ho_ImageChannel[_checkparam.i_Channel_BandDefect], m_WND);
		//	set_draw(m_WND, "margin");
		//	set_color(m_WND, "red");
		//	set_line_width(m_WND, 3);
		//	disp_obj(ho_SelectedRegions8, m_WND);
		//	break;
		//}
		//}
	}
	catch (HException &e)
	{
		if (nullptr!=m_timerChanger)
		{
			m_timerChanger->stop();
			delete m_timerChanger;
			m_timerChanger = nullptr;
		}

		//QMessageBox::warning(nullptr, "", e.message);
	}

}
void QtGuiSetting::SetModelMat(Mat tempgray)
{
=======
>>>>>>> ff61316fb6997015f5341c9e555eab343b892193
	try
	{
		tempgray.copyTo(m_MOriginal);
	}
	catch (cv::Exception)
	{
		return;
	}
}
bool QtGuiSetting::isImage(QFileInfo& info)
{
	QString filename = info.fileName();
	if (filename == "." || filename == "..")
		return false;
	if (filename.contains("def") || filename.contains(".csv"))
		return false;
	return filename.contains(".bmp") || filename.contains(".BMP") ||
		filename.contains(".jpg") || filename.contains(".JPG") ||
		filename.contains(".jpeg") || filename.contains(".JPEG") ||
		filename.contains(".png") || filename.contains(".PNG") ||
		filename.contains(".tif") || filename.contains(".TIF");
}
bool QtGuiSetting::containImages(QDir& dir)
{
	foreach(QFileInfo entry, dir.entryInfoList(QDir::Dirs | QDir::Files)) {
		QString filename = entry.fileName();
		if (filename == "." || filename == "..")
			continue;
		if (entry.isDir() && containImages(QDir(entry.absoluteFilePath())))
			return true;
		if (entry.isFile() && isImage(entry))
			return true;
	}
	return false;
}
void QtGuiSetting::initImageLS(QString str)
{
	QDir dir(str);
	ui.lw_ImageList->clear();
	ui.lw_ImageList->addItem(".");
	ui.lw_ImageList->addItem("..");
	foreach(QFileInfo mfi, dir.entryInfoList(QDir::Dirs | QDir::Files, QDir::Time))
	{
		if (mfi.fileName() == "." || mfi.fileName() == "..")
			continue;
		if (mfi.isDir()) {
			if (containImages(QDir(mfi.absoluteFilePath())))
				ui.lw_ImageList->addItem(mfi.fileName());
			continue;
		}
		if (mfi.isFile() && isImage(mfi))
			ui.lw_ImageList->addItem(mfi.fileName());
	}
}
void QtGuiSetting::closeEvent(QCloseEvent *)
{
	close_window(HTuple(m_WND));
	if (m_bChanged)
	{
		if (QMessageBox::Save == QMessageBox::question(this, QString::fromLocal8Bit("�������޸�"), QString::fromLocal8Bit("�Ƿ񱣴棿"), QMessageBox::Save, QMessageBox::No))
		{
			emit ShouldSaveParam();
			try
			{
			std::ofstream fout(QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml").toStdString());
			fout << m_node;
			fout.close();
			}
			catch (YAML::ParserException e)
			{
				QMessageBox::warning(nullptr, "", e.what());
			}
			catch (YAML::RepresentationException e)
			{
				QMessageBox::warning(nullptr, "", e.what());
			}
			catch (YAML::Exception e)
			{
				QMessageBox::warning(nullptr, "", e.what());
			}
		}
		m_bChanged = false;
		ui.pB_Save->setEnabled(false);
	}
}
void QtGuiSetting::onBtnGetImage()
{
	return;
}
void QtGuiSetting::m_TestApplyParam()
{
	return;
}
void QtGuiSetting::onBtnGetSample()
{
	return;
}
void QtGuiSetting::onShowImage(Mat* tempgray)
{
	try
	{
		Mat temp;
		tempgray->copyTo(temp);
		int zz = ui.ShowLabel->frameWidth();
		QSize ss = ui.ShowLabel->size();
		ss.setWidth(ss.width() - zz * 2);
		ss.setHeight(ss.height() - zz * 2);
		cv::resize(temp, temp, Size(ss.width(), ss.height()));
		disImage = QImage((const unsigned char*)(temp.data), temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
		ui.ShowLabel->setPixmap(QPixmap::fromImage(disImage));
	}
	catch (cv::Exception e)
	{
		return;
	}
	return;
}
void QtGuiSetting::onShowFps(int i)
{
}
extern bool compColx(const Rect &a, const Rect &b);
extern bool compColy(const Rect &a, const Rect &b);
void QtGuiSetting::onSelectTrainFile()
{
}
void QtGuiSetting::onClickedImage(QListWidgetItem *item)
{
	if (nullptr == clickedtimer)
	{
		clickedtimer = new QTimer();
		clickedtimer->setSingleShot(true);
		bool b = connect(clickedtimer, SIGNAL(timeout()), this, SLOT(onSingleClicked()));
		clickedtimer->start(500);
		return;
	}
	else
	{
		clickedtimer->stop();
		delete clickedtimer;
		clickedtimer = nullptr;
	}
	onSelectImageList(item);
}
void QtGuiSetting::onSingleClicked()
{
	if (nullptr != clickedtimer)
	{
		clickedtimer->stop();
		delete clickedtimer;
		clickedtimer = nullptr;
	}
	QString pathselect;
	QListWidgetItem *item = ui.lw_ImageList->currentItem();
	QString sSelectItem = item->text();
	pathselect = m_sImageListPath + "/" + sSelectItem;
	m_currentImagePath = pathselect;
	QFileInfo file(pathselect);
	if (!file.isFile() && m_timerReadList == nullptr)
	{
	}
	else
	{
		m_MOriginal = imread(m_currentImagePath.toStdString().c_str());
		if (m_MOriginal.rows > 0)
		{
			HTuple hv_W, hv_H;
			Hobject ho_Image = Mat2Hobject(m_MOriginal);
			get_image_size(ho_Image, &hv_W, &hv_H);
			set_part(m_WND, 0, 0, hv_H - 1, hv_W - 1);
			disp_obj(ho_Image, m_WND);
		}
	}
}
void QtGuiSetting::onSelectImageList(QListWidgetItem *item)
{
	//&&m_timerReadList==nullptr ��ֹ������һ��Ŀ¼
	QString sSelectItem = item->text();
	if (sSelectItem == "."&&m_timerReadList == nullptr)
	{
		QString newPath = m_sImageListPath.left(m_sImageListPath.indexOf("/") + 1);
		m_sImageListPath = newPath;
		initImageLS(m_sImageListPath);
		return;
	}
	if (sSelectItem == ".."&&m_timerReadList == nullptr)
	{
		QString newPath = m_sImageListPath.left(m_sImageListPath.lastIndexOf("/"));
		//exe���ڵĸ�Ŀ¼
		if (newPath.length() > 2)
		{
			// only allow user to access the data in dataPath
			m_sImageListPath = newPath;
		}
		if (newPath.length() == 2)
		{
			// only allow user to access the data in dataPath
			m_sImageListPath = newPath + "/";
		}
		initImageLS(m_sImageListPath);
		return;
	}
	QString pathselect;
	pathselect = m_sImageListPath + "/" + sSelectItem;
	m_currentImagePath = pathselect;
	QFileInfo file(pathselect);
	if (!file.isFile() && m_timerReadList == nullptr)
	{
		m_sImageListPath = pathselect;
		initImageLS(m_sImageListPath);
	}
	else
	{
		m_MOriginal = imread(std::string(m_currentImagePath.toLocal8Bit()));
		if (m_MOriginal.rows > 0)
		{
			uint m_iResult;	//���μ��������󲻳����˸� 
			int i_captotal = 0;
			QString str;
			bool results = ((CInterCHeck*)p_Parent)->Check(m_MOriginal, &_checkparam, str);
			bool b = ((CInterCHeck*)p_Parent)->RealCheck(str, &_checkparam, m_WND);
		}
	}
}
int QtGuiSetting::showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str)//ȫ������
{
	if (QString::fromLocal8Bit(button2Str) == "")
	{
		QMessageBox msg(icon, QString::fromLocal8Bit(titleStr), QString::fromLocal8Bit(contentStr), QMessageBox::Ok);
		msg.setButtonText(QMessageBox::Ok, QString::fromLocal8Bit(button1Str));
		msg.setWindowIcon(QIcon("./dr.ico"));
		return msg.exec();
	}
	else
	{
		QMessageBox msg(icon, QString::fromLocal8Bit(titleStr), QString::fromLocal8Bit(contentStr), QMessageBox::Yes | QMessageBox::No);
		msg.setButtonText(QMessageBox::Yes, QString::fromLocal8Bit(button1Str));
		msg.setButtonText(QMessageBox::No, QString::fromLocal8Bit(button2Str));
		msg.setWindowIcon(QIcon("./dr.ico"));
		return msg.exec();
	}
}
