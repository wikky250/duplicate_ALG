#include "OffLine_Demo.h"

//日志工具
std::shared_ptr<spd::logger> daily_logger;
//操作员名称
QString g_QSUserName="hanlinzhineng_123_nengzhilinhan_321";
int g_IUserLevel;
//exe运行path
QString AppPath;
//程序终止符
HANDLE g_bShutDown = CreateEventW(nullptr, true, true, NULL);//默认，false自动复位|true手动复位，初始值，名称
//注册相机队列
QVector<CAMERASTRUCT*> g_vectorCamera;				//相机参数列表
//图像缓冲列表
QVector<CCycleBuffer*>	g_ImgBuffer;
//获取图像信号灯
QVector<HANDLE> WriteSign;
//USBKey句柄
HANDLE g_auhandle = (HANDLE)-1;
//算法方法库，共有多少种方法。
QVector<LOADDLLANDRELEASE*> dllVector;
//全局变量，串口句柄
HANDLE g_hCom = (HANDLE)-1;
//检测算法类实例化
QVector<CBaseCheckAlg*> g_CheckClass;						//检测类
//默认模板名称
QString g_qModelName;						//检测类
//检测类个数，通过InitCamera修改
int g_iCameraTotal = 0;
//每个线程控制开关
QVector <CONTROLPARAM> g_controlparam;
//采图多线程模块
QVector<MultGetThread_Run*> m_MultGetThread;
QVector<QThread*> QTH_MultGetThread;
//确定点击开始时间
QDateTime g_current_time;
int g_PhotoTimes = 0;
int MAX_CAPSULECOUNT;
// void ResultCallBack(UI_MONITOR ui, char* i_result)
// {
// 	((OffLine_Demo*)ui)->emit EveryResult2Summary(i_result);
// }
void ShowCallBack(UI_MONITOR ui, int pos, Mat img, int checktimes)
{
	((OffLine_Demo*)ui)->emit SignShowImage(pos, img, checktimes);
}

void CloseCallBack()
{
}


OffLine_Demo::OffLine_Demo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./dr.ico"));//文件copy到了exe所在目录
	qRegisterMetaType<Mat>("Mat");
	qRegisterMetaType<Mat>("Mat&");//注册opencv函数，在槽函数中避免出错
	AppPath = qApp->applicationDirPath();//exe所在目录

	m_sImageListPath = AppPath;
	initImageLS(m_sImageListPath);

	char dest[100], dest2[100];//太小会溢出

	g_IUserLevel = 0;
	strcpy(dest2, "您的用户权限为： ");
	switch (g_IUserLevel)
	{
	case 0:strcpy(dest, "管理员"); break;
	case 1:strcpy(dest, "工程师"); break;
	case 2:strcpy(dest, "操作员"); break;
	case 3:strcpy(dest, "质检员"); break;
		default:break;

	}

	strcat(dest2, dest);




	m_bAllInited = false;//初始化完成：ini 相机
	Camera_Func = nullptr;//包含bool GetAllCamera();int ReadConfig();
	m_bStarting = false;//是否在检测标识符
	m_SLabelStatue = "";//标记是否在检测
	times_listImg = nullptr;
	current_time = QDateTime::currentDateTime();
	QString logpath = AppPath + "/logs/MainDLG_daily_" 
		+ QString::number(current_time.date().year()) + "_"
		+ QString::number(current_time.date().month()) + "_"
		+ QString::number(current_time.date().day()) + "_"
		+ QString::number(current_time.time().hour()) + "_"		 
		+ QString::number(current_time.time().minute())+".txt";//eg:MainDLG_daily_2020_2_29_23_47.txt
	daily_logger = spd::basic_logger_mt("daily_logger", logpath.toStdString());//标准logger格式，每行信息头名称为daily_logger
	daily_logger->flush_on(spd::level::trace);//初始化一次
	m_MultInit = new MultInit_Run(this);//对多线程初始化的类
	bool flag = QObject::connect(this, SIGNAL(StartInitSingle()), m_MultInit, SLOT(ThreadInit()));//window启动时执行一线程的初始化   read111
	QTH_Init = new QThread();//新线程
	m_MultInit->moveToThread(QTH_Init);//将槽函数放到新线程中去执行
	QTH_Init->start();//开始执行
	timer = new QTimer(this);
	timer->setSingleShot(true);//单次触发

	connect(ui.Button_Start, SIGNAL(toggled(bool)), this, SLOT(onStartCheck(bool)));//checked button
	connect(ui.Button_CameraSet, SIGNAL(clicked()), this, SLOT(onCameraSet()));//not checked button
	connect(ui.Button_Exit, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "退出确认", "确认退出系统吗？", "确认", "取消"))
		{
			close();
		}});

	flag = connect(this, SIGNAL(SignShowImage(int, Mat, int)), this, SLOT(SLOTShowImage(int, Mat, int)));//openCV有关

	QFont fo = ui.tableWidget_Result->font();
	fo.setPointSize(18);
	ui.tableWidget_Result->setFont(fo);//设置tableWidget字体
	QStringList title;
	title << QString::fromLocal8Bit("ErrorType")<< QString::fromLocal8Bit("错误类型") << QString::fromLocal8Bit("错误数量");
	ui.tableWidget_Result->setColumnCount(3);//3列
	ui.tableWidget_Result->setHorizontalHeaderLabels(title);//加表头
	ui.tableWidget_Result->setColumnHidden(0, true);//隐藏ErrorType列
	ui.tableWidget_Result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分填充表头
	ui.tableWidget_Result->verticalHeader()->setDefaultSectionSize(20);//默认行高20
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

	connect(ui.lw_ImageList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onDoubleImageList(QListWidgetItem *)));
	LoadImportantValue();
	/*******************eg：
	[ProgramSetting]
	PhotoTimes = 4				//一个胶囊拍几次
	MAX_CAPSULECOUNT = 6		//每个相机拍胶囊数量
	DefaultModel = testA		//默认模板
	IpAddress = 10.86.50.210	//PLC IP
	port = 5000					//PLC端口
	********************/
	

}

bool deleteDir(const QString &path)//删除路径
{
	if (path.isEmpty()) {//字符串类型路径为“”，返回0
		return false;
	}
	QDir dir(path);
	if (!dir.exists()) {//QDir类型路径不存在，返回1
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤，AllEntries 其值为Dirs | Files | Drives，列出目录、文件、驱动器及软链接等所有文件；不列出.和..（只有第一个过滤条件会有这两个东西存在）
	QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
	foreach(QFileInfo file, fileList) { //遍历文件信息
		if (file.isFile()) { // 是文件，删除
			file.dir().remove(file.fileName());
		}
		else { // 递归删除
			deleteDir(file.absoluteFilePath());
		}
	}
	return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

OffLine_Demo::~OffLine_Demo()
{
#ifdef PLCCONNECT
	if (g_SocketPLC != nullptr)
	{
		g_SocketPLC->disconnect();//断开PLC
		delete g_SocketPLC;
		g_SocketPLC = nullptr;
	}
#endif

	//删除DefaultRun，作为下一次检验上次是否正常关闭的标志
	deleteDir(AppPath + "/DefaultModel");
	disconnect(this, SIGNAL(SignShowImage(int, Mat, int)), this, SLOT(SLOTShowImage(int, Mat, int)));//openCV有关避免界面关掉还得接收
	disconnect(this, SIGNAL(SHOWEVERYPLCCONNECT(DataToPC_typ)), this, SLOT(showPLCValue(DataToPC_typ)));

}


void OffLine_Demo::mousePressEvent(QMouseEvent * p)
{
}

void OffLine_Demo::mouseDoubleClickEvent(QMouseEvent * q)
{
}

void OffLine_Demo::showEvent(QShowEvent * p)
{
	if (!m_bAllInited)
	{
		timer->start(1);
	}
	emit StartInitSingle();
}

bool copyDirectoryFiles(const QString & fromDir, const QString & toDir, bool coverFileIfExist)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if (!targetDir.exists()) {    /**< 如果目标目录不存在,则进行创建 */
		if (!targetDir.mkdir(targetDir.absolutePath()))
		{
			return false;
		}
	}

	QFileInfoList fileInfoList = sourceDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList) {
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if (fileInfo.isDir()) {    /**< 当为目录时,递归的进行copy */
			if (!copyDirectoryFiles(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()),
				coverFileIfExist))
				return false;
		}
		else {            /**< 当允许覆盖操作时,将旧文件进行删除操作 */
			if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
				targetDir.remove(fileInfo.fileName());
			}

			/// 进行文件copy
			if (!QFile::copy(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()))) {
				return false;
			}
		}
	}
	return true;
}
bool OffLine_Demo::LoadImportantValue()
{
	/*******************eg：
	[ProgramSetting]
	PhotoTimes = 4				//一个胶囊拍几次
	MAX_CAPSULECOUNT = 6		//每个相机拍胶囊数量
	DefaultModel = testA		//默认模板
	IpAddress = 10.86.50.210	//PLC IP
	port = 5000					//PLC端口
	********************/
	QSettings configIniRead(AppPath + "\\ModelFile\\ProgramSet.ini", QSettings::IniFormat);//读取ini文件
	g_PhotoTimes = 1;//第二个数是默认数值，如果不存在就用默认数值，下同
	MAX_CAPSULECOUNT = configIniRead.value("ProgramSetting/MAX_CAPSULECOUNT", 0).toInt();



	if (MAX_CAPSULECOUNT <= 0)
	{
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("关键错误！未设置胶囊个数！"), 5000);
		//levelOut->show();
	}
	g_qModelName = configIniRead.value("ProgramSetting/DefaultModel", "testA").toString();//当DefaultModel不存在时，返回testA
	QString modelfile = AppPath + "/ModelFile/" + g_qModelName;
	if (QDir(modelfile).isEmpty())//路径不存在
	{
		QMessageBox::critical(nullptr, QString::fromLocal8Bit("关键错误"), QString::fromLocal8Bit("默认模板不存在"));
		return false;
	}

	QDir finder(AppPath + "/DefaultModel");//finder为AppPath位置，找DefaultModel文件夹
	if(!finder.exists())//如果没有上述文件夹
	{
		copyDirectoryFiles(modelfile, AppPath + "/DefaultModel", true);//默认模板的东西拷入DefaultModel中
		g_qModelName=configIniRead.value("ProgramSetting/LastModel", g_qModelName).toString();//写当前模板全局变量 此时不是读
	}
	else//如果有这个文件夹
	{
		if(QMessageBox::No == showMsgBox(QMessageBox::Critical,"警告","上次程序未能正常关闭,是否使用上次模板继续？","是","否"))
		{
			copyDirectoryFiles(modelfile, AppPath + "/DefaultModel", true);
			g_qModelName = configIniRead.value("ProgramSetting/DefaultModel", g_qModelName).toString();//写当前模板全局变量 此时不是读
				//此时不用再设置g_qModelName
		}
		else
		{
			//g_qModelName = txtFilenameInDir(AppPath + "/DefaultModel");//此时不一定是默认模板，而是应用的模板
			g_qModelName = configIniRead.value("ProgramSetting/LastModel", "testA").toString();
		}
	}
	return true;
}

void OffLine_Demo::createFile(QString filePath, QString fileName)//为创建txt文件，名字是模板名
{
	QDir tempDir;
	//临时保存程序当前路径
	QString currentDir = tempDir.currentPath();
	//如果filePath路径不存在，创建它
	if (!tempDir.exists(filePath))
	{
		qDebug() << "不存在该路径" << endl;
		tempDir.mkpath(filePath);
	}
	QFile* tempFile = new QFile;
	//将程序的执行路径设置到filePath下
	tempDir.setCurrent(filePath);
	qDebug() << tempDir.currentPath();
	//检查filePath路径下是否存在文件fileName,如果停止操作。
	if (tempFile->exists(fileName))
	{
		qDebug() << "文件存在";
		return;
	}
	//此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
	tempFile->setFileName(fileName);
	if (!tempFile->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "打开失败";
	}
	tempFile->close();
	//将程序当前路径设置为原来的路径
	tempDir.setCurrent(currentDir);
	qDebug() << tempDir.currentPath();
}

QString OffLine_Demo::txtFilenameInDir(QString path)//截取唯一的.txt文件名
{
	QDir dir(path);
	//查看路径中后缀为.cfg格式的文件
	QStringList filters;
	filters << QString("* .txt");
	dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
	dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
	QString file_name = dir[0].left(dir[0].indexOf("."));//（.的位置）左边的东西截取下来
	return file_name;
}

bool OffLine_Demo::FinishInitOther()
{
	m_bAllInited = true;

	return false;
}

int OffLine_Demo::LoadAlgorithmDLL()
{
	QDir finder(AppPath);//finder为AppPath位置
	if (!finder.exists())
	{
		return -1;
	}
	finder.setFilter(QDir::Files | QDir::NoSymLinks);//设置过滤器，setFilter（）设置搜索条件，QDir::Files - 只列出文件，QDir::NoSymLinks - 不列出符号连接（不支持符号连接的操作系统会忽略它）
	QFileInfoList list = finder.entryInfoList(); //通过entryInfoList则可以获取过滤后所得到的文件夹下的文件信息列表，遍历文件通过操作QFileInfo可得到所需的文件详细信息（大小、类型、后缀等）
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
		if (QString::compare(suffix, QString("dll"), Qt::CaseInsensitive) == 0)//CaseInsensitive不区分大小写比较
		{
			QString absolute_file_path = file_info.baseName();//绝对路径，包含文件但不包含后缀
			QString sss = absolute_file_path.right(10);//右侧10个字符
			if (absolute_file_path.right(10) == "CheckClass")
			{
				QLibrary dynamicDLL(file_info.filePath());
				if (dynamicDLL.load())
				{
					LOADDLLANDRELEASE *libiary = new LOADDLLANDRELEASE();
					libiary->LoadDLL = (pExport)dynamicDLL.resolve("_CreateExportObj@4");
					libiary->UnLoadDLL = (pDeleteExport)dynamicDLL.resolve("_DestroyExportObj@4");

					//
					strcpy(libiary->dllName, (absolute_file_path.mid(3, absolute_file_path.size() - 8)).toStdString().c_str());
					dllVector.push_back(libiary);//放入dllVector
					//delete ss;
				}
				else
				{
					QMessageBox::about(NULL, "", dynamicDLL.errorString());
				}

			}
		}
	}
	return 0;
}

bool OffLine_Demo::InitCheckClass()
{

	int cou = g_vectorCamera.size();//配对的相机参数
	for (int i = 0; i < cou; i++)
	{
		for (int z = 0;z< g_PhotoTimes;z++)
		{
			QLabel *label = new QLabel();
			label->setObjectName(QString::fromUtf8("LabelShow") + QString::number(i)+"_"+QString::number(z));
			label->setFrameShape(QFrame::Box);
			label->setLineWidth(1);
			ui.gridLayout->addWidget(label, i, z, 1, 1);
		}
	}

	//for (int i = 0; i < g_iCameraTotal; i++)//此时g_iCameraTotal== g_vectorCamera.size()
	{
		int i = 0;
		daily_logger->info("InitCheckClass winId{}", i);
		daily_logger->flush();
		pExport CExportDLL = NULL;//导出类
		//for (int j = 0; j < dllVector.size(); j++)
		{
			int j = 0;
			//if (strcmp(g_vectorCamera[i]->c_AlgName, dllVector[j]->dllName) == 0)
			{
				CExportDLL = dllVector[j]->LoadDLL;
				m_UnLoadDLL = dllVector[j]->UnLoadDLL;
				//break;
			}
		}if (CExportDLL != NULL)
		{
			//QString sss = "LabelShow" + QString::number(i,10);
			CBaseCheckAlg *_CheckClass = CExportDLL();
			if (0 == i)
			{
				if (g_auhandle == (HANDLE)-1)
				{
					g_auhandle = _CheckClass->GetEncryptHandle();
				}
			}
			if (NULL != _CheckClass)
			{

				_CheckClass->InitWindow(i, i == 0 ? g_auhandle : NULL, this);
				//this->findChild<QLabel *>("label" + QString::number(i))->setText(QString(g_vectorCamera[i]->c_CameraSign) + "---" + g_vectorCamera[i]->c_CameraOtherSign);
				//设置算法库参数
				_CheckClass->SetParam(-1, "LocalFile");
				_CheckClass->SetShowCallBack(this, ShowCallBack);
				//_CheckClass->SetResultCallBack(this,ResultCallBack);
				_CheckClass->SetCloseCallBack(CloseCallBack);

				g_CheckClass.push_back(_CheckClass);
			}
		}
	}
	if (timerResize == nullptr)
	{
		timerResize = new QTimer();
		timerResize->setSingleShot(true);
		connect(timerResize, SIGNAL(timeout()), this, SLOT(SetEvertDlg()));
		timerResize->start(1000);
	}

	return false;
}

bool OffLine_Demo::InitPicList()
{
#ifndef CALLBACKFUNC
	while (m_MultGetThread.size() != g_iCameraTotal)
	{
		m_MultGetThread.push_back(new MultGetThread_Run(nullptr));
	}
	while (QTH_MultGetThread.size() != g_iCameraTotal)
	{
		QTH_MultGetThread.push_back(new QThread);
	}
#endif
	while (m_MultDecodeThread.size() != g_iCameraTotal)
	{
		m_MultDecodeThread.push_back(new MultDecodeThread_Run(nullptr));
	}
	while (QTH_MultDecodeThread.size() != g_iCameraTotal)
	{
		QTH_MultDecodeThread.push_back(new QThread);
	}
	m_MultSaveThread = new MultSaveThread_Run(nullptr,g_vectorCamera.size());
	QTH_MultSaveThread = new QThread();
	m_MultSaveThread->moveToThread(QTH_MultSaveThread);
	QTH_MultSaveThread->start();
	m_MultSummaryThread = new MultSummaryThread_Run(nullptr, g_vectorCamera.size());
	QTH_MultSummaryThread = new QThread();
	m_MultSummaryThread->moveToThread(QTH_MultSummaryThread);
	QTH_MultSummaryThread->start();
	while (WriteSign.size() < g_iCameraTotal)
	{
		WriteSign.push_back(CreateEventW(NULL, true, true, NULL));
	}
	while (g_controlparam.size() < g_iCameraTotal)
	{
		CONTROLPARAM tempcontrol;
		tempcontrol.hOutTrigger = CreateEventW(NULL, false, false, NULL);
		tempcontrol.hPausle = CreateEventW(0, 1, 1, 0);
		tempcontrol.h_ReadFirstImg = CreateEventW(NULL, true, false, NULL);
		tempcontrol.hResultList = CreateEventW(NULL, true, false, NULL);
		tempcontrol.hResultTrigger = CreateEventW(NULL, false, false, NULL);
		tempcontrol.hFinishGetImage = CreateEventW(NULL, true, true, NULL);
		tempcontrol.hFinishDecodeLine = CreateEventW(NULL, true, true, NULL);
		tempcontrol.hNewImageArrive = CreateEventW(NULL, true, false, NULL);
		SetEvent(tempcontrol.hPausle);
		g_controlparam.push_back(tempcontrol);
	}
	bool flag = false;
	for (int i = 0; i < g_iCameraTotal; i++)
	{
		flag = QObject::connect(this, SIGNAL(STARTCHECK(int, bool)), m_MultGetThread[i], SLOT(ThreadGetImage(int, bool)), Qt::QueuedConnection);
		m_MultGetThread[i]->SetMultIndex(i);
		m_MultGetThread[i]->moveToThread(QTH_MultGetThread[i]);
		QTH_MultGetThread[i]->start();
		flag = QObject::connect(this, SIGNAL(STARTCHECK(int, bool)), m_MultDecodeThread[i], SLOT(ThreadDecodeImage(int, bool)), Qt::QueuedConnection);
		flag = QObject::connect(m_MultGetThread[i], SIGNAL(GETONEIMAGEMAT(Mat)), m_MultDecodeThread[i], SLOT(ThreadDecodeImageMat(Mat)), Qt::QueuedConnection);
		flag = QObject::connect(m_MultDecodeThread[i], SIGNAL(SAVESIGNAL(Mat,QString)), m_MultSaveThread, SLOT(ThreadSave(Mat,QString)), Qt::QueuedConnection);
		flag = QObject::connect(m_MultDecodeThread[i], SIGNAL(OUTRESULTSUMMARY(QString, int, int)), m_MultSummaryThread, SLOT(ThreadSummary(QString, int, int)), Qt::QueuedConnection);
		flag = QObject::connect(m_MultSummaryThread, SIGNAL(SUMMARYRESULTINCIRCLE(QStringList)), this, SLOT(SlotShowResult(QStringList)), Qt::QueuedConnection);
		m_MultDecodeThread[i]->SetMultIndex(i);
		m_MultDecodeThread[i]->moveToThread(QTH_MultDecodeThread[i]);
		QTH_MultDecodeThread[i]->start();
	}
	//设置检测队列
	while (g_iCameraTotal != g_ImgBuffer.size())
	{
		CCycleBuffer *v_uchar/* = new CCycleBuffer(2448 * 2048 * 3 * 5)*/;
		g_ImgBuffer.push_back(v_uchar);
	}
	return false;
}

void OffLine_Demo::SLOTShowImage(int pos, Mat img, int checktimes)
{
	QLabel * label = this->findChild<QLabel *>("LabelShow" + QString::number(pos) + "_" + QString::number(checktimes%g_PhotoTimes));
	int zz = label->frameWidth();
	QSize ss = label->size();
	ss.setWidth(ss.width() - zz * 2);
	ss.setHeight(ss.height() - zz * 2);
	Mat imgsend;
	cv::cvtColor(img, imgsend, COLOR_BGR2RGB);
	cv::resize(imgsend, imgsend, Size(ss.width(), ss.height()));
	QImage disImage = QImage((const unsigned char*)(imgsend.data), imgsend.cols, imgsend.rows, imgsend.step, QImage::Format_RGB888);
	label->setPixmap(QPixmap::fromImage(disImage));
}

bool OffLine_Demo::OpenConnect()
{
	return false;
}

bool OffLine_Demo::SlotShowResult(QStringList strlist)
{
	//"Good,Error1,Error2,Error1,Error0,..."
	QSettings Dir(m_IniResultPath, QSettings::IniFormat);//找到按下开始时创建的路径下的文件 没有就自动创建
	QString total = QString::number(ui.tableWidget_Result->item(0, 2)->text().toInt() + strlist.size());
	ui.tableWidget_Result->item(0, 2)->setText(total);
	Dir.setValue("Result/Total", total);//写总数，没有key值也会创建一个

	for (int i = 0; i < strlist.size(); i++)
	{
		int z = 1;
		for (; z < ui.tableWidget_Result->rowCount(); z++)//最开始count==2，总数和合格数
		{
			if (ui.tableWidget_Result->item(z, 0)->text() == strlist[i])
			{
				int xx = ui.tableWidget_Result->item(z, 2)->text().toInt();
				ui.tableWidget_Result->item(z, 2)->setText(QString::number(ui.tableWidget_Result->item(z, 2)->text().toInt()+1));
				Dir.setValue("Result/" + strlist[i], QString::number(ui.tableWidget_Result->item(z, 2)->text().toInt()));
				break;
			}
		}
		int xx = ui.tableWidget_Result->rowCount();
		if (z == ui.tableWidget_Result->rowCount())//遍历没有，增加新行赋值1
		{
			ui.tableWidget_Result->insertRow(z);
			ui.tableWidget_Result->setItem(z, 0, new QTableWidgetItem(strlist[i]));
			ui.tableWidget_Result->setItem(z, 1, new QTableWidgetItem(strlist[i]));
			ui.tableWidget_Result->setItem(z, 2, new QTableWidgetItem(QString::number(1)));
			Dir.setValue("Result/" + strlist[i], QString::number(ui.tableWidget_Result->item(z, 2)->text().toInt()));
		}
	}
	return false;
}

void OffLine_Demo::onTest()
{
}


void OffLine_Demo::onStartCheck(bool b)
{
	if (b)
	{
 		if (m_MultInit != nullptr)
 		{
 			delete m_MultInit;
 			m_MultInit = nullptr;
 		}
 
 		if (QTH_Init != nullptr)
 		{
 			QTH_Init->quit();
 			QTH_Init->wait();//释放对象锁
 			delete QTH_Init;
 			QTH_Init = nullptr;
 		}
 
 		QThread::msleep(200);
 		daily_logger->info("Start Checking! Connect Camera {}", g_iCameraTotal);
 		//判断有无相机加载
 		if (g_iCameraTotal == 0)
 		{
			//levelOut = new WindowOut;
			//levelOut->getString(QString::fromLocal8Bit("警告！未初始化本地文件！"), 2000);
			//levelOut->show();
 			return;
 		}
 		ui.Button_Start->setText(QString::fromLocal8Bit("停止"));
 		g_current_time = QDateTime::currentDateTime();
		memset(m_Result, 0, 30*sizeof(char));
		int s = ui.lw_ImageList->currentRow();
		if (s > 1)
		{
			emit STARTCHECK(-1,0);
		}
		if(s==-1)
		{
			if (times_listImg ==nullptr)
			{
				times_listImg = new QTimer();
			}
			QObject::connect(times_listImg, SIGNAL(timeout()), this, SLOT(ImgAutoDown()));

			connect(ui.lw_ImageList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(onSelectImageList(QListWidgetItem *, QListWidgetItem *)));
			times_listImg->start(1000);
		}
		if (s>1)
		{
			if (times_listImg == nullptr)
			{
				times_listImg = new QTimer();
			}
			QObject::connect(times_listImg, SIGNAL(timeout()), this, SLOT(ImgAutoDown()));

			connect(ui.lw_ImageList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(onSelectImageList(QListWidgetItem *, QListWidgetItem *)));
			times_listImg->start(1000);
		}
 		QThread::msleep(500);
 		current_time = QDateTime::currentDateTime();
 		QString	StrCurrentTime = current_time.toString("hh:mm:ss:zzz");
		m_IniResultPath = AppPath + "/logs/Result_"//结果文件路径,每次开始的时候创建一个
			+ QString::number(current_time.date().year()) + "_"
			+ QString::number(current_time.date().month()) + "_"
			+ QString::number(current_time.date().day()) + "_"
			+ QString::number(current_time.time().hour()) + "_"
			+ QString::number(current_time.time().minute()) + ".ini";//eg:Result_2020_2_29_23_47.ini


#ifdef PLCCONNECT
		g_SocketPLC->StartWork();
#endif
	}
	else
	{
		onStopCheck();
	}
	return;
}
void OffLine_Demo::onSelectImageList(QListWidgetItem *item, QListWidgetItem *it)
{
	//&&m_timerReadList==nullptr 防止进入深一层目录
	QString sSelectItem = item->text();
// 	if (sSelectItem == ".")
// 	{
// 		QString newPath = m_sImageListPath.left(m_sImageListPath.indexOf("/") + 1);
// 		m_sImageListPath = newPath;
// 		initImageLS(m_sImageListPath);
// 		strcpy(g_vectorCamera[0]->file_path, m_sImageListPath.toLocal8Bit());
// 		return;
// 	}
// 	if (sSelectItem == "..")
// 	{
// 		QString newPath = m_sImageListPath.left(m_sImageListPath.lastIndexOf("/"));
// 		//exe所在的根目录
// 		if (newPath.length() > 2)
// 		{
// 			// only allow user to access the data in dataPath
// 			m_sImageListPath = newPath;
// 		}
// 		if (newPath.length() == 2)
// 		{
// 			// only allow user to access the data in dataPath
// 			m_sImageListPath = newPath + "/";
// 		}
// 		initImageLS(m_sImageListPath);
// 		strcpy(g_vectorCamera[0]->file_path, m_sImageListPath.toLocal8Bit());
// 		return;
// 	}
// 

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
		strcpy(g_vectorCamera[0]->file_path, pathselect.toLocal8Bit());
		emit(STARTCHECK(-1,0));
	}
}

void OffLine_Demo::onDoubleImageList(QListWidgetItem *item)
{
	//&&m_timerReadList==nullptr 防止进入深一层目录
	QString sSelectItem = item->text();
	if (sSelectItem == ".")
	{
		QString newPath = m_sImageListPath.left(m_sImageListPath.indexOf("/") + 1);
		m_sImageListPath = newPath;
		initImageLS(m_sImageListPath);
		strcpy(g_vectorCamera[0]->file_path, m_sImageListPath.toLocal8Bit());
		return;
	}
	if (sSelectItem == "..")
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
		strcpy(g_vectorCamera[0]->file_path, m_sImageListPath.toLocal8Bit());
		return;
	}


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
	if (!file.isFile())
	{
		m_sImageListPath = pathselect;
		initImageLS(m_sImageListPath);
		strcpy(g_vectorCamera[0]->file_path, m_sImageListPath.toLocal8Bit());
	}
	else
	{
		strcpy(g_vectorCamera[0]->file_path, pathselect.toLocal8Bit());
		emit(STARTCHECK(-1,1));
	}
}

void OffLine_Demo::ImgAutoDown()
{
	int x = ui.lw_ImageList->currentRow();
	if (x<2)
	{
		ui.lw_ImageList->setCurrentRow(2);
	}
	else
	{
		int  z = ui.lw_ImageList->count();
		x+=1;
		if (x!=ui.lw_ImageList->count())
		{
			ui.lw_ImageList->setCurrentRow(x);
		}
		else
		{
			times_listImg->stop();
			delete times_listImg;
			times_listImg = nullptr;
		}
	}
}

void OffLine_Demo::SetEvertDlg()
{
	for (int i = 0; i < 1; i++)//此时g_iCameraTotal== g_vectorCamera.size()
	{
		for (int ii = 0;ii < g_PhotoTimes;ii++)
		{
			QLabel *lb = this->findChild<QLabel *>(QString::fromUtf8("LabelShow") + QString::number(i) + "_" + QString::number(ii));
			if (lb != nullptr)
			{
				g_CheckClass[i]->TESTSETSHOW(lb);
			}
		}
		g_CheckClass[i]->StartCheck(g_vectorCamera[i]->c_CameraName, daily_logger);
	}
	if (timerResize != nullptr)
	{
		timerResize->stop();
		delete timerResize;
		timerResize = nullptr;
	}
}


bool OffLine_Demo::isImage(QFileInfo& info)
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

bool OffLine_Demo::containImages(QDir& dir)
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

void OffLine_Demo::initImageLS(QString str)
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
void OffLine_Demo::onStopCheck()
{
	//if Connect Basler camera just stop grabbing 
	if (nullptr!= times_listImg)
	{
		times_listImg->stop();
		delete times_listImg;
		times_listImg = nullptr;
	}
	disconnect(ui.lw_ImageList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(onSelectImageList(QListWidgetItem *, QListWidgetItem *)));
	ui.Button_Start->setText(QString::fromLocal8Bit("开始"));
}

void OffLine_Demo::closeEvent(QCloseEvent * eve)
{
}

void OffLine_Demo::onTestSignal()
{
}

void OffLine_Demo::onCameraSet()
{
	//相机参数设置
	ui.Button_Start->setChecked(false);
	QtCameraSet m_DlgCameraSet;
	m_DlgCameraSet.exec();
	for (int i=0;i<g_CheckClass.size();i++)
	{
		g_CheckClass[i]->SetShowCallBack(this, ShowCallBack);	//与QtGUISetting的line44 对应，需要将回调函数注册回当前窗体 
	}
}

int OffLine_Demo::showMsgBox(QMessageBox::Icon icon,const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str)//全是中文
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

	//  QMessageBox::NoIcon
	//	QMessageBox::Question
	//	QMessageBox::Information
	//	QMessageBox::Warning
	//	QMessageBox::Critical
}

