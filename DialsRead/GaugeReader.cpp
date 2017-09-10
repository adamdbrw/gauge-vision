#include "GaugeReader.h"
#include "DialParser.h"

using namespace cv;

namespace
{
    Mat preprocessImage(const Mat &image)
    {
        if (image.channels() == 1)
            return image;
        
        Mat greyImage;
        cvtColor(image, greyImage, COLOR_BGR2GRAY);

        //blur(greyImage, greyImage, Size(3, 3));
        return greyImage;
    }

    typedef std::vector<Vec3f> Circles;
    struct Circle {int radius; Point center;};
    
    auto getCircle(Vec3f c)
    {
        Point center(cvRound(c[0]), cvRound(c[1]));
        int radius = cvRound(c[2]);
        return Circle{radius, center};
    }
    
    //Finds circles on the image
    Circles findCircles(const Mat &image)
    {
        Circles circles;
        if (!image.data || image.channels() != 1)
        {
            printf("findCircles:: invalid image format\n");
            return circles; 
        }
        
        const int minRadius = 15;
        const int minCirclesDistance = 30;
        HoughCircles(image, circles, CV_HOUGH_GRADIENT, 1, minCirclesDistance,
                     200, 100, minRadius, 0);
        return circles;
    }
    
    void getLargestCircle(const Circles &circles, Point &largestCenter, int &largestRadius)
    {
        largestCenter = Point(-1,-1);
        largestRadius = 0;
        if (circles.empty())
            return;
            
        for(auto circle : circles)
        {
            Circle cir = getCircle(circle);
            if (cir.radius > largestRadius)
            {
                largestRadius = cir.radius;
                largestCenter = cir.center;
            }
        }
    }

    void paintCircles(Mat &image, const Circles &circles)
    {
        for(size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            circle(image, center, 3, Scalar(0,255,0), -1, 8, 0);
            circle(image, center, radius, Scalar(0,0,255), 3, 8, 0);
        }
    }
}

gaugeReading GaugeReader::readGauge(const Mat &image, const DebugDisplay &display,
                                    bool visualise) const
{
    Mat debugImage;
    if (visualise)
        debugImage = image.clone();
    
    Mat greyImage = preprocessImage(image);
    Circles circles = findCircles(greyImage);
    if (visualise)
        paintCircles(debugImage, circles);
    
    Point dialCenter(-1,-1);
    int largestRadius = 0; //TODO not used currently
    getLargestCircle(circles, dialCenter, largestRadius);
    if (largestRadius < 1)
    {
        printf("Can't find any circle in the image!");
        return -1; //TODO!
    }
    if (visualise)
        display.showImage("Circled", debugImage, Point(2, 1));
    
    threshold(greyImage, greyImage, 100, 255, THRESH_BINARY);
    
    if (visualise)
        display.showImage("binary", greyImage, Point(3, 1));
    
    DialParserConfiguration conf; //TODO read from file
    conf.minAngle = 0.785;
    conf.maxAngle = 2*CV_PI - conf.minAngle;
    conf.minValue = 0;
    conf.maxValue = 160;
    DialParser dialParser(conf, greyImage, dialCenter);
    if (!dialParser.hasPointer())
    {
        printf("No pointer detected!!\n");
        return -1; //TODO!
    }
    double v = dialParser.readValuePercent();
    
    if (visualise)
    {
        dialParser.paintPoints(debugImage);
        display.showImage("Pointer", debugImage, Point(4, 1));
        display.displayValue(v, Point(5, 1));
    }

    return v;
}