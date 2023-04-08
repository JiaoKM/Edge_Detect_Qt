#include "edgedetect.h"
#include "ui_edgedetect.h"

edgeDetect::edgeDetect(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::edgeDetect)
{
    ui->setupUi(this);
}

edgeDetect::~edgeDetect()
{
    delete ui;
}


void edgeDetect::on_select_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("选择图像"),"",
                                            tr("Images(*.png *.bmp *.jpg *.jpeg)"));
    if (!filename.isEmpty())
    {
        QImage *tmp = new QImage;
        if (!(tmp->load(filename)))
        {
            QMessageBox::information(this, tr("打开图像失败"),
                                     tr("打开图像失败！"));
            delete tmp;
        }
        else {
            img = tmp;
            flag = 1;
        }

        ui->originImg->setScaledContents(true);
        ui->originImg->setPixmap(QPixmap::fromImage(*img));
    }
}


void edgeDetect::on_edgeDetect_2_clicked()
{
    if (flag == 0)
    {
        QMessageBox::critical(this, tr("提示"), tr("原图未加载"));
        //return;
    }
    else {
        grayScale();
        gauss_process();
        calculate();
        nonMaximumSuppression();
        doubleThresholdDetection();
        suppressLowThreshold();
    }
}

void edgeDetect::on_clearButton_clicked()
{
    ui->originImg->clear();
    ui->edgeDetectedImg->clear();
    flag = 0;
}

void edgeDetect::grayScale()
{
    img_gray = new QImage(img->width(), img->height(), QImage::Format_ARGB32);
    QColor old_color;
    int average;

    for (int i = 0; i < img_gray->width(); ++i) {
        for (int j = 0; j < img_gray->height(); ++j) {
            old_color = img->pixel(i, j);
            average = (old_color.red() + old_color.green()
                       + old_color.blue()) / 3;
            img_gray->setPixel(i, j, qRgb(average, average, average));
        }
    }
    //ui->edgeDetectedImg->setScaledContents(true);
    //ui->edgeDetectedImg->setPixmap(QPixmap::fromImage(*img_gray));
}

void edgeDetect::generic_gauss(double kernel[10][10], int size, double sigma)
{
    double pi = acos(-1);
    double sum = 0;
    int mk = size / 2;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] = exp(-sqrt(pow(i - mk, 2) + pow(j - mk, 2))
                               / (2 * sigma * sigma));
            kernel[i][j] /= 2 * pi * sigma *sigma;
            sum += kernel[i][j];
        }
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] /= sum;
        }
    }
}

void edgeDetect::gauss_process()
{
    int k = 5;
    double kernel[10][10];
    generic_gauss(kernel, k, 1.4);
    QImage* t_img = new QImage(img_gray->width() + k - 1,
                               img_gray->height() + k - 1,
                               QImage::Format_ARGB32);
    img_gauss = new QImage(img_gray->width(), img_gray->height(),
                           QImage::Format_ARGB32);

    //lefttop
    for (int i = 0; i < k / 2; ++i) {
        for (int j = 0; j < k / 2; ++j) {
            t_img->setPixel(i, j, img_gray->pixel(0,0));
        }
    }
    //righttop
    for (int i = img_gray->width() + k / 2; i < t_img->width(); ++i) {
        for (int j = 0; j < k / 2; ++j) {
            t_img->setPixel(i, j, img_gray->pixel(img_gray->width() -1, 0));
        }
    }
    //rightbottom
    for (int i = img_gray->width() + k / 2; i < t_img->width(); ++i) {
        for (int j = img_gray->height() + k / 2; j < t_img->height(); ++j) {
            t_img->setPixel(i, j, img_gray->pixel(img_gray->width() - 1, img_gray->height() - 1));
        }
    }
    //leftbottom
    for (int i = 0; i < k / 2; ++i) {
        for (int j = img_gray->height() + k / 2; j < t_img->height(); ++j) {
            t_img->setPixel(i, j, img_gray->pixel(0, img_gray->height() - 1));
        }
    }
    //top
    for (int i = 0; i < img_gray->width(); ++i) {
        for (int j = 0; j < k / 2; ++j) {
            t_img->setPixel(i + k / 2, j, img_gray->pixel(i, 0));
        }
    }
    //right
    for (int i = t_img->width() - k /2; i < t_img->width(); ++i) {
        for (int j = 0; j < img_gray->height(); ++j) {
            t_img->setPixel(i, j + k / 2, img_gray->pixel(img_gray->width() - 1, j));
        }
    }
    //bottom
    for (int i = 0; i < img_gray->width(); ++i) {
        for (int j = t_img->height() - k / 2; j < t_img->height(); ++j) {
            t_img->setPixel(i + k / 2, j, img_gray->pixel(i, img_gray->height() - 1));
        }
    }
    //left
    for (int i = 0; i < k / 2; ++i) {
        for (int j = 0; j < img_gray->height(); ++j) {
            t_img->setPixel(i, j + k / 2, img_gray->pixel(0, j));
        }
    }
    //mid
    for (int i = 0; i < img_gray->width(); ++i) {
        for (int j = 0; j < img_gray->height(); ++j) {
            t_img->setPixel(i + k / 2, j + k / 2, img_gray->pixel(i, j));
        }
    }

    for (int i = k / 2; i < t_img->width() - k / 2; ++i) {
        for (int j = k / 2; j < t_img->height() - k / 2; ++j) {
            double tmp = 0;
            for (int x = 0; x < k; ++x) {
                for (int y = 0; y < k; ++y) {
                    tmp += kernel[x][y] * qRed(t_img->pixel(i - k / 2 + x, j - k / 2 + y));
                }
            }
            img_gauss->setPixel(i - k / 2, j - k / 2, qRgb(tmp, tmp, tmp));
        }
    }
    //ui->edgeDetectedImg->setScaledContents(true);
    //ui->edgeDetectedImg->setPixmap(QPixmap::fromImage(*img_gauss));
}

void edgeDetect::calculate()
{
    gradx = new double*[img->width()];
    for (int i = 0; i < img->width(); ++i)
        gradx[i] = new double[img->height()];
    grady = new double*[img->width()];
    for (int i = 0; i < img->width(); ++i)
        grady[i] = new double[img->height()];
    grad = new double*[img->width()];
    for (int i = 0; i < img->width(); ++i)
        grad[i] = new double[img->height()];
    dir = new double*[img->width()];
    for (int i = 0; i < img->width(); ++i)
        dir[i] = new double[img->height()];
    int k = 3;
    double kernelx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    double kernely[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    QImage* t_img = new QImage(img_gray->width() + k - 1,
                               img_gray->height() + k - 1,
                               QImage::Format_ARGB32);
    //lefttop
    for (int i = 0; i < k / 2; ++i) {
        for (int j = 0; j < k / 2; ++j) {
            t_img->setPixel(i, j, img_gauss->pixel(0,0));
        }
    }
    //righttop
    for (int i = img_gray->width() + k / 2; i < t_img->width(); ++i) {
        for (int j = 0; j < k / 2; ++j) {
            t_img->setPixel(i, j, img_gauss->pixel(img_gauss->width() -1, 0));
        }
    }
    //rightbottom
    for (int i = img_gray->width() + k / 2; i < t_img->width(); ++i) {
        for (int j = img_gray->height() + k / 2; j < t_img->height(); ++j) {
            t_img->setPixel(i, j, img_gauss->pixel(img_gauss->width() - 1, img_gauss->height() - 1));
        }
    }
    //leftbottom
    for (int i = 0; i < k / 2; ++i) {
        for (int j = img_gray->height() + k / 2; j < t_img->height(); ++j) {
            t_img->setPixel(i, j, img_gauss->pixel(0, img_gauss->height() - 1));
        }
    }
    //top
    for (int i = 0; i < img_gray->width(); ++i) {
        for (int j = 0; j < k / 2; ++j) {
            t_img->setPixel(i + k / 2, j, img_gauss->pixel(i, 0));
        }
    }
    //right
    for (int i = t_img->width() - k /2; i < t_img->width(); ++i) {
        for (int j = 0; j < img_gray->height(); ++j) {
            t_img->setPixel(i, j + k / 2, img_gauss->pixel(img_gauss->width() - 1, j));
        }
    }
    //bottom
    for (int i = 0; i < img_gray->width(); ++i) {
        for (int j = t_img->height() - k / 2; j < t_img->height(); ++j) {
            t_img->setPixel(i + k / 2, j, img_gauss->pixel(i, img_gauss->height() - 1));
        }
    }
    //left
    for (int i = 0; i < k / 2; ++i) {
        for (int j = 0; j < img_gray->height(); ++j) {
            t_img->setPixel(i, j + k / 2, img_gauss->pixel(0, j));
        }
    }
    //mid
    for (int i = 0; i < img_gray->width(); ++i) {
        for (int j = 0; j < img_gray->height(); ++j) {
            t_img->setPixel(i + k / 2, j + k / 2, img_gauss->pixel(i, j));
        }
    }

    for (int i = k / 2; i < t_img->width() - k / 2; ++i) {
        for (int j = k / 2; j < t_img->height() - k / 2; ++j) {
            double tmpx = 0;
            double tmpy = 0;
            for (int x = 0; x < k; ++x) {
                for (int y = 0; y < k; ++y) {
                    tmpx += kernelx[x][y] * qRed(t_img->pixel(i - k / 2 + x, j - k / 2 + y));
                    tmpy += kernely[x][y] * qRed(t_img->pixel(i - k / 2 + x, j - k / 2 + y));
                }
            }
            gradx[i - k / 2][j - k / 2] = tmpx;
            grady[i - k / 2][j - k / 2] = tmpy;
            grad[i - k / 2][j - k / 2] = sqrt(pow(tmpx, 2) + pow(tmpy, 2));
            double theta = atan(tmpy / tmpx) + 90;
            if (theta >= 0 && theta < 45)
                dir[i - k / 2][j - k / 2] = 2;
            else if (theta >= 45 && theta < 90)
                dir[i - k / 2][j - k / 2] = 3;
            else if (theta >= 90 && theta < 135)
                dir[i - k / 2][j - k / 2] = 0;
            else
                dir[i - k / 2][j - k / 2] = 1;
        }
    }
}

void edgeDetect::nonMaximumSuppression()
{
    img_nms = new QImage(img_gauss->width(), img_gauss->height(),
                         QImage::Format_ARGB32);

    double tmp = 0;
    for (int i = 0; i < img_nms->width(); ++i) {
        for (int j = 0; j < img_nms->height(); ++j) {
            tmp += grad[i][j];
        }
    }
    //tmp /= (img_nms->width() * img_nms->height());
    //highThresh *= tmp;
    //lowThresh *= tmp;

    double N, NE, E, SW, W, SE, S, NW;
    double grad1 = 0, grad2 = 0, tantheta = 0;
    for (int i = 1; i < img_nms->width() - 1; ++i) {
        for (int j = 1; j < img_nms->height() - 1; ++j) {
            N = grad[i][j - 1];
            NE = grad[i + 1][j - 1];
            E = grad[i + 1][j];
            SW = grad[i - 1][j + 1];
            W = grad[i - 1][j];
            SE = grad[i + 1][j + 1];
            S = grad[i][j + 1];
            NW = grad[i - 1][j - 1];

            if (dir[i][j] == 0) {
                tantheta = fabs(grady[i][j] / gradx[i][j]);
                grad1 = E * (1 - tantheta) + NE * tantheta;
                grad2 = W * (1 - tantheta) + SW * tantheta;
            }
            else if (dir[i][j] == 1) {
                tantheta = fabs(grady[i][j] / gradx[i][j]);
                grad1 = N * (1 - tantheta) + NE * tantheta;
                grad2 = S * (1 - tantheta) + SW * tantheta;
            }
            else if (dir[i][j] == 2) {
                tantheta = fabs(grady[i][j] / gradx[i][j]);
                grad1 = N * (1 - tantheta) + NW * tantheta;
                grad2 = S * (1 - tantheta) + SE * tantheta;
            }
            else if (dir[i][j] == 3) {
                tantheta = fabs(grady[i][j] / gradx[i][j]);
                grad1 = W * (1 - tantheta) + NW * tantheta;
                grad2 = E * (1 - tantheta) + SE * tantheta;
            }
            //else {
                //grad1 = 0.5;
                //grad2 = 0.5;
            //}

            if (grad[i][j] > grad1 && grad[i][j] > grad2) {
                img_nms->setPixel(i, j, qRgb(255, 255, 255));
                grad[i][j] = highThresh + 1;
            }
            else {
                img_nms->setPixel(i, j ,qRgb(0, 0, 0));
                grad[i][j] = lowThresh + 1;
            }
        }
    }
    //ui->edgeDetectedImg->setScaledContents(true);
    //ui->edgeDetectedImg->setPixmap(QPixmap::fromImage(*img_nms));
}

void edgeDetect::doubleThresholdDetection()
{
    img_dtd = new QImage(img_nms->width(), img_nms->height(),
                         QImage::Format_ARGB32);
    gradcp = new double*[img_nms->width()];
    for (int i = 0; i < img_nms->width(); ++i)
        gradcp[i] = new double[img_nms->height()];

    for (int i = 0; i < img_dtd->width(); ++i) {
        for (int j = 0; j < img_dtd->height(); ++j) {
            if (grad[i][j] > highThresh) {
                gradcp[i][j] = highThresh;
                img_dtd->setPixel(i, j, qRgb(255, 255, 255));
            }
            else if (grad[i][j] > lowThresh) {
                gradcp[i][j] = lowThresh;
                img_dtd->setPixel(i, j, qRgb(255, 255, 255));
            }
            else {
                gradcp[i][j] = 0;
                img_dtd->setPixel(i, j, qRgb(0, 0, 0));
            }
        }
    }
    //ui->edgeDetectedImg->setScaledContents(true);
    //ui->edgeDetectedImg->setPixmap(QPixmap::fromImage(*img_dtd));
}

void edgeDetect::suppressLowThreshold()
{
    img_slt = new QImage(img_dtd->width(), img_dtd->height(),
                         QImage::Format_ARGB32);

    int frac[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1},
                      {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

    for (int i = 1; i < img_slt->width() - 1; ++i) {
        for (int j = 1; j < img_slt->height() - 1; ++j) {
            if (gradcp[i][j] == highThresh)
                img_slt->setPixel(i, j, qRgb(255, 255, 255));
            else if (gradcp[i][j] == lowThresh) {
                for (int k = 0; k < 8; ++k) {
                    if (gradcp[i + frac[k][0]][j + frac[k][1]] == highThresh) {
                        img_slt->setPixel(i, j, qRgb(255, 255, 255));
                        break;
                    }
                    img_slt->setPixel(i, j, qRgb(0, 0, 0));
                }
            }
            else
                img_slt->setPixel(i, j, qRgb(0, 0, 0));
        }
    }

    ui->edgeDetectedImg->setScaledContents(true);
    ui->edgeDetectedImg->setPixmap(QPixmap::fromImage(*img_slt));
}
