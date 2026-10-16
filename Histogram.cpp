#include "Histogram.h"

#include <opencv2/imgproc.hpp>

Histogram1D::Histogram1D()
{
    histSize[0] = 256;
    hranges[0] = 0.0;
    hranges[1] = 256.0;
    ranges[0] = hranges;
    channels[0] = 0;
}

cv::Mat Histogram1D::getHistogram(const cv::Mat& image)
{
    cv::Mat histogram;
    cv::calcHist(&image, 1, channels, cv::Mat(), histogram, 1, histSize, ranges);
    return histogram;
}

cv::Mat Histogram1D::getHueHistogram(const cv::Mat& image, int minSaturation)
{
    cv::Mat histogram;
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    if (minSaturation)
    {
        std::vector<cv::Mat> v;
        cv::split(hsv, v);
        cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);
    }

    cv::calcHist(&hsv, 1, channels, mask, histogram, 1, histSize, ranges);

    return histogram;
}

cv::Mat Histogram1D::GetHistogramImage(const cv::Mat& histogram)
{
    double min = 0;
    double max = 0;
    cv::minMaxLoc(histogram, &min, &max);
    cv::Mat histImage(histogram.cols, histogram.cols, CV_8U, cv::Scalar(255));
    for (int h = 0; h < histogram.cols; ++h)
    {
        float binVal = histogram.at<float>(h);
        if (binVal > 0)
        {
            int intensity = static_cast<int>(binVal * histogram.cols / max);
            cv::line(histImage, cv::Point(h, histogram.cols), cv::Point(h, histogram.cols - intensity), cv::Scalar(0));
        }
    }
    return histImage;
}
