#include "QtGuiSetting.h"
#include "InterCHeck.h"

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
	((CInterCHeck*)p_Parent)->StartCheck("", nullptr);
	((CInterCHeck*)p_Parent)->SetShowCallBack(this, ShowCallBack);


	QObject::connect(ui.tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(onCellChanged(int, int)));

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

void QtGuiSetting::SetParam(CHECKPARAM param)
{
	_checkparam = param;
	ui.tableWidget->blockSignals(true);
	ui.tableWidget->setSortingEnabled(false);
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);
	int rowindex = ui.tableWidget->rowCount();
	if (0 == rowindex)
	{
		ui.tableWidget->insertRow(rowindex);
		QTableWidgetItem* item = new QTableWidgetItem();

		item->setText(QString::fromLocal8Bit("边缘胶囊阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_BandThread));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("边缘胶囊阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		QSlider* hsd = new QSlider(Qt::Horizontal);
		hsd->setMinimum(0);
		hsd->setMaximum(255);
		hsd->setValue(_checkparam.i_BandThread);
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
		ui.tableWidget->setCellWidget(rowindex, 1, hsd);
		connect(hsd, &QSlider::valueChanged, [=]() {
			ui.tableWidget->item(rowindex - 1, 1)->setText(QString::number(hsd->value()));
		});//利用lambda表达式可用


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("中心胶囊阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_MiddleThread));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);


		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("漏粉阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		hsd = new QSlider(Qt::Horizontal);
		hsd->setMinimum(0);
		hsd->setMaximum(255);
		hsd->setValue(_checkparam.i_MiddleThread);
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
		ui.tableWidget->setCellWidget(rowindex, 1, hsd);
		connect(hsd, &QSlider::valueChanged, [=]() {
			ui.tableWidget->item(rowindex - 1, 1)->setText(QString::number(hsd->value()));
		});//利用lambda表达式可用

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("上部轮廓下限"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_UPBoundary));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);
		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("下部轮廓上限"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_UPBoundary));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);
		
		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("漏粉判定区域"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_LeakingRadios));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("漏粉判定阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_LeakingThread));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);
		//////////////////////////////////////////////////////////////////////////
	}

	ui.tableWidget->blockSignals(false);
}

void QtGuiSetting::onCellChanged(int r, int c)
{
	try
	{
		c = 1;//防止键盘table
		QString texts = ui.tableWidget->item(r, c)->text();
		Mat MatToShow(m_MOriginal);

		// Local iconic variables 
		Hobject  ho_EmptyRegion_Out, ho_EmptyRegion_Top;
		Hobject  ho_EmptyRegion_Inner, ho_EmptyRegion_TopUP, ho_EmptyRegion_Intensity;
		Hobject  ho_Image1, ho_Image2, ho_Image3, ho_ImageResult;
		Hobject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
		Hobject  ho_ImageMax2, ho_ImageSub2, ho_Regions, ho_Region;
		Hobject  ho_RegionOpening2, ho_Regionx, ho_Region_1st, ho_Regionsx;
		Hobject  ho_Regions_2nd, ho_Rectangle, ho_RegionIntersection6;
		Hobject  ho_RegionFillUp, ho_RegionOpening7, ho_ConnectedRegions3;
		Hobject  ho_SelectedRegions4, ho_RegionIntersection11, ho_ConnectedRegions5;
		Hobject  ho_SelectedRegions6, ho_RegionIntersection12, ho_ConnectedRegions6;
		Hobject  ho_SelectedRegions7, ho_RegionErosion7, ho_RegionIntersection10;
		Hobject  ho_RegionOpening11, ho_RegionIntersection5, ho_ConnectedRegions;
		Hobject  ho_SelectedRegions, ho_RegionOpening8, ho_RegionTrans2;
		Hobject  ho_RegionIntersection, ho_RegionErosion, ho_ImageReduced;
		Hobject  ho_ImageMax, ho_ImageSub, ho_RegionErosion3, ho_RegionIntersection3;
		Hobject  ho_RegionOpening5, ho_Rectangle2, ho_RegionIntersection7;
		Hobject  ho_ConnectedRegions4, ho_SelectedRegions5, ho_RegionOpening9;
		Hobject  ho_RegionTrans3, ho_Rectangle1, ho_RegionIntersection8;
		Hobject  ho_RegionErosion5, ho_ImageReduced2, ho_ImageMax3;
		Hobject  ho_ImageSub3, ho_RegionErosion6, ho_RegionIntersection9;
		Hobject  ho_RegionOpening10;


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
		switch (r)
		{
		case 0:
		{
			_checkparam.i_BandThread = texts.toInt();
			threshold(ho_ImageResult3, &ho_RegionOpening2, _checkparam.i_BandThread, 255);
			opening_circle(ho_RegionOpening2, &ho_RegionOpening2, 2.5);
			fill_up(ho_RegionOpening2, &ho_RegionOpening2);
			opening_circle(ho_RegionOpening2, &ho_RegionOpening2, 10.5);

			disp_obj(ho_Image, m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			disp_obj(ho_RegionOpening2, m_WND);

			break;
		}
		case 2:
		{
			_checkparam.i_MiddleThread = texts.toInt();
			threshold(ho_ImageResult3, &ho_Region, _checkparam.i_MiddleThread, 255);
			opening_circle(ho_Region, &ho_Region, 3.5);
			disp_obj(ho_Image, m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			disp_obj(ho_Region, m_WND);
			break;
		}
		case 4:
		case 5:
		{

			_checkparam.i_UPBoundary = ui.tableWidget->item(4, c)->text().toInt();
			_checkparam.i_DOWNBoundary = ui.tableWidget->item(5, c)->text().toInt();
			get_image_pointer1(ho_Image, _, _, &hv_Width, &hv_Height);
			hv_R_s = 0;
			HTuple hv_R_sEnd  = _checkparam.i_UPBoundary;
			hv_C_s = 0;
			gen_rectangle1(&ho_Rectangle, hv_R_s, hv_C_s, hv_R_sEnd, hv_Width);
			hv_R_s = hv_Height - _checkparam.i_DOWNBoundary;
			hv_R_sEnd = hv_Height;
			gen_rectangle1(&ho_Rectangle2, hv_R_s, hv_C_s, hv_R_sEnd, hv_Width);
			disp_obj(ho_Image, m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			disp_obj(ho_Rectangle, m_WND);
			disp_obj(ho_Rectangle2, m_WND);
		}
		case 6:
		case 7:
		{
			_checkparam.i_LeakingRadios = ui.tableWidget->item(6, c)->text().toInt();
			_checkparam.i_LeakingThread = ui.tableWidget->item(7, c)->text().toInt();
			gray_dilation_rect(ho_Image3, &ho_ImageMax2, _checkparam.i_LeakingRadios, _checkparam.i_LeakingRadios);
			sub_image(ho_ImageMax2, ho_Image3, &ho_ImageSub2, 1, 0);
			threshold(ho_ImageSub2, &ho_Regions, _checkparam.i_LeakingThread, 255);
			closing_circle(ho_Regions, &ho_Regions, 1.5);
			disp_obj(ho_Image, m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "red");
			disp_obj(ho_Regions, m_WND);
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
		QString newPath = m_sImageListPath.left(m_sImageListPath.indexOf("/") + 1);
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
		m_MOriginal = imread(m_currentImagePath.toStdString().c_str());
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
