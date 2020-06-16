#pragma once

#include <QDialog>
#include "ui_QtCameraSet.h"
#include "AllRelayHead.h"
#include "MultThread_Run.h"
#include "Inputdialog.h"
#include <QTreeWidgetItem>

class QtCameraSet : public QDialog
{
	Q_OBJECT
signals:
	void STARTGRABBING(int);

public:
	QtCameraSet(QWidget *parent = Q_NULLPTR);
	~QtCameraSet();

	void initCameraSetTableWidget(QTableWidget *);
	void initListWidgetofModel();
	bool compareModels();
	void QtCameraSet::SWITCHOSK();//快捷键
	int showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str);//全是中文
	void SHOWOSK();//键盘显示
	bool IsNumber(QString& qstrSrc);//是否是纯数字

private:
	Ui::QtCameraSet ui;
	bool b_changeCamera;//相机参数有人为修改-zxc*凡事拿不准，写上赵晓春
	bool b_autoAutoChange;
	QFont font;
	bool SaveSetParam();
	InputDialog* inputModel;
	QTableWidget* tableWidget;

	QTreeWidgetItem* checkPermissionGroup;
public slots:
	//模拟工作状态开始采集
	void StartGrab(bool);
	void StopGrab();
	void onShowAlgSet();
	//相机连续采集
	void onContinueGrab(bool);
	//相机参数修改响应函数
	void onCameraCellChange(int , int);
	void modelApply();//模板应用按钮槽函数
	void modelDelete();//删除模板文件和listwidget对应的item
	void modelChangeName();
	void modelAdd();
	void applyNameChange(QString str, int ID);
	void onCameraKeyboardOut(int r, int c);//
	void SyncAnotherStartGrab(bool);//两个StartGrab的东西2-1
	void SyncFirstStartGrab(bool);//两个StartGrab的东西2-2

//👇👇👇👇👇树型多选
public: //申明初始化函数
	void init();
	void updateParentItem(QTreeWidgetItem* item);
	void checkPermission();

public slots:   //申明信号与槽,当树形控件的子选项被改变时执行
	void onTreeItemChanged(QTreeWidgetItem* item);
	void updateCheckPermission(const QString& str);//切换用户时更新表
	void initTableOfUserPermission();
	void btn_Enabled(int i);//用户部分按钮是否可用
//👆👆👆👆👆
	void QtCameraSet::findTreeItemText();//找特定名字的treeitem
#ifdef PLCCONNECT
	void onReadPLCParam(bool);
	void onSendPLCCommand(bool);
	DataFromPC_typ getEveryValue();//PC数值发送给PLC
	void showEveryValue(DataToPC_typ);
	MESSAGE_HANDLER ShowFunc(void* context, DataToPC_typ);
#endif
public:
	bool copyDirectoryFiles(const QString& fromDir, const QString& toDir, bool coverFileIfExist);
	bool deleteDir(const QString& path);
};
