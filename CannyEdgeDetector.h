#pragma once
#include <opencv2/core.hpp>

class CannyEdgeDetector
{
public:
    static cv::Mat GetEdges(const cv::Mat& image, int threshold1, int threshold2);
};
