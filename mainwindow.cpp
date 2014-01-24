#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#include <string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug()<<"Dobule"<<sizeof(double);
    ui->setupUi(this);

    cv::Mat leftImage;
    Core core;
    core.setPatternSize(9,6);
    //core.setImageSize(cv::Size(image.cols,image.rows));
    QString url = "/home/haskis/Documents/KalibracjaFilm";
    if(false){
        for(int i=1; i<9;i++)
        {
            qDebug()<<i;
            QString left=url+"00000"+QString::number(i)+".JPG";

            std::string leftS=left.toStdString();
            leftImage = cv::imread(leftS,CV_LOAD_IMAGE_GRAYSCALE);
            core.setImageSize(leftImage.size());
            core.addSampleToCalibration(leftImage);
        }

    core.calibrateCamera(url+"matrix.xml");
    }
    else
        core.loadCalibration((url+".xml").toUtf8().constData());

    QString url2 = "/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0021/";

   /*     cv::Mat Img=cv::imread(url2.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
      url2 = "/home/wiktor/Pictures/Projects/MeilDataSet/2013_11_26/line_0_degrees/000001.JPG";
     core.addImgToOdometry(Img,0,"pose.xml");
      Img=cv::imread(url2.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
     core.addImgToOdometry(Img,1,"pose.xml");
   */
    core.fileWithOdometry.open("/home/haskis/Documents/zFilmu.txt");
    if(core.fileWithOdometry.is_open())
        qDebug()<<"Zapisuję dane z odometrii";
    else
        qDebug()<<"cant open";

    for(int i=0; i<1513;i++){
            char base_name[256]; std::sprintf(base_name,"%06d.png",i);
            std::string  ImgPath  = url2.toStdString() + "/I1_" + base_name;

             cv::Mat Img=cv::imread(ImgPath,CV_LOAD_IMAGE_GRAYSCALE);
             core.addImgToOdometry(Img,i,"pose.xml");
             core.fileWithOdometry<<core.pose<<std::endl;
    }
    core.fileWithOdometry.flush();
    core.fileWithOdometry.close();
    qDebug()<<"Core constructor End";
}
MainWindow::~MainWindow()
{
    delete ui;
}
