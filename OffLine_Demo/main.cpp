#include "OffLine_Demo.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("fusion"));
	QSharedMemory mem("OffLine_Demo");//以系统exe名称为参数，定义共享内存mem
	if (!mem.create(1))//创建共享内存mem，如果该共享内存已存在，则弹出提示对话框，并退出
	{
	}

	OffLine_Demo w;//创建w
	//w.showFullScreen();
	w.show();
	return a.exec();
}
