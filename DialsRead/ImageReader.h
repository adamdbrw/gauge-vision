#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include <opencv2/opencv.hpp>
#include <string>

class ImageReader
{
public:
    cv::Mat readNextImage() const;
};

#endif //IMAGE_READER_H