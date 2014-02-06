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
    squareSize(0.1),
    viso(NULL),
    pose(Matrix::eye(4))
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
bool Core::calibrateCamera(std::string outputURL){

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

    vector<Mat> rvec, tvec;

    bool result = cv::calibrateCamera(objectPoints,imagePoints,imageSize,cameraMatrix,distCoeffs,rvec,tvec);
    //qDebug()<<"result"<<result<<endl;

    //for(int i=0;i<9;i++)
        //qDebug()<<cameraMatrix.at<float>(i);

    if(result){
        saveCalibration(outputURL);
    }

    isCalibrationDone=result;

    //Create visualOdometryMono object
    if(isCalibrationDone){
        VisualOdometryMono::parameters param;
        param.calib.f  = cameraMatrix.data[0]; // focal length in pixels
        param.calib.cu = cameraMatrix.data[2]; // principal point (u-coordinate) in pixels
        param.calib.cv = cameraMatrix.data[5]; // principal point (v-coordinate) in pixels
        //param.base= 0.572;
        viso=new VisualOdometryMono(param);
    }

    return result;
}
void Core::calibrateFromImages(std::string inputImagesPathURL , std::string outputCalibrationDataURL, int numberOfSamples){

    cv::Mat image;
    //Kalibracja z pliku
    for(int i=1; i<numberOfSamples;i++)
    {
        char base_name[256];
        sprintf(base_name,"%06d.JPG",i);
        string actualImageURL  = inputImagesPathURL +"/" + base_name;

        image = cv::imread(actualImageURL,CV_LOAD_IMAGE_GRAYSCALE);
        if(! image.data ){
            cout << "Could not open or find the image for calibration" << std::endl ;
            break;
        }
        
        //Temp
        setImageSize(image.size());
        // /Temp
        
        addSampleToCalibration(image);
    }
    calibrateCamera(outputCalibrationDataURL);
}

void Core::saveCalibration(std::string outputURL){

     cv::FileStorage outputFile(outputURL, FileStorage::WRITE);
     outputFile<<"cameraMatrix"<<cameraMatrix;
     outputFile.release();
}

void Core::loadCalibration(std::string inputURL){

    cv::FileStorage inputFile(inputURL, FileStorage::READ);
    if(!inputFile.isOpened()){
        std::cerr<<"Cannot open calibrationData file in Core::loadCalibration";
        return;
    }
    inputFile["cameraMatrix"] >> cameraMatrix;
    isCalibrationDone = true;

//Create visualOdometryMono object
    VisualOdometryMono::parameters param;

    param.calib.f  = cameraMatrix.at<double>(0); // focal length in pixels
    param.calib.cu = cameraMatrix.at<double>(2); // principal point (u-coordinate) in pixels
    param.calib.cv = cameraMatrix.at<double>(5); // principal point (v-coordinate) in pixels

    //param.base= 0.572;
    viso=new VisualOdometryMono(param);

}

bool Core::addImgToOdometry(cv::Mat img){

    if(!isCalibrationDone){
        cerr<<"To do Odometry you need to calibrate Camera (in Core::addImgToOdometry";
        return false;
    }

    // image dimensions
    int32_t width= img.cols;
    int32_t height= img.rows;

    // compute visual odometry
    int32_t dims[] = {width,height,width};

    if (viso->process(img.datastart,dims)) {

        pose = pose* Matrix::inv(viso->getMotion());
        //double num_matches = viso->getNumberOfMatches();
        //double num_inliers = viso->getNumberOfInliers();
        //cout << ", Matches: " << num_matches;
        //cout << ", Inliers: " << 100.0*num_inliers/num_matches << " %" << ", Current pose: " << endl;
        //cout << pose << endl << endl;

      } else {
        cout << " ... failed!" << endl;
        return false;
      }

  return true;
}
