#include "QtGuiSetting.h"
#include "InterCHeck.h"
#include <QtWidgets/QComboBox>
#include <QDesktopWidget>
#include <math.h>
#include<QMessageBox>
#include<QCheckBox>
#include<QMyTreeWidget.h>;
typedef void* UI_MONITOR;
QString AppPath;
void ShowCallBack(UI_MONITOR ui, int pos, Mat img, int times)
{
	((QtGuiSetting*)ui)->emit SignShowImage(pos, img, times);
}
template <typename T>
T getValue(YAML::Node _param, QString c_CameraName, QString errortype, QString errorparam, QString defaultvalue)
{
	T val;
	try
	{
		val = _param[c_CameraName.toStdString()][errortype.toStdString()][errorparam.toStdString()]["value"].as<T>();
	}
	catch (YAML::Exception e)
	{
		return defaultvalue.toDouble();
	}
	return val;
}
bool NodeToParam(CHECKPARAM& checkparam, YAML::Node& _param)
{
	checkparam.i_DiffArea = getValue<int>(_param, checkparam.c_CameraName, QString::fromLocal8Bit("Param_药板面积"), QString::fromLocal8Bit("差异面积"), "1500");
	checkparam.i_SubArea = getValue<int>(_param, checkparam.c_CameraName, QString::fromLocal8Bit("Param_药板面积"), QString::fromLocal8Bit("缺损面积"), "10000");
	checkparam.i_Channel_Pill = getValue<int>(_param, checkparam.c_CameraName, QString::fromLocal8Bit("Param_泡罩轮廓"), QString::fromLocal8Bit("泡罩颜色通道"), "0");
	checkparam.i_Gray_Pill = getValue<int>(_param, checkparam.c_CameraName, QString::fromLocal8Bit("Param_泡罩轮廓"), QString::fromLocal8Bit("泡罩灰度阈值"), "150");
	checkparam.i_Area_Pill = getValue<int>(_param, checkparam.c_CameraName, QString::fromLocal8Bit("Param_泡罩轮廓"), QString::fromLocal8Bit("泡罩面积阈值"), "15000");
	return true;
}
bool LoadCheckParam(CHECKPARAM* checkparam, QString* str)
{
	YAML::Node _param;
	if (nullptr == str)
	{
		_param = YAML::LoadFile(QString(AppPath + "/DefaultModel/CheckParam.yaml").toStdString());
		QSettings configIniRead(AppPath + "\\DefaultModel\\CheckParam.ini", QSettings::IniFormat);
		configIniRead.setIniCodec("UTF-8");
		QString cameraname = checkparam->c_CameraName;
	}
	else
	{
		_param = YAML::LoadFile(str->toStdString().c_str());
	}

	if (!_param[checkparam->c_CameraName].IsDefined())
	{
		return false;
	}
	NodeToParam(*checkparam, _param);


	return false;
}
QtGuiSetting::QtGuiSetting(QDialog* parent, void* AlgPointer)
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
	setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
	setWindowState(Qt::WindowMaximized);
	// 	ui.tableWidget->setColumnCount(2);
	// 	QStringList LIST;
	// 	LIST << QString::fromLocal8Bit("项目") << QString::fromLocal8Bit("参数");
	// 	ui.tableWidget->setHorizontalHeaderLabels(LIST);
	// 	ui.tableWidget->setColumnWidth(1, 40);
	// 	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// 	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	// 	ui.tableWidget->viewport()->installEventFilter(this);
	// 	ui.tableWidget->verticalHeader()->setDefaultSectionSize(35);//默认行高20
	pp = parent;
	//HTuple hv_WindowID;
	//open_window(ui.ShowLabel->y(), ui.ShowLabel->x(), ui.ShowLabel->width(), ui.ShowLabel->height(), (long)((QWidget*)ui.ShowLabel->parent())->winId(), "", "", &hv_WindowID);
	//m_WND = hv_WindowID[0];
	ui.ShowLabel->setMouseTracking(true);
	//	ui.tableWidget->setMouseTracking(true);
	ui.lw_ImageList->viewport()->installEventFilter(this);
	bool b = ui.lw_ImageList->hasMouseTracking();
	setMouseTracking(true);
	m_bButton = false;
	m_SAppPath = qApp->applicationDirPath();//exe文件夹
	m_sImageListPath = m_SAppPath;
	initImageLS(m_sImageListPath);
	connect(ui.lw_ImageList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onClickedImage(QListWidgetItem*)));
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
	((CInterCHeck*)p_Parent)->StartCheck("", nullptr, m_MOriginal.cols, m_MOriginal.rows, m_MOriginal.channels());
	//QDesktopWidget* dwsktopwidget = QApplication::desktop();
	//QRect deskrect = dwsktopwidget->availableGeometry();
	//QRect screenrect = dwsktopwidget->screenGeometry();
	//if (deskrect.height() < 1100)
	//{
	//	this->resize(768 * 2 + 5, 1366 / 2);
	//	//ui.groupBox->move(ui.ShowLabel->pos().x() + ui.ShowLabel->width() + 2, ui.ShowLabel->pos().y());
	//	//ui.lw_ImageList->move(ui.tableWidget->pos().x() + ui.tableWidget->width() + 2, ui.tableWidget->pos().y());
	//}
	//	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onPopKeyboard(int, int)));
	connect(ui.treeWidget, &QMyTreeWidget::TempSave, [=](QString str)
	{
		CHECKPARAM _checkparam;
		strcpy(_checkparam.c_CameraName, _CameraName);
		NodeToParam(_checkparam, ui.treeWidget->_mparam);
		ResultStruct st;
		Mat imgtemp = m_MOriginal.clone();
		bool results = ((CInterCHeck*)p_Parent)->Check(imgtemp, &_checkparam, st);
		ShowImagetoLabel(imgtemp);
		m_bChanged = true;
		ui.pB_Save->setEnabled(true);
	});
}
void QtGuiSetting::mouseReleaseEvent(QMouseEvent* p)
{
	setCursor(Qt::ArrowCursor);
	m_bButton = false;
	hideKeyBoard();

	return;
}
bool QtGuiSetting::eventFilter(QObject* watched, QEvent* event)
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
void QtGuiSetting::resizeEvent(QResizeEvent* event)
{
	try
	{
		// 		Mat tempgray;
		// 		if (m_MOriginal.rows == 0 || m_MOriginal.cols == 0)
		// 		{
		// 			return;
		// 		}
		// 		cvtColor(m_MOriginal, tempgray, COLOR_BGR2RGB);
		// 		ShowImagetoLabel(&tempgray);
		if (m_MOriginal.empty())
		{
			return;
		}
		//Hobject ho_Image = Mat2Hobject(m_MOriginal);
		//HTuple hv_Width, hv_Height;
		//get_image_size(ho_Image, &hv_Width, &hv_Height);
		//set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		//disp_obj(ho_Image, m_WND);
	}
	catch (...)
	{
		return;
	}
	return;
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
void QtGuiSetting::mousePressEvent(QMouseEvent* p)
{
	if (ui.ShowLabel->geometry().contains(this->mapFromGlobal(QCursor::pos())) && p->button() == Qt::LeftButton)
	{
		hideKeyBoard();
		//HTuple hv_Width, hv_Height;
		//if (m_MOriginal.empty())
		//{
		//	return;
		//}
		//Hobject ho_Image = Mat2Hobject(m_MOriginal);
		//get_image_size(ho_Image, &hv_Width, &hv_Height);
		//set_part(m_WND, 0, 0, hv_Height - 1, hv_Width - 1);
		//disp_obj(ho_Image, m_WND);
		return;
	}
	return;
}
void QtGuiSetting::mouseMoveEvent(QMouseEvent* p)
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
	// 			ShowImagetoLabel(&img2show);
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
	if (img.channels() == 1) {
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
			QFile::remove(QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml"));

			ui.treeWidget->SaveYAMLFile(AppPath + "/Temp.yaml");
			QFile::copy(QString(qApp->applicationDirPath() + "/Temp.yaml"), QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml"));
			emit ShouldSaveParam();
			m_bChanged = false;
			ui.pB_Save->setEnabled(false);
		}
	}
}
void QtGuiSetting::onAutoDetest()
{
}

void QtGuiSetting::onSelectImageChannel(int channels)
{
	QComboBox* obj = qobject_cast<QComboBox*>(sender());//判断是哪个按钮触发了槽函数
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
		//		disp_obj(ho_Image1, m_WND);
		break;
	case 1:
		//		disp_obj(ho_Image2, m_WND);
		break;
	case 2:
		//disp_obj(ho_Image3, m_WND);
		break;
	case 3:
		//disp_obj(ho_ImageResult1, m_WND);
		break;
	case 4:
		//disp_obj(ho_ImageResult2, m_WND);
		break;
	case 5:
		//disp_obj(ho_ImageResult3, m_WND);
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
	if (nullptr == m_timerChanger)
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

			if (nullptr != m_timerChanger)
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
void QtGuiSetting::SetParam(char* c_CameraName)
{
	m_node = YAML::LoadFile(QString(qApp->applicationDirPath() + "/DefaultModel/CheckParam.yaml").toStdString());
	strcpy(_CameraName, c_CameraName);
	ui.treeWidget->clear();
	ui.treeWidget->ReadYAMLFile(m_node, c_CameraName);
}
void QtGuiSetting::onCellChanged(int r, int c)
{
}
void QtGuiSetting::SetModelMat(Mat tempgray)
{
	if (tempgray.empty())
	{
		m_MOriginal = imread((qApp->applicationDirPath() + "/DefaultModel/StandTemp.bmp").toStdString());
	}
	else
	{
		tempgray.copyTo(m_MOriginal);
	}
	ShowImagetoLabel(m_MOriginal);
}
void QtGuiSetting::closeEvent(QCloseEvent*)
{
	//close_window(HTuple(m_WND));
	onSaveParam();
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
void QtGuiSetting::ShowImagetoLabel(Mat tempgray)
{
	try
	{
		Mat temp;
		cv::cvtColor(tempgray, temp, cv::COLOR_BGR2RGB);
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
extern bool compColx(const Rect& a, const Rect& b);
extern bool compColy(const Rect& a, const Rect& b);
void QtGuiSetting::onSelectTrainFile()
{
}
void QtGuiSetting::onClickedImage(QListWidgetItem* item)
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
	QListWidgetItem* item = ui.lw_ImageList->currentItem();
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
			//HTuple hv_W, hv_H;
			//Hobject ho_Image = Mat2Hobject(m_MOriginal);
			//get_image_size(ho_Image, &hv_W, &hv_H);
			//set_part(m_WND, 0, 0, hv_H - 1, hv_W - 1);
			//disp_obj(ho_Image, m_WND);
		}
	}
}
void QtGuiSetting::onSelectImageList(QListWidgetItem* item)
{
	//&&m_timerReadList==nullptr 防止进入深一层目录
	QString sSelectItem = item->text();
	if (sSelectItem == "." && m_timerReadList == nullptr)
	{
		QString newPath = m_sImageListPath.left(m_sImageListPath.indexOf("/") + 1);
		m_sImageListPath = newPath;
		initImageLS(m_sImageListPath);
		return;
	}
	if (sSelectItem == ".." && m_timerReadList == nullptr)
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
			ResultStruct str;
			CHECKPARAM _checkparam;

			strcpy(_checkparam.c_CameraName, _CameraName);
			NodeToParam(_checkparam, ui.treeWidget->_mparam);
			cv::Mat img = m_MOriginal.clone();
			bool results = ((CInterCHeck*)p_Parent)->Check(img, &_checkparam, str);
			ShowImagetoLabel(img);
		}
	}
}
int QtGuiSetting::showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str)//全是中文
{
	if (QString::fromLocal8Bit(button2Str) == "")
	{
		QMessageBox msg(icon, QString::fromLocal8Bit(titleStr), QString::fromLocal8Bit(contentStr), QMessageBox::Ok);
		msg.setButtonText(QMessageBox::Ok, QString::fromLocal8Bit(button1Str));
		msg.setWindowIcon(QIcon("./Logo_Icon.ico"));
		return msg.exec();
	}
	else
	{
		QMessageBox msg(icon, QString::fromLocal8Bit(titleStr), QString::fromLocal8Bit(contentStr), QMessageBox::Yes | QMessageBox::No);
		msg.setButtonText(QMessageBox::Yes, QString::fromLocal8Bit(button1Str));
		msg.setButtonText(QMessageBox::No, QString::fromLocal8Bit(button2Str));
		msg.setWindowIcon(QIcon("./Logo_Icon.ico"));
		return msg.exec();
	}
}