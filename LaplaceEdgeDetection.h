#pragma once
#include <opencv2/dnn/dnn.hpp>

class LaplaceEdgeDetection
{
public:
    static cv::Mat GetEdges(cv::Mat image, int aperture, int threshold = 128);
};
