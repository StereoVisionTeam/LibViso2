#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <stdlib.h>
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cv::Mat leftImage;
    Core core;
    core.setPatternSize(9,6);
    //core.setImageSize(cv::Size(image.cols,image.rows));
    QString url = "/home/haskis/Pictures/Projects/Calibration/";
    for(int i=1; i<9;i++){
         QString left=url+"left0"+QString::number(i)+".jpg";

         std::string leftS=left.toStdString();
         leftImage = cv::imread(leftS,CV_LOAD_IMAGE_GRAYSCALE);
         core.setImageSize(leftImage.size());
         core.addSampleToCalibration(leftImage);
    }
    core.calibrateCamera();
    std::cout<<"end constructor";


}

MainWindow::~MainWindow()
{
    delete ui;
}
