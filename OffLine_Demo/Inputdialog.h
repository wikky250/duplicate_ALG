#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <Windows.h>
#pragma comment(lib, "user32.lib")//这两句和键盘有关
#include <QCloseEvent>
namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT



public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();
    void SHOWOSK();
    void HideOSK();
    void changingModel();
    int showMsgBox(const char* titleStr, const char* contentStr, const char* button1Str, const char* button2Str);//全是中文
private:
    Ui::InputDialog *ui;

protected:
    void closeEvent(QCloseEvent* event);
};

#endif // INPUTDIALOG_H
