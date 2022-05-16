// Copyright (c) Kangsan Bae warimo47@naver.com All rights reserved.

#include <iostream>

#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world455d.lib")
#else
#pragma comment(lib, "opencv_world455.lib")
#endif

using namespace cv;
using namespace std;

void VideoCrop();
void VideoResizing();
void VideoPlay();

Mat originalImg;
Mat sobelEdgeImg;
Mat thresholdImg;

void CallBackFuncFrame(int event, int x, int y, int flags, void* userdata)
{
    // frame.ptr<uchar>()
    uchar* imgData = originalImg.data;
    uchar b = imgData[y * originalImg.cols * 3 + x * 3];
    uchar g = imgData[y * originalImg.cols * 3 + x * 3 + 1];
    uchar r = imgData[y * originalImg.cols * 3 + x * 3 + 2];

    if (event == EVENT_LBUTTONDOWN)
    {
        cout << "Frame  (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (event == EVENT_RBUTTONDOWN)
    {
        cout << "Frame  (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (event == EVENT_MBUTTONDOWN)
    {
        cout << "Frame  (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        // cout << "(" << x << ", " << y << ")" << endl;
    }
}

void CallBackFuncThresh(int event, int x, int y, int flags, void* userdata)
{
    // frame.ptr<uchar>()
    uchar* imgData = thresholdImg.data;
    uchar b = imgData[y * thresholdImg.cols * 3 + x * 3 + 0];
    uchar g = imgData[y * thresholdImg.cols * 3 + x * 3 + 1];
    uchar r = imgData[y * thresholdImg.cols * 3 + x * 3 + 2];

    if (event == EVENT_LBUTTONDOWN)
    {
        cout << "Thresh (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (event == EVENT_RBUTTONDOWN)
    {
        cout << "Thresh (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (event == EVENT_MBUTTONDOWN)
    {
        cout << "Thresh (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        // cout << "(" << x << ", " << y << ")" << endl;
    }
}

void HoughLinesSegments();

int main()
{
    namedWindow("Original Img", 1);
    namedWindow("Sobel Edge Img", 1);
    namedWindow("Threshold Img", 1);

    //set the callback function for any mouse event
    setMouseCallback("Original Img", CallBackFuncFrame, NULL);
    setMouseCallback("Threshold Img", CallBackFuncThresh, NULL);

    VideoPlay();

    destroyAllWindows();

    return(0);
}

void VideoPlay()
{
    VideoCapture cap("IMG_2816_480p.mp4");

    if (!cap.isOpened())
        cout << "Error when reading stream";
    
    int inputKey = 0;
    double ret;
    int thresh = 177;

    while (true)
    {
        cap >> originalImg;
        if (originalImg.empty())
            break;

        Sobel(originalImg, sobelEdgeImg, originalImg.depth(), 1, 0);
        ret = threshold(originalImg, thresholdImg, thresh, 255, THRESH_BINARY);

        // cout << ret << endl;

        imshow("Original Img", originalImg);
        imshow("Sobel Edge Img", sobelEdgeImg);
        imshow("Threshold Img", thresholdImg);

        inputKey = waitKey(0);
        if (inputKey == 'q')
        {
            break;
        }
        else if (inputKey == 'w')
        {
            thresh++;
        }
        else if (inputKey == 's')
        {
            thresh--;
        }
        cout << "Thresh : " << thresh << endl;
    }

    cout << "VideoPlay() end" << endl;

    cap.release();
}

void HoughLinesSegments()
{
    Mat src = imread("building.jpg", IMREAD_GRAYSCALE);
    if (src.empty()) {
        cerr << "image load error" << endl;
        return;
    }

    Mat edge;
    Canny(src, edge, 50, 150);

    vector<Vec4i> lines;
    HoughLinesP(edge, lines, 1, CV_PI / 180, 160, 50, 5);

    Mat dst;
    cvtColor(edge, dst, COLOR_GRAY2BGR);


    for (Vec4i l : lines) {
        line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 0), 1, LINE_AA);
    }

    imshow("src", src);
    imshow("dst", dst);

    waitKey();
    destroyAllWindows();
}

void VideoCrop()
{
    cv::Size video_size(480, 640);
    VideoWriter wri("IMG_2816_480p.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), 60.0, video_size, true);

    VideoCapture cap("IMG_2816_486p.mp4");
    if (!cap.isOpened())
        cout << "Error when reading stream";
    namedWindow("Frame", 1);
    namedWindow("resizeImg", 1);

    Rect cropRegion(3, 4, 480, 640);

    Mat frame;
    Mat resizeImg;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;
        
        resizeImg = frame(cropRegion);

        wri << resizeImg;

        imshow("Frame", frame);
        imshow("resizeImg", resizeImg);

        if (waitKey(10) >= 0)
        {
            break;
        }
    }

    cout << "Program done." << endl;

    cap.release();
    wri.release();
}

void VideoResizing()
{
    cv::Size video_size(486, 864);
    VideoWriter wri("IMG_2816_486p.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), 60.0, video_size, true);

    VideoCapture cap("IMG_2816.mp4");
    if (!cap.isOpened())
        cout << "Error when reading stream";
    namedWindow("Frame", 1);
    namedWindow("resizeImg", 1);

    Mat frame;
    Mat resizeImg;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        resize(frame, resizeImg, Size(486, 864), INTER_AREA);

        wri << resizeImg;

        imshow("Frame", frame);
        imshow("resizeImg", resizeImg);

        if (waitKey(10) >= 0)
        {
            break;
        }
    }

    cout << "Program done." << endl;

    cap.release();
    wri.release();
}