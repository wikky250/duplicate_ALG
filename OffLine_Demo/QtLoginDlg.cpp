#include "QtLoginDlg.h"
#include "AllRelayHead.h"
extern QString g_QSUserName;
extern int g_IUserLevel;
extern QString AppPath;
#include <QButtonGroup>

QtLoginDlg::QtLoginDlg(QDialog* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./dr.ico"));//文件copy到了exe所在目录
	this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); //setWindowFlags(Qt::Window | Qt::WindowTitleHint);
	char* configpath = new char[MAX_PATH];
	strcpy(configpath, (AppPath + "\\ModelFile\\ProgramSet.ini").toStdString().c_str());//c_str返回char*
	QVector<std::string> v_line;//存储相机序列号
	std::ifstream cin(configpath);//将文件路径放入标准流
	delete configpath;
	std::string filename;
	std::string line;
	int r_n = 0, ipos = 0;
	QStringList ls_user;
	if (cin) // 有该文件  
	{//确定ini文件中有多少相机配置文件，并将IP存入v_line
		while (getline(cin, line)) // line中不包括每行的换行符  
		{
			if (line.find("USER_") != std::string::npos)//std::string::npos指不存在
			{
				line.erase(0, 6);
				line.erase(line.length() - 1, 1);
				ls_user.push_back(line.c_str());
			}
		}
	}
	if (ls_user.size() != 0)
	{
		ui.cB_ListUser->addItems(ls_user);
	}
	ui.pB_Login->setEnabled(false);
	connect(ui.pB_Login, SIGNAL(clicked()), this, SLOT(onEnsure()));
	connect(ui.cB_ListUser, SIGNAL(currentTextChanged(QString)), this, SLOT(onUserListChange(QString)));
	connect(ui.lE_Password, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(onLineeditChanged()));
	connect(ui.pB_Exit, &QPushButton::clicked, [=]() {
		close(); });
	connect(ui.btn_Group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
		[=](QAbstractButton* button) {ui.lE_Password->setText(ui.lE_Password->text() + button->text()); });
}

QtLoginDlg::~QtLoginDlg()
{

}

void QtLoginDlg::onUserListChange(QString)
{
	ui.lE_Password->clear();
}

void QtLoginDlg::onLineeditChanged()
{
	int i = ui.lE_Password->text().length();
	if (i < 4)
	{
		ui.pB_Login->setEnabled(false);
	}
	else
	{
		ui.pB_Login->setEnabled(true);
	}
}

void QtLoginDlg::onEnsure()
{
	QString username = ui.cB_ListUser->currentText();
	username = "USER_" + username;
	QSettings configIniRead(AppPath + "\\ModelFile\\ProgramSet.ini", QSettings::IniFormat);//读取ini文件
	QString password = configIniRead.value(username + "/Password", -1).toString(); //密码
	if (password != ui.lE_Password->text())
	{
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("密码输入错误，请确认后重新输入！"),5000);
		//levelOut->show();
		ui.lE_Password->setText("");
		return;
	}
	int userlevel = configIniRead.value(username + "/Level", -1).toInt();//用户等级
	g_QSUserName = username;
	g_IUserLevel = userlevel;
	this->close();


}

int QtLoginDlg::showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str)//全是中文
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

