#include "ImageComparator.h"

#include <opencv2/imgproc.hpp>

void ImageComparator::setReferenceImage(const cv::Mat& image)
{
    hist.setNBins(nBins);
    refH = hist.getHistogram(image);
}

double ImageComparator::compare(const cv::Mat& image)
{
    inputH = hist.getHistogram(image);
    return cv::compareHist(refH, inputH, cv::HISTCMP_INTERSECT);
}
