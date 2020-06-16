#include "QtCameraSet.h"
extern QString AppPath;
extern std::shared_ptr<spd::logger> daily_logger;
extern QVector<CAMERASTRUCT*> g_vectorCamera;
extern QVector<MultGetThread_Run*> m_MultGetThread;
extern QVector<CBaseCheckAlg*> g_CheckClass;
extern CBaseCheckAlg* EnsureAlg(QString str);
#ifdef PLCCONNECT
extern Socket_CPP* g_SocketPLC;
#endif
extern QString g_qModelName;//当前应用模板

extern QString g_QSUserName;
extern int g_IUserLevel;

//设置外触发Line1
extern void SetTriggerLine(int i, int line);
//设置软触发
extern void SetTriggerSoft(int i);

QtCameraSet::QtCameraSet(QWidget* parent)
	: QDialog(parent)

{
	ui.setupUi(this);
	setWindowIcon(QIcon("./dr.ico"));
	ui.lb_AppliedName->setText(g_qModelName);//更新lable 4-3

	if (g_IUserLevel==1)//工程师移除一栏
	{
		ui.tabWidget->removeTab(3);
	}
	if (g_IUserLevel == 0)
	{
		initTableOfUserPermission();//用户处表格
	}

	initListWidgetofModel();//初始化model listWidget
	//should reconstruct the position
	//2020年2月24日10:30:39
	connect(ui.pB_StartGrab, SIGNAL(toggled(bool)), this, SLOT(StartGrab(bool)));//采集测试
	connect(ui.pB_StartGrab, SIGNAL(toggled(bool)), this, SLOT(SyncAnotherStartGrab(bool)));//采集测试
	connect(ui.pB_StartGrabAnother, SIGNAL(toggled(bool)), this, SLOT(SyncFirstStartGrab(bool)));//采集测试
	connect(ui.pB_AlgSetting, SIGNAL(clicked()), this, SLOT(StopGrab()));//算法设置
	connect(ui.pB_AlgSetting, SIGNAL(clicked()), this, SLOT(onShowAlgSet()));//同上
	connect(ui.pB_StartContinueGrab, SIGNAL(toggled(bool)), this, SLOT(onContinueGrab(bool)));//相机调试
	connect(ui.pb_StReadPLC, SIGNAL(toggled(bool)), this, SLOT(onReadPLCParam(bool)));//读取
	connect(ui.pb_StWritePLC, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));//写入
	for (int i = 0; i < 30; i++)
	{
		QString objname = "pb_cmdTestKick";
		objname = objname + QString::number(i);
		QPushButton* btn = findChild<QPushButton*>(objname); //返回this中一个名为objname的QPushButton孩子，即使按钮不是父亲的直接孩子：30个剔废按钮
		if (btn != nullptr)
		{
			connect(btn, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));//30个剔废按钮checked
		}

	}
	connect(ui.pb_cmdHome, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));
	connect(ui.pb_cmdStart, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));
	connect(ui.pb_cmdStop, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));
	connect(ui.pb_cmdEStop, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));
	connect(ui.pb_cmdJog, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));
	connect(ui.pb_cmdErrorAck, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));						//报警复位, 1:复位
	connect(ui.pb_cmdResetCounter, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//复位计数变量, 1:复位
	connect(ui.pb_cmdParaSave, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));						//参数保存命令, 1:保存
	connect(ui.pb_cmdParaLoad, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));						//参数读取命令, 1:读取
	connect(ui.pb_cmdTestFlash0, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));			//手动闪光, 1:闪光,自动复位
	connect(ui.pb_cmdTestFlash1, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));
	connect(ui.pb_cmdTestFlash2, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));

	connect(ui.pb_cmdTestValveUp, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));						//手动升降气缸, 1:Push, 2:Back
	connect(ui.pb_cmdTestValveClip, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//手动夹紧气缸, 1:Push, 2:Back
	connect(ui.pb_cmdTestValveDrop, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//手动落囊气缸, 1:Push, 2:Back
	connect(ui.pb_cmdTestInverter, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//手动胶囊料斗启动, 1:Start, 2:Stop
	connect(ui.pb_cmdTestLampRead, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//手动红灯输出, 1:输出 , 2: 复位
	connect(ui.pb_cmdTestLampYellow, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//手动黄灯输出, 1:输出 , 2: 复位
	connect(ui.pb_cmdTestLampGreen, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//手动绿灯输出, 1:输出 , 2: 复位
	connect(ui.pb_cmdTestBuzzer, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));						//手动蜂鸣器输出, 1:输出 , 2: 复位
	connect(ui.pb_cmdTestPhoto, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));						//手动拍照, 1:输出 , 2: 复位
	connect(ui.pb_cmdTestFlashPhoto, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//手动闪光加拍照, 1:启动
	connect(ui.pb_cmdTestCapPhoto, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));					//手动胶囊拍照
	connect(ui.pb_cmdRotateCtl, SIGNAL(toggled(bool)), this, SLOT(onSendPLCCommand(bool)));						//手动转囊启停

	connect(ui.pB_Model_Apply, SIGNAL(clicked()), this, SLOT(modelApply()));//clicked();
	connect(ui.pB_Model_Exit1, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "退出确认", "确认退出系统设置页面吗？", "确认", "取消"))
		{
			close();
		}});
	connect(ui.pB_Model_Exit2, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "退出确认", "确认退出系统设置页面吗？", "确认", "取消"))
		{
			close();
		}});
	connect(ui.pB_Model_Exit3, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "退出确认", "确认退出系统设置页面吗？", "确认", "取消"))
		{
			close();
		}});

	//model部分的信号与槽↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
	//Users部分的信号与槽↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	ui.widget_UsersChoice->setVisible(0);//默认不显示 新建用户
	ui.widget_UsersChoice_2->setVisible(0);//默认不显示 查看权限
	ui.widget_UsersChoice_2->move(220, 60);//默认不显示
	connect(ui.pB_Users_Exit, &QPushButton::clicked, [=]() {
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Question, "退出确认", "确认退出系统设置页面吗？", "确认", "取消"))
		{
			close();
		}});
	connect(ui.pB_Users_Add, &QPushButton::clicked, [=]() {
		if (ui.pB_Users_Add->text() == QString::fromLocal8Bit("添加"))
		{
			ui.widget_UsersChoice->setVisible(1);
			ui.pB_Users_Add->setText(QString::fromLocal8Bit("取消添加"));
			ui.widget_UsersChoice_2->setVisible(0);
			ui.pB_Users_Check->setText(QString::fromLocal8Bit("查看权限"));
		}
		else if (ui.pB_Users_Add->text() == QString::fromLocal8Bit("取消添加"))
		{
			ui.widget_UsersChoice->setVisible(0);
			ui.pB_Users_Add->setText(QString::fromLocal8Bit("添加"));
		}
		});
	connect(ui.treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onTreeItemChanged(QTreeWidgetItem*)));
	connect(ui.treeWidget_2, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onTreeItemChanged(QTreeWidgetItem*)));
	init();//添加部分
	checkPermission();//查看权限部分

	connect(ui.pB_Users_Check, &QPushButton::clicked, [=]() {
		if (ui.pB_Users_Check->text() == QString::fromLocal8Bit("查看权限"))
		{
			ui.widget_UsersChoice_2->setVisible(1);
			ui.pB_Users_Check->setText(QString::fromLocal8Bit("取消查看"));
			ui.widget_UsersChoice->setVisible(0);
			ui.pB_Users_Add->setText(QString::fromLocal8Bit("添加"));
		}
		else if (ui.pB_Users_Check->text() == QString::fromLocal8Bit("取消查看"))
		{
			ui.widget_UsersChoice_2->setVisible(0);
			ui.pB_Users_Check->setText(QString::fromLocal8Bit("查看权限"));
		}
		});
	connect(ui.comboBox, SIGNAL(currentTextChanged(const QString & str)), this, SLOT(updateCheckPermission(const QString & str)));
	connect(ui.comboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, QOverload<const QString&>::of(&QtCameraSet::updateCheckPermission));

	init();
	connect(ui.tabWidget_Users, SIGNAL(currentChanged(int)), this, SLOT(btn_Enabled(int)));
	if (ui.tabWidget_Users->currentIndex() == 0)
	{
		ui.pB_Users_Add->setEnabled(false);
		ui.pB_Users_Delete->setEnabled(false);
		ui.pB_Users_Change->setEnabled(false);

	}
	else {
		ui.pB_Users_Add->setEnabled(true);
		ui.pB_Users_Delete->setEnabled(true);
		ui.pB_Users_Change->setEnabled(true);
	}
	connect(ui.pB_Users_Apply, &QPushButton::clicked, [=]() {
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("请退出本检测系统重新登录即可！"), 2000);
		//levelOut->show();
		});

	connect(ui.pB_Next, &QPushButton::clicked, [=]() {showMsgBox(QMessageBox::Question, "大哥好", "能不能给我一首歌的时间去完成该功能？", "可以", "没问题"); });
	//Users部分的信号与槽↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

	bool b = connect(this, SIGNAL(SHOWEVERYPLCVALUE(DataToPC_typ)), this, SLOT(showEveryValue(DataToPC_typ)));
	//全局

	b_changeCamera = false;
	b_autoAutoChange = false;
	int cou = g_vectorCamera.size();//相机参数vector
	QFont font;
	font = ui.tabWidget_CameraVec->font();//左下角白框的字体——[SimSun,21]
	font.setPointSize(21);
	if (0 == cou)
	{
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("严重错误！未连接相机，请连接相机后重试！"), 2000);
		//levelOut->show();
		return;
	}

	int h = ui.gB_ShowArea->height() / cou;//宽度
	int w = ui.gB_ShowArea->width() / cou;//高度
	int n = font.pointSize();//字体大小
	for (int i = 0; i < cou; i++)//新建和相机数一样的
	{
		QLabel* label;
		label = new QLabel(ui.gB_ShowArea);//在最上面图片显示区域新建label
		label->setObjectName(QString::fromUtf8("label") + QString::number(i));//eg:一个相机建一个label0，两个建label0,label1，以此类推
		label->setGeometry(QRect(0, 0 + h * i, ui.gB_ShowArea->width(), h));//大小区域为：从屏幕图片显示区（0，h*i）位置开始，显示一个gB_ShowArea->width()*h的界面（宽50，高25）
		label->setFrameShape(QFrame::Box);//设置外边框
		bool b = connect(this, SIGNAL(STARTGRABBING(int)), m_MultGetThread[i], SLOT(ThreadGetImage(int)));//若相机调试发出信号，多线程开始取图
		m_MultGetThread[i]->SetDirectShowDlg(label);//m_LabelShow = label

		QWidget* tab = new QWidget();//新建tab
		tab->setFont(font);//设置tab字体
		tab->setObjectName(QString::fromUtf8("tab_") + g_vectorCamera[i]->c_CameraName);//tab_23170685
		ui.tabWidget_CameraVec->addTab(tab, g_vectorCamera[i]->c_CameraName);//将tab添加到左下角tabwidget boject name:tab_23170685 tttle:23170685
		tableWidget = new QTableWidget(tab);//tab下面加tablewidget
		tableWidget->setObjectName(QString::fromUtf8("tableWidget_") + g_vectorCamera[i]->c_CameraName);//tableWidget_23170685
		int tw = tab->width();
		int th = tab->height();
		tableWidget->setGeometry(QRect(9, 9, tab->height() - 50, tab->width() - 40));//设置widget尺寸 黑边是边界

		QStringList strlist;
		strlist << QString::fromLocal8Bit("参数") << QString::fromLocal8Bit("数值");
		tableWidget->setColumnCount(2);//两列
		tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分填充表头
		tableWidget->verticalHeader()->setDefaultSectionSize(20);//默认行高20
		tableWidget->setHorizontalHeaderLabels(strlist);//水平表头参数、数值
		tableWidget->horizontalHeader()->setVisible(true);//表头可见
		font = tableWidget->horizontalHeader()->font();//表头字体
		font.setPointSize(18);//字号
		tableWidget->horizontalHeader()->setFont(font);//设置表头字体
		font.setPointSize(16);//字号
		tableWidget->setFont(font);//设置内容字体

		initCameraSetTableWidget(tableWidget);//初始化tabwidget

		connect(tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(onCameraCellChange(int, int)));//int-row,int-column 伴有键盘取消操作
		connect(tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onCameraKeyboardOut(int, int)));//int-row,int-column
	}

	
}

QtCameraSet::~QtCameraSet()
{
	StopGrab();
	ui.pb_StReadPLC->setChecked(false);//读取按钮弹起
	QThread::msleep(100);
	for (int i = 0; i < g_vectorCamera.size(); i++)
	{
		m_MultGetThread[i]->SetDirectShowDlg(nullptr);
	}
	if (b_changeCamera)//参数有更改的话，下面显示提示框
	{
		if (QMessageBox::Yes == showMsgBox(QMessageBox::Warning, "修改确认", "参数已修改，是否保存？", "保存", "取消"));
		{
			SaveSetParam();//退出询问是否保存参数
		}
	}
	disconnect(this, SIGNAL(SHOWEVERYPLCVALUE(DataToPC_typ)), this, SLOT(showEveryValue(DataToPC_typ)));
}

void QtCameraSet::initCameraSetTableWidget(QTableWidget* table)//初始化左下角部分
{
	QString title = table->objectName();//title是table的对象名 tableWdiget_23170685
	int i = 0;
	for (; i < g_vectorCamera.size(); i++)
	{
		if (title.contains(g_vectorCamera[i]->c_CameraName))//title包含相机名 23170685
		{
			break;
		}
	}
	if (i < g_vectorCamera.size())
	{
		int currentcolumn = table->rowCount();//行数
		table->insertRow(currentcolumn);//插入行
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("触发源")));//0列设置
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));//单元格不可编辑
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));//单元格不可选择
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->TriggerBy)));//1列设置

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("曝光时长")));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->i_ExpouseTime)));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("调用算法")));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString(g_vectorCamera[i]->c_AlgName)));
		table->item(currentcolumn, 1)->setFlags(table->item(currentcolumn, 1)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 1)->setFlags(table->item(currentcolumn, 1)->flags() & (~Qt::ItemIsSelectable));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("连续触发帧率")));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->i_LineRateHZ)));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("ROI宽度")));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->i_imgWidth)));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("ROI高度")));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->i_imgHeight)));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("ROI_X偏移量")));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->i_OffsetX)));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("ROI_Y偏移量")));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->i_OffsetY)));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("存图标识符")));//存图标识符，-1，NG存图；0，全部保存；1，OK存图
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->i_SaveOKorNG)));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("存图循环标识符"))); //存图循环标识符，0，全部存图； > 0，张数内循环；OK, NG图像进行单独计数。
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(g_vectorCamera[i]->i_SaveLoop)));

		currentcolumn = table->rowCount();
		table->insertRow(currentcolumn);
		table->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("相机序号")));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		table->item(currentcolumn, 0)->setFlags(table->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		table->setItem(currentcolumn, 1, new QTableWidgetItem(QString(g_vectorCamera[i]->c_CameraSign).mid(3)));
	}
}

void QtCameraSet::initListWidgetofModel()//模型listwidget初始化显示，遍历文件夹，找到模板名称并显示
{
	QFont font;
	font = ui.tabWidget_CameraVec->font();//左下角白框的字体——[SimSun,21]
	font.setPointSize(18);
	ui.listWidget->setFont(font);

	QSettings readDefaultModel(AppPath + "\\ModelFile\\ProgramSet.ini", QSettings::IniFormat);
	QString defaultModel = readDefaultModel.value("ProgramSetting/DefaultModel", "testA").toString();
	ui.lb_DefaultName->setText(defaultModel);//更新lable 4-4
	QDir dir(AppPath + "\\ModelFile");
	if (!dir.exists())//不存在，退出
	{
		return;
	}
	dir.setFilter(QDir::Dirs);//筛选目录
	QFileInfoList list = dir.entryInfoList();//文件信息list

	int file_count = list.count();
	if (file_count <= 0)
	{
		return;
	}
	QStringList string_list;
	int i, j = 1;//j用于标记是否存在默认模板
	for (i = 0; i < file_count; i++)
	{
		QFileInfo file_info = list.at(i);
		QString folderName = file_info.fileName();
		string_list.append(folderName);
	}
	string_list.removeAt(0);//去掉.
	string_list.removeAt(0);//去掉..
	ui.listWidget->addItems(string_list);
	//显示模板👇👇当前应用模板

	for (i = 0; i < string_list.count(); i++)
	{
		if (string_list.at(i) == g_qModelName)//找到的默认模板和ini设置的一致
		{
			ui.listWidget->setCurrentRow(i);
			j = 0;//如果存在置为0
		}
	}
	if (j) {
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("严重错误！不存在初始模板！"), 2000);
		//levelOut->show();
	}

}

bool QtCameraSet::compareModels()//哈希码比较两个文件
{
	QString strA1 = AppPath + "/ModelFile/" + g_qModelName + "/CameraConfig.ini";//源目录文件1
	QString strB1 = AppPath + "/DefaultModel" + "/CameraConfig.ini";//当前应用目录文件1
	QString strA2 = AppPath + "/ModelFile/" + g_qModelName + "/CheckParam.ini";//源目录文件2
	QString strB2 = AppPath + "/DefaultModel" + "/CheckParam.ini";//当前应用目录文件2
	QFile theFile1(strA1);
	theFile1.open(QIODevice::ReadOnly);
	QByteArray ba = QCryptographicHash::hash(theFile1.readAll(), QCryptographicHash::Md5);
	theFile1.close();
	qDebug() << ba.toHex().constData();

	QFile theFile2(strB1);
	theFile2.open(QIODevice::ReadOnly);
	QByteArray bb = QCryptographicHash::hash(theFile2.readAll(), QCryptographicHash::Md5);
	theFile2.close();
	qDebug() << bb.toHex().constData();

	QFile theFile3(strA2);
	theFile3.open(QIODevice::ReadOnly);
	QByteArray bc = QCryptographicHash::hash(theFile3.readAll(), QCryptographicHash::Md5);
	theFile3.close();
	qDebug() << bc.toHex().constData();

	QFile theFile4(strB2);
	theFile4.open(QIODevice::ReadOnly);
	QByteArray bd = QCryptographicHash::hash(theFile4.readAll(), QCryptographicHash::Md5);
	theFile4.close();
	qDebug() << bd.toHex().constData();
	if (ba == bb && bc == bd)
	{
		return true;
	}

	else
	{
		return false;
	}


}

void QtCameraSet::modelApply()//应用按钮有关
{
	QString selectedModel = ui.listWidget->currentItem()->text();

	if (g_qModelName == selectedModel)//模板名相同
	{
		if (compareModels())//同时内容也相同
		{
			//levelOut = new WindowOut;
			//levelOut->getString(QString::fromLocal8Bit("当前模板数据已是最新！无需点击应用"), 2000);
			//levelOut->show();
			return;
		}
		else//内容有变化
		{
			int ret = showMsgBox(QMessageBox::Question, "更新确认", "是否确认更新当前正在使用的模板?", "确认", "取消");
			if (ret == QMessageBox::Yes)
			{
				//将默认文件覆盖到之前应用的模板👇👇👇
				QString modelFilePath = AppPath + "/ModelFile/" + g_qModelName;
				QString defaultModelFilePath = AppPath + "/DefaultModel";
				copyDirectoryFiles(defaultModelFilePath, modelFilePath, true);//true代表覆盖已有的
				//levelOut = new WindowOut;
				//levelOut->getString(QString::fromLocal8Bit("当前模板已更新至最新！"), 2000);
				//levelOut->show();
				return;
			}
		}

	}
	else//模板名不同
	{
		if (compareModels())//但是之前的两个相同的模板名里的内容是相同的，就不用去管它
		{
			int ret = showMsgBox(QMessageBox::Question, "确认提示", "是否应用为当前模板？", "确认", "取消");
			if (QMessageBox::Yes == ret)
			{
				QString modelFilePath = AppPath + "/ModelFile/" + g_qModelName;
				QString defaultModelFilePath = AppPath + "/DefaultModel";

				//将当前所选模板复制到默认文件夹，并将currentModel更新👇👇👇
				//deleteDir(defaultModelFilePath);
				modelFilePath = AppPath + "/ModelFile/" + selectedModel;
				copyDirectoryFiles(modelFilePath, defaultModelFilePath, true);
				g_qModelName = selectedModel;////将之后的应用放入当前model

				QSettings Dir(AppPath + "\\ModelFile\\ProgramSet.ini", QSettings::IniFormat);//找到文件
				Dir.setValue("ProgramSetting/LastModel", selectedModel);//写当前模板

				ui.lb_AppliedName->setText(g_qModelName);//更新lable 4-2
				QString defaultModelValue = Dir.value("ProgramSetting/DefaultModel", "zxc").toString();

				if (selectedModel != defaultModelValue)//看当前的是不是默认模板
				{
					int msgres = showMsgBox(QMessageBox::Question, "确认提示", "是否设置为默认模板？", "是", "否");
					if (QMessageBox::Yes == msgres)
					{
						Dir.setValue("ProgramSetting/DefaultModel", selectedModel);//写默认模板
						ui.lb_DefaultName->setText(selectedModel);//更新lable 4-1
					}
				}
				//levelOut = new WindowOut;
				//levelOut->getString(QString::fromLocal8Bit("设置已完成！"), 2000);
				//levelOut->show();
				return;
			}
		}
		else
		{
			int ret = showMsgBox(QMessageBox::Question, "确认提示", "上一个应用的模板数据未保存，需要保存还是丢弃？", "保存", "丢弃");
			if (ret == QMessageBox::Yes)
			{
				//将默认文件覆盖到之前应用的模板👇👇👇
				QString modelFilePath = AppPath + "/ModelFile/" + g_qModelName;
				QString defaultModelFilePath = AppPath + "/DefaultModel";
				copyDirectoryFiles(defaultModelFilePath, modelFilePath, true);//true代表覆盖已有的
				//levelOut = new WindowOut;
				//levelOut->getString(QString::fromLocal8Bit("上一个应用的模板数据已更新至最新！"), 2000);
				//levelOut->show();

				return;
			}
			else
			{
				QString modelFilePath = AppPath + "/ModelFile/" + g_qModelName;
				QString defaultModelFilePath = AppPath + "/DefaultModel";
				copyDirectoryFiles(modelFilePath, defaultModelFilePath, true);//true代表覆盖已有的
				//levelOut = new WindowOut;
				//levelOut->getString(QString::fromLocal8Bit("上一个应用的模板数据已更新至最新！"), 2000);
				//levelOut->show();
			}
			int res = showMsgBox(QMessageBox::Question, "确认提示", "是否应用选中模板为当前模板？", "确认", "取消");
			if (ret == QMessageBox::Yes)
			{
				QString modelFilePath = AppPath + "/ModelFile/" + g_qModelName;
				QString defaultModelFilePath = AppPath + "/DefaultModel";

				//将当前所选模板复制到默认文件夹，并将currentModel更新👇👇👇
				//deleteDir(defaultModelFilePath);
				modelFilePath = AppPath + "/ModelFile/" + selectedModel;
				copyDirectoryFiles(modelFilePath, defaultModelFilePath, true);
				g_qModelName = selectedModel;////将之后的应用放入当前model

				QSettings Dir(AppPath + "\\ModelFile\\ProgramSet.ini", QSettings::IniFormat);//找到文件
				Dir.setValue("ProgramSetting/LastModel", selectedModel);//写当前模板

				ui.lb_AppliedName->setText(g_qModelName);//更新lable 4-5

				QString defaultModelValue = Dir.value("ProgramSetting/DefaultModel", "zxc").toString();

				if (selectedModel != defaultModelValue)//看当前的是不是默认模板
				{
					int msgres = showMsgBox(QMessageBox::Question, "确认提示", "是否设置为默认模板？", "是", "否");
					if (QMessageBox::Yes == msgres)
					{
						Dir.setValue("ProgramSetting/DefaultModel", selectedModel);//写默认模板
						ui.lb_DefaultName->setText(selectedModel);//更新lable 4-6
					}
				}
				//levelOut = new WindowOut;
				//levelOut->getString(QString::fromLocal8Bit("设置已完成！"), 2000);
				//levelOut->show();
				return;

			}
		}
	}
}

bool QtCameraSet::SaveSetParam()
{
	QSettings configIniWrite(AppPath + "\\ModelFile\\" + g_qModelName + "\\CameraConfig.ini", QSettings::IniFormat);
	for (int i = 0; i < g_vectorCamera.size(); i++)
	{
		QString cam = g_vectorCamera[i]->c_CameraName;
		configIniWrite.setValue(cam + "/TriggerBy", g_vectorCamera[i]->TriggerBy);//将修改的参数存入ini，下同
		configIniWrite.setValue(cam + "/ExpouseTime", g_vectorCamera[i]->i_ExpouseTime);
		configIniWrite.setValue(cam + "/LineRateHZ", g_vectorCamera[i]->i_LineRateHZ);
		configIniWrite.setValue(cam + "/imgWidth", g_vectorCamera[i]->i_imgWidth);
		configIniWrite.setValue(cam + "/imgHeight", g_vectorCamera[i]->i_imgHeight);
		configIniWrite.setValue(cam + "/OffsetX", g_vectorCamera[i]->i_OffsetX);
		configIniWrite.setValue(cam + "/OffsetY", g_vectorCamera[i]->i_OffsetY);
		configIniWrite.setValue(cam + "/SaveOKorNG", g_vectorCamera[i]->i_SaveOKorNG);
		configIniWrite.setValue(cam + "/SaveLoop", g_vectorCamera[i]->i_SaveLoop);
		configIniWrite.setValue(cam + "/CameraSign", g_vectorCamera[i]->c_CameraSign);
	}
	return false;
}


void QtCameraSet::onShowAlgSet()//算法设置
{
	QString str = ui.tabWidget_CameraVec->tabText(ui.tabWidget_CameraVec->currentIndex());//当前选中的相机

	CBaseCheckAlg* _tempalg = EnsureAlg(str);
	if (_tempalg != nullptr)//算法指针不为空
	{
		QRect s = this->geometry();//
		if (_tempalg->ShowParamDlg(this, true) == 1)//参数值改了时为1
		{
			//拷回原来的目录👇👇👇
			int ret = showMsgBox(QMessageBox::Information, "模板参数修改", "默认模板参数已修改\n是否保存？", "是", "否");
			if (ret == QMessageBox::No)//不保存
			{
				return;
			}
			else//保存
			{
				QSettings readDefaultModel(AppPath + "\\ModelFile\\ProgramSet.ini", QSettings::IniFormat);
				QString defaultModel = readDefaultModel.value("ProgramSetting/DefaultModel", "testA").toString();
				QString modelFilePath = AppPath + "/ModelFile/" + defaultModel;//默认模板源目录
				QString defaultModelFilePath = AppPath + "/DefaultModel";//默认模板目录，开系统有，关系统删
				copyDirectoryFiles(defaultModelFilePath, modelFilePath, true);//算法改后拷回源目录
				//levelOut = new WindowOut;
				//levelOut->getString(QString::fromLocal8Bit("默认模板参数已保存！"), 2000);
				//levelOut->show();
			}

		}
	}

}

void QtCameraSet::onContinueGrab(bool b)
{
	QString str = ui.tabWidget_CameraVec->tabText(ui.tabWidget_CameraVec->currentIndex());//标签索引以0开始，该标签上的text赋值给str
	int i = 0;
	for (; i < g_vectorCamera.size(); i++)
	{
		if (str == g_vectorCamera[i]->c_CameraName)
		{
			break;//如果名字一样跳出for循环
		}
	}
	if (i == g_vectorCamera.size())
	{
		return;//未找到相同的相机return掉
	}
	else
	{
		if (b)//由信号传递而来，按下是1，再按下是0，以此循环，1执行这一段
		{
			ui.pB_StartGrab->setChecked(false);//将右侧开始测试按钮复位
			emit STARTGRABBING(i);//发送取图信号，连接多线程槽函数
			QThread::msleep(10);
			SetTriggerSoft(i);//设置连续采集
		}
		else//0执行这一段
		{
#ifdef BASLER
			g_vectorCamera[i]->cb_Camera.StopGrabbing();//停止取图
#endif
#ifdef DAHENG
		//发送停采命令
			CGXFeatureControlPointer m_objFeatureControlPtr = g_vectorCamera[i]->m_objDevicePtr->GetRemoteFeatureControl();
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			g_vectorCamera[i]->m_objStreamPtr->StopGrab();

			//注销采集回调函数
			g_vectorCamera[i]->m_objStreamPtr->UnregisterCaptureCallback();
#endif
		}
	}


}

//should change with initTableWidget function as the same time
void QtCameraSet::onCameraCellChange(int r, int c)//tableWidget_camera参数的变化，r-行 c-列//每次数值变化会执行一次该行
{
	((QTableWidget*)sender())->blockSignals(true);

	SWITCHOSK();
	if (b_autoAutoChange)//这个不知道干啥用的，可能类似于blockSignals
	{
		b_autoAutoChange = false;
		((QTableWidget*)sender())->blockSignals(false);
		return;
	}
	QString sendername = ((QTableWidget*)sender())->objectName();//改变的对象object名称
	int i = 0;
	for (; i < g_vectorCamera.size(); i++)
	{
		if (sendername.contains(g_vectorCamera[i]->c_CameraName))
		{
			break;
		}
	}
#ifdef DAHENG
	try
	{
		//发送停采命令
		CGXFeatureControlPointer m_objFeatureControlPtr = g_vectorCamera[i]->m_objDevicePtr->GetRemoteFeatureControl();
		if (r == 4 || r == 5 || r == 6 || r == 7)
		{
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			g_vectorCamera[i]->m_objStreamPtr->StopGrab();

			//注销采集回调函数
			g_vectorCamera[i]->m_objStreamPtr->UnregisterCaptureCallback();
		}
#endif // DAHENG

		if (i <= g_vectorCamera.size())
		{
#ifdef BASLER
			if (!g_vectorCamera[i]->cb_Camera.IsOpen())
			{
				g_vectorCamera[i]->cb_Camera.Open();
			}
#endif
			switch (r)
			{
			case 0:
			{
#ifdef BASLER
				if (g_vectorCamera[i]->cb_Camera.IsOpen())
#endif
				{
					int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();

					int valueBefore = g_vectorCamera[i]->TriggerBy;
					if (!((values == -1) || (values == 0 && IsNumber(((QTableWidget*)sender())->item(r, c)->text())) || (values == 1) || (values == 2)))
					{
						((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
						//levelOut = new WindowOut;
						//levelOut->getString(QString::fromLocal8Bit("请输入-1到2之间的整数"), 2000);
						//levelOut->show();
						break;
					}
					g_vectorCamera[i]->TriggerBy = values;
					switch (values)
					{
					case -1:
					{
						daily_logger->info("ThreadGetImage No.{} SetTriggerSoft", values);
						SetTriggerSoft(i);
						break;
					}
					case 0:
					case 1:
					case 2:
					{
						daily_logger->info("ThreadGetImage No.{} SetTriggerLine1", values);
						SetTriggerLine(i, r);
						break;
					}
					default:
						break;
					}
					b_changeCamera = true;
				}
				break;
			}
			case 1:
			{
				//expouse time
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
				int valueBefore = g_vectorCamera[i]->i_ExpouseTime;
				if (!((values >= 29) && (values <= 100000)))
				{
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("请输入29到100000之间的整数"), 2000);//(basler相机本身支持到29-1000，0000) 大恒20-100，0000了通用就这样设置
					//levelOut->show();
					break;
				}
				g_vectorCamera[i]->i_ExpouseTime = values;
				b_changeCamera = true;
#ifdef BASLER
				if (g_vectorCamera[i]->cb_Camera.IsOpen())
				{
					g_vectorCamera[i]->cb_Camera.ExposureTime.SetValue(values);
				}
#endif
#ifdef DAHENG
				m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(values);
#endif
				break;
			}
			case 2:
			{
				break;
			}
			case 3:
			{//FPS
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();//basler支持0.00288到1000000,大恒是0.8开始到10000，为了通用就如下这样设置
				int valueBefore = g_vectorCamera[i]->i_LineRateHZ;
				if (!((values >= 1) && (values <= 10000)))
				{
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("请输入1到10000之间的整数"), 2000);
					//levelOut->show();
					break;
				}
				g_vectorCamera[i]->i_LineRateHZ = values;
				b_changeCamera = true;
#ifdef BASLER
				if (g_vectorCamera[i]->cb_Camera.IsOpen())
				{
					g_vectorCamera[i]->cb_Camera.AcquisitionFrameRateEnable.SetValue(true);
					g_vectorCamera[i]->cb_Camera.AcquisitionFrameRate.SetValue(values);
				}
#endif
#ifdef DAHENG
				m_objFeatureControlPtr->GetEnumFeature("AcquisitionFrameRateMode")->SetValue("On");
				m_objFeatureControlPtr->GetFloatFeature("AcquisitionFrameRate")->SetValue(values);
#endif
				break;
			}
			case 4:
			{
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
				int valueBefore = g_vectorCamera[i]->i_imgWidth;
#ifdef BASLER
				if ((values < 4) || (values > 2464) || (values % 4 != 0))
				{

					//levelOut.getString(QString::fromLocal8Bit("请输入4到2464之间（包含）,\n且增量为4的整数"));
					//levelOut.show();
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					break;
				}
				int valueOffset = g_vectorCamera[i]->i_OffsetX;
				if ((values + valueOffset) > 2464)
				{

					//levelOut.getString(QString::fromLocal8Bit("由于ROI宽度与ROI_X偏移量之和大于2464，\n已将ROI_X偏移量设置为0，如有需要请重新设置"));
					//levelOut.show();
					((QTableWidget*)sender())->item(6, 1)->setText(QString::number(0));
				}
				g_vectorCamera[i]->i_imgWidth = values;
				b_changeCamera = true;
#endif
#ifdef DAHENG
				if (!((values >= 64) && (values <= 1280)))
				{
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("请输入64（包含）到1280（包含）之间的整数"), 2000);
					//levelOut->show();
					break;
				}
				values = values / 16 * 16;
				int valueOffset = g_vectorCamera[i]->i_OffsetX;
				if (!((values + valueOffset) <= 1280))
				{
					((QTableWidget*)sender())->item(6, 1)->setText(QString::number(0));
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("由于ROI宽度与ROI_X偏移量之和大于1280，\n已将ROI_X偏移量设置为0，如有需要请重新设置"), 2000);
					//levelOut->show();

				}
				((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
				m_objFeatureControlPtr->GetIntFeature("Width")->SetValue(values);
				g_vectorCamera[i]->i_imgWidth = values;
				b_changeCamera = true;

#endif
				break;
			}
			case 5:
			{
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
				int valueBefore = g_vectorCamera[i]->i_imgHeight;
#ifdef BASLER
				if ((values < 2) || (values > 2056) || (values % 2 != 0))
				{
					//levelOut.getString(QString::fromLocal8Bit("请输入2到2056之间（包含）,\n且增量为2的整数"));
					//levelOut.show();
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					break;
				}
				int valueOffset = g_vectorCamera[i]->i_OffsetY;
				if ((values + valueOffset) > 2056)
				{
					//levelOut.getString(QString::fromLocal8Bit("由于ROI高度与ROI_Y偏移量之和大于2056，\n已将ROI_Y偏移量设置为0，如有需要请重新设置"));
					//levelOut.show();
					((QTableWidget*)sender())->item(7, 1)->setText(QString::number(0));
				}
				g_vectorCamera[i]->i_imgHeight = values;
				b_changeCamera = true;
#endif
#ifdef DAHENG
				if (!((values >= 64) && (values <= 1024)))
				{
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("请输入64（包含）到1024（包含）之间的整数"), 2000);
					//levelOut->show();
					break;
				}
				values = values / 2 * 2;
				int valueOffset = g_vectorCamera[i]->i_OffsetY;
				if (!((values + valueOffset) <= 1024))
				{
					((QTableWidget*)sender())->item(7, 1)->setText(QString::number(0));
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("由于ROI高度与ROI_Y偏移量之和大于1024，\n已将ROI_Y偏移量设置为0，如有需要请重新设置"), 2000);
					//levelOut->show();
				}
				((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
				m_objFeatureControlPtr->GetIntFeature("Height")->SetValue(values);
				g_vectorCamera[i]->i_imgHeight = values;
				b_changeCamera = true;

#endif
				break;
			}
			case 6:
			{
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
				int valueBefore = g_vectorCamera[i]->i_OffsetX;
				int valueImgWidth = g_vectorCamera[i]->i_imgWidth;
#ifdef BASLER
				if ((values + valueImgWidth < 4) || (values + valueImgWidth > 2464) || (values % 4 != 0) || values < 0 || (!IsNumber(((QTableWidget*)sender())->item(r, c)->text())))
				{
					//levelOut.getString(QString::fromLocal8Bit("ROI宽度与ROI_X偏移量之和须介于4和2464之间（包含），\n且增量为4的非负数，请重新输入"));
					//levelOut.show();
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					break;
				}
				g_vectorCamera[i]->i_OffsetX = values;
				b_changeCamera = true;
#endif
#ifdef DAHENG
				if (!((values + valueImgWidth >= 64) && (values + valueImgWidth <= 1280) && values >= 0 && (IsNumber(((QTableWidget*)sender())->item(r, c)->text()))))
				{
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("ROI宽度与ROI_X偏移量之和须介于64（包含）和\n1280（包含）之间，且为非负数，请重新输入"), 2000);
					//levelOut->show();
					break;
				}
				values = values / 16 * 16;
				((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
				m_objFeatureControlPtr->GetIntFeature("OffsetX")->SetValue(values);
				g_vectorCamera[i]->i_OffsetX = values;
				b_changeCamera = true;

#endif
				break;
			}
			case 7:
			{
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
				int valueBefore = g_vectorCamera[i]->i_OffsetY;
				int valueImgHeight = g_vectorCamera[i]->i_imgHeight;
#ifdef BASLER
				if ((values + valueImgHeight < 2) || (values + valueImgHeight > 2056) || (values % 2 != 0) || values < 0 || (!IsNumber(((QTableWidget*)sender())->item(r, c)->text())))
				{
					//levelOut.getString(QString::fromLocal8Bit("ROI高度与ROI_Y偏移量之和须介于2和2056之间（包含），\n且增量为2的非负数，请重新输入"));
					//levelOut.show();
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化  baslor should change as below
					break;
				}
				g_vectorCamera[i]->i_OffsetY = values;
				b_changeCamera = true;
#endif
#ifdef DAHENG
				if (!((values + valueImgHeight >= 64) && (values + valueImgHeight <= 1024) && values >= 0 && (IsNumber(((QTableWidget*)sender())->item(r, c)->text()))))
				{
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("ROI高度与ROI_Y偏移量之和须介于64（包含）和\n1024（包含）之间的非负数，请重新输入"), 2000);
					//levelOut->show();
					break;
				}
				values = values / 2 * 2;
				((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
				m_objFeatureControlPtr->GetIntFeature("OffsetY")->SetValue(values);
				g_vectorCamera[i]->i_OffsetY = values;
				b_changeCamera = true;

#endif
				break;
			}
			case 8:
			{
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
				int valueBefore = g_vectorCamera[i]->i_SaveOKorNG;
				if (!((values == -1) || (values == 0 && (IsNumber(((QTableWidget*)sender())->item(r, c)->text()))) || (values == 1)))
				{
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("请输入-1（包含）到1（包含）之间的整数"), 2000);
					//levelOut->show();
					break;
				}
				g_vectorCamera[i]->i_SaveOKorNG = values;
				b_changeCamera = true;
				break;
			}
			case 9:
			{
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
				int valueBefore = g_vectorCamera[i]->i_SaveLoop;
				if (!(values >= 0 && (IsNumber(((QTableWidget*)sender())->item(r, c)->text()))))
				{
					((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
					//levelOut = new WindowOut;
					//levelOut->getString(QString::fromLocal8Bit("请输入非负整数"), 2000);
					//levelOut->show();
					break;
				}
				g_vectorCamera[i]->i_SaveLoop = values;
				b_changeCamera = true;
				break;
			}

			case 10:
			{
				int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
				int valueBefore = QString(g_vectorCamera[i]->c_CameraSign).mid(3).toInt();
				for (int j = 0; j < g_vectorCamera.size(); j++)
				{
					QString str = ((QTableWidget*)sender())->objectName().mid(12);
					if (str != g_vectorCamera[j]->c_CameraName
						&& values == QString(g_vectorCamera[j]->c_CameraSign).mid(3).toInt())
					{
						((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
						//levelOut = new WindowOut;
						//levelOut->getString(QString::fromLocal8Bit("序号冲突！\n请修改相机序号为") + QString(g_vectorCamera[j]->c_CameraName) + QString::fromLocal8Bit("的相机序号再修改该值"), 2000);
						//levelOut->show();
						break;

					}
					if (!(IsNumber(((QTableWidget*)sender())->item(r, c)->text())))
					{
						((QTableWidget*)sender())->item(r, c)->setText(QString::number(valueBefore));//取消变化
						//levelOut = new WindowOut;
						//levelOut->getString(QString::fromLocal8Bit("请输入数字！"), 2000);
						//levelOut->show();
						break;

					}
				}

				char src[100], dest[100], dest2[100];//太小会溢出

				strcpy(dest2, "POS");
				strcpy(dest, ((QTableWidget*)sender())->item(r, c)->text().toStdString().c_str());

				strcat(dest2, dest);
				strcpy(g_vectorCamera[i]->c_CameraSign, dest2);
				b_changeCamera = true;
				break;
			}

			default:
				break;
			}
		}
#ifdef DAHENG

		if (r == 4 || r == 5 || r == 6 || r == 7)
		{
			if (ui.pB_StartContinueGrab->isChecked())
			{
				onContinueGrab(true);
			}
			if (ui.pB_StartGrab->isChecked())
			{
				StartGrab(true);
			}
		}
	}
	catch (std::exception & e)
	{
		QString st = e.what();
		return;
	}
#endif // DAHENG
	((QTableWidget*)sender())->blockSignals(false);
}

#ifdef PLCCONNECT

void message_handler(void* context, DataToPC_typ data)
{
	((QtCameraSet*)context)->emit SHOWEVERYPLCVALUE(data);
}


void QtCameraSet::onReadPLCParam(bool b)
{
	if (b)
	{
		g_SocketPLC->set_message_handler(&message_handler, this);
		ui.pb_StReadPLC->setStyleSheet("background: rgb(0,255,0)");
	}
	else
	{
		g_SocketPLC->set_message_handler(nullptr, this);
		ui.pb_StReadPLC->setStyleSheet("");
		ui.pb_StReadPLC->setFont(font);
	}

}
void QtCameraSet::onSendPLCCommand(bool b)
{
	QPushButton* obj = qobject_cast<QPushButton*>(sender());//判断是哪个按钮触发了槽函数
	QString objname = obj->objectName();//获取触发者objectname
	DataFromPC_typ typ;
	typ.Dummy = 0;//占空
	typ.Telegram_typ = 1;//命令报文
	if (obj != nullptr)
	{

		if (b)
		{
			if (objname == "pb_StWritePLC")
			{
				typ = getEveryValue();
				typ.Telegram_typ = 2;
				ui.pb_StWritePLC->setStyleSheet("background: rgb(0,255,0)");
				g_SocketPLC->Communicate_PLC(&typ, nullptr);//此处只发送，不接收
			}//参数报文
			else {
				if (objname.contains("pb_cmdTestKick"))
				{
					int index_cmdkick = objname.mid(14).toInt();//从0开始，14位之后的字符转为int，0-29
					typ.Machine_Cmd.cmdTestKick[index_cmdkick] = 1;//结构体数据赋值
				}
				if (objname == "pb_cmdHome") typ.Machine_Cmd.cmdHome = 1;//结构体数据赋值
				if (objname == "pb_cmdStart") typ.Machine_Cmd.cmdStart = 1;
				if (objname == "pb_cmdStop") typ.Machine_Cmd.cmdStop = 1;
				if (objname == "pb_cmdEStop")
				{
					typ.Machine_Cmd.cmdEStop = 1;//非点动按钮,点动需要到最下面setChecked(false)
					obj->setText(QString::fromLocal8Bit("急停\n按下！"));
				}
				if (objname == "pb_cmdJog")
				{
					typ.Machine_Cmd.cmdJog = 1;
					obj->setText(QString::fromLocal8Bit("运行..."));
				}
				if (objname == "pb_cmdErrorAck") typ.Machine_Cmd.cmdErrorAck = 1;					//报警复位, 1:复位
				if (objname == "pb_cmdResetCounter") typ.Machine_Cmd.cmdResetCounter = 1;				//复位计数变量, 1:复位
				if (objname == "pb_cmdParaSave") typ.Machine_Cmd.cmdParaSave = 1;						//参数保存命令, 1:保存
				if (objname == "pb_cmdParaLoad") typ.Machine_Cmd.cmdParaLoad = 1;						//参数读取命令, 1:读取
				if (objname == "pb_cmdTestFlash0") typ.Machine_Cmd.cmdTestFlash[0] = 1;			//手动闪光, 1:闪光,自动复位
				if (objname == "pb_cmdTestFlash1") typ.Machine_Cmd.cmdTestFlash[1] = 1;
				if (objname == "pb_cmdTestFlash2") typ.Machine_Cmd.cmdTestFlash[2] = 1;

				if (objname == "pb_cmdTestValveUp") typ.Machine_Cmd.cmdTestValveUp = 1;						//手动升降气缸, 1:Push, 2:Back
				if (objname == "pb_cmdTestValveClip") typ.Machine_Cmd.cmdTestValveClip = 1;					//手动夹紧气缸, 1:Push, 2:Back
				if (objname == "pb_cmdTestValveDrop") typ.Machine_Cmd.cmdTestValveDrop = 1;					//手动落囊气缸, 1:Push, 2:Back
				if (objname == "pb_cmdTestInverter") typ.Machine_Cmd.cmdTestInverter = 1;				//手动胶囊料斗启动, 1:Start, 2:Stop
				if (objname == "pb_cmdTestLampRead") typ.Machine_Cmd.cmdTestLampRead = 1;				//手动红灯输出, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestLampYellow") typ.Machine_Cmd.cmdTestLampYellow = 1;					//手动黄灯输出, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestLampGreen") typ.Machine_Cmd.cmdTestLampGreen = 1;					//手动绿灯输出, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestBuzzer") typ.Machine_Cmd.cmdTestBuzzer = 1;						//手动蜂鸣器输出, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestPhoto") typ.Machine_Cmd.cmdTestPhoto = 1;						//手动拍照, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestFlashPhoto") typ.Machine_Cmd.cmdTestFlashPhoto = 1;					//手动闪光加拍照, 1:启动
				if (objname == "pb_cmdTestCapPhoto") typ.Machine_Cmd.cmdTestCapPhoto = 1;				//手动胶囊拍照
				if (objname == "pb_cmdRotateCtl") typ.Machine_Cmd.cmdRotateCtl = 1;						//手动转囊启停
				qDebug() << "setCheck(true)";
			}
		}
		if (!b)
		{
			if (objname == "pb_StWritePLC")
			{
				ui.pb_StWritePLC->setStyleSheet("");
				ui.pb_StWritePLC->setFont(font);
			}
			else {
				if (objname.contains("pb_cmdTestKick"))
				{
					int index_cmdkick = objname.mid(14).toInt();//从0开始，14位之后的字符转为int，0-29
					typ.Machine_Cmd.cmdTestKick[index_cmdkick] = 0;//结构体数据赋值
				}
				if (objname == "pb_cmdHome")typ.Machine_Cmd.cmdHome = 0;
				if (objname == "pb_cmdStart") typ.Machine_Cmd.cmdStart = 0;
				if (objname == "pb_cmdStop") typ.Machine_Cmd.cmdStop = 0;
				if (objname == "pb_cmdEStop")
				{
					typ.Machine_Cmd.cmdEStop = 0;//非点动按钮
					obj->setText(QString::fromLocal8Bit("急停"));
				}
				if (objname == "pb_cmdJog")
				{

					typ.Machine_Cmd.cmdJog = 0; //非点动按钮
					obj->setText(QString::fromLocal8Bit("点动"));
				}
				if (objname == "pb_cmdErrorAck") typ.Machine_Cmd.cmdErrorAck = 0;					//报警复位, 1:复位
				if (objname == "pb_cmdResetCounter") typ.Machine_Cmd.cmdResetCounter = 0;				//复位计数变量, 1:复位
				if (objname == "pb_cmdParaSave") typ.Machine_Cmd.cmdParaSave = 0;						//参数保存命令, 1:保存
				if (objname == "pb_cmdParaLoad") typ.Machine_Cmd.cmdParaLoad = 0;						//参数读取命令, 1:读取
				if (objname == "pb_cmdTestFlash0") typ.Machine_Cmd.cmdTestFlash[0] = 0;			//手动闪光, 1:闪光,自动复位
				if (objname == "pb_cmdTestFlash1") typ.Machine_Cmd.cmdTestFlash[1] = 0;
				if (objname == "pb_cmdTestFlash2") typ.Machine_Cmd.cmdTestFlash[2] = 0;

				if (objname == "pb_cmdTestValveUp") typ.Machine_Cmd.cmdTestValveUp = 0;						//手动升降气缸, 1:Push, 2:Back
				if (objname == "pb_cmdTestValveClip") typ.Machine_Cmd.cmdTestValveClip = 0;					//手动夹紧气缸, 1:Push, 2:Back
				if (objname == "pb_cmdTestValveDrop") typ.Machine_Cmd.cmdTestValveDrop = 0;					//手动落囊气缸, 1:Push, 2:Back
				if (objname == "pb_cmdTestInverter") typ.Machine_Cmd.cmdTestInverter = 0;				//手动胶囊料斗启动, 1:Start, 2:Stop
				if (objname == "pb_cmdTestLampRead") typ.Machine_Cmd.cmdTestLampRead = 0;				//手动红灯输出, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestLampYellow") typ.Machine_Cmd.cmdTestLampYellow = 0;					//手动黄灯输出, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestLampGreen") typ.Machine_Cmd.cmdTestLampGreen = 0;					//手动绿灯输出, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestBuzzer") typ.Machine_Cmd.cmdTestBuzzer = 0;						//手动蜂鸣器输出, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestPhoto") typ.Machine_Cmd.cmdTestPhoto = 0;						//手动拍照, 1:输出 , 2: 复位
				if (objname == "pb_cmdTestFlashPhoto") typ.Machine_Cmd.cmdTestFlashPhoto = 0;					//手动闪光加拍照, 1:启动
				if (objname == "pb_cmdTestCapPhoto") typ.Machine_Cmd.cmdTestCapPhoto = 0;				//手动胶囊拍照
				if (objname == "pb_cmdRotateCtl") typ.Machine_Cmd.cmdRotateCtl = 0;						//手动转囊启停

			}
		}
		if (objname != "pb_StWritePLC")//写入debug两次发送/旨在上面发送一次
		{
			g_SocketPLC->Communicate_PLC(&typ, nullptr);//此处只发送，不接收
		}//参数报文
		

		if (objname == "pb_StWritePLC" || objname.contains("pb_cmdTestKick") || objname == "pb_cmdHome" || objname == "pb_cmdStart"
			|| objname == "pb_cmdStop" || objname == "pb_cmdErrorAck"
			|| objname == "pb_cmdResetCounter" || objname == "pb_cmdParaSave" || objname == "pb_cmdParaLoad"
			//|| objname == "pb_cmdTestFlash0" || objname == "pb_cmdTestFlash1" || objname == "pb_cmdTestFlash2"
			|| objname == "pb_cmdTestValveUp" || objname == "pb_cmdTestValveClip" || objname == "pb_cmdTestValveDrop"
			|| objname == "pb_cmdTestInverter" || objname == "pb_cmdTestLampRead" || objname == "pb_cmdTestLampYellow"
			|| objname == "pb_cmdTestLampGreen" || objname == "pb_cmdTestBuzzer" || objname == "pb_cmdTestPhoto"
			|| objname == "pb_cmdTestFlashPhoto" || objname == "pb_cmdTestCapPhoto" || objname == "pb_cmdRotateCtl")
		{
			obj->setChecked(false);
		}

	}


}
DataFromPC_typ QtCameraSet::getEveryValue()
{
	DataFromPC_typ tmp;
	tmp.Dummy = 0;
	tmp.Telegram_typ = ui.lE_Telegram_typ->text().toInt();

	//系统参数
	tmp.Machine_Para.FeedAxisHomeOffset = QString::number(ui.lE_FeedAxisHomeOffset->text().toFloat() * 100).toInt();//qstring-float-qstring-int
	tmp.Machine_Para.ClipPhase1 = QString::number(ui.lE_ClipPhase1->text().toFloat() * 100).toInt();
	tmp.Machine_Para.ClipPhase2 = QString::number(ui.lE_ClipPhase2->text().toFloat() * 100).toInt();
	tmp.Machine_Para.UpPhase1 = QString::number(ui.lE_UpPhase1->text().toFloat() * 100).toInt();
	tmp.Machine_Para.UpPhase2 = QString::number(ui.lE_UpPhase2->text().toFloat() * 100).toInt();
	tmp.Machine_Para.DropPhase1 = QString::number(ui.lE_DropPhase1->text().toFloat() * 100).toInt();
	tmp.Machine_Para.DropPhase2 = QString::number(ui.lE_DropPhase2->text().toFloat() * 100).toInt();
	tmp.Machine_Para.tClip1 = ui.lE_tClip1->text().toFloat();
	tmp.Machine_Para.tUp1 = ui.lE_tUp1->text().toFloat();
	tmp.Machine_Para.tUp2 = ui.lE_tUp2->text().toFloat();
	tmp.Machine_Para.tDrop1 = ui.lE_tDrop1->text().toFloat();
	tmp.Machine_Para.tDrop2 = ui.lE_tDrop2->text().toFloat();
	tmp.Machine_Para.FeedLength = QString::number(ui.lE_FeedLength->text().toFloat() * 100).toInt();
	tmp.Machine_Para.FlashTime = ui.lE_FlashTime->text().toFloat();
	tmp.Machine_Para.PhotoTime = ui.lE_PhotoTime->text().toFloat();
	tmp.Machine_Para.RejectTime = ui.lE_RejectTime->text().toFloat();
	tmp.Machine_Para.PhotoDelay = ui.lE_PhotoDelay->text().toFloat();
	tmp.Machine_Para.PhotoPhase = QString::number(ui.lE_PhotoPhase->text().toFloat() * 100).toInt();
	tmp.Machine_Para.RejectPhase = QString::number(ui.lE_RejectPhase->text().toFloat() * 100).toInt();
	tmp.Machine_Para.PhotoTimes = ui.lE_PhotoTimes->text().toInt();
	tmp.Machine_Para.RotateSpeed = ui.lE_RotateSpeed->text().toFloat();

	//运行数据
	tmp.Run_Para.RunSpeed = ui.lE_RunSpeed->text().toInt();
	tmp.Run_Para.SysPhase = ui.lE_SysPhase->text().toInt();
	tmp.Run_Para.enPhoto = ui.lE_enPhoto->text().toInt();
	tmp.Run_Para.enReject = ui.lE_enReject->text().toInt();
	tmp.Run_Para.CheckCount = ui.lE_CheckCount->text().toInt();
	tmp.Run_Para.RejectCount = ui.lE_RejectCount->text().toInt();
	tmp.Run_Para.ForceRejectCount = ui.lE_ForceRejectCount->text().toInt();


	//照相处理结果和按钮在上面⬆⬆⬆⬆⬆⬆

	return tmp;
}
void QtCameraSet::showEveryValue(DataToPC_typ data)
{//报文类型
	ui.lE_Telegram_typ->setText(QString::number(data.Telegram_typ));

	//运行数据
	ui.lE_RunSpeed->setText(QString::number(data.ActData.RunSpeed));
	ui.lE_SysPhase->setText(QString::number(data.ActData.SysPhase));
	ui.lE_enPhoto->setText(QString::number(data.ActData.enPhoto));
	ui.lE_enReject->setText(QString::number(data.ActData.enReject));
	ui.lE_CheckCount->setText(QString::number(data.ActData.CheckCount));
	ui.lE_RejectCount->setText(QString::number(data.ActData.RejectCount));
	ui.lE_ForceRejectCount->setText(QString::number(data.ActData.ForceRejectCount));

	//系统状态
	ui.lE_AlarmStatus->setText(QString::number(data.Status.AlarmStatus));
	ui.lE_Alarm16->setText((char*)(data.Status.Alarm));
	ui.lE_ServoErrorNum0->setText(QString::number(data.Status.ServoErrorNum[0]));
	ui.lE_ServoErrorNum1->setText(QString::number(data.Status.ServoErrorNum[1]));
	ui.lE_SysPhase_1->setText(QString::number(data.Status.SysPhase));
	ui.lE_HomeOK->setText(QString::number(data.Status.HomeOK));

	//系统参数
	ui.lE_FeedAxisHomeOffset->setText(QString::number(data.Machine_Para.FeedAxisHomeOffset / 100.0, 'f', 2));//显示到小数点后两位
	ui.lE_ClipPhase1->setText(QString::number(data.Machine_Para.ClipPhase1 / 100.0, 'f', 2));
	ui.lE_ClipPhase2->setText(QString::number(data.Machine_Para.ClipPhase2 / 100.0, 'f', 2));
	ui.lE_UpPhase1->setText(QString::number(data.Machine_Para.UpPhase1 / 100.0, 'f', 2));
	ui.lE_UpPhase2->setText(QString::number(data.Machine_Para.UpPhase2 / 100.0, 'f', 2));
	ui.lE_DropPhase1->setText(QString::number(data.Machine_Para.DropPhase1 / 100.0, 'f', 2));
	ui.lE_DropPhase2->setText(QString::number(data.Machine_Para.DropPhase2 / 100.0, 'f', 2));
	ui.lE_tClip1->setText(QString::number(data.Machine_Para.tClip1));
	ui.lE_tClip2->setText(QString::number(data.Machine_Para.tClip2));
	ui.lE_tUp1->setText(QString::number(data.Machine_Para.tUp1));
	ui.lE_tUp2->setText(QString::number(data.Machine_Para.tUp2));
	ui.lE_tDrop1->setText(QString::number(data.Machine_Para.tDrop1));
	ui.lE_tDrop2->setText(QString::number(data.Machine_Para.tDrop2));
	ui.lE_FeedLength->setText(QString::number(data.Machine_Para.FeedLength / 100.0, 'f', 2));
	ui.lE_FlashTime->setText(QString::number(data.Machine_Para.FlashTime));
	ui.lE_PhotoTime->setText(QString::number(data.Machine_Para.PhotoTime));
	ui.lE_RejectTime->setText(QString::number(data.Machine_Para.RejectTime));
	ui.lE_PhotoDelay->setText(QString::number(data.Machine_Para.PhotoDelay));
	ui.lE_PhotoPhase->setText(QString::number(data.Machine_Para.PhotoPhase / 100.0, 'f', 2));
	ui.lE_RejectPhase->setText(QString::number(data.Machine_Para.RejectPhase / 100.0, 'f', 2));
	ui.lE_PhotoTimes->setText(QString::number(data.Machine_Para.PhotoTimes));
	ui.lE_RotateSpeed->setText(QString::number(data.Machine_Para.RotateSpeed));

	//输入点
	ui.lE_EStop->setText(data.Inputs.EStop ? "1" : "0");
	ui.lE_AxisFeedPosEnd->setText(data.Inputs.AxisFeedPosEnd ? "1" : "0");
	ui.lE_AxisFeedNegEnd->setText(data.Inputs.AxisFeedNegEnd ? "1" : "0");
	ui.lE_AxisFeedHome->setText(data.Inputs.AxisFeedHome ? "1" : "0");
	ui.lE_AirPressChk->setText(data.Inputs.AirPressChk ? "1" : "0");

	//输出点
	ui.lE_Inveter->setText(data.Outputs.Inveter ? "1" : "0");
	ui.lE_ClipValve->setText(data.Outputs.ClipValve ? "1" : "0");
	ui.lE_UpValve->setText(data.Outputs.UpValve ? "1" : "0");
	ui.lE_DropValve->setText(data.Outputs.DropValve ? "1" : "0");
	ui.lE_LampRed->setText(data.Outputs.LampRed ? "1" : "0");
	ui.lE_LampYellow->setText(data.Outputs.LampYellow ? "1" : "0");
	ui.lE_LampGreen->setText(data.Outputs.LampGreen ? "1" : "0");
	ui.lE_Buzzer->setText(data.Outputs.Buzzer ? "1" : "0");
	ui.lE_Photo->setText(data.Outputs.Photo ? "1" : "0");
	ui.lE_Flash0->setText(data.Outputs.Flash[0] ? "1" : "0");
	ui.lE_Flash1->setText(data.Outputs.Flash[1] ? "1" : "0");
	ui.lE_Flash2->setText(data.Outputs.Flash[2] ? "1" : "0");
	ui.lE_Reject0->setText(data.Outputs.Reject[0] ? "1" : "0");
	ui.lE_Reject1->setText(data.Outputs.Reject[1] ? "1" : "0");
	ui.lE_Reject2->setText(data.Outputs.Reject[2] ? "1" : "0");
	ui.lE_Reject3->setText(data.Outputs.Reject[3] ? "1" : "0");
	ui.lE_Reject4->setText(data.Outputs.Reject[4] ? "1" : "0");
	ui.lE_Reject5->setText(data.Outputs.Reject[5] ? "1" : "0");
	ui.lE_Reject6->setText(data.Outputs.Reject[6] ? "1" : "0");
	ui.lE_Reject7->setText(data.Outputs.Reject[7] ? "1" : "0");
	ui.lE_Reject8->setText(data.Outputs.Reject[8] ? "1" : "0");
	ui.lE_Reject9->setText(data.Outputs.Reject[9] ? "1" : "0");
	ui.lE_Reject10->setText(data.Outputs.Reject[10] ? "1" : "0");
	ui.lE_Reject11->setText(data.Outputs.Reject[11] ? "1" : "0");
	ui.lE_Reject12->setText(data.Outputs.Reject[12] ? "1" : "0");
	ui.lE_Reject13->setText(data.Outputs.Reject[13] ? "1" : "0");
	ui.lE_Reject14->setText(data.Outputs.Reject[14] ? "1" : "0");
	ui.lE_Reject15->setText(data.Outputs.Reject[15] ? "1" : "0");
	ui.lE_Reject16->setText(data.Outputs.Reject[16] ? "1" : "0");
	ui.lE_Reject17->setText(data.Outputs.Reject[17] ? "1" : "0");
	ui.lE_Reject18->setText(data.Outputs.Reject[18] ? "1" : "0");
	ui.lE_Reject19->setText(data.Outputs.Reject[19] ? "1" : "0");
	ui.lE_Reject20->setText(data.Outputs.Reject[20] ? "1" : "0");
	ui.lE_Reject21->setText(data.Outputs.Reject[21] ? "1" : "0");
	ui.lE_Reject22->setText(data.Outputs.Reject[22] ? "1" : "0");
	ui.lE_Reject23->setText(data.Outputs.Reject[23] ? "1" : "0");
	ui.lE_Reject24->setText(data.Outputs.Reject[24] ? "1" : "0");
	ui.lE_Reject25->setText(data.Outputs.Reject[25] ? "1" : "0");
	ui.lE_Reject26->setText(data.Outputs.Reject[26] ? "1" : "0");
	ui.lE_Reject27->setText(data.Outputs.Reject[27] ? "1" : "0");
	ui.lE_Reject28->setText(data.Outputs.Reject[28] ? "1" : "0");
	ui.lE_Reject29->setText(data.Outputs.Reject[29] ? "1" : "0");
}

MESSAGE_HANDLER QtCameraSet::ShowFunc(void* context, DataToPC_typ)
{
	return MESSAGE_HANDLER();
}
#endif

//逻辑互锁👇👇
void QtCameraSet::SyncAnotherStartGrab(bool b)//两个StartGrab的东西2-1
{
	if (b)
	{
		ui.pB_StartGrabAnother->setChecked(true);
		ui.pB_StartGrabAnother->setText(QString::fromLocal8Bit("停\n止"));
	}
	else
	{
		ui.pB_StartGrabAnother->setChecked(false);
		ui.pB_StartGrabAnother->setText(QString::fromLocal8Bit("采\n集"));
	}

}
void QtCameraSet::SyncFirstStartGrab(bool b)//两个StartGrab的东西2-2
{
	if (ui.pB_StartGrabAnother->text() == QString::fromLocal8Bit("采\n集"))//抬起状态
	{
		ui.pB_StartGrab->setChecked(true);
	}
	else
	{
		ui.pB_StartGrab->setChecked(false);
	}


}
//逻辑互锁👆👆
void QtCameraSet::StartGrab(bool b)
{
	if (b)
	{
		if (ui.pB_StartContinueGrab->isChecked())
		{
			ui.pB_StartContinueGrab->setChecked(false);
		}
		ui.pB_StartGrab->setText(QString::fromLocal8Bit("停止测试"));

		emit STARTGRABBING(-1);
	}
	else
	{
		StopGrab();
		ui.pB_StartGrab->setText(QString::fromLocal8Bit("采集测试"));

	}

}

void QtCameraSet::StopGrab()
{
	for (int i = 0; i < g_vectorCamera.size(); i++)
	{
	}
}
bool QtCameraSet::copyDirectoryFiles(const QString& fromDir, const QString& toDir, bool coverFileIfExist)//拷贝文件夹/目录功能
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

bool QtCameraSet::deleteDir(const QString& path)//eg：deleteDir(AppPath + "/DefaultModel");//删除文件夹/目录功能
{
	if (path.isEmpty()) {
		return false;
	}
	QDir dir(path);
	if (!dir.exists()) {
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
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

void QtCameraSet::modelDelete()//删除指定模板功能
{
	QString modelPath = ui.listWidget->currentItem()->text();
	if (g_qModelName == modelPath)//正在应用的模板不可删除
	{
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("当前正在应用该模板,无法删除"), 2000);
		//levelOut->show();
		return;
	}
	QSettings configIniRead(AppPath + "\\ModelFile\\ProgramSet.ini", QSettings::IniFormat);
	QString defaultModel = configIniRead.value("ProgramSetting/DefaultModel", "testA").toString();
	if (defaultModel == modelPath)//默认模板不可删除
	{
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("该模板是默认模板,无法删除"), 2000);
		//levelOut->show();
		return;
	}
	int ret = showMsgBox(QMessageBox::Warning, "警告", "模板一经删除无法恢复，是否确认删除？", "确认", "取消");
	if (ret == QMessageBox::Yes)
	{
		deleteDir(AppPath + "/ModelFile/" + modelPath);
		delete ui.listWidget->currentItem();
	}


}
void QtCameraSet::modelAdd()
{
	QString modelPath = ui.listWidget->currentItem()->text();

	char src[100], dest[100], dest2[100];//太小会溢出

	strcpy(dest2, "请选择添加方式: \n复制：通过复制模板 ");
	strcpy(dest, modelPath.toStdString().c_str());
	strcpy(src, " 的参数添加模板\n新建：根据当前采样参数添加新建模板\n取消：取消添加模板操作");

	strcat(dest, src);
	strcat(dest2, dest);
	//👆👆👆👆👆👆

	QMessageBox msg(QMessageBox::Information, QString::fromLocal8Bit("新建方式确认"), QString::fromLocal8Bit(dest2), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	msg.setButtonText(QMessageBox::Yes, QString::fromLocal8Bit("复制"));
	msg.setButtonText(QMessageBox::No, QString::fromLocal8Bit("新建"));
	msg.setButtonText(QMessageBox::Cancel, QString::fromLocal8Bit("取消"));
	msg.setWindowIcon(QIcon("./dr.ico"));
	int res = msg.exec();

	if (res == QMessageBox::Yes)//复制当前并取名
	{
		inputModel = new InputDialog(this);
		inputModel->setModal(true);
		inputModel->move(0, 400);
		inputModel->show();
		return;
	}
	if (res == QMessageBox::No)//新建
	{
		return;
	}
	if (res == QMessageBox::Cancel)//取消
	{
		return;
	}

}

void QtCameraSet::modelChangeName()
{
	QString modelPath = ui.listWidget->currentItem()->text();
	if (g_qModelName == modelPath)//正在应用的模板不可重命名
	{
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("当前正在应用该模板,不可重命名"), 2000);
		//levelOut->show();
		return;
	}
	QSettings configIniRead(AppPath + "\\ModelFile\\ProgramSet.ini", QSettings::IniFormat);
	QString defaultModel = configIniRead.value("ProgramSetting/DefaultModel", "testA").toString();
	if (defaultModel == modelPath)//默认模板不可删除
	{
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("该模板是默认模板,不可重命名"), 2000);
		//levelOut->show();
		return;
	}
	inputModel = new InputDialog(this);
	//connect(inputModel, SIGNAL(toQtCameraSet(QString)), this, SLOT(applyModelNameChanged(QString)));//难点
	inputModel->setModal(true);
	inputModel->move(0, 400);
	inputModel->show();


}


void QtCameraSet::applyNameChange(QString str, int ID)//重命名成功
{
	if (ID == 1)
	{
		QString strNow = AppPath + "\\ModelFile\\" + ui.listWidget->currentItem()->text();//点中的
		QString strShouldBeChanged = AppPath + "\\ModelFile\\" + str;
		std::fstream f;
		f.open(strNow.toStdString().c_str());//修改该目录名称
		rename(strNow.toStdString().c_str(), strShouldBeChanged.toStdString().c_str());//后侧覆盖前侧
		f.close();
		//QFile file(strShouldBeChanged + "/" + ui.listWidget->currentItem()->text() + ".txt");//修改指定文件名称
		//file.rename(strShouldBeChanged + "/" + str+ ".txt");//后侧覆盖前侧
		//找到所有模板名称👇👇
		QDir dir(AppPath + "\\ModelFile");
		dir.setFilter(QDir::Dirs);//筛选目录
		QFileInfoList list = dir.entryInfoList();//文件信息list

		int file_count = list.count();
		QStringList string_list;
		int i;
		for (i = 0; i < file_count; i++)
		{
			QFileInfo file_info = list.at(i);
			QString folderName = file_info.fileName();
			string_list.append(folderName);
		}

		string_list.removeAt(0);//去掉.
		string_list.removeAt(0);//去掉..
		ui.listWidget->clear();
		ui.listWidget->addItems(string_list);
		//显示默认模板👇👇
		for (i = 0; i < string_list.count(); i++)
		{

			if (string_list.at(i) == str)//找到的刚才修改的那个名称
			{
				ui.listWidget->setCurrentRow(i);
			}
		}
	}

	if (ID == 2)
	{
		QString strNow = AppPath + "\\ModelFile\\" + ui.listWidget->currentItem()->text();//点中的
		QString strAdded = AppPath + "\\ModelFile\\" + str;

		copyDirectoryFiles(strNow, strAdded, true);//true代表覆盖已有的,前面拷贝到后面

		QDir dir(AppPath + "\\ModelFile");
		dir.setFilter(QDir::Dirs);//筛选目录
		QFileInfoList list = dir.entryInfoList();//文件信息list

		int file_count = list.count();
		QStringList string_list;
		int i;
		for (i = 0; i < file_count; i++)
		{
			QFileInfo file_info = list.at(i);
			QString folderName = file_info.fileName();
			string_list.append(folderName);
		}

		string_list.removeAt(0);//去掉.
		string_list.removeAt(0);//去掉..
		ui.listWidget->clear();
		ui.listWidget->addItems(string_list);
		//显示默认模板👇👇
		for (i = 0; i < string_list.count(); i++)
		{

			if (string_list.at(i) == str)//找到的刚才修改的那个名称
			{
				ui.listWidget->setCurrentRow(i);
			}
		}
	}
}

void QtCameraSet::onCameraKeyboardOut(int r, int c)
{
	if (r != 2 && c > 0)
	{
		SHOWOSK();
	}
}

int QtCameraSet::showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str)//全是中文
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

void QtCameraSet::SHOWOSK()//键盘显示
{
	//The code from david!!!
	PVOID OldValue;
	BOOL bRet = Wow64DisableWow64FsRedirection(&OldValue);
	//QString csProcess = "C:/Program/FilesCommon/Filesmicrosoft/sharedink/TabTip.exe";
	//QString csProcess = AppPath + "/osk.exe";
	QString csProcess = "C:/WINDOWS/system32/osk.exe";
	QString params = "";
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//ShellExecute(NULL, L"open", (LPCWSTR)csProcess.utf16(), (LPCWSTR)params.utf16(), NULL, SW_SHOWNORMAL);
	if (bRet)
	{
		Wow64RevertWow64FsRedirection(OldValue);
	}
}
void QtCameraSet::SWITCHOSK()//快捷键
{
	keybd_event(0x11, 0, 0, 0);
	keybd_event(0x5B, 0, 0, 0);
	keybd_event(0x4F, 0, 0, 0);
	keybd_event(0x11, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(0x5B, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(0x4F, 0, KEYEVENTF_KEYUP, 0);//ctrl+win+o切换
}

bool QtCameraSet::IsNumber(QString& qstrSrc)//是否是纯数字
{
	QByteArray ba = qstrSrc.toLatin1();
	const char* s = ba.data();
	bool bret = true;
	while (*s)
	{
		if ((*s >= '0' && *s <= '9'))
		{

		}
		else
		{
			bret = false;
			break;
		}
		s++;
	}
	return bret;
}
//————————————————
//版权声明：本文为CSDN博主「Good运」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https ://blog.csdn.net/qq_36391817/article/details/80708590

//下面是树型多选框函数
void QtCameraSet::init()
{
	ui.treeWidget->clear();    //初始化树形控件
	ui.treeWidget->setHeaderHidden(true);  //隐藏表头
	QFont serifFont("Times", 16);
	ui.treeWidget->setFont(serifFont);

	//定义第一个树形组 爷爷项
	QTreeWidgetItem* group = new QTreeWidgetItem(ui.treeWidget);
	group->setText(0, QString::fromLocal8Bit("设备使用权限"));    //树形控件显示的文本信息
	//group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);   //设置树形控件子项的属性
	//Qt::ItemIsUserCheckable | Qt::ItemIsSelectable 两个都是方框是否可选状态，暂时没用
	//Qt::ItemIsEnabled 使能，不使能会显示为灰色，可以在查看的时候而非添加的时候用
	//Qt::ItemIsEditable 文字可编辑与否，我们都不让编辑
	group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
	group->setCheckState(0, Qt::Unchecked); //初始状态没有被选中
	//第一组子项
	QTreeWidgetItem* group1 = new QTreeWidgetItem(group);
	group1->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	group1->setText(0, QString::fromLocal8Bit("设备运行"));  //设置子项显示的文本
	group1->setCheckState(0, Qt::Checked); //设置子选项的显示格式和状态

	QTreeWidgetItem* group2 = new QTreeWidgetItem(group);
	group2->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	group2->setText(0, QString::fromLocal8Bit("设置"));
	group2->setCheckState(0, Qt::Unchecked);

	//父亲项
	QTreeWidgetItem* group21 = new QTreeWidgetItem(group2);
	group21->setText(0, QString::fromLocal8Bit("模板管理"));
	group21->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	group21->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group22 = new QTreeWidgetItem(group2);
	group22->setText(0, QString::fromLocal8Bit("相机参数"));
	group22->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	group22->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group23 = new QTreeWidgetItem(group2);
	group23->setText(0, QString::fromLocal8Bit("PLC设置"));
	group23->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	group23->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group24 = new QTreeWidgetItem(group2);
	group24->setText(0, QString::fromLocal8Bit("用户管理"));
	group24->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	group24->setCheckState(0, Qt::Unchecked);

	//孙子项1
	QTreeWidgetItem* group211 = new QTreeWidgetItem(group21);   //指定子项属于哪一个父项
	group211->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group211->setText(0, QString::fromLocal8Bit("保存/应用"));
	group211->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group212 = new QTreeWidgetItem(group21);
	group212->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group212->setText(0, QString::fromLocal8Bit("添加"));
	group212->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group213 = new QTreeWidgetItem(group21);
	group213->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group213->setText(0, QString::fromLocal8Bit("删除"));
	group213->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group214 = new QTreeWidgetItem(group21);
	group214->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group214->setText(0, QString::fromLocal8Bit("修改名称"));
	group214->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group215 = new QTreeWidgetItem(group21);
	group215->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group215->setText(0, QString::fromLocal8Bit("算法设置"));
	group215->setCheckState(0, Qt::Unchecked);
	//孙子项2
	QTreeWidgetItem* group221 = new QTreeWidgetItem(group22);
	group221->setText(0, QString::fromLocal8Bit("相机调试"));
	group221->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group221->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group222 = new QTreeWidgetItem(group22);
	group222->setText(0, QString::fromLocal8Bit("采集测试"));
	group222->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group222->setCheckState(0, Qt::Unchecked);

	//孙子项3
	QTreeWidgetItem* group231 = new QTreeWidgetItem(group23);
	group231->setText(0, QString::fromLocal8Bit("参数读取"));
	group231->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group231->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group232 = new QTreeWidgetItem(group23);
	group232->setText(0, QString::fromLocal8Bit("参数写入"));
	group232->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group232->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group233 = new QTreeWidgetItem(group23);
	group233->setText(0, QString::fromLocal8Bit("采集"));
	group233->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group233->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group234 = new QTreeWidgetItem(group23);
	group234->setText(0, QString::fromLocal8Bit("控制测试"));
	group234->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group234->setCheckState(0, Qt::Unchecked);

	//孙子项4
	QTreeWidgetItem* group241 = new QTreeWidgetItem(group24);
	group241->setText(0, QString::fromLocal8Bit("添加用户"));
	group241->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group241->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group242 = new QTreeWidgetItem(group24);
	group242->setText(0, QString::fromLocal8Bit("切换用户"));
	group242->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group242->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group243 = new QTreeWidgetItem(group24);
	group243->setText(0, QString::fromLocal8Bit("删除用户"));
	group243->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group243->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group244 = new QTreeWidgetItem(group24);
	group244->setText(0, QString::fromLocal8Bit("查看权限"));
	group244->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group244->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group245 = new QTreeWidgetItem(group24);
	group245->setText(0, QString::fromLocal8Bit("更改权限"));
	group245->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group245->setCheckState(0, Qt::Unchecked);

	QTreeWidgetItem* group3 = new QTreeWidgetItem(group);
	group3->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled| Qt::ItemIsSelectable);
	group3->setText(0, QString::fromLocal8Bit("数据导出"));
	group3->setCheckState(0, Qt::Unchecked);

	//ui->treeWidget->expandAll();  //展开树
	ui.treeWidget->expandToDepth(1);
}

void QtCameraSet::updateParentItem(QTreeWidgetItem* item)
{

	QTreeWidgetItem* parent = item->parent();

	if (parent == NULL)
		return;

	int nSelectedCount = 0;//选中数
	int nHalfSelectedCount = 0;//半选中数
	int childCount = parent->childCount();//孩子数

	for (int i = 0; i < childCount; i++) //判断有多少个子项被选中
	{
		QTreeWidgetItem* childItem = parent->child(i);
		if (childItem->checkState(0) == Qt::Checked)
		{
			nSelectedCount++;
		}
		else if (childItem->checkState(0) == Qt::PartiallyChecked)
		{
			nHalfSelectedCount++;
		}
	}

	if ((nSelectedCount + nHalfSelectedCount) <= 0)  //如果没有子项被选中，父项设置为未选中状态
		parent->setCheckState(0, Qt::Unchecked);
	else if ((childCount > nHalfSelectedCount&& nHalfSelectedCount > 0) || (childCount > nSelectedCount&& nSelectedCount > 0))// && nSelectedCount < childCount)    //如果有部分子项被选中，父项设置为部分选中状态，即用灰色显示
		parent->setCheckState(0, Qt::PartiallyChecked);
	else if (nSelectedCount == childCount)    //如果子项全部被选中，父项则设置为选中状态
		parent->setCheckState(0, Qt::Checked);

	updateParentItem(parent);//

}

void QtCameraSet::onTreeItemChanged(QTreeWidgetItem* item)//利用changed自动递归。
{
	int count = item->childCount(); //返回子项的个数

	if (Qt::Checked == item->checkState(0))
	{
		item->setCheckState(0, Qt::Checked);
		if (count > 0)
		{
			for (int i = 0; i < count; i++)
			{
				item->child(i)->setCheckState(0, Qt::Checked);
			}
		}
		else { updateParentItem(item); }
	}
	else if (Qt::Unchecked == item->checkState(0))
	{
		if (count > 0)
		{
			for (int i = 0; i < count; i++)
			{
				item->child(i)->setCheckState(0, Qt::Unchecked);
			}
		}
		else { updateParentItem(item); }
	}

}

//上面是树型多选框函数

//下面是查看权限
void QtCameraSet::checkPermission()
{
	ui.treeWidget_2->clear();    //初始化树形控件
	ui.treeWidget_2->setHeaderHidden(true);  //隐藏表头
	QFont serifFont("Times", 16);
	ui.treeWidget_2->setFont(serifFont);

	//定义第一个树形组 爷爷项
	checkPermissionGroup = new QTreeWidgetItem(ui.treeWidget_2);
	QString str = ui.comboBox->currentText();
	checkPermissionGroup->setText(0, str);    //树形控件显示的文本信息
	//group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);   //设置树形控件子项的属性
	//Qt::ItemIsUserCheckable | Qt::ItemIsSelectable 两个都是方框是否可选状态，暂时没用
	//Qt::ItemIsEnabled 使能，不使能会显示为灰色，可以在查看的时候而非添加的时候用
	//Qt::ItemIsEditable 文字可编辑与否，我们都不让编辑
	checkPermissionGroup->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);   //设置树形控件子项的属性
	checkPermissionGroup->setCheckState(0, Qt::Checked); //初始状态没有被选中
	checkPermissionGroup->setBackground(0, QBrush(QColor("#880f97ff")));//AARRGGBB /RRGGBB

	//第一组子项
	QTreeWidgetItem* group1 = new QTreeWidgetItem(checkPermissionGroup);
	// 	QFont headFont("Times", 16,QFont::Bold);
	group1->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group1->setText(0, QString::fromLocal8Bit("设备运行"));  //设置子项显示的文本
	group1->setCheckState(0, Qt::Checked); //设置子选项的显示格式和状态

	QTreeWidgetItem* group2 = new QTreeWidgetItem(checkPermissionGroup);
	group2->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group2->setText(0, QString::fromLocal8Bit("设置"));
	group2->setCheckState(0, Qt::Checked);
	//设置蓝色group2->setBackground(0, QBrush(QColor("#0000FF")));

	//父亲项
	QTreeWidgetItem* group21 = new QTreeWidgetItem(group2);
	group21->setText(0, QString::fromLocal8Bit("模板管理"));
	group21->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group21->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group22 = new QTreeWidgetItem(group2);
	group22->setText(0, QString::fromLocal8Bit("相机参数"));
	group22->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group22->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group23 = new QTreeWidgetItem(group2);
	group23->setText(0, QString::fromLocal8Bit("PLC设置"));
	group23->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group23->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group24 = new QTreeWidgetItem(group2);
	group24->setText(0, QString::fromLocal8Bit("用户管理"));
	group24->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group24->setCheckState(0, Qt::Checked);

	//孙子项1
	QTreeWidgetItem* group211 = new QTreeWidgetItem(group21);   //指定子项属于哪一个父项
	group211->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group211->setText(0, QString::fromLocal8Bit("保存/应用"));
	group211->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group212 = new QTreeWidgetItem(group21);
	group212->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group212->setText(0, QString::fromLocal8Bit("添加"));
	group212->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group213 = new QTreeWidgetItem(group21);
	group213->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group213->setText(0, QString::fromLocal8Bit("删除"));
	group213->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group214 = new QTreeWidgetItem(group21);
	group214->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group214->setText(0, QString::fromLocal8Bit("修改名称"));
	group214->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group215 = new QTreeWidgetItem(group21);
	group215->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group215->setText(0, QString::fromLocal8Bit("算法设置"));
	group215->setCheckState(0, Qt::Checked);
	//孙子项2
	QTreeWidgetItem* group221 = new QTreeWidgetItem(group22);
	group221->setText(0, QString::fromLocal8Bit("相机调试"));
	group221->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group221->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group222 = new QTreeWidgetItem(group22);
	group222->setText(0, QString::fromLocal8Bit("采集测试"));
	group222->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group222->setCheckState(0, Qt::Checked);

	//孙子项3
	QTreeWidgetItem* group231 = new QTreeWidgetItem(group23);
	group231->setText(0, QString::fromLocal8Bit("参数读取"));
	group231->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group231->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group232 = new QTreeWidgetItem(group23);
	group232->setText(0, QString::fromLocal8Bit("参数写入"));
	group232->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group232->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group233 = new QTreeWidgetItem(group23);
	group233->setText(0, QString::fromLocal8Bit("采集"));
	group233->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group233->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group234 = new QTreeWidgetItem(group23);
	group234->setText(0, QString::fromLocal8Bit("控制测试"));
	group234->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group234->setCheckState(0, Qt::Checked);

	//孙子项4
	QTreeWidgetItem* group241 = new QTreeWidgetItem(group24);
	group241->setText(0, QString::fromLocal8Bit("添加用户"));
	group241->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group241->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group242 = new QTreeWidgetItem(group24);
	group242->setText(0, QString::fromLocal8Bit("切换用户"));
	group242->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group242->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group243 = new QTreeWidgetItem(group24);
	group243->setText(0, QString::fromLocal8Bit("删除用户"));
	group243->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group243->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group244 = new QTreeWidgetItem(group24);
	group244->setText(0, QString::fromLocal8Bit("查看权限"));
	group244->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group244->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group245 = new QTreeWidgetItem(group24);
	group245->setText(0, QString::fromLocal8Bit("更改权限"));
	group245->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group245->setCheckState(0, Qt::Checked);

	QTreeWidgetItem* group3 = new QTreeWidgetItem(checkPermissionGroup);
	group3->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	group3->setText(0, QString::fromLocal8Bit("数据导出"));
	group3->setCheckState(0, Qt::Checked);

	ui.treeWidget_2->expandAll();  //展开树
	//ui.treeWidget_2->expandToDepth(1);
}

void QtCameraSet::updateCheckPermission(const QString& str)
{
	checkPermissionGroup->setText(0, str);
	QTreeWidgetItemIterator it(ui.treeWidget_2);
	if (checkPermissionGroup->text(0) == QString::fromLocal8Bit("管理员"))//0
	{
		while (*it) {
			if ((*it)->text(0) == QString::fromLocal8Bit("设备运行")
				|| (*it)->text(0) == QString::fromLocal8Bit("设置")
				|| (*it)->text(0) == QString::fromLocal8Bit("数据导出"))
			{
				(*it)->setCheckState(0, Qt::Checked);
			}
			++it;
		}
	}
	else if (checkPermissionGroup->text(0) == QString::fromLocal8Bit("工程师"))//1
	{
		while (*it) {
			if ((*it)->text(0) == QString::fromLocal8Bit("设备运行")
				|| (*it)->text(0) == QString::fromLocal8Bit("设置"))
			{
				(*it)->setCheckState(0, Qt::Checked);
			}
			if ((*it)->text(0) == QString::fromLocal8Bit("用户管理")
				|| (*it)->text(0) == QString::fromLocal8Bit("数据导出"))
			{
				(*it)->setCheckState(0, Qt::Unchecked);
			}
			++it;
		}
	}
	else if (checkPermissionGroup->text(0) == QString::fromLocal8Bit("操作员"))//2
	{
		while (*it) {
			if ((*it)->text(0) == QString::fromLocal8Bit("设备运行"))
			{
				(*it)->setCheckState(0, Qt::Checked);
			}
			if ((*it)->text(0) == QString::fromLocal8Bit("设置")
				|| (*it)->text(0) == QString::fromLocal8Bit("数据导出"))
			{
				(*it)->setCheckState(0, Qt::Unchecked);
			}
			++it;
		}
	}
	else if (checkPermissionGroup->text(0) == QString::fromLocal8Bit("质检员"))//3
	{
		while (*it) {
			if ((*it)->text(0) == QString::fromLocal8Bit("数据导出"))
			{
				(*it)->setCheckState(0, Qt::Checked);
			}
			if ((*it)->text(0) == QString::fromLocal8Bit("设备运行")
				|| (*it)->text(0) == QString::fromLocal8Bit("设置"))
			{
				(*it)->setCheckState(0, Qt::Unchecked);
			}
			++it;
		}
	}

}

void QtCameraSet::initTableOfUserPermission()
{
	QFont font;
	font = ui.tabWidget_CameraVec->font();//左下角白框的字体——[SimSun,21]
	font.setPointSize(21);

	QWidget* tab = new QWidget();//新建tab
	tab->setFont(font);//设置tab字体
	tab->setObjectName(QString::fromUtf8("tab_0"));//tab_23170685
	ui.tabWidget_Users->addTab(tab, QString::fromLocal8Bit("默认用户"));//将tab添加到左下角tabwidget boject name:tab_23170685 tttle:23170685
	QTableWidget* tableWidget = new QTableWidget(tab);//tab下面加tablewidget
	tableWidget->setObjectName(QString::fromLocal8Bit("tableWidget_0"));//tableWidget_23170685
	tableWidget->setGeometry(QRect(9, 9, tab->height() - 50, tab->width() - 40));//设置widget尺寸 黑边是边界

	QStringList strlist;
	strlist << QString::fromLocal8Bit("用户名称") << QString::fromLocal8Bit("权限级别");
	tableWidget->setColumnCount(2);//两列
	tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分填充表头
	tableWidget->verticalHeader()->setDefaultSectionSize(20);//默认行高20
	tableWidget->setHorizontalHeaderLabels(strlist);//水平表头参数、数值
	tableWidget->verticalHeader()->setVisible(false);
	tableWidget->horizontalHeader()->setVisible(true);//表头可见
	font = tableWidget->horizontalHeader()->font();//表头字体
	font.setPointSize(18);//字号
	tableWidget->horizontalHeader()->setFont(font);//设置表头字体

	font.setPointSize(16);//字号
	tableWidget->setFont(font);//设置内容字体

	int currentcolumn = tableWidget->rowCount();//行数
	tableWidget->insertRow(currentcolumn);//插入行
	tableWidget->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("管理员")));//0列设置
	tableWidget->item(currentcolumn, 0)->setFlags(tableWidget->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));//单元格不可编辑
	tableWidget->item(currentcolumn, 0)->setFlags(tableWidget->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));//单元格不可选择
	tableWidget->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(0)));//1列设置
	tableWidget->item(currentcolumn, 1)->setFlags(tableWidget->item(currentcolumn, 1)->flags() & (~Qt::ItemIsEditable));
	tableWidget->item(currentcolumn, 1)->setFlags(tableWidget->item(currentcolumn, 1)->flags() & (~Qt::ItemIsSelectable));
	if (g_IUserLevel==0)
	{
		tableWidget->item(currentcolumn, 0)->setBackground(QBrush(QColor("#8889ff81")));//AARRGGBB /RRGGBB
		tableWidget->item(currentcolumn, 1)->setBackground(QBrush(QColor("#8889ff81")));//AARRGGBB /RRGGBB
	}
	
	//currentcolumn->setBackgroundColor(QColor(255, 0, 0));

	currentcolumn = tableWidget->rowCount();
	tableWidget->insertRow(currentcolumn);
	tableWidget->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("工程师")));
	tableWidget->item(currentcolumn, 0)->setFlags(tableWidget->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
	tableWidget->item(currentcolumn, 0)->setFlags(tableWidget->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
	tableWidget->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(1)));
	tableWidget->item(currentcolumn, 1)->setFlags(tableWidget->item(currentcolumn, 1)->flags() & (~Qt::ItemIsEditable));
	tableWidget->item(currentcolumn, 1)->setFlags(tableWidget->item(currentcolumn, 1)->flags() & (~Qt::ItemIsSelectable));

	currentcolumn = tableWidget->rowCount();
	tableWidget->insertRow(currentcolumn);
	tableWidget->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("操作员")));
	tableWidget->item(currentcolumn, 0)->setFlags(tableWidget->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
	tableWidget->item(currentcolumn, 0)->setFlags(tableWidget->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
	tableWidget->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(2)));
	tableWidget->item(currentcolumn, 1)->setFlags(tableWidget->item(currentcolumn, 1)->flags() & (~Qt::ItemIsEditable));
	tableWidget->item(currentcolumn, 1)->setFlags(tableWidget->item(currentcolumn, 1)->flags() & (~Qt::ItemIsSelectable));

	currentcolumn = tableWidget->rowCount();
	tableWidget->insertRow(currentcolumn);
	tableWidget->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("质检员")));
	tableWidget->item(currentcolumn, 0)->setFlags(tableWidget->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
	tableWidget->item(currentcolumn, 0)->setFlags(tableWidget->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
	tableWidget->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(3)));
	tableWidget->item(currentcolumn, 1)->setFlags(tableWidget->item(currentcolumn, 1)->flags() & (~Qt::ItemIsEditable));
	tableWidget->item(currentcolumn, 1)->setFlags(tableWidget->item(currentcolumn, 1)->flags() & (~Qt::ItemIsSelectable));

	//第二个表
	tab = new QWidget();//新建tab
	tab->setObjectName(QString::fromUtf8("tab_1"));//tab_23170685
	ui.tabWidget_Users->addTab(tab, QString::fromLocal8Bit("自定义用户"));//将tab添加到左下角tabwidget boject name:tab_23170685 tttle:23170685
	tableWidget = new QTableWidget(tab);//tab下面加tablewidget
	tableWidget->setObjectName(QString::fromLocal8Bit("tableWidget_0"));//tableWidget_23170685
	tableWidget->setGeometry(QRect(9, 9, tab->height() - 50, tab->width() - 40));//设置widget尺寸 黑边是边界

	strlist << QString::fromLocal8Bit("用户名称") << QString::fromLocal8Bit("权限级别");
	tableWidget->setColumnCount(2);//两列
	tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分填充表头
	tableWidget->verticalHeader()->setDefaultSectionSize(20);//默认行高20
	tableWidget->setHorizontalHeaderLabels(strlist);//水平表头参数、数值
	tableWidget->verticalHeader()->setVisible(false);
	tableWidget->horizontalHeader()->setVisible(true);//表头可见
	font = tableWidget->horizontalHeader()->font();//表头字体
	font.setPointSize(18);//字号
	tableWidget->horizontalHeader()->setFont(font);//设置表头字体

	font.setPointSize(16);//字号
	tableWidget->setFont(font);//设置内容字体
}

void QtCameraSet::btn_Enabled(int i)
{
	if (i == 0)
	{
		ui.pB_Users_Add->setEnabled(false);
		ui.pB_Users_Delete->setEnabled(false);
		ui.pB_Users_Change->setEnabled(false);

	}
	else {
		ui.pB_Users_Add->setEnabled(true);
		ui.pB_Users_Delete->setEnabled(true);
		ui.pB_Users_Change->setEnabled(true);
	}
}
