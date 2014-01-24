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
viso=NULL;
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
bool Core::calibrateCamera(QString url){

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
    if(result){ saveCalibration(url.toUtf8().constData());}
    calibOk=result;
    return result;
}
void Core::saveCalibration(std::string path){

  /*  QFile file(path);
      file.open(QIODevice::WriteOnly | QIODevice::Text);
      QTextStream out(&file);
      for(int i=0;i<3;i++){
          for(int j=0;j<3;j++){
              out<<cameraMatrix[0].at<double>(i,j)<<";";

          }
          out<<"\n";
      }


      // optional, as QFile destructor will already do it:
      file.close();*/
      cv::FileStorage file2(path, FileStorage::WRITE);
     file2<<"cameraMatrix"<<cameraMatrix;
     file2.release();

}
void Core::loadCalibration(std::string path){

  /*  if (path.isEmpty())
             return;
         else {

             QFile file(path);

             if (!file.open(QIODevice::ReadOnly)) {
                 //komunikat
                 return;
             }

          QTextStream in(&file);
             QString line = in.readLine();
            qDebug()<<line;

}*/
   cv::FileStorage file2(path, FileStorage::READ);
   file2["cameraMatrix"]>> cameraMatrix;
   calibOk=1;
   VisualOdometryMono::parameters param;

param.calib.f  = cameraMatrix.data[0]; // focal length in pixels
param.calib.cu = cameraMatrix.data[2]; // principal point (u-coordinate) in pixels
param.calib.cv = cameraMatrix.data[5]; // principal point (v-coordinate) in pixels
viso=new VisualOdometryMono(param);

}
bool Core::addImgToOdometry(cv::Mat img, int frameNo, QString urlToFile){

   cv::FileStorage file(urlToFile.toUtf8().constData(), FileStorage::WRITE);




  // sequence directory

  // set most important visual odometry parameters
  // for a full parameter list, look at: viso_stereo.h


  // calibration parameters for sequence 2010_03_09_drive_0019

  //param.base     = 1; // baseline in meters

  // init visual odometry


  // current pose (this matrix transforms a point from the current
  // frame's camera coordinates to the first frame's camera coordinates)
  Matrix pose = Matrix::eye(4);
  // loop through all frames i=0:372



    // input file names


    // catch image read/write errors here
    try {

      // load left and right input image

      // image dimensions
      int32_t width= img.cols;
      int32_t height= img.rows;
      // convert input images to uint8_t buffer

    std::vector<int> a;
      // status


      // compute visual odometry
      int32_t dims[] = {width,height,width};
      if (viso->process(img.datastart,dims)) {

        // on success, update current pose
        pose = pose * Matrix::inv(viso->getMotion());

        // output some statistics
        double num_matches = viso->getNumberOfMatches();
        double num_inliers = viso->getNumberOfInliers();
        cout << ", Matches: " << num_matches;
        cout << ", Inliers: " << 100.0*num_inliers/num_matches << " %" << ", Current pose: " << endl;
        cout << pose << endl << endl;
        cv::Mat pose2;

      // pose.getData((FLOAT *)pose2.data);
       // file<<("pose"+frameNo)<<pose2;

      } else {
        cout << " ... failed!" << endl;
      }


    // catch image read errors here
    } catch (...) {
      cerr << "ERROR: Couldn't read input files!" << endl;
      return 1;
    }


  //file<<"a";
  //file.flush();

  // output
  cout << "Demo complete! Exiting ..." << endl;

  // exit
  return 0;


}
