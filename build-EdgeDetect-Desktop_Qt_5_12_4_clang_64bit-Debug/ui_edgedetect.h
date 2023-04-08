/********************************************************************************
** Form generated from reading UI file 'edgedetect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDGEDETECT_H
#define UI_EDGEDETECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_edgeDetect
{
public:
    QPushButton *select;
    QPushButton *edgeDetect_2;
    QLabel *originImg;
    QLabel *edgeDetectedImg;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *clearButton;

    void setupUi(QDialog *edgeDetect)
    {
        if (edgeDetect->objectName().isEmpty())
            edgeDetect->setObjectName(QString::fromUtf8("edgeDetect"));
        edgeDetect->resize(681, 277);
        select = new QPushButton(edgeDetect);
        select->setObjectName(QString::fromUtf8("select"));
        select->setGeometry(QRect(530, 100, 113, 32));
        edgeDetect_2 = new QPushButton(edgeDetect);
        edgeDetect_2->setObjectName(QString::fromUtf8("edgeDetect_2"));
        edgeDetect_2->setGeometry(QRect(530, 150, 113, 32));
        originImg = new QLabel(edgeDetect);
        originImg->setObjectName(QString::fromUtf8("originImg"));
        originImg->setGeometry(QRect(20, 60, 231, 181));
        originImg->setMaximumSize(QSize(16777215, 16777215));
        edgeDetectedImg = new QLabel(edgeDetect);
        edgeDetectedImg->setObjectName(QString::fromUtf8("edgeDetectedImg"));
        edgeDetectedImg->setGeometry(QRect(270, 60, 231, 181));
        edgeDetectedImg->setMaximumSize(QSize(16777215, 16777215));
        label_3 = new QLabel(edgeDetect);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 30, 60, 16));
        label_4 = new QLabel(edgeDetect);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(280, 30, 81, 16));
        clearButton = new QPushButton(edgeDetect);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(530, 200, 113, 32));

        retranslateUi(edgeDetect);

        QMetaObject::connectSlotsByName(edgeDetect);
    } // setupUi

    void retranslateUi(QDialog *edgeDetect)
    {
        edgeDetect->setWindowTitle(QApplication::translate("edgeDetect", "edgeDetect", nullptr));
        select->setText(QApplication::translate("edgeDetect", "\351\200\211\346\213\251\345\233\276\347\211\207", nullptr));
        edgeDetect_2->setText(QApplication::translate("edgeDetect", "\345\244\204\347\220\206\345\233\276\347\211\207", nullptr));
        originImg->setText(QString());
        edgeDetectedImg->setText(QString());
        label_3->setText(QApplication::translate("edgeDetect", "\345\216\237\345\233\276\357\274\232", nullptr));
        label_4->setText(QApplication::translate("edgeDetect", "\345\244\204\347\220\206\345\220\216\345\233\276\347\211\207\357\274\232", nullptr));
        clearButton->setText(QApplication::translate("edgeDetect", "\346\270\205\347\251\272\345\233\276\347\211\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class edgeDetect: public Ui_edgeDetect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDGEDETECT_H
