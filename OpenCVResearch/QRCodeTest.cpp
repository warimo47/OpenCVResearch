// Copyright (c) Kangsan Bae warimo47@naver.com All rights reserved.

#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world455d.lib")
#else
#pragma comment(lib, "opencv_world455.lib")
#endif

int main()
{
    cv::Mat img = cv::imread("QR_Code_Images/qr_code_test_108px_4.jpg");

    cv::QRCodeDetector decoder = cv::QRCodeDetector();
    std::vector<cv::Point> points;
    std::string data = decoder.detectAndDecode(img, points);

    std::cout << "\nDecoded data: " << data << std::endl << std::endl;

    cv::Point pt1;
    cv::Point pt2;

    for (int i = 0; i < points.size(); ++i)
    {
        pt1 = points[i];
        pt2 = points[(i + 1) % 4];
        line(img, pt1, pt2, cv::Scalar(255, 0, 0), 3);
    }

    cv::imshow("Detected QR code", img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}