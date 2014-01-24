#ifndef CORE_H
#define CORE_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QString>
#include "libviso2/viso_mono.h"
#include <png++/png.hpp>

class Core
{
public:
    Core();

    bool addSampleToCalibration(cv::Mat &calibrationImage);
    bool calibrateCamera(QString url);
    void setPatternSize(cv::Size x){ patternSize =x;
                                 isSetPatternSizeDone=true;}
    void setPatternSize(int x, int y){ patternSize = cv::Size(x,y);
                                       isSetPatternSizeDone=true;}
    void setImageSize(cv::Size x) { imageSize= x;
                                isSetImageSizeDone=true;}
    void setsquareSize(double x){ squareSize= x;}
    void saveCalibration(std::string path);
    void loadCalibration(std::string path);
    bool addImgToOdometry(cv::Mat img, int frameNo, QString urlToFile);
    bool calibOk;


private:
    int calibrationSamplesCounter;
    int goodSamplesCounter;
    bool isSetPatternSizeDone;
    bool isSetImageSizeDone;

    int minimumPatternSizeWidth; //default =2
    int minimumPatternSizeHeight; //default =2

    double squareSize; //size of squares in calibration chessboard;
    cv::vector<cv::vector<cv::Point3f> > objectPoints;

    int minimumSamplesForCalibration; //default =8
    //Number of inner corners per a chessboard row and column for (8x8) board it's (7x7)
    cv::Size patternSize;

    cv::Size imageSize;
    cv::Mat cameraMatrix, distCoeffs;
    cv::vector<cv::vector<cv::Point2f> > imagePoints;
    VisualOdometryMono *viso;


};

#endif // CORE_H
