#pragma once

#include <QDialog>
#include "ui_QtLoginDlg.h"
#include <QMessageBox>
#include <QCloseEvent>

class QtLoginDlg : public QDialog
{
	Q_OBJECT

public:
	QtLoginDlg(QDialog *parent = Q_NULLPTR);
	~QtLoginDlg();
	int showMsgBox(QMessageBox::Icon icon, const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str);//全是中文
private:
	Ui::QtLoginDlg ui;

public slots:
	void onEnsure();
	void onUserListChange(QString);
	void onLineeditChanged();

};
