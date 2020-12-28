#pragma once
#include <QtCore/QtCore>
#include <QTreeWidget>
#include <QObject>
#include "yaml-cpp/yaml.h"
#if DEBUG
#pragma comment(lib,"yaml-cppd.lib")
#else
#pragma comment(lib,"yaml-cpp.lib")

#endif

#include <QTextEdit>
#include <QTimer>
#include <QSlider>
#include <QLineEdit>

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
		times = new QTimer(this);
		times->setSingleShot(true);
		connect(times, &QTimer::timeout, [=]()
		{
			this->setVisible(false);
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
		//备注，千万别使用QSlider::wheelEvent(e);否则等于没重写
	}
};

class QMyTreeWidget : public QTreeWidget
{
	Q_OBJECT
signals:
	void TempSave(QString);
private:
	YAML::Node _param;
	QTextDocument *document = nullptr;
	QTextEdit *editor = nullptr;
	QMyTextEdit * detailtext = nullptr;
public:
	YAML::Node _mparam;
	QMyTreeWidget(QWidget *parent);
	~QMyTreeWidget();
	bool ReadYAMLFile(QString filepath);
	bool ReadYAMLFile(YAML::Node, char* cameraname = nullptr);
	bool SaveYAMLFile(QString filepath);
public slots:
	void SliderValueChanged(int);
	void ComboValueChanged(int);
	void CheckValueChanged(int);
	void LineValueChanged(QString);
	void showCnDetail(QTreeWidgetItem*, int);
	void textAreaChanged();
};
