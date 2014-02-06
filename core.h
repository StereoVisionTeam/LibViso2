#ifndef CORE_H
#define CORE_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <QString>
#include <QDebug>
#include <fstream>
#include <iostream>
#include "libviso2/viso_mono.h"
//#include "libviso2/viso_stereo.h"
//#include <png++/png.hpp>
/*-----------------------------------------

-Dopracowanie zapisu do pliku

-Dokumentacja na 31
    -ogólny opis pracy
    -załącznik licencje
    -Technikalia:
    --format zapisu do pliku.
    --opis parametrów do ustawienia przed odpaleniem
-Po 10 razy algorytm przemielić
*/

class Core
{
public:
    Core();

//Calibration
    bool addSampleToCalibration(cv::Mat &calibrationImage);
    bool calibrateCamera(std::string outputURL);
    void setPatternSize(cv::Size x){ patternSize =x;
                                 isSetPatternSizeDone=true;}
    void setPatternSize(int x, int y){ patternSize = cv::Size(x,y);
                                       isSetPatternSizeDone=true;}
    void setImageSize(cv::Size x) { imageSize= x;
                                isSetImageSizeDone=true;}
    void setsquareSize(double x){ squareSize= x;}
    void saveCalibration(std::string outputURL);
    void loadCalibration(std::string inputURL);
    void calibrateFromImages(std::string inputURL,std::string outputCalibrationDataURL, int numberOfSamples);

    bool addImgToOdometry(cv::Mat img);

    Matrix pose;
    std::ofstream fileWithOdometry;

private:

//Calibration
    int calibrationSamplesCounter;
    int goodSamplesCounter;
    bool isCalibrationDone;
    int minimumPatternSizeWidth; //default =2
    int minimumPatternSizeHeight; //default =2
    double squareSize; //size of squares in calibration chessboard;
    cv::vector<cv::vector<cv::Point3f> > objectPoints;
    int minimumSamplesForCalibration; //default =8    
    cv::Size patternSize; //Number of inner corners per a chessboard row and column for (8x8) board it's (7x7)
    bool isSetPatternSizeDone;
    cv::Mat cameraMatrix, distCoeffs;
    cv::vector<cv::vector<cv::Point2f> > imagePoints;

//Odometry
    VisualOdometryMono *viso;

//Basic
    cv::Size imageSize;
    bool isSetImageSizeDone;

};

#endif // CORE_H
