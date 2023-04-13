#include "homework1.h"
#include <QtWidgets/QApplication>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    homework1 w;
    w.show();
    return a.exec();
}
