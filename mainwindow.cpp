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
    QString url = "/home/wiktor/Pictures/Projects/MeilDataSet/2013_11_26/Calibration/try2/";
  /*  for(int i=1; i<9;i++){
         QString left=url+"00000"+QString::number(i)+".JPG";
         std::cout<<i;
         std::string leftS=left.toStdString();
         leftImage = cv::imread(leftS,CV_LOAD_IMAGE_GRAYSCALE);
         core.setImageSize(leftImage.size());
         core.addSampleToCalibration(leftImage);
    }*/

    //core.calibrateCamera(url+"matrix.xml");
    core.loadCalibration((url+"matrix.xml").toUtf8().constData());
 QString url2 = "/home/wiktor/Pictures/Projects/MeilDataSet/2013_11_26/line_0_degrees/";

   /*     cv::Mat Img=cv::imread(url2.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
      url2 = "/home/wiktor/Pictures/Projects/MeilDataSet/2013_11_26/line_0_degrees/000001.JPG";
     core.addImgToOdometry(Img,0,"pose.xml");
      Img=cv::imread(url2.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
     core.addImgToOdometry(Img,1,"pose.xml");
   */
    for(int i=0; i<9;i++){
             QString ImgPath=url2+"00000"+QString::number(i)+".JPG";
            // std::cout<<ImgPath.toStdString();
             cv::Mat Img=cv::imread(ImgPath.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
             core.addImgToOdometry(Img,i,"pose.xml");
    }
std::cout<<"end constructor";
}

MainWindow::~MainWindow()
{
    delete ui;
}
