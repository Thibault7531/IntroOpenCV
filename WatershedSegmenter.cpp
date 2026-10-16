#include "WatershedSegmenter.h"

#include <opencv2/imgproc.hpp>

void WatershedSegmenter::setMarkers(const cv::Mat& markers)
{
    markers.convertTo(this->markers, CV_32S);
}

cv::Mat WatershedSegmenter::process(const cv::Mat& image)
{
    cv::watershed(image, markers);
    return markers;
}

cv::Mat WatershedSegmenter::getSegmentation()
{
    cv::Mat tmp;
    markers.convertTo(tmp, CV_8U);
    return tmp;
}

cv::Mat WatershedSegmenter::getWatersheds()
{
    cv::Mat tmp;
    markers.convertTo(tmp, CV_8U, 255, 255);
    return tmp;
}
