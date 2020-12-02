#include "QMyTreeWidget.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <fstream>
#include <QHeaderView>
#include <QFontDialog>
#include <QScrollBar>
QMyTreeWidget::QMyTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	
	this -> setHeaderHidden(true);
	this->setColumnCount(4);
	this->header()->hideSection(3);
	this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	QObject::connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(showCnDetail(QTreeWidgetItem*, int)));

	this->setStyleSheet("QTreeWidget::item{height:60px;}\
	QTreeView::branch:has-children:!has-siblings:closed,\
		QTreeView::branch:closed:has-children:has-siblings\
		{\
			padding: -3px;\
			image:url(./340.png);\
		}\
		QTreeView::branch:open:has-children:!has-siblings,\
			QTreeView::branch:open:has-children:has-siblings\
		{\
			padding: -3px;\
			image:url(./440.png);\
		}");

	this->verticalScrollBar()->setStyleSheet("QScrollBar{background:White; width: 40px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	this->setFont(QFont(QString::fromLocal8Bit("����"), 20));
}
enum MyEnum
{
	SCALAR = 0,
};
struct CharacterType {
	std::string type;
	int value;
	int Smallest;
	int Biggest;
	std::string CN_Discrib;
};
struct ERRORSetting {
	int ability;
	int value;
};
namespace YAML {
	template<>
	struct convert<CharacterType> {
		static bool decode(const Node& node, CharacterType& cType) {
			cType.type = node["type"].as<std::string>();
			cType.value = node["value"].as<int>();
			cType.Smallest = node["Smallest"].as<int>();
			cType.Biggest = node["Biggest"].as<int>();
			cType.CN_Discrib = node["CN_Discrib"].as<std::string>();
			return true;
		}
	};
	template<>
	struct convert<ERRORSetting> {

		static bool decode(const Node& node, ERRORSetting& cType) {
			cType.ability = node["ability"].as<int>();
			cType.value = node["value"].as<int>();
			return true;
		}
	};
}
bool QMyTreeWidget::ReadYAMLFile(QString filepath)
{
   



// 	YAML::Node  _param = YAML::LoadFile(filepath.toStdString());
// 
// 	
// 
// 
// 	int banthread = _param[QString::fromLocal8Bit("Error_ҩ��ȱ��").toStdString().c_str()][QString::fromLocal8Bit("ҩ����ֵ").toStdString().c_str()]["value"].as<int>();
// 
// 
// 
// 	return  false;
	

	this->clear();
	QFileInfo file(filepath);
	if (!file.isFile())
	{
		return false;
	}
	try
	{
		_param = YAML::LoadFile(filepath.toStdString());
		if (_param.IsNull())
		{
			return false;
		}
		int n;
		for (YAML::iterator it = _param.begin(); it != _param.end(); it++)
		{
			std::string name = it->first.as<std::string>();
			if (_param[name].IsDefined())
			{
				if (name.find("Error_")!=std::string::npos)
				{
					QTreeWidgetItem* pItemError = new QTreeWidgetItem(QStringList() << name.c_str());
					this->addTopLevelItem(pItemError);
					//if (_param[name].IsSequence())
					{
						YAML::Node commands = _param[name];


 						for (YAML::const_iterator it = commands.begin();it != commands.end();++it)
						{
 							std::string key = it->first.as<std::string>();       // <- key
							if (key == "Checkable")
							{
								//if (command.begin()->first.as<std::string>() == "ability")
								{
									QCheckBox* checkbox = new QCheckBox;
									checkbox->setObjectName(QString(name.c_str()) + "+" + key.c_str());

									checkbox->setStyleSheet("QCheckBox{ background:transparent}\
                            QCheckBox::indicator {width: 40px; height: 40px; }\
                            QCheckBox::indicator:unchecked{background:url(./240.png)}\
                            QCheckBox::indicator:checked{background:url(./540.png)}");

             /*               QCheckBox::indicator:unchecked{background:url(:/Qt_Test/240.png)}\
                            QCheckBox::indicator:checked{background:url(:/Qt_Test/140.png)}");
*/
									ERRORSetting ty = it->second.as<ERRORSetting>();
									if (it->second["value"].as<int>() == 0)
										checkbox->setChecked(false);
									else
										checkbox->setChecked(true);
									if (it->second["ability"].as<int>() == 0)
									{
										checkbox->setEnabled(false); 
										checkbox->setStyleSheet("QCheckBox{ background:transparent}\
                            QCheckBox::indicator {width: 40px; height: 40px; }\
                            QCheckBox::indicator:unchecked{background:url(./240.png)}\
                            QCheckBox::indicator:checked{background:url(./540.png)}");
									}
									else {
										checkbox->setStyleSheet("QCheckBox{ background:transparent}\
                            QCheckBox::indicator {width: 40px; height: 40px; }\
                            QCheckBox::indicator:unchecked{background:url(./240.png)}\
                            QCheckBox::indicator:checked{background:url(./140.png)}");
									}
									this->setItemWidget(pItemError, 1, checkbox);
									QObject::connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(CheckValueChanged(int)));
								}
							}
							else
							{
								QTreeWidgetItem *pItem = new QTreeWidgetItem(QStringList() << key.c_str());
								pItemError->addChild(pItem);
								CharacterType ty = it->second.as<CharacterType>();
								if (ty.type == "Scale")
								{

									QMySlider * control = new QMySlider(Qt::Horizontal);
									control->setObjectName(QString(name.c_str())+ "+" + key.c_str());
									control->setMinimum(ty.Smallest);
									control->setMaximum(ty.Biggest);
									control->setValue(ty.value);

									control->setStyleSheet("  \
     QSlider::add-page:Horizontal\
     {     \
        background-color: rgb(87, 97, 106);\
        height:4px;\
     }\
     QSlider::sub-page:Horizontal \
    {\
        background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(7,208,255, 255), stop:1 rgba(7,208,255, 255));\
        height:4px;\
     }\
    QSlider::groove:Horizontal \
    {\
        background:transparent;\
        height:6px;\
    }\
    QSlider::handle:Horizontal \
    {\
        height: 25px;\
        width:35px;\
        border-image: url(./btn.png);\
        margin: -15 0px; \
    }\
    ");
// 									QLineEdit* le = new QLineEdit();
// 									le->setObjectName("le" + QString(name.c_str()) + "+" + key.c_str());
// 									le->setText(QString::number(ty.value));
// 									le->setEnabled(false);
									pItem->setData(1, Qt::DisplayRole, ty.value);
									pItem->setData(3, Qt::DisplayRole, ty.CN_Discrib.c_str());
									this->setItemWidget(pItem, 2, control);


									QObject::connect(control, SIGNAL(valueChanged(int)),this,SLOT(SliderValueChanged(int)));
								}
							}
 						}
					}
 				}
			}
		}
	}
	catch (YAML::ParserException e)
	{
		QMessageBox::warning(nullptr, "", e.what());
	}
	catch (YAML::RepresentationException e)
	{
		QMessageBox::warning(nullptr, "", e.what());
	}
	catch (YAML::Exception e)
	{
		QMessageBox::warning(nullptr, "", e.what());
	}
	return false;
}

bool QMyTreeWidget::SaveYAMLFile(QString filepath)
{

	std::ofstream fout(filepath.toStdString().c_str());

		fout << _param;

		fout.close();

	try
	{
	}
	catch (YAML::ParserException e)
	{
		QMessageBox::warning(nullptr, "", e.what());
	}
	catch (YAML::RepresentationException e)
	{
		QMessageBox::warning(nullptr, "", e.what());
	}
	catch (YAML::Exception e)
	{
		QMessageBox::warning(nullptr, "", e.what());
	}
	return false;
}

void QMyTreeWidget::CheckValueChanged(int i)
{
	QCheckBox* sind = qobject_cast<QCheckBox*>(QObject::sender());
	QString objectname = sind->objectName();
	QString errtype = objectname.left(objectname.indexOf("+"));
	QString errname = objectname.mid(objectname.indexOf("+") + 1);
	_param[errtype.toStdString().c_str()][errname.toStdString().c_str()]["value"] = i;
}

void QMyTreeWidget::showCnDetail(QTreeWidgetItem * pItem, int i)
{
	if (i!=2)
	{
		QString sts = pItem->data(3, Qt::DisplayRole).toString();
		if (sts.length()==0)
		{
			return;
		}
		QPoint pt = cursor().pos();
		QMyTextEdit * detailtext = new QMyTextEdit(this);
		pt = this->mapFromGlobal(pt);
		detailtext->setGeometry(pt.x(), pt.y(), 100, 50);
		detailtext->topLevelWidget();
		detailtext->setText(pItem->data(3,Qt::DisplayRole).toString());
		detailtext->setWindowFlag(Qt::FramelessWindowHint);
		detailtext->show();
		
	}



}

void QMyTreeWidget::SliderValueChanged(int i)
{
	QSlider* sind = qobject_cast<QSlider*>(QObject::sender());
	QString objectname = sind->objectName();
	QString errtype = objectname.left(objectname.indexOf("+") );
	QString errname = objectname.mid(objectname.indexOf("+") +1);
	_param[errtype.toStdString().c_str()][errname.toStdString().c_str()]["value"] = i;
	QTreeWidgetItem* cur = this->currentItem();
	cur->setData(1, Qt::DisplayRole, i);

}

QMyTreeWidget::~QMyTreeWidget()
{
}
