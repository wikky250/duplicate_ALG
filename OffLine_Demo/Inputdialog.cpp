#include "Inputdialog.h"
#include "ui_Inputdialog.h"
#include <Tlhelp32.h>//terminate process
extern QString AppPath;
#include <QMessageBox>
#include <QIntValidator>
#include <QDir>
#include <QFileInfoList>

InputDialog::InputDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::InputDialog)
{
	ui->setupUi(this);
	SHOWOSK();
	this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);//ȥ�����Ͻ��ʺ�
	ui->lineEdit->setFocus();
	QRegExp regx("[a-zA-Z0-9_]+$");//������ʽQRegExp,ֻ�����������ġ����֡���ĸ���»����Լ��ո�,[\u4e00 - \u9fa5a - zA - Z0 - 9_] + $
	ui->lineEdit->setValidator(new QRegExpValidator(regx, this));
	connect(ui->lineEdit, &QLineEdit::returnPressed, [=]() {changingModel(); });
	connect(ui->pushButton, &QPushButton::clicked, [=]() {changingModel(); });//����lambda���ʽ����
}

InputDialog::~InputDialog()
{
	delete ui;
}

int InputDialog::showMsgBox(const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str)//ȫ������
{
	if (QString::fromLocal8Bit(button2Str) =="")
	{
		QMessageBox msg(QMessageBox::Information, QString::fromLocal8Bit(titleStr), QString::fromLocal8Bit(contentStr), QMessageBox::Ok);
		msg.setButtonText(QMessageBox::Ok, QString::fromLocal8Bit(button1Str));
		msg.setWindowIcon(QIcon("./dr.ico"));
		return msg.exec();
	}
	else
	{
		QMessageBox msg(QMessageBox::Information, QString::fromLocal8Bit(titleStr), QString::fromLocal8Bit(contentStr), QMessageBox::Yes|QMessageBox::No);
		msg.setButtonText(QMessageBox::Yes, QString::fromLocal8Bit(button1Str));
		msg.setButtonText(QMessageBox::No, QString::fromLocal8Bit(button2Str));
		msg.setWindowIcon(QIcon("./dr.ico"));
		return msg.exec();
	}


}
void InputDialog::SHOWOSK()
{
	//The code from david!!!
	PVOID OldValue;
	BOOL bRet = Wow64DisableWow64FsRedirection(&OldValue);
	//QString csProcess = "C:/Program/FilesCommon/Filesmicrosoft/sharedink/TabTip.exe";
	QString csProcess = AppPath + "/osk.exe";
	QString params = "";
	ShellExecute(NULL, L"open", (LPCWSTR)csProcess.utf16(), (LPCWSTR)params.utf16(), NULL, SW_SHOWNORMAL);
	if (bRet)
	{
		Wow64RevertWow64FsRedirection(OldValue);
	}
}

void InputDialog::HideOSK()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//�������ǻ�������н��̵���Ϣ��
//����hSnapShot�г�ȡ���ݵ�һ��PROCESSENTRY32�ṹ��
//����ṹ������һ�����̣���ToolHelp32 API��һ���֡�
//��ȡ���ݿ�Process32First()��Process32Next()������������
//�������ǽ���Process32Next()������ԭ���ǣ�
//BOOL WINAPI Process32Next(HANDLE hSnapshot,LPPROCESSENTRY32 lppe);
//���ǳ���Ĵ����м��룺
	PROCESSENTRY32* processInfo = new PROCESSENTRY32;
	// ��������PROCESSENTRY32��dwSize��Ա��ֵ ;
	processInfo->dwSize = sizeof(PROCESSENTRY32);
	int index = 0;
	//�������ǽ����վ����PROCESSENTRY32�ṹ����Process32Next()��
	//ִ��֮��PROCESSENTRY32 �ṹ����ý��̵���Ϣ������ѭ��������ֱ����������FALSE��
	//printf("****************��ʼ�оٽ���****************/n");
	int ID = 0;
	while (Process32Next(hSnapShot, processInfo) != FALSE)
	{
		index++;
		//printf("****************** %d ******************/n",index);
		//printf("PID       Name      Current Threads/n");
		//printf("%-15d%-25s%-4d/n",processInfo->th32ProcessID,processInfo->szExeFile,processInfo->cntThreads);
		int size = WideCharToMultiByte(CP_ACP, 0, processInfo->szExeFile, -1, NULL, 0, NULL, NULL);
		char* ch = new char[size + 1];
		if (WideCharToMultiByte(CP_ACP, 0, processInfo->szExeFile, -1, ch, size, NULL, NULL))
		{
			if (strstr(ch, "osk.exe"))//ʹ����δ����ʱ��ֻ��Ҫ�ı�"cmd.exe".����ĳ���Ҫ�����Ľ������Ϳ����ˡ�//win10�Դ������޸�Ȩ�ޣ����Խ���ɱ�������õĵ�����exe
			{
				ID = processInfo->th32ProcessID;
				// qDebug()<<"ID ="<<ID;
				HANDLE hProcess;
				// ���������ú��� TerminateProcess()��ֹ���̣�
				// ����������PROCESS_ALL_ACCESS
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ID);
				//if(hProcess==NULL)
				//{
				  //  printf("Unable to get handle of process: ");
				  //  printf("Error is: %d",GetLastError());
				//}
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
		}
	}
	CloseHandle(hSnapShot);
	delete processInfo;
}

void InputDialog::changingModel()
{
	if (ui->lineEdit->text().isEmpty())//δ�����ַ�
	{
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("ģ�����Ʋ�����Ϊ�գ�"),2000);
		//levelOut->show();
		ui->lineEdit->setFocus();
	}
	else//�������ַ������15����ֻ֧����ĸ���ֺ�_,�»�������Ҫ��Ĭ������״̬ΪӢ��
	{
		QString str = ui->lineEdit->text();
		QDir dir(AppPath + "\\ModelFile");
		dir.setFilter(QDir::Dirs);//ɸѡĿ¼
		QFileInfoList list = dir.entryInfoList();//�ļ���Ϣlist

		int file_count = list.count();
		QStringList string_list;
		int i;//j���ڱ���Ƿ����Ĭ��ģ��
		for (i = 0; i < file_count; i++)
		{
			QFileInfo file_info = list.at(i);
			QString folderName = file_info.fileName();
			if (folderName == str)//����
			{
				//levelOut = new WindowOut;
				//levelOut->getString(QString::fromLocal8Bit("ģ�����Ʋ�����������ģ�������ظ���"), 2000);
				//levelOut->show();
				ui->lineEdit->setFocus();
				return;
			}
		}
		close();
		HideOSK();
		//levelOut = new WindowOut;
		//levelOut->getString(QString::fromLocal8Bit("��ϲ�����Ĳ����ѳɹ���"), 2000);
		//levelOut->show();

	}
}

void InputDialog::closeEvent(QCloseEvent* event) //���ڹر�Event��д
{
	HideOSK();
	this->close();
}
