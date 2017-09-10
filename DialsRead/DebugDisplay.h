#ifndef DEBUG_DISPLAY_H
#define DEBUG_DISPLAY_H

#include <opencv2/opencv.hpp>
#include <string>

class DebugDisplay
{
    public:
        void showImage(const std::string& nameCore, const cv::Mat image, cv::Point ltposition);
        void displayValue(double percentValue, cv::Point ltPosition);
        ~DebugDisplay();
};

#endif //DEBUG_DISPLAY_H