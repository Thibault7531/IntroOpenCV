#pragma once

#include <cstdint>
#include <opencv2/core/mat.hpp>

class EdgeDetectionStats
{
public:
    static uint32_t GetEdgePixelCount(cv::Mat image);
    static uint32_t GetCorrectEdgePixelCount(cv::Mat image, cv::Mat reference);
    static uint32_t GetFalsePositivePixelCount(cv::Mat image, cv::Mat reference);
    static uint32_t GetFalseNegativePixelCount(cv::Mat image, cv::Mat reference);

    static float GetPerformace(cv::Mat image, cv::Mat reference);
    static float GetFalsePositiveRate(cv::Mat image, cv::Mat reference);
    static float GetFalseNegativeRate(cv::Mat image, cv::Mat reference);
};
