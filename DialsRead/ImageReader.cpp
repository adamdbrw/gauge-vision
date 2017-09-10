#include "ImageReader.h"

using namespace cv;

namespace
{
    const std::string debugImageFile = "dial1.jpeg";
}

Mat ImageReader::readNextImage() const
{
    std::string path = "./images/" + debugImageFile;
    Mat image = imread(path, CV_LOAD_IMAGE_COLOR);
    if (!image.data)
    {
         printf("Could not load image from file %s, but going on to test api for resilience\n", path.c_str());
    }
    return image;
}