#include "DialParser.h"
#include <stack>

using namespace cv;

namespace
{
    double distance(const Point &a, const Point &b)
    {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }
}

bool comparePoints::operator()(const Point& lhs, const Point& rhs) const
{
    return ((lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y)));
}

DialParser::DialParser(DialParserConfiguration conf, const cv::Mat &image,
  const cv::Point center) : mConfig(conf), mDialCenter(center)
{
    updateImage(image);
}

void DialParser::updateImage(const cv::Mat &image)
{
    if (!image.data)
    {
        printf("Incorrect image passed - no data!\n");
    }
    mPointerPoints = findPointerPoints(image);
    if (hasPointer())
    {
        Point end = findPointerEnd();
        mAngle = calculateAngle(end);
    }
}

bool DialParser::hasPointer() const
{
    return !mPointerPoints.empty();
}

double DialParser::readValuePercent() const
{
    return calculatePercentage();
}

double DialParser::readValue() const
{
    return calculateValue();
}

Point DialParser::findPointerEnd() const
{
    assert(hasPointer());
    if (!hasPointer())
        return Point(-1,-1); //it's a private function, should never be called like this
    
    int dist = 0;
    Point furthestPoint = mDialCenter;
    for (auto p : mPointerPoints)
    {
        int newdist = distance(mDialCenter, p);
        if (newdist > dist)
        {
            dist = newdist;
            furthestPoint = p;
        }
    }
    return furthestPoint;
}
    
PointerPoints DialParser::findPointerPoints(const cv::Mat &binaryImage) const
{
    PointerPoints pointsOfPointer;
    
    const int channels = binaryImage.channels();
    if (channels != 1)
        return pointsOfPointer; //Incorrect image passed!

    const int startingTolerance = 7; //accept other-colored pixels for this region
   
    PointerPoints checkedPoints;
    std::stack<Point> toCheck;
    toCheck.push(mDialCenter);
    while (!toCheck.empty())
    {
        Point p = toCheck.top();
        int x = p.x;
        int y = p.y;
        toCheck.pop();
        checkedPoints.insert(p);
        int newdist = distance(mDialCenter, p);
        if (newdist > startingTolerance && binaryImage.at<uchar>(p) != 0)
            continue;
            
        pointsOfPointer.insert(p);
        
        //put next points to check
        if (x+1 < binaryImage.rows)
        {
            Point right(x+1, y);
            if (checkedPoints.count(right) == 0)
                toCheck.push(right);
        }
        if (x-1 > 0)
        {
            Point left(x-1, y);
            if (checkedPoints.count(left) == 0)
                toCheck.push(left);
        }
        if (y+1 < binaryImage.cols)
        {
            Point down(x, y+1);
            if (checkedPoints.count(down) == 0)
                toCheck.push(down);
        }
        if (y-1 > 0)
        {
            Point up(x, y-1);
            if (checkedPoints.count(up) == 0)
                toCheck.push(up);
        }
    }

    return pointsOfPointer;
}

double DialParser::calculateAngle(Point far) const
{
    Point mid = mDialCenter;
    double angle = atan2(mid.y - far.y, mid.x - far.x);
    double offset = 0;
    //printf("angle to x axis %lf, cvpi2 is %lf", angle, CV_PI/2);
    if (far.x < mid.x && far.y < mid.y)
    {
        //printf("Quadrant IV\n");
        offset = CV_PI/2;
        angle = angle + offset;
    }
    else if (far.x < mid.x && far.y > mid.y)
    {
        //printf("Quadrant III\n");
        offset = 0;
        angle = CV_PI/2 + angle;
    }
    else if (far.x > mid.x && far.y < mid.y)
    {
        //printf("Quadrant I\n");
        offset = CV_PI/2;
        angle = angle + offset;
    }
    else if (far.x > mid.x && far.y > mid.y)
    {
        //printf("Quadrant II\n");
        offset = 3*CV_PI/2;
        angle = CV_PI + angle + offset;
    }
    return angle;
}

//Angle withy y axis, going + from down to right, going - from down to left
double DialParser::calculatePercentage() const
{   //TODO
    double range = mConfig.maxAngle - mConfig.minAngle;
    double valPerRadian = (mConfig.maxValue - mConfig.minValue)/(range);
    double value = (mAngle-mConfig.minAngle)*valPerRadian;
    double percent = (value*100)/(mConfig.maxValue);
    printf("value is %lf percent, which on a dial from %lf to %lf is %lf\n",
            percent, mConfig.maxValue, mConfig.minValue, value);

    return percent;
}

double DialParser::calculateValue() const
{
    double percentage = calculatePercentage();
    double value = (percentage*(mConfig.maxValue-mConfig.minValue)/100);
    return value;
}

void DialParser::paintPoints(Mat &image)
{
    if (image.channels() == 3)
    {            
        for (auto p : mPointerPoints)
        {
            Vec3b col = image.at<Vec3b>(p);
            col[0] = 255;
            col[1] = 0;
            col[2] = 255;
            image.at<Vec3b>(p) = col;
        }
        line(image, mDialCenter, findPointerEnd(), Scalar(0,0,255));
    }
    else
    {
        printf("Error: add impl. for single channel visualisation\n");
    }
}