#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>

class handler : public QObject
{
    Q_OBJECT
public:
    static handler *getInstance();//静态函数只能操作下面的静态变量，只能将下面的返回给其他人用，
//    ，不用实例化，直接去调用。单例模式只有一个对象*hand。
    void changeOK(QString str);
    int theID;//区分功能
private:   //单例模式，将public改为private 。无法在外部new一个新的对象。
    explicit handler(QObject *parent = nullptr);

signals:
    void applyChange(QString,int);//只定义，不用声明，也没有声明
public slots:

private:
    static handler *hand;
};

#endif // HANDLER_H
