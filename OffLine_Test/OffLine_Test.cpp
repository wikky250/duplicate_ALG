#include "OffLine_Test.h"

OffLine_Test::OffLine_Test(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);


	AppPath = qApp->applicationDirPath();//exe所在目录
	m_sImageListPath = AppPath;
	initImageLS(m_sImageListPath);
	InitAlgo();

	QStringList title;
	title << QString::fromLocal8Bit("ErrorType") << QString::fromLocal8Bit("类型") << QString::fromLocal8Bit("数量");
	ui.tableWidget_Result->setColumnCount(3);//3列
	ui.tableWidget_Result->setHorizontalHeaderLabels(title);//加表头
	ui.tableWidget_Result->setColumnHidden(0, true);//隐藏ErrorType列
	ui.tableWidget_Result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分填充表头
	ui.tableWidget_Result->verticalHeader()->setDefaultSectionSize(35);//默认行高20
	ui.tableWidget_Result->verticalHeader()->setVisible(false);//不显示行头
	int z = 0;
	ui.tableWidget_Result->insertRow(z);//加一行
	ui.tableWidget_Result->setItem(z, 0, new QTableWidgetItem(QString::fromLocal8Bit("Total")));//第0列，已隐藏
	ui.tableWidget_Result->setItem(z, 1, new QTableWidgetItem(QString::fromLocal8Bit("总数")));//第1列
	ui.tableWidget_Result->setItem(z, 2, new QTableWidgetItem(QString::number(0)));//第2列
	z++;
	ui.tableWidget_Result->insertRow(z);//再加一行
	ui.tableWidget_Result->setItem(z, 0, new QTableWidgetItem(QString::fromLocal8Bit("Good")));//第0列，已隐藏
	ui.tableWidget_Result->setItem(z, 1, new QTableWidgetItem(QString::fromLocal8Bit("合格数")));//第1列
	ui.tableWidget_Result->setItem(z, 2, new QTableWidgetItem(QString::number(0)));//第2列

	bool b = connect(ui.lw_ImageList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onDoubleImageList(QListWidgetItem*)));
	b = connect(ui.lw_ImageList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(onSelectImageList(QListWidgetItem*, QListWidgetItem*)));
	b = connect(ui.Button_Start, SIGNAL(toggled(bool)), this, SLOT(ontoggledStartBtn(bool)));
	ti = new QTimer();
	ti->setSingleShot(true);
	b = connect(ti, SIGNAL(timeout()), this, SLOT(ImgListAutoDown()));
	b = connect(this, SIGNAL(CHECKRESULT(QStringList)), this, SLOT(SlotShowResult(QStringList)));
	b = connect(ui.checkBox_OK, &QCheckBox::toggled, [=](bool b)
		{
			m_bOKSTOP = b;
		});
	b = connect(ui.checkBox_NG, &QCheckBox::toggled, [=](bool b)
		{
			m_bNGSTOP = b;
		});
}

void OffLine_Test::initImageLS(QString str)
{
	ui.lw_ImageList->blockSignals(true);
	ui.lw_ImageList->clear();
	if (str == ".")
	{
		BOOL fResult;
		// 遍历磁盘
		for (wchar_t d = 'A'; d <= 'Z'; d++)
		{
			TCHAR szTemp[3] = { d, ':', '\0' };
			// 获取磁盘类型
			UINT uType = GetDriveType(szTemp);
			// DRIVE_UNKNOWN  无法确定驱动器类型。
			// DRIVE_NO_ROOT_DIR 根路径无效; 例如，指定路径上没有安装卷。
			// DRIVE_REMOVABLE 驱动器有可移动介质; 例如，软盘驱动器，拇指驱动器或闪存卡读卡器。
			// DRIVE_FIXED 驱动器有固定的媒体; 例如，硬盘驱动器或闪存驱动器。
			// DRIVE_REMOTE 该驱动器是远程（网络）驱动器。
			// DRIVE_CDROM 该驱动器是一个CD-ROM驱动器。
			// DRIVE_RAMDISK 驱动器是RAM磁盘。
			switch (uType)
			{
			case DRIVE_FIXED:
			{
				ui.lw_ImageList->addItem(QString(d) + ":");
			}
			default:
			{
				continue;
			}
			}
		}
		ui.lw_ImageList->blockSignals(false);
		return;
	}
	QDir dir(str);
	ui.lw_ImageList->addItem(".");
	ui.lw_ImageList->addItem("..");
	ui.lw_ImageList->setSortingEnabled(true);
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
	ui.lw_ImageList->blockSignals(false);
}
bool OffLine_Test::isImage(QFileInfo& info)
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

QString GetFileProductName(QString fullName)
{
	DWORD dwLen = 0;
	char* lpData = NULL;

	BOOL bSuccess = FALSE;
	QString fileInfomation;
	dwLen = GetFileVersionInfoSize(fullName.toStdWString().c_str(), 0);
	if (0 == dwLen)
	{
		return "";
	}
	lpData = new char[dwLen + 1];

	bSuccess = GetFileVersionInfo(fullName.toStdWString().c_str(), 0, dwLen, lpData);
	if (!bSuccess)
	{
		delete[] lpData;
		return "";
	}

	LPVOID lpBuffer = NULL;
	UINT uLen = 0;
	bSuccess = VerQueryValue(lpData,
		(TEXT("\\VarFileInfo\\Translation")),
		&lpBuffer,
		&uLen);
	QString strTranslation, str1, str2;
	unsigned short int* p = (unsigned short int*)lpBuffer;
	str1.setNum(*p, 16);
	str1 = "000" + str1;
	strTranslation += str1.mid(str1.size() - 4, 4);
	str2.setNum(*(++p), 16);
	str2 = "000" + str2;
	strTranslation += str2.mid(str2.size() - 4, 4);
	QString code = "\\StringFileInfo\\" + strTranslation + "\\ProductName";//FileVersion
	bSuccess = VerQueryValue(lpData,
		(code.toStdWString().c_str()),
		&lpBuffer,
		&uLen);
	if (!bSuccess)
	{
		delete[] lpData;
		return "";
	}
	fileInfomation += QString::fromUtf16((const unsigned short int*)lpBuffer);
	delete[] lpData;//此处不需要释放
	return fileInfomation;
}
cv::Mat OffLine_Test::ReadImage(QString file)
{
	return cv::imread(file.toLocal8Bit().toStdString().c_str());
}
bool OffLine_Test::InitAlgo()
{
	QDir finder(AppPath);
	if (!finder.exists())
	{
		return -1;
	}
	finder.setFilter(QDir::Files | QDir::NoSymLinks);
	QFileInfoList list = finder.entryInfoList();
	int file_count = list.count();
	if (file_count <= 0)
	{
		return -1;
	}
	QString Liststring_list;
	for (int i = 0; i < file_count; i++)
	{
		QFileInfo file_info = list.at(i);
		QString suffix = file_info.suffix();//文件后缀
		if (QString::compare(suffix, QString("dll"), Qt::CaseInsensitive) == 0)//CaseInsensitive不区分大小写比较 QString::compare两个参数相等时为0
		{
			QString absolute_file_path = file_info.baseName();//绝对路径，包含文件但不包含后缀 basename作用是得到特定的路径中的最后一个'/',后面的内容但不包含后缀
			QString sss = absolute_file_path.right(10);//右侧10个字符
			if (absolute_file_path.right(10) == "CheckClass")
			{
				QString aDebug = file_info.filePath();//完整路径
				QLibrary* dynamicDLL = new QLibrary(file_info.filePath());
				if (dynamicDLL->load())//加载库文件
				{
					pExport CExportDLL = (pExport)(dynamicDLL->resolve("CreateExportObj"));//导出类
					pDeleteExport m_UnLoadDLL = (pDeleteExport)(dynamicDLL->resolve("DestroyExportObj"));
					_CheckClass = CExportDLL(false);
					_CheckClass->SetParam(0, "LOCALPATH");
				}
				else
				{
					emit ERRORMESSAGE(0, dynamicDLL->errorString(), 0);
				}
			}
		}
	}
	return true;
}
void resizeByNN(uchar* input, uchar* output, int height_in, int width_in, int channels, int height_out, int width_out) {
	uchar* data_source = input;
	uchar* data_half = output;

	int bpl_source = width_in * 3;
	int bpl_dst = width_out * 3;

	int pos = 0;
	int sep = 0;
	uchar* sr = nullptr;
	uchar* hr = nullptr;
	float step = 0.0;
	float step_x = float(width_in) / float(width_out);
	float step_y = float(height_in) / float(height_out);

	for (int i = 0; i < height_out; i++) {
		for (int j = 0; j < width_out; j++) {
			sep = int(step_y * i);
			step = int(j * step_x);
			sr = data_source + sep * bpl_source;
			hr = data_half + i * bpl_dst + j * channels;
			pos = step * channels;
			memcpy(hr, sr + pos, channels);
		}
	}
	return;
}

void OffLine_Test::ShowMatToLabel(cv::Mat img)
{
	char* d;


	resizeByNN(img.data, m_tempshow.data, img.rows, img.cols, img.channels(), m_tempshow.rows, m_tempshow.cols);

	cv::cvtColor(m_tempshow, m_tempRGB, cv::COLOR_BGR2RGB);

	disImage = QImage((const unsigned char*)(m_tempRGB.data), m_tempRGB.cols, m_tempRGB.rows, m_tempRGB.step, QImage::Format_RGB888);
	ui.label_Show->setPixmap(QPixmap::fromImage(disImage));
	ui.label_Show->show();
	QThread::msleep(1);
}

void OffLine_Test::onSelectImageList(QListWidgetItem* item, QListWidgetItem* it)
{
	//&&m_timerReadList==nullptr 防止进入深一层目录
	QString sSelectItem = item->text();

	QString pathselect;
	int x = m_sImageListPath.lastIndexOf('/');
	if (m_sImageListPath.lastIndexOf('/') == m_sImageListPath.length() - 1)
	{
		pathselect = m_sImageListPath + sSelectItem;
	}
	else
	{
		pathselect = m_sImageListPath + "/" + sSelectItem;
	}
	QFileInfo file(pathselect);
	if (file.isFile())
	{
		ui.label_3->setText(QString::fromLocal8Bit(pathselect.toLocal8Bit()));
		ImgRead = ReadImage(pathselect);
		if (m_w!=ImgRead.cols||m_h!=ImgRead.cols)
		{
			m_w = ImgRead.cols;
			m_h = ImgRead.rows;
			m_c = 3;
			_CheckClass->StartCheck("LOCALPATH", nullptr, m_w, m_h, m_c);
		}
		ResultStruct strResult;
		m_matCheck = ImgRead.clone();
		_CheckClass->Check(m_matCheck, nullptr, strResult);
		ShowMatToLabel(m_matCheck);
		
		emit CHECKRESULT(QStringList(QString(strResult.error_describe)));
		if ((strResult._bResultNGOK&& m_bOKSTOP)
			||(!strResult._bResultNGOK&&m_bNGSTOP))
		{
			ui.Button_Start->setChecked(false);
			return;
		}
		if (m_bContinueCheck)
		{
			ti->start(10);
		}
	}
}

void OffLine_Test::resizeEvent(QResizeEvent* event)
{
	QSize sz = ui.label_Show->size();
	m_tempshow = cv::Mat(sz.height(), sz.width(), CV_8UC3);

}

void OffLine_Test::ontoggledStartBtn(bool b)
{
	if (b)
	{
		ui.Button_Start->setText(QString::fromLocal8Bit("停止"));
	}
	else
	{
		ui.Button_Start->setText(QString::fromLocal8Bit("开始"));
	}
	m_bContinueCheck = b;
	if (m_bContinueCheck)
	{
		ImgListAutoDown();
	}

}

void OffLine_Test::ImgListAutoDown()
{
	int x = ui.lw_ImageList->currentRow();
	if (x < 2)
	{
		ui.lw_ImageList->setCurrentRow(2);
	}
	else
	{
		int  z = ui.lw_ImageList->count();
		x += 1;
		if (x != ui.lw_ImageList->count())
		{
			ui.lw_ImageList->setCurrentRow(x);
		}
		else
		{
			ui.Button_Start->setChecked(false);
			ui.Button_Start->setText(QString::fromLocal8Bit("开始"));
		}
	}
}

bool OffLine_Test::SlotShowResult(QStringList strlist)
{
	QString total = QString::number(ui.tableWidget_Result->item(0, 2)->text().toInt() + strlist.size());//减掉NULL数
	ui.tableWidget_Result->item(0, 2)->setText(total);
	for (int i = 0; i < strlist.size(); i++)
	{
		int z = 1;
		for (; z < ui.tableWidget_Result->rowCount(); z++)//最开始count==2，总数和合格数
		{
			if (ui.tableWidget_Result->item(z, 0)->text() == strlist[i])
			{
				int xx = ui.tableWidget_Result->item(z, 2)->text().toInt();
				ui.tableWidget_Result->item(z, 2)->setText(QString::number(ui.tableWidget_Result->item(z, 2)->text().toInt() + 1));
				break;
			}
		}
		int xx = ui.tableWidget_Result->rowCount();
		if (z == ui.tableWidget_Result->rowCount())//遍历没有，增加新行赋值1
		{
			if (strlist[i] != "NULL")
			{
				ui.tableWidget_Result->insertRow(z);
				ui.tableWidget_Result->setItem(z, 0, new QTableWidgetItem(strlist[i]));
				ui.tableWidget_Result->setItem(z, 1, new QTableWidgetItem(strlist[i]));
				ui.tableWidget_Result->setItem(z, 2, new QTableWidgetItem(QString::number(1)));
			}
		}
	}
	return false;
}

bool OffLine_Test::containImages(QDir& dir)
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


void OffLine_Test::onDoubleImageList(QListWidgetItem* item)
{
	//&&m_timerReadList==nullptr 防止进入深一层目录
	QString sSelectItem = item->text();
	if (sSelectItem == ".")
	{
		m_sImageListPath = "";
		initImageLS(sSelectItem);
		return;
	}
	if (sSelectItem == "..")
	{
		if (m_sImageListPath.length() == 3)
		{
			m_sImageListPath = "";
			initImageLS(".");
			return;
		}
		QString newPath = m_sImageListPath.left(m_sImageListPath.lastIndexOf("/"));
		if (m_sImageListPath.split("/").size() < 2)
		{
			// only allow user to access the data in dataPath
			m_sImageListPath = ".";
		}
		else//exe所在的根目录
			if (newPath.length() >= 2)
			{
				// only allow user to access the data in dataPath
				m_sImageListPath = newPath;
			}

		if (newPath.length() == 2)
		{
			// only allow user to access the data in dataPath
			m_sImageListPath +="/";
		}
		initImageLS(m_sImageListPath);
		ui.label_3->setText(m_sImageListPath);
		return;
	}


	QString pathselect;
	int x = m_sImageListPath.lastIndexOf('/');
	if (m_sImageListPath == ".")
	{
		pathselect = sSelectItem;
	}
	else
	{
		if (m_sImageListPath.lastIndexOf('/') == m_sImageListPath.length() - 1)
		{
			pathselect = m_sImageListPath + sSelectItem;
		}
		else
		{
			pathselect = m_sImageListPath + "/" + sSelectItem;
		}
	}
	QFileInfo file(pathselect);
	if (!file.isFile())
	{
		m_sImageListPath = pathselect;
		initImageLS(m_sImageListPath + "/");
	}
	else
	{
		ui.label_3->setText(pathselect);
	}
}