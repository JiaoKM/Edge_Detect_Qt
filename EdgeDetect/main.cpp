#include "edgedetect.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    edgeDetect w;
    w.show();
    return a.exec();
}
