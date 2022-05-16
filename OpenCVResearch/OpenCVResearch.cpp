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

int main()
{
    

    return(0);
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