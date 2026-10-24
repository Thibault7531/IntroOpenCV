#include "EdgeDetectionStats.h"

float EdgeDetectionStats::GetFalsePositiveRate(Statistics stats)
{
    return (float)stats.falsePositive / (stats.truePositive + stats.falsePositive + stats.falseNegative);
}

float EdgeDetectionStats::GetFalseNegativeRate(Statistics stats)
{
    return (float)stats.falseNegative / (stats.truePositive + stats.falsePositive + stats.falseNegative);
}

EdgeDetectionStats::Statistics EdgeDetectionStats::GetStatistics(cv::Mat image, cv::Mat reference)
{
    Statistics statistics;
    std::vector<cv::Point> detectedEdges;
    std::vector<cv::Point> referenceEdges;

    for (uint32_t y = 0; y < image.rows; y++)
    {
        for (uint32_t x = 0; x < image.cols; x++)
        {
            if (image.at<uchar>(y, x) <= 128)
            {
                detectedEdges.push_back(cv::Point(x, y));
            }
            if (reference.at<uchar>(y, x) <= 128)
            {
                referenceEdges.push_back(cv::Point(x, y));
            }
        }
    }

    std::vector<PairCandidate> pairCandidates;

    for (uint32_t detectedEdgeIndex = 0; detectedEdgeIndex < detectedEdges.size(); detectedEdgeIndex++)
    {
        cv::Point detectedEdge = detectedEdges[detectedEdgeIndex];

        for (uint32_t referenceEdgeIndex = 0; referenceEdgeIndex < referenceEdges.size(); referenceEdgeIndex++)
        {
            cv::Point referenceEdge = referenceEdges[referenceEdgeIndex];

            float rowDistance = abs(detectedEdge.y - referenceEdge.y);
            float columnDistance = abs(detectedEdge.x - referenceEdge.x);

            if (rowDistance <= 1 && columnDistance <= 1)
            {
                float distance = cv::norm(detectedEdge - referenceEdge);
                pairCandidates.push_back(PairCandidate{detectedEdgeIndex, referenceEdgeIndex, distance});
            }
        }
    }

    std::ranges::sort(pairCandidates, [](const PairCandidate& a, const PairCandidate& b) {
        return a.distance < b.distance;
    });

    std::vector<bool> detectedEdgesUsed(detectedEdges.size(), false);
    std::vector<bool> referenceEdgesUsed(referenceEdges.size(), false);

    statistics.truePositive = 0;

    for (auto& pairCandidate : pairCandidates)
    {
        if (!detectedEdgesUsed[pairCandidate.detectedEdgeIndex] && !referenceEdgesUsed[pairCandidate.referenceEdgeIndex])
        {
            detectedEdgesUsed[pairCandidate.detectedEdgeIndex] = true;
            referenceEdgesUsed[pairCandidate.referenceEdgeIndex] = true;
            statistics.truePositive++;
        }
    }

    statistics.falsePositive = detectedEdges.size() - statistics.truePositive;
    statistics.falseNegative = referenceEdges.size() - statistics.truePositive;

    if (statistics.truePositive + statistics.falsePositive == 0) statistics.precision = 0.0f;
    else statistics.precision = (float)statistics.truePositive / (statistics.truePositive + statistics.falsePositive);

    if (statistics.truePositive + statistics.falseNegative == 0) statistics.recall = 0.0f;
    else statistics.recall = (float)statistics.truePositive / (statistics.truePositive + statistics.falseNegative);

    if (2.0f * statistics.truePositive + statistics.falsePositive + statistics.falseNegative == 0) statistics.f1Score = 0.0f;
    else statistics.f1Score = 2.0f * statistics.truePositive / (2.0f * statistics.truePositive + statistics.falsePositive + statistics.falseNegative);

    return statistics;
}
