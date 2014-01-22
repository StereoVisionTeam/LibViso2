/*
Copyright 2012. All rights reserved.
Institute of Measurement and Control Systems
Karlsruhe Institute of Technology, Germany

This file is part of libviso2.
Authors: Andreas Geiger

libviso2 is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or any later version.

libviso2 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
libviso2; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA 
*/

/*
  Documented C++ sample code of stereo visual odometry (modify to your needs)
  To run this demonstration, download the Karlsruhe dataset sequence
  '2010_03_09_drive_0019' from: www.cvlibs.net!
  Usage: ./viso2 path/to/sequence/2010_03_09_drive_0019
*/

#include "demo.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

using namespace std;

int demo(int argc, char *argv[]){
  // we need the path name to 2010_03_09_drive_0019 as input argument
  if (argc<2) {
    cerr << "Usage: ./viso2 path/to/sequence/2009_09_08_drive_0021" << endl;
    return 1;
  }
    ofstream file;

    file.open("WynikProgramuMonoRightCoNaFilmie.txt");
    if(file.is_open()) cout<<"OTWARTY";

  // sequence directory
  string dir = argv[1];
  
  // set most important visual odometry parameters
  // for a full parameter list, look at: viso_stereo.h
  VisualOdometryMono::parameters param;
  
  // calibration parameters for sequence 2010_03_09_drive_0019 
  param.calib.f  = 903.29; // focal length in pixels
  param.calib.cu = 663.99; // principal point (u-coordinate) in pixels
  param.calib.cv = 245.21; // principal point (v-coordinate) in pixels
  //param.base     = 1; // baseline in meters
  
  // init visual odometry
  VisualOdometryMono viso(param);
  
  // current pose (this matrix transforms a point from the current
  // frame's camera coordinates to the first frame's camera coordinates)
  Matrix pose = Matrix::eye(4);
  Mat left,right;

  // loop through all frames i=0:372

  for (int32_t i=0; i<1513; i++) {

    // input file names
    char base_name[256]; sprintf(base_name,"%06d.png",i);
    string left_img_file_name  = dir + "/I1_" + base_name;
    string right_img_file_name = dir + "/I2_" + base_name;

    // catch image read/write errors here
    try {
   ;
      // load left and right input image
      left= imread(left_img_file_name,CV_LOAD_IMAGE_GRAYSCALE);
      right=imread(right_img_file_name,CV_LOAD_IMAGE_GRAYSCALE);
      // image dimensions
      int32_t width= left.cols;
      int32_t height= left.rows;
      // convert input images to uint8_t buffer

    std::vector<int> a;
      // status
      cout << "Processing: Frame: " << i;
      
      // compute visual odometry
      int32_t dims[] = {width,height,width};
      if (viso.process(right.datastart,dims)) {

        // on success, update current pose
        pose = pose * Matrix::inv(viso.getMotion());

        // output some statistics
        double num_matches = viso.getNumberOfMatches();
        double num_inliers = viso.getNumberOfInliers();
        cout << ", Matches: " << num_matches;
        cout << ", Inliers: " << 100.0*num_inliers/num_matches << " %" << ", Current pose: " << endl;
        cout << pose << endl << endl;
        file<<pose<<endl;

      } else {
        cout << " ... failed!" << endl;
      }


    // catch image read errors here
    } catch (...) {
      cerr << "ERROR: Couldn't read input files!" << endl;
      return 1;
    }

  }
  //file<<"a";
  //file.flush();
  file.close();
  // output
  cout << "Demo complete! Exiting ..." << endl;

  // exit
  return 0;
}

//Zapis do pliku z wyników/kalibracja
//Przetwarzanie danych z
