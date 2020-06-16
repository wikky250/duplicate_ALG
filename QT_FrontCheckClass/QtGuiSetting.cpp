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
	ui.setupUi(this);
	ui.tableWidget->setColumnCount(2);
	QStringList LIST;
	LIST << QString::fromLocal8Bit("项目") << QString::fromLocal8Bit("参数");
	ui.tableWidget->setHorizontalHeaderLabels(LIST);
	ui.tableWidget->setColumnWidth(1, 40);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->viewport()->installEventFilter(this);
	pp = parent;
	ui.ShowLabel->setMouseTracking(true);
	ui.tableWidget->setMouseTracking(true);
	setMouseTracking(true);
	m_bButton = false;
	m_SAppPath = qApp->applicationDirPath();//exe文件夹
	m_sImageListPath = m_SAppPath;
	initImageLS(m_sImageListPath);
	connect(ui.lw_ImageList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onSelectImageList(QListWidgetItem *)));
	connect(ui.pB_ContinueTest, SIGNAL(toggled(bool)), this, SLOT(onContinueTest(bool)));
	connect(ui.pB_Save, SIGNAL(clicked()), this, SLOT(onSaveParam()));
	connect(ui.pB_Exit, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "退出确认", "确认退出算法设置页面吗？", "确认", "取消"))
		{
			close();
		}});
	//设置check parent 算法显示位置
	connect(this, SIGNAL(SignShowImage(int, Mat, int)), this, SLOT(SLOTShowImage(int, Mat, int)));
	p_Parent = AlgPointer;
	((CInterCHeck*)p_Parent)->StartCheck("", nullptr);
	((CInterCHeck*)p_Parent)->SetShowCallBack(this, ShowCallBack);
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
		Mat tempgray;
		if (m_MOriginal.rows == 0 || m_MOriginal.cols == 0)
		{
			return;
		}
		cvtColor(m_MOriginal, tempgray, COLOR_BGR2RGB);
		onShowImage(&tempgray);
	}
	catch (cv::Exception* e)
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
	if (img.channels()==1)
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
	if (ui.lw_ImageList->count()<3)
	{
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("连续测试错误"), QString::fromLocal8Bit("未定位到图像文件"));
		return;
	}
	int index_list = ui.lw_ImageList->currentRow();
	if (index_list == ui.lw_ImageList->count()-1)
	{
		m_timerReadList->stop();
		delete m_timerReadList;
		m_timerReadList = nullptr;
		ui.pB_ContinueTest->setChecked(false);
		return;
	}
	if (index_list <2)
	{
		ui.lw_ImageList->setCurrentRow(2);
		ui.lw_ImageList->emit itemDoubleClicked(ui.lw_ImageList->currentItem());
	}
	else
	{
		ui.lw_ImageList->setCurrentRow(index_list+1);
		ui.lw_ImageList->emit itemDoubleClicked(ui.lw_ImageList->currentItem());
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

void QtGuiSetting::SetParam(CHECKPARAM param)
{
	_checkparam = param;
	ui.tableWidget->setSortingEnabled(false);
	int rowindex = ui.tableWidget->rowCount();
	if (0 == rowindex)
	{
		ui.tableWidget->insertRow(rowindex);
		QTableWidgetItem* item = new QTableWidgetItem();

		item->setText(QString::fromLocal8Bit("R通道阈值"));
		item->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowindex, 0, item);
		item = new QTableWidgetItem();
		item->setText(QString::number(_checkparam.i_tempThread));
		item->setTextAlignment(Qt::AlignRight);
		ui.tableWidget->setItem(rowindex, 1, item);

		//////////////////////////////////////////////////////////////////////////
	}
	QObject::connect(ui.tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(onCellChanged(int, int)));
}

void QtGuiSetting::onCellChanged(int r, int c)
{
	QString texts = ui.tableWidget->item(r, c)->text();
	Mat MatToShow(m_MOriginal);
	switch (r)
	{
	case 0:
	{
		_checkparam.i_tempThread = texts.toInt();
		if (m_MOriginal.rows > 0)
		{
			Mat img(m_MOriginal);
			Mat imgRGB[3];
			cv::split(img, imgRGB);
			Mat imgR;
			cv::threshold(imgRGB[2], imgR, _checkparam.i_tempThread, 255, THRESH_BINARY);
			Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));

			cv::morphologyEx(imgR, imgR, MORPH_OPEN, element);
			std::vector<std::vector<cv::Point>> contours;
			cv::findContours(imgR, contours, RETR_LIST, CHAIN_APPROX_NONE);
			for (int i = 0; i < contours.size(); i++)
			{
				cv::drawContours(MatToShow, contours, i, Scalar(255, 0, 0), 3);
			}
		}
		break;

	}
	}
	if (MatToShow.rows > 0)
	{
		int zz = ui.ShowLabel->frameWidth();
		QSize ss = ui.ShowLabel->size();
		ss.setWidth(ss.width() - zz * 2);
		ss.setHeight(ss.height() - zz * 2);
		Mat imgsend;
		cv::cvtColor(MatToShow, imgsend, COLOR_BGR2RGB);
		cv::resize(imgsend, imgsend, Size(ss.width(), ss.height()));
		QImage disImage = QImage((const unsigned char*)(imgsend.data), imgsend.cols, imgsend.rows, imgsend.step, QImage::Format_RGB888);
		ui.ShowLabel->setPixmap(QPixmap::fromImage(disImage));
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
		if (m_MOriginal.rows>0)
		{
			uint m_iResult;	//单次检测结果，最大不超过八个
			int i_captotal = 0;
			QString str;
			bool results = ((CInterCHeck*)p_Parent)->Check(m_MOriginal, &_checkparam, str);
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
