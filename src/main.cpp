#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/features2d/features2d.hpp"

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#if defined(_MSC_VER)
#include <tchar.h>
#include <strsafe.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#elif defined(__GNUC__) || defined(__GNUG__)
#include <dirent.h>
#endif

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( string& img_filename );

string window_name = "obstacle detection";

int main( int argc, const char** argv )
{
    Mat img;
    Mat mask;
    Mat res;
    Mat pred;
    
    std::ofstream outfile("detection.txt");

    for(int cnt = 1; cnt < 145; cnt++) {

        

        std::string n = std::to_string(cnt);
        std::string img_filename = "../data/DS8/rgb/" + n + ".png";
        std::string mask_filename = "../data/DS8/mask/" + n + ".png";
        std::string pred_filename = "../data/DS8/prediction/" + n + ".png";
    
        img = imread(img_filename, CV_LOAD_IMAGE_COLOR);
        if (!img.data) {
	    cout << "Unable to read input img: " << img_filename << endl;
	    continue;
        }
        mask = imread(mask_filename, CV_LOAD_IMAGE_COLOR);
        if (!mask.data) {
	    cout << "Unable to read input mask: " << mask_filename << endl;
	    continue;
        }
        pred = imread(pred_filename, CV_LOAD_IMAGE_COLOR);
        if (!pred.data) {
	    cout << "Unable to read input pred: " << pred_filename << endl;
	    continue;
        }

        res = img.clone();

        for(int j = 0; j < pred.cols; j++) { 
            bool h = false;
            for(int i = 0; i < pred.rows; i++) { 
            
                if(pred.at<cv::Vec3b>(i,j)[0] == 255 &&
                   pred.at<cv::Vec3b>(i,j)[1] == 0 &&
                   pred.at<cv::Vec3b>(i,j)[2] == 255 )
                {
                    h = true;
                    res.at<cv::Vec3b>(i,j)[0] = 0;
                    res.at<cv::Vec3b>(i,j)[1] = 0;
                    res.at<cv::Vec3b>(i,j)[2] = 0;
                }
                else {
                    if(h == false) {
                        res.at<cv::Vec3b>(i,j)[0] = 0;
                        res.at<cv::Vec3b>(i,j)[1] = 0;
                        res.at<cv::Vec3b>(i,j)[2] = 0;
                    }
                }
            }
        }

        imshow( "res prima", res );

        for(int i=0; i < mask.rows; i++) { 
            for(int j=0; j < mask.cols; j++) { 
                if(mask.at<cv::Vec3b>(i,j)[0] > 100 &&
                   mask.at<cv::Vec3b>(i,j)[1] < 100 &&
                   mask.at<cv::Vec3b>(i,j)[2] < 100 )
                {
                    res.at<cv::Vec3b>(i,j)[0] = 0;
                    res.at<cv::Vec3b>(i,j)[1] = 0;
                    res.at<cv::Vec3b>(i,j)[2] = 0;
                }
            }
        }

        Mat im(res.size(), CV_8UC1);
        cv::cvtColor(res, im, CV_BGR2GRAY);

        double minArea = 20;
	
	std::vector < std::vector<Point> > contours;

        findContours(im, contours, RETR_LIST, CHAIN_APPROX_NONE);
        Moments moms;
        double area;

        std::vector<Rect> detections;
        
        for (size_t contourIdx = 0; contourIdx < contours.size(); ++contourIdx)
        {
            moms = moments(Mat(contours[contourIdx]));
            area = moms.m00;
            if (area > minArea)
            {
                drawContours( res, contours, contourIdx, Scalar(0,0,255), 2 );
                vector<Point> contours_poly;
                Rect r;
                approxPolyDP(contours[contourIdx], contours_poly, 3, true);
                r = boundingRect(contours_poly);
                detections.push_back(r);
                rectangle(img, r.tl(), r.br(), Scalar(0,0,255), 2);
            }
	}

        outfile << cnt << "," << detections.size() << ",";

        for (auto it = detections.begin(); it != detections.end(); ++it) {
            Rect r = *it;
            outfile << r.tl().x << "," << r.tl().y << "," << r.width << "," << r.height << ",";
        }

        outfile << endl;        	


        //-- Show what you got
        imshow( window_name, img );
        imshow( "res", res );
        cvWaitKey(0);    

        std::string nn;
        std::stringstream ss;
        ss << cnt;
        nn = ss.str();

        imwrite(nn+".png", img);        
    }

    outfile.close();
}

