#ifndef EDGEDETECT_H
#define EDGEDETECT_H

#include <QDialog>
#include <QString>
#include <QImage>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QColor>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class edgeDetect; }
QT_END_NAMESPACE

class edgeDetect : public QDialog
{
    Q_OBJECT

public:
    edgeDetect(QWidget *parent = nullptr);
    ~edgeDetect();
    void grayScale();
    void generic_gauss(double kernel[10][10], int size, double sigma);
    void gauss_process();
    void calculate();
    void nonMaximumSuppression();
    void doubleThresholdDetection();
    void suppressLowThreshold();

private slots:
    void on_select_clicked();

    void on_edgeDetect_2_clicked();

    void on_clearButton_clicked();

private:
    Ui::edgeDetect *ui;
    QImage* img;
    QImage* img_gray;
    QImage* img_gauss;
    QImage* img_nms;
    QImage* img_dtd;
    QImage* img_slt;
    double **gradx, **grady, **grad, **dir, **gradcp;
    double highThresh = 100, lowThresh = 50;
    int flag = 0;
};
#endif // EDGEDETECT_H
