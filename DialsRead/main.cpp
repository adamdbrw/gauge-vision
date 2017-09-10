#include <stdio.h>
#include <chrono>
#include <iostream>
#include "ImageReader.h"
#include "DebugDisplay.h"
#include "GaugeReader.h"
#include "Utils.h"

typedef std::chrono::high_resolution_clock Clock;
using namespace cv;

int main()
{
    const int frames = 25;
    const int fps = 5;
    const int sleepLen = 1000 / fps;
    int remainingFrames = frames;
    ImageReader imageReader;
    DebugDisplay display;
    GaugeReader gaugeReader;
    while (remainingFrames > 0)
    {
        auto t1 = Clock::now();
        remainingFrames--;
        Mat image = imageReader.readNextImage();
        if (!image.data) 
            continue;
            
        //display.showImage("Original", image, Point(1,1));
        gaugeReading val = gaugeReader.readGauge(image, display, true);
        printf("Calculated value %lf\n", val);

        auto t2 = Clock::now();
        int ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        printf("Took %d ms\n", ms);
        if (ms < sleepLen)
        {
            Utils::sleepms(sleepLen - ms);
        }
    }

    waitKey();
    cv::destroyAllWindows();
	return 0;
}
