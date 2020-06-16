#include "handler.h"

handler::handler(QObject *parent) : QObject(parent)
{

}
handler*handler::hand=new handler;

handler*handler::getInstance()
{
    return hand;
}
//简单的单例设置方法。具有全局变量的特性，是为了在item中的东西可以在mainwindow中得到。

void handler::changeOK(QString str)
{
    emit applyChange(str,theID);//发射信号。
}


//单例的运用（代替不好的全局变量）
//1：item 打到老鼠调用addScore
//2：addScore函数发射信号beatMouse
//3：mainwindow连接hand 发射信号 this 加分槽函数
//4：槽函数内分数自加10，并且显示
