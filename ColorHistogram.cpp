#include "ColorHistogram.h"

#include <opencv2/imgproc.hpp>

ColorHistogram::ColorHistogram()
{
    histSize[0] = histSize[1] = histSize[2] = 256;
    hranges[0] = 0.0;
    hranges[1] = 256.0;
    ranges[0] = hranges;
    ranges[1] = hranges;
    ranges[2] = hranges;
    channels[0] = 0;
    channels[1] = 1;
    channels[2] = 2;
}

cv::Mat ColorHistogram::getHistogram(const cv::Mat& image)
{
    cv::Mat histogram;
    cv::calcHist(&image, 1, channels, cv::Mat(), histogram, 1, histSize, ranges);
    return histogram;
}
