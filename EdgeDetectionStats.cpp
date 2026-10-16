#include "EdgeDetectionStats.h"

uint32_t EdgeDetectionStats::GetEdgePixelCount(cv::Mat image)
{
    auto it = image.begin<int>();
    auto itend = image.end<int>();
    uint32_t count = 0;
    for (; it != itend; ++it)
    {
        if (*it <= 128) count++;
    }
    return count;
}

uint32_t EdgeDetectionStats::GetCorrectEdgePixelCount(cv::Mat image, cv::Mat reference)
{
    auto it = image.begin<int>();
    auto itref = reference.begin<int>();
    auto itend = image.end<int>();
    uint32_t count = 0;
    for (; it != itend; ++it, ++itref)
    {
        if (*it <= 128 && *itref <= 128) count++;
    }
    return count;
}

uint32_t EdgeDetectionStats::GetFalsePositivePixelCount(cv::Mat image, cv::Mat reference)
{
    return GetEdgePixelCount(image) - GetCorrectEdgePixelCount(image, reference);
}

uint32_t EdgeDetectionStats::GetFalseNegativePixelCount(cv::Mat image, cv::Mat reference)
{
    return GetEdgePixelCount(reference) - GetCorrectEdgePixelCount(image, reference);
}

float EdgeDetectionStats::GetPerformace(cv::Mat image, cv::Mat reference)
{
    uint32_t correctEdges = GetCorrectEdgePixelCount(image, reference);
    uint32_t falsePositiveEdges = GetFalsePositivePixelCount(image, reference);
    uint32_t falseNegativeEdges = GetFalseNegativePixelCount(image, reference);
    return (float)correctEdges / (correctEdges + falsePositiveEdges + falseNegativeEdges);
}

float EdgeDetectionStats::GetFalsePositiveRate(cv::Mat image, cv::Mat reference)
{
    uint32_t correctEdges = GetCorrectEdgePixelCount(image, reference);
    uint32_t falsePositiveEdges = GetFalsePositivePixelCount(image, reference);
    uint32_t falseNegativeEdges = GetFalseNegativePixelCount(image, reference);
    return (float)falsePositiveEdges / (correctEdges + falsePositiveEdges + falseNegativeEdges);
}

float EdgeDetectionStats::GetFalseNegativeRate(cv::Mat image, cv::Mat reference)
{
    uint32_t correctEdges = GetCorrectEdgePixelCount(image, reference);
    uint32_t falsePositiveEdges = GetFalsePositivePixelCount(image, reference);
    uint32_t falseNegativeEdges = GetFalseNegativePixelCount(image, reference);
    return (float)falseNegativeEdges / (correctEdges + falsePositiveEdges + falseNegativeEdges);
}
