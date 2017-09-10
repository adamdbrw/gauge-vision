#ifndef GAUGE_READER_H
#define GAUGE_READER_H

#include <opencv2/core/mat.hpp>
#include "DebugDisplay.h"

typedef double gaugeReading;

class GaugeReader
{
public:
    gaugeReading readGauge(const cv::Mat &image, const DebugDisplay &display,
                           bool visualise = false) const;
};

#endif //GAUGE_READER_H