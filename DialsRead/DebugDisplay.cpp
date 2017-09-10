#include "DebugDisplay.h"

using namespace cv;

void DebugDisplay::showImage(const std::string &nameRaw, const Mat image, Point lt) const
{
    //printf("Show window at %d %d\n", x, y);
    const int size = 200;
    const int xmargin = 0;
    const int ymargin = 30;
    const int initialX = 70;
    const int initialY = 25;
    
    Mat displayImage;
    resize(image, displayImage, Size(size, size));
    std::string name = nameRaw + std::to_string(lt.y);
    namedWindow(name, WINDOW_AUTOSIZE);
    imshow(name, displayImage);
    moveWindow(name, lt.x * (size+xmargin) + initialX, lt.y * (size+ymargin) + initialY);
}

void DebugDisplay::displayValue(double percentValue, Point ltPosition) const
{
    Mat textwin = Mat::zeros(250,250,CV_8UC3);
    rectangle(textwin, Point(40,20), Point(60, 180), Scalar(255,255,255), CV_FILLED);
    rectangle(textwin, Point(40,180), Point(60, 180-((160*percentValue)/100)), Scalar(0,0,255), CV_FILLED);
    showImage("value", textwin, ltPosition);
}

DebugDisplay::~DebugDisplay()
{
    cv::destroyAllWindows();
}