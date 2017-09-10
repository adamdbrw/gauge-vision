#ifndef DIAL_PARSER_H
#define DIAL_PARSER_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <set>

struct comparePoints 
{
    bool operator()(const cv::Point& lhs, const cv::Point& rhs) const;
};

typedef std::set<cv::Point, comparePoints> PointerPoints;

struct DialParserConfiguration
{   //Dial dead zone and value range
    double minValue;
    double maxValue;
    double minAngle;
    double maxAngle;
};

class DialParser
{
public:
    DialParser(DialParserConfiguration conf, const cv::Mat &binaryImage,
               const cv::Point center);
    void updateImage(const cv::Mat &binaryImage);

    //Returns the value showed by the pointer according to configuration
    bool hasPointer() const;
    double readValuePercent() const;
    double readValue() const;
    
    void paintPoints(cv::Mat &image);
    
private:
    DialParserConfiguration mConfig;
    const cv::Point mDialCenter;
    PointerPoints mPointerPoints;
    double mAngle;

    PointerPoints findPointerPoints(const cv::Mat &image) const;
    cv::Point findPointerEnd() const;
    double calculateAngle(cv::Point end) const;
    double calculatePercentage() const;
    double calculateValue() const;
};

#endif //DIAL_PARSER_H