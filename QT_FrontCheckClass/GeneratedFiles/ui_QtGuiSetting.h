/********************************************************************************
** Form generated from reading UI file 'QtGuiSetting.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGUISETTING_H
#define UI_QTGUISETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGuiSetting
{
public:
    QLabel *ShowLabel;
    QTableWidget *tableWidget;
    QListWidget *lw_ImageList;
    QPushButton *pB_Exit;
    QPushButton *pB_ContinueTest;
    QPushButton *pB_Save;

    void setupUi(QWidget *QtGuiSetting)
    {
        if (QtGuiSetting->objectName().isEmpty())
            QtGuiSetting->setObjectName(QString::fromUtf8("QtGuiSetting"));
        QtGuiSetting->setWindowModality(Qt::NonModal);
        QtGuiSetting->resize(768, 1366);
        QtGuiSetting->setMinimumSize(QSize(768, 1366));
        QtGuiSetting->setMaximumSize(QSize(768, 1366));
        QFont font;
        font.setPointSize(21);
        QtGuiSetting->setFont(font);
        ShowLabel = new QLabel(QtGuiSetting);
        ShowLabel->setObjectName(QString::fromUtf8("ShowLabel"));
        ShowLabel->setGeometry(QRect(2, 2, 764, 679));
        ShowLabel->setFrameShape(QFrame::Box);
        tableWidget = new QTableWidget(QtGuiSetting);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(2, 690, 380, 671));
        lw_ImageList = new QListWidget(QtGuiSetting);
        lw_ImageList->setObjectName(QString::fromUtf8("lw_ImageList"));
        lw_ImageList->setGeometry(QRect(390, 690, 371, 521));
        pB_Exit = new QPushButton(QtGuiSetting);
        pB_Exit->setObjectName(QString::fromUtf8("pB_Exit"));
        pB_Exit->setGeometry(QRect(650, 1230, 111, 91));
        pB_ContinueTest = new QPushButton(QtGuiSetting);
        pB_ContinueTest->setObjectName(QString::fromUtf8("pB_ContinueTest"));
        pB_ContinueTest->setGeometry(QRect(390, 1230, 121, 91));
        pB_ContinueTest->setCheckable(true);
        pB_Save = new QPushButton(QtGuiSetting);
        pB_Save->setObjectName(QString::fromUtf8("pB_Save"));
        pB_Save->setEnabled(false);
        pB_Save->setGeometry(QRect(520, 1230, 121, 91));

        retranslateUi(QtGuiSetting);

        QMetaObject::connectSlotsByName(QtGuiSetting);
    } // setupUi

    void retranslateUi(QWidget *QtGuiSetting)
    {
        QtGuiSetting->setWindowTitle(QCoreApplication::translate("QtGuiSetting", "QtGuiSetting", nullptr));
        ShowLabel->setText(QString());
        pB_Exit->setText(QCoreApplication::translate("QtGuiSetting", "\351\200\200\345\207\272", nullptr));
        pB_ContinueTest->setText(QCoreApplication::translate("QtGuiSetting", "\350\277\236\347\273\255\n"
"\346\265\213\350\257\225", nullptr));
        pB_Save->setText(QCoreApplication::translate("QtGuiSetting", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtGuiSetting: public Ui_QtGuiSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUISETTING_H
