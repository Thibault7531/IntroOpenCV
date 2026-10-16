#pragma once
#include <opencv2/core/mat.hpp>
#include <opencv2/dnn/version.hpp>

#include "Histogram.h"

class ImageComparator
{
public:
    ImageComparator()
        : nBins{8} {}

    void setNumberOfBins(int nBins) { this->nBins = nBins; }
    int getNumberOfBins() const { return nBins; }
    void setReferenceImage(const cv::Mat& image);
    double compare(const cv::Mat& image);

private:
    cv::Mat refH;
    cv::Mat inputH;

    Histogram1D hist;
    int nBins;
};
