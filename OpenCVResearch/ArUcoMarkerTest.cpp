// Copyright (c) Kangsan Bae warimo47@naver.com All rights reserved.

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ArUco/aruco.h"

#ifdef _DEBUG
#pragma comment(lib, "opencv_world455d.lib")
#else
#pragma comment(lib, "opencv_world455.lib")
#endif

int main()
{
    cv::Mat image = cv::imread("ArUco_Marker_Images/multi_test_4.jpg");

    aruco::MarkerDetector MDetector;

    // aruco::Dictionary::DICT_TYPES::ARUCO_MIP_36h12;
    // MDetector.setDictionary("ARUCO_MIP_36h12");

    // detect
    std::vector<aruco::Marker> markers = MDetector.detect(image);

    // print info to console
    for (size_t i = 0; i < markers.size(); ++i)
    {
        std::cout << markers[i] << std::endl;

        // draw in the image
        markers[i].draw(image);
    }

    cv::namedWindow("ArUco Marker Test", 1);
    cv::imshow("ArUco Marker Test", image);
    cv::waitKey(0);
}
