#include <stdio.h>
#include <chrono>
#include <iostream>
#include "ImageReader.h"
#include "DialParser.h"
#include "DebugDisplay.h"
#include "Utils.h"

typedef std::chrono::high_resolution_clock Clock;
using namespace cv;

Mat preprocessImage(const Mat &image)
{
    Mat greyImage;
    cv::cvtColor(image, greyImage, cv::COLOR_BGR2GRAY);
    
    //showImage("Grey", greyImage, 310, 10);
    //GaussianBlur(greyImage, greyImage, Size(9, 9), 2, 2);
    //blur(greyImage, greyImage, Size(3, 3));
    return greyImage;
}

//Finds the radius of the largest one
void findAndMarkCircles(Mat &image, Mat &greyImage, Point &largestCenter, int &largestRadius)
{
    std::vector<Vec3f> circles;
    const int minRadius = 15;
    const int minCirclesDistance = 30;
    /// Apply the Hough Transform to find the circles
    HoughCircles(greyImage, circles, CV_HOUGH_GRADIENT, 1, minCirclesDistance,
                 200, 100, minRadius, 0);

    /// Draw the circles detected
    for(size_t i = 0; i < circles.size(); i++)
    {
        //printf("Circle detected..\n");
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle(image, center, 3, Scalar(0,255,0), -1, 8, 0);
        // circle outline
        circle(image, center, radius, Scalar(0,0,255), 3, 8, 0);
        if (radius > largestRadius)
        {
            largestRadius = radius;
            largestCenter = center;
        }
    }
}

void printTime(std::chrono::time_point<std::chrono::high_resolution_clock> &c1,
               std::chrono::time_point<std::chrono::high_resolution_clock> &c2,
               std::string message)
{
    //std::cout << message + " took" + " ";
    //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(c2 - c1).count() << "\n";
}

int main()
{
    DialParserConfiguration conf;
    conf.minAngle = 0.785;
    conf.maxAngle = 2*CV_PI - conf.minAngle;
    conf.minValue = 0;
    conf.maxValue = 160;
    
    const int frames = 25;
    const int fps = 5;
    const int sleepLen = 1000 / fps;
    int remainingFrames = frames;
    ImageReader reader;
    DebugDisplay display;
    while (remainingFrames > 0)
    {
        auto t1 = Clock::now();
        remainingFrames--;
        Mat image = reader.readNextImage();
        if (!image.data) 
            continue;
            
        display.showImage("Original", image, Point(1,1));
        Mat greyImage = preprocessImage(image);

        Point dialCenter(-1,-1);
        int largestRadius = -1;
        findAndMarkCircles(image, greyImage, dialCenter, largestRadius);
        if (largestRadius < 1)
        {
            printf("Can't find any circle in the image!");
            continue;
        }
        display.showImage("Circled", image, Point(2, 1));
        
        threshold(greyImage, greyImage, 100, 255, THRESH_BINARY);
        display.showImage("binary", greyImage, Point(3, 1));
        
        DialParser dialParser(conf, greyImage, dialCenter);
        if (!dialParser.hasPointer())
        {
            printf("No pointer detected!!\n");
            continue;
        }
        dialParser.paintPoints(image);
        display.showImage("Pointer", image, Point(4, 1));
        double v = dialParser.readValuePercent();
        display.displayValue(v, Point(5, 1));
        printf("Calculated value %lf\n", v);
        auto t2 = Clock::now();
        int ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        printf("Took %d ms\n", ms);
        if (ms < sleepLen)
        {
            Utils::sleepms(sleepLen - ms);
        }
    }

    //waitKey();
    cv::destroyAllWindows();
	return 0;
}
