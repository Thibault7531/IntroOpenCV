#pragma once

#include <cstdint>
#include <opencv2/core/mat.hpp>

class EdgeDetectionStats
{
public:
    struct Statistics
    {
        uint32_t truePositive;
        uint32_t falsePositive;
        uint32_t falseNegative;

        float precision;
        float recall;
        float f1Score;
    };
    struct PairCandidate
    {
        uint32_t detectedEdgeIndex;
        uint32_t referenceEdgeIndex;
        float distance;
    };

    static float GetFalsePositiveRate(Statistics stats);
    static float GetFalseNegativeRate(Statistics stats);

    static Statistics GetStatistics(cv::Mat image, cv::Mat reference);
};
