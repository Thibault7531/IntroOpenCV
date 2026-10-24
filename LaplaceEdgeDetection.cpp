#include "LaplaceEdgeDetection.h"

#include <opencv2/imgproc.hpp>

cv::Mat LaplaceEdgeDetection::GetEdges(cv::Mat image, int aperture, int threshold)
{
    cv::Mat gauss1;
    cv::GaussianBlur(image, gauss1, cv::Size(), 2.0);
    cv::Mat gauss2;
    cv::GaussianBlur(image, gauss2, cv::Size(), 2.2);

    cv::Mat blur;
    cv::subtract(gauss1, gauss2, blur, cv::Mat(), CV_32F);

    cv::Mat laplace;
    cv::Laplacian(blur, laplace, CV_32F, aperture);

    //double minVal, maxVal;
    //cv::minMaxLoc(laplace, &minVal, &maxVal);
    //double scale = 127 / std::max(-minVal, maxVal);

    cv::Mat absLaplacian;
    cv::convertScaleAbs(laplace, absLaplacian);

    //cv::Mat edges;
    //laplace.convertTo(edges, CV_8U, scale, 128);

    cv::Mat signImage;
    cv::threshold(absLaplacian, signImage, threshold, 255, cv::THRESH_BINARY);

    cv::Mat binary;
    signImage.convertTo(binary, CV_8U);

    cv::Mat dilated;
    cv::dilate(binary, dilated, cv::Mat());
    return 255 - (dilated);
}
