#include "QtGuiSetting.h"
#include "InterCHeck.h"
#include <QtWidgets/QComboBox>
#include <QDesktopWidget>
typedef void*   UI_MONITOR;
void ShowCallBack(UI_MONITOR ui, int pos, Mat img, int times)
{
	((QtGuiSetting*)ui)->emit SignShowImage(pos, img, times);
}
QtGuiSetting::QtGuiSetting(QWidget *parent, void* AlgPointer)
	: QDialog(parent)
{
	b_GetAuthority = false;
	m_sSelectFile = "";
	m_strSaveFile = "";
	m_strTrainFile = "";
	m_bChanged = false;
	m_bRectChanged = false;
	m_timerReadList = nullptr;
	clickedtimer = nullptr;
	ui.setupUi(this);
	ui.tableWidget->setColumnCount(2);
	QStringList LIST;
	LIST << QString::fromLocal8Bit("项目") << QString::fromLocal8Bit("参数");
	ui.tableWidget->setHorizontalHeaderLabels(LIST);
	ui.tableWidget->setColumnWidth(1, 40);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->viewport()->installEventFilter(this);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(35);//默认行高20
	pp = parent;
	ui.ShowLabel->setMouseTracking(true);
	HTuple hv_WindowID;
	open_window(ui.ShowLabel->y(), ui.ShowLabel->x(), ui.ShowLabel->width(), ui.ShowLabel->height(), (long)((QWidget*)ui.ShowLabel->parent())->winId(), "", "", &hv_WindowID);
	m_WND = hv_WindowID[0];
	ui.tableWidget->setMouseTracking(true);
	setMouseTracking(true);
	m_bButton = false;
	m_SAppPath = qApp->applicationDirPath();//exe文件夹
	m_sImageListPath = m_SAppPath;
	initImageLS(m_sImageListPath);
	connect(ui.lw_ImageList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onClickedImage(QListWidgetItem *)));
	//connect(ui.lw_ImageList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onSelectImageList(QListWidgetItem *)));
	connect(ui.pB_ContinueTest, SIGNAL(toggled(bool)), this, SLOT(onContinueTest(bool)));
	connect(ui.pB_Save, SIGNAL(clicked()), this, SLOT(onSaveParam()));
	connect(ui.pB_Exit, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "退出确认", "确认退出算法设置页面吗？", "确认", "取消"))
		{
			close();
		}});
	//设置check parent 算法显示位置
	connect(this, SIGNAL(SignShowImage(int, Mat, int)), this, SLOT(SLOTShowImage(int, Mat, int)));
	connect(ui.pB_Calibration, SIGNAL(clicked()), this, SLOT(onAutoDetest()));
	p_Parent = AlgPointer;
	((CInterCHeck*)p_Parent)->StartCheck("", nullptr,m_MOriginal.cols,m_MOriginal.rows);
	((CInterCHeck*)p_Parent)->SetShowCallBack(this, ShowCallBack);
	QObject::connect(ui.tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(onCellChanged(int, int)));
	QDesktopWidget *dwsktopwidget = QApplication::desktop();
	QRect deskrect = dwsktopwidget->availableGeometry();
	QRect screenrect = dwsktopwidget->screenGeometry();
	if (deskrect.height() < 1100)
	{
		this->resize(768*2+5, 1366/2);
		ui.groupBox->move(ui.ShowLabel->pos().x() + ui.ShowLabel->width() + 2, ui.ShowLabel->pos().y());
		//ui.lw_ImageList->move(ui.tableWidget->pos().x() + ui.tableWidget->width() + 2, ui.tableWidget->pos().y());
	}
}
void QtGuiSetting::mouseReleaseEvent(QMouseEvent *p)
{
	setCursor(Qt::ArrowCursor);
	m_bButton = false;
	return;
}
bool QtGuiSetting::eventFilter(QObject * watched, QEvent * event)
{
	if (watched == ui.tableWidget->viewport())
	{
		if (event->type() == QEvent::MouseMove) {
			mouseMoveEvent((QMouseEvent *)event);
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
		HTuple hv_Width, hv_Height;
		Hobject ho_Image = Mat2Hobject(m_MOriginal);
		get_image_size(ho_Image, &hv_Width, &hv_Height);
		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		disp_obj(ho_Image, m_WND);
		return;
		QRect rec = ui.ShowLabel->geometry();
		m_PointOriginal = p->pos();
		m_PointOriginal.setX(m_PointOriginal.x() - rec.x() - ui.ShowLabel->frameWidth());
		m_PointOriginal.setY(m_PointOriginal.y() - rec.y() - ui.ShowLabel->frameWidth());
		m_bButton = true;
	}
	return;
}
void QtGuiSetting::mouseMoveEvent(QMouseEvent *p)
{
	QPoint poi = QCursor::pos();
	if (ui.ShowLabel->geometry().contains(this->mapFromGlobal(poi)) && b_GetAuthority)
	{
		setCursor(Qt::CrossCursor);
		m_PointFinale = p->pos();
		if (m_bButton)
		{
			QRect rec = ui.ShowLabel->geometry();
			QPoint pt = p->pos();
			pt.setX(pt.x() - rec.x() - ui.ShowLabel->frameWidth());
			pt.setY(pt.y() - rec.y() - ui.ShowLabel->frameWidth());
			double scale_x = m_MOriginal.cols*1.0 / (rec.width() - ui.ShowLabel->frameWidth() * 2);
			double scale_y = m_MOriginal.rows*1.0 / (rec.height() - ui.ShowLabel->frameWidth() * 2);
			if (pt.x() > m_PointOriginal.x())
			{
				m_RectToDraw.setLeft(m_PointOriginal.x());
				m_RectToDraw.setRight(pt.x());
			}
			else
			{
				m_RectToDraw.setRight(m_PointOriginal.x());
				m_RectToDraw.setLeft(pt.x());
			}
			if (pt.y() > m_PointOriginal.y())
			{
				m_RectToDraw.setTop(m_PointOriginal.y());
				m_RectToDraw.setBottom(pt.y());
			}
			else
			{
				m_RectToDraw.setBottom(m_PointOriginal.y());
				m_RectToDraw.setTop(pt.y());
			}
			cv::Mat img2show;
			cvtColor(m_MOriginal, img2show, COLOR_BGR2RGB);
			m_RectOpencv = cv::Rect(m_RectToDraw.x()*scale_x, m_RectToDraw.y()*scale_y, m_RectToDraw.width()*scale_x, m_RectToDraw.height()*scale_y);
			cv::rectangle(img2show, m_RectOpencv, cv::Scalar(255, 0, 0), 5, 8, 0);
			onShowImage(&img2show);
			m_bChanged = true;
		}
	}
	else
		setCursor(Qt::ArrowCursor);
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
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("连续测试错误"), QString::fromLocal8Bit("未定位到图像文件"));
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
		if (QMessageBox::Save == QMessageBox::question(this, QString::fromLocal8Bit("参数已修改"), QString::fromLocal8Bit("是否保存？"), QMessageBox::Save, QMessageBox::No))
		{
			emit ShouldSaveParam(&_checkparam);
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
	QComboBox* obj = qobject_cast<QComboBox*>(sender());//判断是哪个按钮触发了槽函数
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
	if (obj->objectName() =="BandChannel")
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
void QtGuiSetting::SetParam(CHECKPARAM param)
{
	_checkparam = param;
	ui.tableWidget->blockSignals(true);
	ui.tableWidget->setSortingEnabled(false);
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);
	int rowindex = ui.tableWidget->rowCount();
	QTableWidgetItem* item;
	QStringList stlist;
	QComboBox *box;
	QSlider* hsd;
	if (0 == rowindex)
	{
		//rowindex = ui.tableWidget->rowCount();
		//ui.tableWidget->insertRow(rowindex);
		//item = new QTableWidgetItem();
		//item->setText(QString::fromLocal8Bit("泡罩板通道"));
		//item->setTextAlignment(Qt::AlignCenter);
		//ui.tableWidget->setItem(rowindex, 0, item);
		//stlist.clear();
		//stlist << "R" << "G" << "B" << "H" << "S" << "V";
		//box = new QComboBox();
		//box->setObjectName("BandChannel");
		//box->addItems(stlist);
		//box->setCurrentIndex(_checkparam.i_BandChannel);
		//connect(box, SIGNAL(activated(int)), this, SLOT(onSelectImageChannel(int)));
		//ui.tableWidget->setCellWidget(rowindex, 1, box);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("泡罩板面积"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_BandArea));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);
		
		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("板面缺陷1面积"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_BandException1_Area));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("板面缺陷2阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_BandException2_Value));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("板面缺陷2阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		hsd = new QSlider(Qt::Horizontal);
		hsd->setMinimum(0);
		hsd->setMaximum(255);
		hsd->setValue(_checkparam.i_BandException2_Value);
		hsd->setStyleSheet("  \
     QSlider::add-page:Horizontal\
     {     \
        background-color: rgb(87, 97, 106);\
        height:4px;\
     }\
     QSlider::sub-page:Horizontal \
    {\
        background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(7,208,255, 255), stop:1 rgba(7,208,255, 255));\
        height:4px;\
     }\
    QSlider::groove:Horizontal \
    {\
        background:transparent;\
        height:6px;\
    }\
    QSlider::handle:Horizontal \
    {\
        height: 25px;\
        width:35px;\
        border-image: url(./ico/btn.png);\
        margin: -15 0px; \
    }\
    ");
		connect(hsd, &QSlider::valueChanged, [=]() {
			ui.tableWidget->item(rowindex - 1, 1)->setText(QString::number(hsd->value()));
		});//利用lambda表达式可用
		ui.tableWidget->setCellWidget(rowindex, 1, hsd);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("板面缺陷3面积"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_BandException3_Area));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		
		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("内部缺陷面积"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_InterException_Area));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);



		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("内部缺陷开运算"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.d_InterException_Open));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("药剂区域半径"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_PillRadius));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("板面缺陷2半径增量"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_BandException2_Radius));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);
 	}
	ui.tableWidget->blockSignals(false);
}
void QtGuiSetting::onCellChanged(int r, int c)
{
	try
	{
		c = 1;//防止键盘table
		HTuple hv_Width, hv_Height;

		Mat MatToShow(m_MOriginal);
		//Local iconic variables 
		Hobject  ho_R, ho_G, ho_B, ho_H, ho_S;
		Hobject  ho_V, ho_Region, ho_RegionFillUp, ho_ConnectedRegions;
		Hobject  ho_SelectedRegions, ho_RegionBand, ho_BandErosion;
		Hobject  ho_ImageSub2, ho_ImageReduced, ho_Region1, ho_RegionFillUp1;
		Hobject  ho_RegionOpening, ho_ConnectedRegions1, ho_RegionClosing4;
		Hobject  ho_PillRegions, ho_PillDilation, ho_RegionDifference1;
		Hobject  ho_ImageReduced1, ho_Region3, ho_RegionOpening2;
		Hobject  ho_RegionFillUp2, ho_ConnectedRegions5, ho_RegionSeam;
		Hobject  ho_RegionTrans3, ho_RegionUnion1, ho_Rectangle;
		Hobject  ho_RegionDifference, ho_RegionErosion, ho_ImageMax;
		Hobject  ho_ImageSub, ho_Region2, ho_RegionOpening1, ho_RegionIntersection;
		Hobject  ho_ConnectedRegions3, ho_RegionTrans, ho_SelectedRegions2;
		Hobject  ho_RegionData, ho_RegionUnion, ho_RegionClosing;
		Hobject  ho_ConnectedRegions4, ho_SelectedRegions3, ho_RegionIntersection4;
		Hobject  ho_ImageReduced4, ho_ImageMax3, ho_RegionDynThresh;
		Hobject  ho_RegionFillUp4, ho_RegionOpening8, ho_Region4;
		Hobject  ho_ConnectedRegions6, ho_RegionClosing1, ho_RegionOpening3;
		Hobject  ho_SelectedRegions4, ho_RegionIntersection1, ho_RegionLeft;
		Hobject  ho_ImageMean, ho_ImageMax4, ho_ImageSub4, ho_Region8;
		Hobject  ho_RegionIntersection6, ho_RegionFillUp6, ho_RegionOpening11;
		Hobject  ho_ConnectedRegions10, ho_SelectedRegions7, ho_Region9;
		Hobject  ho_Circle2, ho_RegionDifference3, ho_RegionIntersection7;
		Hobject  ho_RegionClosing6, ho_ImageReduced2, ho_ImageMax1;
		Hobject  ho_ImageSub1, ho_ImageReduced3, ho_Region5, ho_RegionFillUp3;
		Hobject  ho_RegionClosing2, ho_RegionOpening4, ho_ConnectedRegions7;
		Hobject  ho_RegionTrans1, ho_SelectedRegions5, ho_BandStrange;
		Hobject  ho_PillInter, ho_PillDilation1, ho_RegionErosion3;
		Hobject  ho_PillDilation2, ho_PillImage, ho_ImageMax2, ho_ImageSub3;
		Hobject  ho_Region6, ho_RegionOpening6, ho_RegionClosing3;
		Hobject  ho_ConnectedRegions9, ho_SelectedRegions6, ho_ConnectedRegions8;
		Hobject  ho_SortedRegions, ho_RegionOpening5, ho_ObjectSelected;
		Hobject  ho_Circle, ho_RegionDifference2, ho_RegionOpening7;
		Hobject  ho_RegionDilation, ho_RegionIntersection3;


		// Local control variables 
		HTuple  hv_ImageFiles, hv_Index, hv_Area, hv_ExpDefaultCtrlDummyVar;
		HTuple  hv_Number, hv_Row5, hv_Column5, hv_Radius3, hv_Area1;
		HTuple  hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2;
		HTuple  hv_Number1, hv_Row1, hv_Column1, hv_Phi1, hv_Length11;
		HTuple  hv_Length21, hv_Area6, hv_Area2, hv_Area7, hv_Row4;
		HTuple  hv_Column4, hv_Radius2, hv_Area8, hv_Area3, hv_Index1;
		HTuple  hv_Row2, hv_Column2, hv_Radius, hv_Area4, hv_Area5;
		HTuple  hv_Number2, hv_Area9;
		Hobject ho_ImageChannel[6];
		if (m_MOriginal.empty())
		{
			return;
		}
		Hobject ho_Image = Mat2Hobject(m_MOriginal);
		decompose3(ho_Image, &ho_ImageChannel[0], &ho_ImageChannel[1], &ho_ImageChannel[2]);
		trans_from_rgb(ho_ImageChannel[0], ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageChannel[3], &ho_ImageChannel[4], &ho_ImageChannel[5], "hsv");
		get_image_size(ho_Image, &hv_Width, &hv_Height);
		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		//_checkparam.i_BandArea = ((QComboBox*)(ui.tableWidget->cellWidget(0, 1)))->currentIndex();
		_checkparam.i_BandArea = ui.tableWidget->item(0, c)->text().toInt();
		//_checkparam.i_PillChannel1 = ((QComboBox*)(ui.tableWidget->cellWidget(3, 1)))->currentIndex();
		_checkparam.i_BandException1_Area = ui.tableWidget->item(1, c)->text().toInt();
		_checkparam.i_BandException2_Value = ui.tableWidget->item(2, c)->text().toInt();
		_checkparam.i_BandException3_Area = ui.tableWidget->item(4, c)->text().toInt();
		_checkparam.i_InterException_Area = ui.tableWidget->item(5, c)->text().toInt();
		_checkparam.d_InterException_Open = ui.tableWidget->item(6, c)->text().toDouble();
		_checkparam.i_PillRadius = ui.tableWidget->item(7, c)->text().toInt();
		_checkparam.i_BandException2_Radius = ui.tableWidget->item(8, c)->text().toInt();


		//**找到整体药版区域
		threshold(ho_ImageChannel[0], &ho_Region, 120, 255);
		fill_up(ho_Region, &ho_RegionFillUp);
		connection(ho_RegionFillUp, &ho_ConnectedRegions);
		select_shape_std(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
		shape_trans(ho_SelectedRegions, &ho_RegionBand, "convex");
		area_center(ho_RegionBand, &hv_Area, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);

		//**药剂数量
		erosion_circle(ho_RegionBand, &ho_BandErosion, 10.5);
		sub_image(ho_ImageChannel[1], ho_ImageChannel[2], &ho_ImageSub2, 1, 0);
		reduce_domain(ho_ImageSub2, ho_BandErosion, &ho_ImageReduced);
		threshold(ho_ImageReduced, &ho_Region1, 25, 255);
		fill_up(ho_Region1, &ho_RegionFillUp1);
		opening_circle(ho_RegionFillUp1, &ho_RegionOpening, 3.5);
		connection(ho_RegionOpening, &ho_ConnectedRegions1);
		closing_circle(ho_ConnectedRegions1, &ho_RegionClosing4, 3.5);
		select_shape(ho_RegionClosing4, &ho_PillRegions, (HTuple("area").Append("circularity")),
			"and", (HTuple(16500).Append(0.8)), (HTuple(22500).Append(1.0)));

		//去除药剂区域
		smallest_circle(ho_PillRegions, &hv_Row5, &hv_Column5, &hv_Radius3);
		gen_circle(&ho_PillDilation, hv_Row5, hv_Column5, (hv_Radius3 / hv_Radius3) * _checkparam.i_PillRadius);
		difference(ho_BandErosion, ho_PillDilation, &ho_RegionDifference1);

		//**喷印日期
		shape_trans(ho_PillRegions, &ho_RegionTrans3, "convex");
		union1(ho_RegionTrans3, &ho_RegionUnion1);
		smallest_rectangle2(ho_RegionUnion1, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
			&hv_Length2);
		gen_rectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2 + 500);
		difference(ho_RegionBand, ho_Rectangle, &ho_RegionDifference);
		//connection (RegionDifference, ConnectedRegions2)

		//select_shape_std (ConnectedRegions2, SelectedRegions1, 'max_area', 70)
		erosion_circle(ho_RegionDifference, &ho_RegionErosion, 15);
		gray_dilation_rect(ho_ImageChannel[1], &ho_ImageMax, 7, 7);
		sub_image(ho_ImageMax, ho_ImageChannel[1], &ho_ImageSub, 1, 0);
		threshold(ho_ImageSub, &ho_Region2, 10, 255);
		opening_circle(ho_Region2, &ho_RegionOpening1, 2);
		intersection(ho_RegionErosion, ho_RegionOpening1, &ho_RegionIntersection);
		connection(ho_RegionIntersection, &ho_ConnectedRegions3);
		shape_trans(ho_ConnectedRegions3, &ho_RegionTrans, "convex");
		select_shape(ho_RegionTrans, &ho_SelectedRegions2, "area", "and", 200, 99999);
		count_obj(ho_SelectedRegions2, &hv_Number1);
		if (0 != (hv_Number1 == 0))
		{

			gen_empty_obj(&ho_RegionData);
			// stop(); only in hdevelop
			//continue
		}
		else
		{
			union1(ho_SelectedRegions2, &ho_RegionUnion);
			closing_rectangle1(ho_RegionUnion, &ho_RegionClosing, 1, 100);
			connection(ho_RegionClosing, &ho_ConnectedRegions4);
			select_shape_std(ho_ConnectedRegions4, &ho_SelectedRegions3, "max_area", 70);
			smallest_rectangle2(ho_SelectedRegions3, &hv_Row1, &hv_Column1, &hv_Phi1, &hv_Length11,
				&hv_Length21);
			gen_rectangle2(&ho_RegionData, hv_Row1, hv_Column1, hv_Phi1, hv_Length11, hv_Length21);
			//喷印日期缺陷
			intersection(ho_RegionDifference, ho_RegionData, &ho_RegionData);
		}
		//***去除药剂区域和日期去区域
		difference(ho_RegionDifference1, ho_RegionData, &ho_RegionLeft);
		//***版面异常1
		mean_image(ho_ImageChannel[5], &ho_ImageMean, 11, 11);
		gray_dilation_rect(ho_ImageMean, &ho_ImageMax4, 15, 15);
		sub_image(ho_ImageMax4, ho_ImageMean, &ho_ImageSub4, 1, 0);
		threshold(ho_ImageSub4, &ho_Region8, 10, 255);
		intersection(ho_Region8, ho_RegionLeft, &ho_RegionIntersection6);
		fill_up(ho_RegionIntersection6, &ho_RegionFillUp6);
		//opening_circle (Region8, RegionOpening9, 3.5)
		//*     intersection (RegionOpening9, RegionLeft, RegionIntersection5)
		opening_circle(ho_RegionFillUp6, &ho_RegionOpening11, 3.5);
		connection(ho_RegionOpening11, &ho_ConnectedRegions10);
		select_shape(ho_ConnectedRegions10, &ho_SelectedRegions7, (HTuple("contlength").Append("area")),
			"and", (HTuple(20).Append(_checkparam.i_BandException1_Area)), (HTuple(5000).Append(99999)));
		area_center(ho_SelectedRegions7, &hv_Area7, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		//**版面异常2
		threshold(ho_ImageChannel[1], &ho_Region9, 0, _checkparam.i_BandException2_Value);
		smallest_circle(ho_PillRegions, &hv_Row4, &hv_Column4, &hv_Radius2);
		gen_circle(&ho_Circle2, hv_Row4, hv_Column4, hv_Radius2+_checkparam.i_BandException2_Radius);
		//shape_trans (PillRegions, PillRegions, 'convex')
		difference(ho_PillDilation, ho_Circle2, &ho_RegionDifference3);
		intersection(ho_Region9, ho_RegionDifference3, &ho_RegionIntersection7);
		closing_circle(ho_RegionIntersection7, &ho_RegionClosing6, 3.5);

		//***版面异常3
		reduce_domain(ho_ImageChannel[2], ho_RegionLeft, &ho_ImageReduced2);
		gray_dilation_rect(ho_ImageReduced2, &ho_ImageMax1, 7, 7);
		sub_image(ho_ImageMax1, ho_ImageReduced2, &ho_ImageSub1, 1, 0);
		reduce_domain(ho_ImageSub1, ho_BandErosion, &ho_ImageReduced3);
		threshold(ho_ImageReduced3, &ho_Region5, 40, 255);
		fill_up(ho_Region5, &ho_RegionFillUp3);
		closing_circle(ho_RegionFillUp3, &ho_RegionClosing2, 3.5);
		opening_circle(ho_RegionClosing2, &ho_RegionOpening4, 2.5);
		connection(ho_RegionOpening4, &ho_ConnectedRegions7);
		shape_trans(ho_ConnectedRegions7, &ho_RegionTrans1, "convex");
		select_shape(ho_RegionTrans1, &ho_SelectedRegions5, "area", "and", _checkparam.i_BandException3_Area, 99999);
		union1(ho_SelectedRegions5, &ho_BandStrange);
		area_center(ho_BandStrange, &hv_Area3, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);
		//**片剂内部异常
		gen_empty_obj(&ho_PillInter);
		//dilation_circle (PillDilation, PillDilation1, 10)
		erosion_circle(ho_PillDilation, &ho_RegionErosion3, 10.5);
		shape_trans(ho_RegionErosion3, &ho_PillDilation1, "convex");
		union1(ho_PillDilation1, &ho_PillDilation2);
		//union1 (PillRegions, PillRegions)
		reduce_domain(ho_ImageChannel[1], ho_PillDilation2, &ho_PillImage);
		//texture_laws (PillImage, ImageTexture, 'le', 3, 5)

		gray_dilation_rect(ho_PillImage, &ho_ImageMax2, 5, 5);
		sub_image(ho_ImageMax2, ho_PillImage, &ho_ImageSub3, 1, 0);
		threshold(ho_ImageSub3, &ho_Region6, 25, 255);
		//fill_up (Region6, RegionFillUp5)
		opening_circle(ho_Region6, &ho_RegionOpening6, _checkparam.d_InterException_Open);
		closing_circle(ho_RegionOpening6, &ho_RegionClosing3, 3.5);
		connection(ho_RegionClosing3, &ho_ConnectedRegions9);
		select_shape(ho_ConnectedRegions9, &ho_SelectedRegions6, "area", "and", _checkparam.i_InterException_Area, 99999);
		area_center(ho_SelectedRegions6, &hv_Area9, &hv_ExpDefaultCtrlDummyVar, &hv_ExpDefaultCtrlDummyVar);

		switch (r)
		{
		case 0:
		{
			disp_obj(ho_ImageChannel[0], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "blue");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionBand, m_WND);
			set_color(m_WND, "red");
			set_tposition(m_WND, 10, 10);
			write_string(m_WND, "泡罩版面积：" + hv_Area);
			break;
		}
		case 1:
		{
			disp_obj(ho_ImageChannel[5], m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions7, m_WND);
			set_tposition(m_WND, 10,10);
			write_string(m_WND, "缺陷面积：" + hv_Area7);
			break;
		}
		case 2:
		{
			disp_obj(ho_ImageChannel[1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Region9, m_WND);
			//set_tposition(m_WND, 10, 10);
			//write_string(m_WND, "最大阈值：" + HTuple(_checkparam.i_BandException2_Value));
			break;
		}
		case 4:
		{
			disp_obj(ho_ImageChannel[2], m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_BandStrange, m_WND);
			set_tposition(m_WND, 10, 10);
			write_string(m_WND, "缺陷面积：" + hv_Area3);
			break;
		}
		case 5:
		{
			disp_obj(ho_ImageChannel[1], m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_SelectedRegions6, m_WND);
			set_tposition(m_WND, 10, 10);
			write_string(m_WND, "缺陷面积：" + hv_Area9);
			break;
		}
		case 6:
		{
			disp_obj(ho_ImageChannel[1], m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_RegionOpening6, m_WND);
			//set_tposition(m_WND, 10, 10);
			//write_string(m_WND, "开运算半径：" + HTuple(_checkparam.d_InterException_Open));
			break;
		}
		case 7:
		{
			disp_obj(ho_Image, m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_PillDilation, m_WND);
			break;
		}
		case 8:
		{
			disp_obj(ho_Image, m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			set_line_width(m_WND, 3);
			disp_obj(ho_Circle2, m_WND);
			break;
		}
		}
	}
	catch (HException &e)
	{
	}
	m_bChanged = true;
	ui.pB_Save->setEnabled(true);
}
void QtGuiSetting::SetModelMat(Mat tempgray)
{
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
		if (QMessageBox::Save == QMessageBox::question(this, QString::fromLocal8Bit("参数已修改"), QString::fromLocal8Bit("是否保存？"), QMessageBox::Save, QMessageBox::No))
		{
			emit ShouldSaveParam(&_checkparam);
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
	//&&m_timerReadList==nullptr 防止进入深一层目录
	QString sSelectItem = item->text();
	if (sSelectItem == "."&&m_timerReadList == nullptr)
	{
		QString newPath = m_sImageListPath.left(m_sImageListPath.indexOf("/")+1);
		m_sImageListPath = newPath;
		initImageLS(m_sImageListPath);
		return;
	}
	if (sSelectItem == ".."&&m_timerReadList == nullptr)
	{
		QString newPath = m_sImageListPath.left(m_sImageListPath.lastIndexOf("/"));
		//exe所在的根目录
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
			uint m_iResult;	//单次检测结果，最大不超过八个 
			int i_captotal = 0;
			QString str;
			bool results = ((CInterCHeck*)p_Parent)->Check(m_MOriginal, &_checkparam, str);
			bool b = ((CInterCHeck*)p_Parent)->RealCheck(str, &_checkparam, m_WND);
		}
	}
}
int QtGuiSetting::showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str)//全是中文
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
