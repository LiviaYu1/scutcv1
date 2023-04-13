#pragma once

#include <QtWidgets/QWidget>
#include "ui_homework1.h"
#include<qpushbutton.h>
#include<qlabel.h>
#include<qfiledialog.h>
#include<qfileinfo.h>
#include<qfile.h>
#include<qpixmap.h>
#include <opencv2/opencv.hpp>
#include<string>
#include<sstream>
#include<qimage.h>

class homework1 : public QWidget
{
    Q_OBJECT

public:
    //button
    QPushButton* closeButton;
    QPushButton* choseButton;
    QPushButton* bilateralFilterButton;
    QPushButton* fourierButton;
    QPushButton* myFourierButton;
    //lable
    QLabel* sourcepic;
    QLabel* newpic;

    homework1(QWidget *parent = nullptr);
    ~homework1();

    QString fileurl;
public slots:
    void setsourcepic();
    void bilateralFilter();
    void fourier();
    void myfourier();
private:
    
    Ui::homework1Class *ui;
};
