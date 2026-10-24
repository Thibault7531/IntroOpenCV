#include "SobelEdgeDetection.h"

#include <opencv2/imgproc.hpp>

cv::Mat SobelEdgeDetection::GetEdges(cv::Mat image, int threshold, int kernelSize)
{
    cv::Mat SobelX;
    cv::Sobel(image, SobelX, CV_16S, 1, 0, kernelSize);

    cv::Mat SobelY;
    cv::Sobel(image, SobelY, CV_16S, 0, 1, kernelSize);

    cv::Mat Sobel = abs(SobelX) + abs(SobelY);

    double sobMin, sobMax;
    cv::minMaxLoc(Sobel, &sobMin, &sobMax);

    cv::Mat SobelImage;
    Sobel.convertTo(SobelImage, CV_8U, -255.0/sobMax, 255.0);

    cv::threshold(SobelImage, SobelImage, threshold, 255, cv::THRESH_BINARY);
    //cv::cvtColor(SobelImage, SobelImage, cv::COLOR_BGR2GRAY);

    return SobelImage;
}
