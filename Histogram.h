#pragma once
#include <opencv2/core.hpp>

class Histogram1D
{
public:
    Histogram1D();

    void setChannel(int channel) { channels[0] = channel; }
    void setRange(float min, float max) { hranges[0] = min; hranges[1] = max; }
    float getMinValue() const { return hranges[0]; }
    float getMaxValue() const { return hranges[1]; }
    void setNBins(int bins) { histSize[0] = bins; }
    int getNBins() const { return histSize[0]; }

    cv::Mat getHistogram(const cv::Mat& image);
    cv::Mat getHueHistogram(const cv::Mat& image, int minSaturation = 0);
    static cv::Mat GetHistogramImage(const cv::Mat& histogram);

private:
    int histSize[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];
};
