// Copyright (c) Kangsan Bae warimo47@naver.com All rights reserved.

#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/opencv.hpp>
#include "json/json.h"

#ifdef _DEBUG
#pragma comment(lib, "opencv_world455d.lib")
#else
#pragma comment(lib, "opencv_world455.lib")
#endif

#include "Pos.h"

using namespace cv;
using namespace std;

void VideoPlay();

bool ProcessKeyboardEvent(int* thresh);

void HoughLinesSegments();

void CallBackFuncOriginal(int event, int x, int y, int flags, void* userdata);
void CallBackFuncThresh(int event, int x, int y, int flags, void* userdata);

void JsonRead();
void JsonWrite();

void VideoCrop();
void VideoResizing();

Mat originalImg;
Mat maskImg;
Mat sobelEdgeImg;
Mat thresholdImg;
int cursorX = 0;
int cursorY = 0;
vector<Pos> targetPixels;

int main()
{
    JsonRead();

    namedWindow("Original Img", 1);
    namedWindow("Mask Img", 1);
    // namedWindow("Sobel Edge Img", 1);
    // namedWindow("Threshold Img", 1);

    // set the callback function for any mouse event
    setMouseCallback("Original Img", CallBackFuncOriginal, NULL);
    // setMouseCallback("Threshold Img", CallBackFuncThresh, NULL);

    VideoPlay();

    destroyAllWindows();

    JsonWrite();

    return 0;
}

void VideoPlay()
{
    VideoCapture cap("IMG_2816_480p.mp4");

    if (!cap.isOpened())
        std::cout << "Error when reading stream";
    
    double ret;
    int thresh = 177;

    Mat originalImgCopy;
    Mat MaskImgCopy;
    maskImg = Mat::zeros(640, 480, CV_8UC3);
    
    while (true)
    {
        cap >> originalImg;
        if (originalImg.empty())
            break;

        // maskImg = originalImg.clone();
        // Sobel(originalImg, sobelEdgeImg, originalImg.depth(), 1, 0);
        // ret = threshold(originalImg, thresholdImg, thresh, 255, THRESH_BINARY);

        // cout << ret << endl;
        // Draw cursor
        originalImgCopy = originalImg.clone();
        originalImgCopy.data[cursorY * originalImgCopy.cols * 3 + cursorX * 3 + 0] = 255;
        originalImgCopy.data[cursorY * originalImgCopy.cols * 3 + cursorX * 3 + 1] = 0;
        originalImgCopy.data[cursorY * originalImgCopy.cols * 3 + cursorX * 3 + 2] = 255;
        MaskImgCopy = maskImg.clone();
        MaskImgCopy.data[cursorY * MaskImgCopy.cols * 3 + cursorX * 3 + 0] = 255;
        MaskImgCopy.data[cursorY * MaskImgCopy.cols * 3 + cursorX * 3 + 1] = 0;
        MaskImgCopy.data[cursorY * MaskImgCopy.cols * 3 + cursorX * 3 + 2] = 255;

        imshow("Original Img", originalImgCopy);
        imshow("Mask Img", MaskImgCopy);
        // imshow("Sobel Edge Img", sobelEdgeImg);
        // imshow("Threshold Img", thresholdImg);

        if (ProcessKeyboardEvent(&thresh) == false)
            break;
    }

    std::cout << "VideoPlay() end" << endl;

    cap.release();
}

bool ProcessKeyboardEvent(int* thresh)
{
    int inputKey = waitKeyEx(0);
    if (inputKey == 'q')
    {
        return false;
    }
    else if (inputKey == 'w')
    {
        (*thresh)++;
        std::cout << "Thresh : " << thresh << endl;
    }
    else if (inputKey == 's')
    {
        (*thresh)--;
        std::cout << "Thresh : " << thresh << endl;
    }
    else if (inputKey == 0x250000)
    {
        cursorX--;
        if (cursorX < 0)
            cursorX = 0;
    }
    else if (inputKey == 0x260000)
    {
        cursorY--;
        if (cursorY < 0)
            cursorY = 0;
    }
    else if (inputKey == 0x270000)
    {
        cursorX++;
        if (cursorX > 479)
            cursorX = 479;
    }
    else if (inputKey == 0x280000)
    {
        cursorY++;
        if (cursorY > 639)
            cursorY = 639;
    }
    else if (inputKey == ' ')
    {
        targetPixels.push_back(Pos{ cursorY, cursorX });
        maskImg.data[cursorY * maskImg.cols * 3 + cursorX * 3 + 0] = 0;
        maskImg.data[cursorY * maskImg.cols * 3 + cursorX * 3 + 1] = 0;
        maskImg.data[cursorY * maskImg.cols * 3 + cursorX * 3 + 2] = 255;
    }
    else
    {
        std::cout << "inputKey : " << inputKey << endl;
    }
}

void CallBackFuncOriginal(int event, int x, int y, int flags, void* userdata)
{
    // frame.ptr<uchar>()
    uchar* imgData = originalImg.data;
    uchar b = imgData[y * originalImg.cols * 3 + x * 3 + 0];
    uchar g = imgData[y * originalImg.cols * 3 + x * 3 + 1];
    uchar r = imgData[y * originalImg.cols * 3 + x * 3 + 2];

    if (event == EVENT_LBUTTONDOWN)
    {
        std::cout << "Frame  (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
        cursorY = y;
        cursorX = x;
    }
    else if (event == EVENT_RBUTTONDOWN)
    {
        std::cout << "Frame  (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (event == EVENT_MBUTTONDOWN)
    {
        std::cout << "Frame  (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        // cout << "(" << x << ", " << y << ")" << endl;
    }
}

void CallBackFuncThresh(int eventType, int x, int y, int flags, void* userdata)
{
    // frame.ptr<uchar>()
    uchar* imgData = thresholdImg.data;
    uchar b = imgData[y * thresholdImg.cols * 3 + x * 3 + 0];
    uchar g = imgData[y * thresholdImg.cols * 3 + x * 3 + 1];
    uchar r = imgData[y * thresholdImg.cols * 3 + x * 3 + 2];

    if (eventType == EVENT_LBUTTONDOWN)
    {
        std::cout << "Thresh (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (eventType == EVENT_RBUTTONDOWN)
    {
        std::cout << "Thresh (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (eventType == EVENT_MBUTTONDOWN)
    {
        std::cout << "Thresh (" << x << ", " << y << ") R " << static_cast<int>(r) << " G " << static_cast<int>(g) << " B " << static_cast<int>(b) << endl;
    }
    else if (eventType == EVENT_MOUSEMOVE)
    {
        // cout << "(" << x << ", " << y << ")" << endl;
    }
}

void HoughLinesSegments()
{
    Mat src = imread("building.jpg", IMREAD_GRAYSCALE);
    if (src.empty())
    {
        cerr << "image load error" << endl;
        return;
    }

    Mat edge;
    Canny(src, edge, 50, 150);

    vector<Vec4i> lines;
    HoughLinesP(edge, lines, 1, CV_PI / 180, 160, 50, 5);

    Mat dst;
    cvtColor(edge, dst, COLOR_GRAY2BGR);


    for (Vec4i l : lines)
    {
        line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 0), 1, LINE_AA);
    }

    imshow("src", src);
    imshow("dst", dst);

    waitKey();
    destroyAllWindows();
}

void JsonRead()
{
    std::ifstream fin;
    fin.open("PositionList.json", std::ios::in);

    std::string str;
    str.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

    fin.close();

    Json::Reader reader;
    Json::Value root;

    if (reader.parse(str, root) == false)
    {
        std::cerr << "Failed to parse Json : " << reader.getFormattedErrorMessages() << std::endl;
    }

    Json::Value PosList = root.get("Pos", root);
    Json::Value Pos = PosList[0];
    Json::Value y = Pos.get("y", Pos);
    Json::Value x = Pos.get("x", Pos);

    int _y = y.asInt();
    int _x = x.asInt();
    
    int a = 3;
}

void JsonWrite()
{
    std::ofstream fout("PositionList.json");

    std::string allStr = "{\n\t\"Pos\": [\n";
    std::string strLine = "";

    for (int tpi = 0; tpi < targetPixels.size(); ++tpi)
    {
        strLine = "\t\t{\n";
        strLine = strLine + "\t\t\t\"y\": " + std::to_string(targetPixels[tpi]._y) + ",\n";
        strLine = strLine + "\t\t\t\"x\": " + std::to_string(targetPixels[tpi]._x) + "\n";
        strLine = strLine + "\t\t}";
        if (tpi == targetPixels.size() - 1)
        {
            allStr = allStr + strLine;
            break;
        }
        strLine = strLine + ",\n";
        allStr = allStr + strLine;
    }
    allStr = allStr + "\n\t]\n}";
    
    // allStr = "{\n\t\"Pos\": [\n\t\t{\n\t\t\t\"y\": 102,\n\t\t\t\"x\": 123\n\t\t}\n\t]\n}";
    
    fout.write(allStr.c_str(), allStr.size());

    fout.close();
}

void VideoCrop()
{
    cv::Size video_size(480, 640);
    VideoWriter wri("IMG_2816_480p.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), 60.0, video_size, true);

    VideoCapture cap("IMG_2816_486p.mp4");
    if (!cap.isOpened())
        std::cout << "Error when reading stream";
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

    std::cout << "Program done." << endl;

    cap.release();
    wri.release();
}

void VideoResizing()
{
    cv::Size video_size(486, 864);
    VideoWriter wri("IMG_2816_486p.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), 60.0, video_size, true);

    VideoCapture cap("IMG_2816.mp4");
    if (!cap.isOpened())
        std::cout << "Error when reading stream";
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

    std::cout << "Program done." << endl;

    cap.release();
    wri.release();
}
