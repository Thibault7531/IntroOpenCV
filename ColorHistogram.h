#pragma once
#include <opencv2/dnn/dnn.hpp>

class ColorHistogram
{
public:
    ColorHistogram();

    cv::Mat getHistogram(const cv::Mat& image);

private:
    int histSize[3];
    float hranges[2];
    const float* ranges[3];
    int channels[3];
};
