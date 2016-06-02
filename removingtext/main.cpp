//
//  main.cpp
//  Example2
//
//  Created by Adobe on 17/05/16.
//  Copyright © 2016 Adobe. All rights reserved.
//
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
std::vector<cv::Rect> detectLetters(cv::Mat img , int x , int y,  int s)
{
    std::vector<cv::Rect> boundRect;
    cv::Mat img_gray, img_sobel, img_threshold, element;
    cvtColor(img, img_gray, CV_BGR2GRAY);
    cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(x, y) );
    cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element); //Does the trick
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(img_threshold, contours, 0, 1);
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
        if (contours[i].size()>s)
        {
            cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
            cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
            if (appRect.width>appRect.height)
                boundRect.push_back(appRect);
        }
    //cout<<boundRect.size()<<endl;
    return boundRect;
}
int main(int argc,char** argv)
{
    cv::Mat img_gray, img_sobel, img_threshold, element;
    //Read
    cv::Mat img1=cv::imread("15.jpg");
    cv::Mat img2=cv::imread("15.jpg");
    cv::Mat img3= img1.clone();
    img3.setTo(255);
    //Detect
    std::vector<cv::Rect> letterBBoxes1=detectLetters(img1 ,17 , 3 ,100);
    std::vector<cv::Rect> letterBBoxes2=detectLetters(img2, 17 ,3,100);
    //std::vector<cv::Rect> letterBBoxes3=detectLetters(img3(letterBBoxes1[2]) , 21, 3,300);
    cv::Mat img4=img3(letterBBoxes1[2]);
    cout<<letterBBoxes1[2].area()<<endl;
    //cout<<letterBBoxes3[0].area()<<endl;
    //double ratio= letterBBoxes1[0].area()/letterBBoxes3[0].area();
    //            namedWindow( "Contours", WINDOW_AUTOSIZE );
    //            imshow( "Contours", img3(letterBBoxes2[1]) );
    //            waitKey(10000000);
    //Display
    //cout<<letterBBoxes3.size()<<endl;
    //cout<<ratio<<endl;
    cvtColor(img1, img_gray, CV_BGR2GRAY);
    cv::threshold(img_gray, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    //cv::threshold(img_gray,img_threshold,200,255,cv::THRESH_BINARY);
    for(int i=0; i< letterBBoxes1.size(); i++){
        img_threshold(letterBBoxes1[i] ).setTo(255);
         img3(letterBBoxes1[i] ).setTo(0);
        //cvSet(img1(letterBBoxes1[i] ), CV_RGB(0,0,0));
    }
    
    dilate(img_threshold, img_sobel, Mat(), Point(-1, -1), 5, 1, 1);
    cv::imwrite( "imgOut456.jpg", img_sobel);

    for(int i=0; i< letterBBoxes2.size(); i++)
        cv::rectangle(img2,letterBBoxes2[i],cv::Scalar(0,255,0),3,8,0);
    //cv::imwrite( "imgOut5.jpg", img2);
//    for(int i=0; i< letterBBoxes3.size(); i++)
//        cv::rectangle(img4,letterBBoxes3[i],cv::Scalar(0,0,255),3,8,0);
    cv::imwrite( "imgOut46.jpg", img3);
    return 0;
}

