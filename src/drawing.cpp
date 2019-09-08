#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main( void ){
  char window[] = "occupancy map";

  Mat rgb;
  rgb = imread("1.png", CV_LOAD_IMAGE_COLOR);   // Read the file

  if(!rgb.data)                              // Check for invalid input
  {
      cout <<  "Could not open or find the rgb image" << std::endl ;
      return -1;
  }

  Mat pred = imread("2.png", CV_LOAD_IMAGE_COLOR);   // Read the file

  if(!pred.data)                              // Check for invalid input
  {
      cout <<  "Could not open or find the horizon image" << std::endl ;
      return -1;
  }

  imshow("pred",pred);

  Mat occupancy_image = rgb.clone();

  int limit = 0;

  for(int j = 0; j < pred.cols; j++) { 
      for(int i = 0; i < pred.rows; i++) {            
          if(pred.at<cv::Vec3b>(i,j)[0] == 255 &&
             pred.at<cv::Vec3b>(i,j)[1] == 0 &&
             pred.at<cv::Vec3b>(i,j)[2] == 255 )
          {
              occupancy_image.at<cv::Vec3b>(i,j)[0] = 255;
              occupancy_image.at<cv::Vec3b>(i,j)[1] = 0;
              occupancy_image.at<cv::Vec3b>(i,j)[2] = 255;
              if(i > limit)
                  limit = i;
          }
      }
  }
  
  cout << limit << endl;

  Rect first(0, limit, occupancy_image.cols/4, (occupancy_image.rows-limit)/3);
  cout << first << endl;
  Rect second(occupancy_image.cols/4, limit, occupancy_image.cols/2, (occupancy_image.rows-limit)/3);
  cout << second << endl;
  Rect third(occupancy_image.cols*0.75, limit, occupancy_image.cols/4, (occupancy_image.rows-limit)/3);
  cout << third << endl;
  Rect fourth(0, limit+((occupancy_image.rows-limit)*0.34), occupancy_image.cols/4, (occupancy_image.rows-limit)*0.67);
  cout << fourth << endl;
  Rect fifth(occupancy_image.cols/4, limit+((occupancy_image.rows-limit)*0.34), occupancy_image.cols/2, (occupancy_image.rows-limit)*0.67);
  cout << fifth << endl;
  Rect sixth(occupancy_image.cols*0.75, limit+((occupancy_image.rows-limit)*0.34), occupancy_image.cols/4, (occupancy_image.rows-limit)*0.67);
  cout << sixth << endl;

  /*
line(occupancy_image,
       Point(0, limit),
       Point(occupancy_image.cols-1, limit),
       Scalar(0,0,0),
       2);
*/
  rectangle(occupancy_image,first,Scalar(0,0,0), 2);
  rectangle(occupancy_image,second,Scalar(0,0,0), 2);
  rectangle(occupancy_image,third,Scalar(0,0,0), 2);
  rectangle(occupancy_image,fourth,Scalar(0,0,0), 2);
  rectangle(occupancy_image,fifth,Scalar(0,0,0), 2);
  rectangle(occupancy_image,sixth,Scalar(0,0,0), 2);
  

  imshow(window, occupancy_image );
  moveWindow( window, 200, 200 );
  waitKey( 0 );
  return(0);
}

