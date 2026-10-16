#include "CannyEdgeDetector.h"

#include <opencv2/imgproc.hpp>

cv::Mat CannyEdgeDetector::GetEdges(const cv::Mat& image, int threshold1, int threshold2)
{
    cv::Mat edges;
    cv::Canny(image, edges, threshold1, threshold2);
    return 255 - edges;
}
