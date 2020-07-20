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
	// Local iconic variables 
	Hobject ho_Image = Mat2Hobject(m_MOriginal);
	Hobject  ho_Image1, ho_Image2, ho_Image3;
	Hobject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
	Hobject  ho_Region, ho_RegionOpening, ho_Region_Cap, ho_ConnectedRegions;
	Hobject  ho_SelectedRegions, ho_Region_OCR, ho_Regionx, ho_Region_1st;
	Hobject  ho_Regionsx, ho_Regions_2nd, ho_ObjectsConcat, ho_RegionIntersection;
	Hobject  ho_RegionClosing, ho_ConnectedRegions1, ho_SelectedRegions1;
	Hobject  ho_Region_Inner, ho_RegionUnion, ho_RegionDifference;
	Hobject  ho_RegionUnion1, ho_Region_Out;


	// Local control variables 
	HTuple  hv_ImageFiles, hv_Index, hv_Width, hv_Height;
	HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2, hv_Height_Cap;
	HTuple  hv_Height_CapMin, hv_Height_CapMax, hv_Row1_Min;
	HTuple  hv_Row2_Max, hv_Row11, hv_Column11, hv_Row21, hv_Column21;
	HTuple  hv_Height_In, hv_Height_InMin, hv_Height_InMax;
	HTuple  hv_Row12, hv_Column12, hv_Row22, hv_Column22, hv_Height_Out;
	HTuple  hv_Height_OutMin, hv_Height_OutMax;
	get_image_size(ho_Image, &hv_Width, &hv_Height);
	decompose3(ho_Image, &ho_Image1, &ho_Image2, &ho_Image3);
	trans_from_rgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2,
		&ho_ImageResult3, "hsv");

	threshold(ho_ImageResult3, &ho_Region, 100, 255);
	opening_circle(ho_Region, &ho_RegionOpening, 3.5);
	union1(ho_RegionOpening, &ho_Region_Cap);


	connection(ho_Region_Cap, &ho_ConnectedRegions);
	select_shape(ho_ConnectedRegions, &ho_SelectedRegions, "height", "and", 100,
		99999);
	smallest_rectangle1(ho_SelectedRegions, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
	hv_Height_Cap = hv_Row2 - hv_Row1;
	tuple_min(hv_Height_Cap, &hv_Height_CapMin);
	tuple_max(hv_Height_Cap, &hv_Height_CapMax);
	hv_Height_CapMin = hv_Height_CapMin - 10;
	hv_Height_CapMax += 10;


	//字符部分
	threshold(ho_Image1, &ho_Region_OCR, 0, 140);
	//双色第一部分
	threshold(ho_ImageResult2, &ho_Regionx, 0, 70);
	opening_circle(ho_Regionx, &ho_Region_1st, 1.5);
	//双色第二部分
	threshold(ho_Image1, &ho_Regionsx, 55, 120);
	opening_circle(ho_Regionsx, &ho_Regions_2nd, 1.5);

	concat_obj(ho_Region_1st, ho_Regions_2nd, &ho_ObjectsConcat);
	intersection(ho_ObjectsConcat, ho_SelectedRegions, &ho_RegionIntersection);
	closing_circle(ho_RegionIntersection, &ho_RegionClosing, 3.5);
	connection(ho_RegionClosing, &ho_ConnectedRegions1);
	select_shape(ho_ConnectedRegions1, &ho_SelectedRegions1, "height", "and", (hv_Height_CapMin / 2) - 20,
		(hv_Height_CapMax / 2) + 20);


	tuple_min(hv_Row1, &hv_Row1_Min);
	tuple_max(hv_Row2, &hv_Row2_Max);
	select_shape(ho_SelectedRegions1, &ho_Region_Inner, "row", "and", hv_Row1_Min + (hv_Height_CapMin / 2),
		hv_Row2_Max - (hv_Height_CapMin / 2));
	fill_up(ho_Region_Inner, &ho_Region_Inner);
	smallest_rectangle1(ho_Region_Inner, &hv_Row11, &hv_Column11, &hv_Row21, &hv_Column21);
	hv_Height_In = hv_Row21 - hv_Row11;
	tuple_min(hv_Height_In, &hv_Height_InMin);
	tuple_max(hv_Height_In, &hv_Height_InMax);

	_checkparam.i_Low_LimitIn = hv_Height_InMin[0].I() - 5;
	_checkparam.i_High_LimitIn = hv_Height_InMax[0].I() + 5;
	union1(ho_Region_Inner, &ho_RegionUnion);
	difference(ho_SelectedRegions1, ho_RegionUnion, &ho_RegionDifference);
	union1(ho_RegionDifference, &ho_RegionUnion1);
	connection(ho_RegionUnion1, &ho_Region_Out);
	fill_up(ho_Region_Out, &ho_Region_Out);
	smallest_rectangle1(ho_Region_Out, &hv_Row12, &hv_Column12, &hv_Row22, &hv_Column22);
	hv_Height_Out = hv_Row22 - hv_Row12;
	tuple_min(hv_Height_Out, &hv_Height_OutMin);
	tuple_max(hv_Height_Out, &hv_Height_OutMax);

	_checkparam.i_Low_LimitOut = hv_Height_OutMin[0].I() - 5;
	_checkparam.i_High_LimitOut = hv_Height_OutMax[0].I() + 5;

	set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
	disp_obj(ho_Image, m_WND);
	set_tposition(m_WND, 100, 100);
	set_color(m_WND, "white");
	set_draw(m_WND, "margin");
	set_line_width(m_WND, 5);
	set_color(m_WND, "cyan");
	disp_obj(ho_Region_Out, m_WND);
	set_tposition(m_WND, 50, 10);
	write_string(m_WND, HTuple("外部区域高度上下限") + " " + hv_Height_OutMin + " " + hv_Height_OutMax);
	set_color(m_WND, "blue");
	disp_obj(ho_Region_Inner, m_WND);
	set_tposition(m_WND, 70, 10);
	write_string(m_WND, HTuple("内部区域高度上下限") + " " + hv_Height_InMin + " " + hv_Height_InMax);
	SetParam(_checkparam);
	m_bChanged = true;
	ui.pB_Save->setEnabled(false);
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

		item->setText(QString::fromLocal8Bit("通道阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_BandThread));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("R通道阈值"));
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
		item->setText(QString::fromLocal8Bit("外部阈值低"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_Low_LimitOut));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("外部阈值高"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_High_LimitOut));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("内部阈值低"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_Low_LimitIn));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("内部阈值高"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_High_LimitIn));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		rowindex = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(rowindex);
		item = new QTableWidgetItem();
		item->setText(QString::fromLocal8Bit("漏粉阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_Innerthread));
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
		hsd->setValue(_checkparam.i_Innerthread);
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
		//////////////////////////////////////////////////////////////////////////
	}
	ui.tableWidget->blockSignals(false);
}

void QtGuiSetting::onCellChanged(int r, int c)
{
	try
	{
		QString texts = ui.tableWidget->item(r, c)->text();
		Mat MatToShow(m_MOriginal);

		// Local iconic variables 
		Hobject  ho_Image1, ho_Image2, ho_Image3;
		Hobject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
		Hobject  ho_Region, ho_RegionOpening, ho_Region_Cap, ho_Region_OCR;
		Hobject  ho_Regionx, ho_Region_1st, ho_Regionsx, ho_Regions_2nd;
		Hobject  ho_EmptyObject_Area, ho_EmptyObjectWidth, ho_EmptyObjectHeight;
		Hobject  ho_EmptyObject_Inner, ho_EmptyObject_Leaking, ho_EmptyObject_Other;
		Hobject  ho_Rectangle, ho_RegionIntersection, ho_ConnectedRegions;
		Hobject  ho_SelectedRegions, ho_RegionIntersection1, ho_ConnectedRegions1;
		Hobject  ho_SelectedRegions1, ho_RegionTrans_Out_1st, ho_Rectangle2;
		Hobject  ho_RegionIntersection2, ho_ConnectedRegions2, ho_SelectedRegions2;
		Hobject  ho_RegionTrans_Out_2nd, ho_Rectangle3, ho_RegionTrans;
		Hobject  ho_RegionFillUp, ho_Region_CapOut, ho_Rectangle1;
		Hobject  ho_RegionIntersection3, ho_RegionClosing3, ho_RegionOpening2;
		Hobject  ho_ImageReduced, ho_Regions, ho_RegionDifference;
		Hobject  ho_ImageMin, ho_ImageSub, ho_RegionClosing, ho_RegionOpening1, ho_ConnectedRegions3;
		Hobject  ho_SelectedRegions3, ho_RegionUnion1;


		// Local control variables 
		HTuple  hv_ImageFiles, hv_Index, hv_Width, hv_Height;
		HTuple  hv_b_Width, hv_b_Height, hv_b_Inner, hv_b_Leaking;
		HTuple  hv_b_Other, hv_Low_LimitOut, hv_High_LimitOut, hv_Low_LimitIn;
		HTuple  hv_High_LimitIn, hv_Co_Index, hv_b_Skip, hv_Ro_Index;
		HTuple  hv_R_s, hv_C_s, hv_Row13, hv_Column13, hv_Row23, hv_Area1;
		HTuple  hv_Column23, hv_tmp_H, hv_Center_R, hv_Row11, hv_Column11;
		HTuple  hv_Row21, hv_Column21, hv_Cent_R, hv_High_Limit;
		HTuple  hv_Low_Limit, hv_Row12, hv_Column12, hv_Row22, hv_Column22;
		HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2, hv_Area;
		HTuple  hv_ExpDefaultCtrlDummyVar, hv_BandThread, hv_InnerThread;
		if (m_MOriginal.empty())
		{
			return;
		}
		Hobject ho_Image = Mat2Hobject(m_MOriginal);
		get_image_size(ho_Image, &hv_Width, &hv_Height);
		set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		switch (r)
		{
		case 0:
		{
			_checkparam.i_BandThread = texts.toInt();
			decompose3(ho_Image, &ho_Image1, &ho_Image2, &ho_Image3);
			trans_from_rgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2, &ho_ImageResult3, "hsv");
			threshold(ho_ImageResult3, &ho_Region, _checkparam.i_BandThread, 255);
			disp_obj(ho_Image, m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			disp_obj(ho_Region, m_WND);

			break;
		}
		case 2:
		{
			_checkparam.i_Low_LimitOut = texts.toInt();
			break;
		}
		case 3:
		{
			_checkparam.i_High_LimitOut = texts.toInt();
			break;
		}
		case 4:
		{
			_checkparam.i_Low_LimitIn = texts.toInt();
			break;
		}
		case 5:
		{
			_checkparam.i_High_LimitIn = texts.toInt();
			break;
		}
		case 6:
			_checkparam.i_Innerthread = texts.toInt();
			get_image_size(ho_Image, &hv_Width, &hv_Height);
			decompose3(ho_Image, &ho_Image1, &ho_Image2, &ho_Image3);
			trans_from_rgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2,
				&ho_ImageResult3, "hsv");

			threshold(ho_ImageResult3, &ho_Region, _checkparam.i_BandThread, 255);

			union1(ho_Region, &ho_Region_Cap);
			fill_up(ho_Region_Cap, &ho_Region_Cap);
			opening_circle(ho_Region_Cap, &ho_Region_Cap, 30.5);
			erosion_circle(ho_Region_Cap, &ho_Region_Cap, 3.5);

			union1(ho_Region_Cap, &ho_Region_CapOut);
			reduce_domain(ho_Image3, ho_Region_CapOut, &ho_ImageReduced);
			gray_dilation_rect(ho_ImageReduced, &ho_ImageMin, 5, 5);
			sub_image(ho_ImageMin, ho_ImageReduced, &ho_ImageSub, 1, 0);
			threshold(ho_ImageSub, &ho_Regions, _checkparam.i_Innerthread, 255);
			closing_circle(ho_Regions, &ho_RegionClosing, 1.5);
			opening_circle(ho_RegionClosing, &ho_RegionOpening1, 1.5);
			connection(ho_RegionOpening1, &ho_ConnectedRegions3);
			union1(ho_ConnectedRegions3, &ho_RegionUnion1);
			disp_obj(ho_Image, m_WND);
			set_draw(m_WND, "margin");
			set_color(m_WND, "cyan");
			disp_obj(ho_Region_CapOut, m_WND);
			set_draw(m_WND, "fill");
			set_color(m_WND, "red");
			disp_obj(ho_RegionUnion1, m_WND);
			break;
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
