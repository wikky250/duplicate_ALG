#include "OffLine_Demo.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("fusion"));
	QSharedMemory mem("OffLine_Demo");//��ϵͳexe����Ϊ���������干���ڴ�mem
	if (!mem.create(1))//���������ڴ�mem������ù����ڴ��Ѵ��ڣ��򵯳���ʾ�Ի��򣬲��˳�
	{
	}

	OffLine_Demo w;//����w
	//w.showFullScreen();
	w.show();
	return a.exec();
}
