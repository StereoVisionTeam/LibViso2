#include "libviso2/demo.h"
#include <core.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

int main(int argc, char *argv[])
{    
    cv::Mat image;
    cv::Mat image2;
    Core core;
    core.setPatternSize(9,6);
    string calibrationImagesURL = "/home/haskis/Pictures/Projects/FromKicman/Calibration/try2";
    string calibrationDataURL = "/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0010/Kalibracja_1.xml";
    string odometryImagesURL = "/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0010";

    string odometryURL[6];
    odometryURL[0]="/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0010/";
    odometryURL[1]="/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0012/";
    odometryURL[2]="/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0015/";
    odometryURL[3]="/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0016/";
    odometryURL[4]="/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0019/";
    odometryURL[5]="/home/haskis/Pictures/Projects/LibViso2DataSet/2009_09_08_drive_0021/";

    int numberOfFrames[6];
    numberOfFrames[0]=1423;
    numberOfFrames[1]=2578;
    numberOfFrames[2]=1020;
    numberOfFrames[3]=1205;
    numberOfFrames[4]=1248;
    numberOfFrames[5]=1513;

    //From images
    //core.calibrateFromImages(calibrationImagesURL,calibrationDataURL,9);

    //From file
    core.loadCalibration(calibrationDataURL);
    for(int o=0;o<6;o++)
    for( int k=0; k<=5; k++){

        QString path= QString::fromStdString(odometryURL[o]) + QString::number(k) + ".txt";
        core.fileWithOdometry.open(path.toUtf8().constData());
        
        if(core.fileWithOdometry.is_open())
            qDebug()<<"Zapisuję dane z odometrii";
        else
            qDebug()<<"cant open";
    
        for(int i=0; i<numberOfFrames[o];i++){
            char base_name[256]; std::sprintf(base_name,"%06d.png",i);
            std::string  actualImageURL = odometryURL[o] + "/I1_" + base_name;

            image=cv::imread(actualImageURL,CV_LOAD_IMAGE_GRAYSCALE);

            core.addImgToOdometry(image);

            core.fileWithOdometry<<core.pose<<std::endl;

        }
        core.fileWithOdometry.flush();
        core.fileWithOdometry.close();
        core.pose = Matrix::eye(4);
    }
    qDebug()<<"Core constructor End";

    return 0;
}
