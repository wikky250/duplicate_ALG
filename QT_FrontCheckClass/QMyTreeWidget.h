#pragma once
#include <QtCore/QtCore>
#include <QTreeWidget>
#include <QObject>
#include "yaml-cpp/yaml.h"
#pragma comment(lib,"yaml-cppd.lib")

#include <QTextEdit>
#include <QTimer>
#include <QSlider>

class QMyTextEdit: public QTextEdit
{
	Q_OBJECT
public:
	QMyTextEdit(QWidget *parent)
		: QTextEdit(parent)
	{

	}
private:
	QTimer* times = nullptr;
public slots:
	void showEvent(QShowEvent *)
	{
		times = new QTimer();
		times->setSingleShot(true);
		connect(times, &QTimer::timeout, [=]()
		{
			delete this;
		});
		times->start(2000);
	}

};


class QMySlider : public QSlider
{
	Q_OBJECT
public:
	QMySlider(Qt::Orientation orientation,QWidget *parent= nullptr)
		: QSlider(orientation,parent)
	{

	}
	void wheelEvent(QWheelEvent* e)
	{
		QWidget::wheelEvent(e);
		//��ע��ǧ���ʹ��QSlider::wheelEvent(e);�������û��д
	}
};

class QMyTreeWidget : public QTreeWidget
{
	Q_OBJECT

private:
	YAML::Node _param;
public:
	QMyTreeWidget(QWidget *parent);
	~QMyTreeWidget();
	bool ReadYAMLFile(QString filepath);
	bool SaveYAMLFile(QString filepath);
public slots:
	void SliderValueChanged(int);
	void CheckValueChanged(int);
	void showCnDetail(QTreeWidgetItem*, int);
};
