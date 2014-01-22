#include "core.h"

using namespace cv;
using namespace std;
Core::Core():
    calibrationSamplesCounter(0),
    goodSamplesCounter(0),
    isSetPatternSizeDone(false),
    isSetImageSizeDone(false),
    minimumPatternSizeHeight(2),
    minimumPatternSizeWidth(2),
    minimumSamplesForCalibration(4),
    squareSize(0.1)
{
}


bool Core::addSampleToCalibration(Mat &calibrationImage){

    if(!isSetPatternSizeDone){
        cerr<<"SetPatternSize is not done (in Core::addSampleToCalibration)";
        return false;
    }
    if(patternSize.width<minimumPatternSizeWidth ||
       patternSize.height<minimumPatternSizeHeight){
        cerr<<"PatternSize is lower that required minimum (in Core::addSampleToCalibration)";
        return false;
    }
    if(!isSetImageSizeDone){
        cerr<<"calibration image size is not set ( in Core::addSampleToCalibration)";
        return false;
    }
    if(calibrationImage.cols != imageSize.width || calibrationImage.rows!=imageSize.height){
        cerr<<"calibration image size is not equal to internal imageSize ( in Core::addSampleToCalibration)";
        return false;
    }


    cv::vector<Point2f> corners;

    bool result;

    result = findChessboardCorners(calibrationImage,
                                   patternSize,
                                   corners,
                                   CALIB_CB_ADAPTIVE_THRESH+CALIB_CB_NORMALIZE_IMAGE );

    //We accept only samples with full visibility of corners
    if(result)
        goodSamplesCounter++;
    else
        return false;

    //If we found corners then we need to do some subpix corners interpolation

    cornerSubPix(calibrationImage, corners, Size(11, 11), Size(-1, -1),
                 TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

    imagePoints.push_back(corners);

    return true;
}
bool Core::calibrateCamera(){

    if(goodSamplesCounter<minimumSamplesForCalibration){
        cerr<<"Not enough calibration samples (in Core::calibrateCamera)";
        return false;
    }

    //Assign values to objectPoints in object coordinate space
    for(int i = 0; i <goodSamplesCounter ; i++ ){
        objectPoints.push_back(vector<Point3f>());
        {
            for(int j = 0; j < patternSize.height; j++ )
                for(int k = 0; k < patternSize.width; k++ )
                    objectPoints[i].push_back(Point3f(float(j*squareSize), float(k*squareSize), 0));
        }
    }

    //For Check
  //  for(int i=0;i<objectPoints.size();i++)
    //    for(int j=0;j<objectPoints[i].size();j++)
      //      qDebug()<<objectPoints[i][j].x<<objectPoints[i][j].y<<objectPoints[i][j].z;

    vector<Mat> rvec, tvec;

    bool result = cv::calibrateCamera(objectPoints,imagePoints,imageSize,cameraMatrix,distCoeffs,rvec,tvec);
    std::cout<<"result"<<endl;
    std::cout<<cameraMatrix;

    return result;
}
