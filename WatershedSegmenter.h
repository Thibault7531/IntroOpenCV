#pragma once
#include <opencv2/core.hpp>

class WatershedSegmenter
{
public:
    void setMarkers(const cv::Mat& markers);
    cv::Mat process(const cv::Mat& image);
    cv::Mat getSegmentation();
    cv::Mat getWatersheds();

private:
    cv::Mat markers;
};
