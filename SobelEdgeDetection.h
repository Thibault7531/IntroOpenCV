#pragma once
#include <opencv2/core.hpp>

class SobelEdgeDetection
{
public:
    static cv::Mat GetEdges(cv::Mat image, int threshold, int kernelSier = 3);
};
