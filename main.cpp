#include <filesystem>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv5/opencv2/core.hpp>
#include <opencv5/opencv2/highgui.hpp>

#include "CannyEdgeDetector.h"
#include "EdgeDetectionStats.h"
#include "LaplaceEdgeDetection.h"
#include "SobelEdgeDetection.h"

int main()
{
    std::printf("Edge Detection Test\n");
    std::printf("\n");
    std::printf("1. Test Sobel algorithm\n");
    std::printf("2. Test Laplace algorithm\n");
    std::printf("3. Test Canny algorithm\n");
    std::printf("0. Exit\n");
    std::printf("\n");
    std::printf("Choice: ");
    std::cout << std::flush;

    int choice;
    std::scanf("%d", &choice);

    if (choice == 0)
    {
        return 0;
    }

    if (choice == 1)
    {
SobelImageSelection:
        std::printf("\n");
        std::printf("Choose image (\"All\" for all images): ");
        std::cout << std::flush;

        std::filesystem::path imagePath;
        std::cin >> imagePath;

        if (imagePath != "All")
        {
            cv::Mat image = cv::imread(std::filesystem::path("images") / imagePath, cv::IMREAD_GRAYSCALE);
            std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.stem().string() + "_gt_binary.pgm");
            cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);

            if (image.empty() || reference.empty())
            {
                std::printf("Error: image or reference not found\n");
                goto SobelImageSelection;
            }
        }

        int kernelSize = 1;
        int threshold = 128;
        if (imagePath == "All")
        {
            SobelKernelSizeSelection:
            std::printf("Choose kernel size (1 - 31 odd): ");
            std::cout << std::flush;

            std::scanf("%d", &kernelSize);

            if (kernelSize < 1 || kernelSize > 31 || kernelSize % 2 == 0)
            {
                std::printf("Error: invalid kernel size\n");
                goto SobelKernelSizeSelection;
            }

            SobelThresholdSelection:
                    std::printf("Choose threshold (0 - 255): ");
            std::cout << std::flush;

            std::scanf("%d", &threshold);

            if (threshold < 0 || threshold > 255)
            {
                std::printf("Error: invalid threshold\n");
                goto SobelThresholdSelection;
            }
        }

        std::printf("\n");

        if (imagePath == "All")
        {
            float averagePerformance = 0.0f;
            float averageFalsePositiveRate = 0.0f;
            float averageFalseNegativeRate = 0.0f;
            std::vector<float> Performances;
            std::vector<float> FalsePositiveRates;
            std::vector<float> FalseNegativeRates;
            int imageCount = 0;
            float maxPerformance = -1.0f;
            std::string maxPerformanceImagePath;
            float maxFalsePositiveRate = -1.0f;
            std::string maxFalsePositiveImagePath;
            float maxFalseNegativeRate = -1.0f;
            std::string maxFalseNegativeImagePath;

            for (auto imagePath : std::filesystem::directory_iterator("images"))
            {
                if (imagePath.is_regular_file())
                {
                    cv::Mat image = cv::imread(imagePath.path(), cv::IMREAD_GRAYSCALE);
                    std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.path().stem().string() + "_gt_binary.pgm");
                    cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);

                    if (!reference.empty())
                    {
                        cv::Mat edges = SobelEdgeDetection::GetEdges(image, threshold, kernelSize);

                        EdgeDetectionStats::Statistics statistics = EdgeDetectionStats::GetStatistics(edges, reference);

                        float performance = statistics.f1Score;
                        float falsePositiveRate = EdgeDetectionStats::GetFalsePositiveRate(statistics);
                        float falseNegativeRate = EdgeDetectionStats::GetFalseNegativeRate(statistics);

                        averagePerformance += performance;
                        averageFalsePositiveRate += falsePositiveRate;
                        averageFalseNegativeRate += falseNegativeRate;
                        Performances.push_back(performance);
                        FalsePositiveRates.push_back(falsePositiveRate);
                        FalseNegativeRates.push_back(falseNegativeRate);
                        imageCount++;

                        if (performance > maxPerformance)
                        {
                            maxPerformance = performance;
                            maxPerformanceImagePath = imagePath.path().string();
                        }
                        if (falsePositiveRate > maxFalsePositiveRate)
                        {
                            maxFalsePositiveRate = falsePositiveRate;
                            maxFalsePositiveImagePath = imagePath.path().string();
                        }
                        if (falseNegativeRate > maxFalseNegativeRate)
                        {
                            maxFalseNegativeRate = falseNegativeRate;
                            maxFalseNegativeImagePath = imagePath.path().string();
                        }
                    }
                }
            }

            averagePerformance /= imageCount;
            averageFalsePositiveRate /= imageCount;
            averageFalseNegativeRate /= imageCount;

            std::printf("Image count: %d\n", imageCount);
            std::printf("Average performance: %f\n", averagePerformance);
            std::printf("Average false positive rate: %f\n", averageFalsePositiveRate);
            std::printf("Average false negative rate: %f\n", averageFalseNegativeRate);
            std::printf("Median performance: %f\n", Performances[Performances.size() / 2]);
            std::printf("Median false positive rate: %f\n", FalsePositiveRates[FalsePositiveRates.size() / 2]);
            std::printf("Median false negative rate: %f\n", FalseNegativeRates[FalseNegativeRates.size() / 2]);
            std::printf("Max performance: %f\n", maxPerformance);
            std::printf("Max performance image path: %s\n", maxPerformanceImagePath.c_str());
            std::printf("Max false positive rate: %f\n", maxFalsePositiveRate);
            std::printf("Max false positive rate image path: %s\n", maxFalsePositiveImagePath.c_str());
            std::printf("Max false negative rate: %f\n", maxFalseNegativeRate);
            std::printf("Max false negative rate image path: %s\n", maxFalseNegativeImagePath.c_str());
            std::cout << std::flush;
        }
        else
        {
            cv::Mat image = cv::imread(std::filesystem::path("images") / imagePath, cv::IMREAD_GRAYSCALE);
            std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.stem().string() + "_gt_binary.pgm");
            cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);
            cv::Mat edges = SobelEdgeDetection::GetEdges(image, threshold, kernelSize);

            cv::namedWindow("Image", cv::WINDOW_NORMAL);
            cv::imshow("Image", image);

            cv::namedWindow("Reference", cv::WINDOW_NORMAL);
            cv::imshow("Reference", reference);

            cv::namedWindow("Edges", cv::WINDOW_NORMAL);
            cv::imshow("Edges", edges);
            cv::createTrackbar("Kernel Size", "Edges", nullptr, 15);
            cv::createTrackbar("Threshold", "Edges", nullptr, 255);

            while (true)
            {
                int newThreshold = cv::getTrackbarPos("Threshold", "Edges");
                int newKernelSize = cv::getTrackbarPos("Kernel Size", "Edges");
                if (newThreshold != threshold || newKernelSize != kernelSize)
                {
                    threshold = newThreshold;
                    kernelSize = newKernelSize;
                    if (kernelSize%2 == 1 && kernelSize > 0 && kernelSize <= 31)
                    {
                        edges = SobelEdgeDetection::GetEdges(image, threshold, kernelSize);

                        EdgeDetectionStats::Statistics statistics = EdgeDetectionStats::GetStatistics(edges, reference);

                        float performance = statistics.f1Score;
                        float falsePositiveRate = EdgeDetectionStats::GetFalsePositiveRate(statistics);
                        float falseNegativeRate = EdgeDetectionStats::GetFalseNegativeRate(statistics);

                        std::cout << "Performance: " << performance << std::endl;
                        std::cout << "False Positive Rate: " << falsePositiveRate << std::endl;
                        std::cout << "False Negative Rate: " << falseNegativeRate << std::endl;

                        cv::putText(edges, "Performance: " + std::to_string(performance), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
                        cv::imshow("Edges", edges);
                    }
                }

                int k = cv::waitKey(10) & 0xFF;
                if (k == 27) break;
            }
        }
    }

    if (choice == 2)
    {
LaplaceImageSelection:
        std::printf("\n");
        std::printf("Choose image (\"All\" for all images): ");
        std::cout << std::flush;

        std::filesystem::path imagePath;
        std::cin >> imagePath;

        if (imagePath != "All")
        {
            cv::Mat image = cv::imread(std::filesystem::path("images") / imagePath, cv::IMREAD_GRAYSCALE);
            std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.stem().string() + "_gt_binary.pgm");
            cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);

            if (image.empty() || reference.empty())
            {
                std::printf("Error: image or reference not found\n");
                goto LaplaceImageSelection;
            }
        }

        int kernelSize = 1;
        int threshold = 128;
        if (imagePath == "All")
        {
            LaplaceKernelSizeSelection:
                    std::printf("Choose kernel size (1 - 31 odd): ");
            std::cout << std::flush;

            std::scanf("%d", &kernelSize);

            if (kernelSize < 1 || kernelSize > 31 || kernelSize % 2 == 0)
            {
                std::printf("Error: invalid kernel size\n");
                goto LaplaceKernelSizeSelection;
            }

            LaplaceThresholdSelection:
                    std::printf("Choose threshold (0 - 255): ");
            std::cout << std::flush;

            std::scanf("%d", &threshold);

            if (threshold < 0 || threshold > 255)
            {
                std::printf("Error: invalid threshold\n");
                goto LaplaceThresholdSelection;
            }
        }

        std::printf("\n");

        if (imagePath == "All")
        {
            float averagePerformance = 0.0f;
            float averageFalsePositiveRate = 0.0f;
            float averageFalseNegativeRate = 0.0f;
            std::vector<float> Performances;
            std::vector<float> FalsePositiveRates;
            std::vector<float> FalseNegativeRates;
            int imageCount = 0;
            float maxPerformance = -1.0f;
            std::string maxPerformanceImagePath;
            float maxFalsePositiveRate = -1.0f;
            std::string maxFalsePositiveImagePath;
            float maxFalseNegativeRate = -1.0f;
            std::string maxFalseNegativeImagePath;

            for (auto imagePath : std::filesystem::directory_iterator("images"))
            {
                if (imagePath.is_regular_file())
                {
                    cv::Mat image = cv::imread(imagePath.path(), cv::IMREAD_GRAYSCALE);
                    std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.path().stem().string() + "_gt_binary.pgm");
                    cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);

                    if (!reference.empty())
                    {
                        cv::Mat edges = LaplaceEdgeDetection::GetEdges(image, kernelSize, threshold);

                        EdgeDetectionStats::Statistics statistics = EdgeDetectionStats::GetStatistics(edges, reference);

                        float performance = statistics.f1Score;
                        float falsePositiveRate = EdgeDetectionStats::GetFalsePositiveRate(statistics);
                        float falseNegativeRate = EdgeDetectionStats::GetFalseNegativeRate(statistics);

                        averagePerformance += performance;
                        averageFalsePositiveRate += falsePositiveRate;
                        averageFalseNegativeRate += falseNegativeRate;
                        Performances.push_back(performance);
                        FalsePositiveRates.push_back(falsePositiveRate);
                        FalseNegativeRates.push_back(falseNegativeRate);
                        imageCount++;

                        if (performance > maxPerformance)
                        {
                            maxPerformance = performance;
                            maxPerformanceImagePath = imagePath.path().string();
                        }
                        if (falsePositiveRate > maxFalsePositiveRate)
                        {
                            maxFalsePositiveRate = falsePositiveRate;
                            maxFalsePositiveImagePath = imagePath.path().string();
                        }
                        if (falseNegativeRate > maxFalseNegativeRate)
                        {
                            maxFalseNegativeRate = falseNegativeRate;
                            maxFalseNegativeImagePath = imagePath.path().string();
                        }
                    }
                }
            }

            averagePerformance /= imageCount;
            averageFalsePositiveRate /= imageCount;
            averageFalseNegativeRate /= imageCount;

            std::printf("Image count: %d\n", imageCount);
            std::printf("Average performance: %f\n", averagePerformance);
            std::printf("Average false positive rate: %f\n", averageFalsePositiveRate);
            std::printf("Average false negative rate: %f\n", averageFalseNegativeRate);
            std::printf("Median performance: %f\n", Performances[Performances.size() / 2]);
            std::printf("Median false positive rate: %f\n", FalsePositiveRates[FalsePositiveRates.size() / 2]);
            std::printf("Median false negative rate: %f\n", FalseNegativeRates[FalseNegativeRates.size() / 2]);
            std::printf("Max performance: %f\n", maxPerformance);
            std::printf("Max performance image path: %s\n", maxPerformanceImagePath.c_str());
            std::printf("Max false positive rate: %f\n", maxFalsePositiveRate);
            std::printf("Max false positive rate image path: %s\n", maxFalsePositiveImagePath.c_str());
            std::printf("Max false negative rate: %f\n", maxFalseNegativeRate);
            std::printf("Max false negative rate image path: %s\n", maxFalseNegativeImagePath.c_str());
            std::cout << std::flush;
        }
        else
        {
            cv::Mat image = cv::imread(std::filesystem::path("images") / imagePath, cv::IMREAD_GRAYSCALE);
            std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.stem().string() + "_gt_binary.pgm");
            cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);
            cv::Mat edges = LaplaceEdgeDetection::GetEdges(image, kernelSize, threshold);

            cv::namedWindow("Image", cv::WINDOW_NORMAL);
            cv::imshow("Image", image);

            cv::namedWindow("Reference", cv::WINDOW_NORMAL);
            cv::imshow("Reference", reference);

            cv::namedWindow("Edges", cv::WINDOW_NORMAL);
            cv::imshow("Edges", edges);
            cv::createTrackbar("Threshold", "Edges", nullptr, 255);
            cv::createTrackbar("KernelSize", "Edges", nullptr, 15);

            while (true)
            {
                int newThreshold = cv::getTrackbarPos("Threshold", "Edges");
                int newKernelSize = cv::getTrackbarPos("KernelSize", "Edges");
                if (newThreshold != threshold || newKernelSize != kernelSize)
                {
                    threshold = newThreshold;
                    kernelSize = newKernelSize;
                    if (kernelSize%2 == 1 && kernelSize > 0 && kernelSize <= 31)
                    {
                        edges = LaplaceEdgeDetection::GetEdges(image, kernelSize, threshold);

                        EdgeDetectionStats::Statistics statistics = EdgeDetectionStats::GetStatistics(edges, reference);

                        float performance = statistics.f1Score;
                        float falsePositiveRate = EdgeDetectionStats::GetFalsePositiveRate(statistics);
                        float falseNegativeRate = EdgeDetectionStats::GetFalseNegativeRate(statistics);

                        std::cout << "Performance: " << performance << std::endl;
                        std::cout << "False Positive Rate: " << falsePositiveRate << std::endl;
                        std::cout << "False Negative Rate: " << falseNegativeRate << std::endl;

                        cv::putText(edges, "Performance: " + std::to_string(performance), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
                        cv::imshow("Edges", edges);
                    }
                }

                int k = cv::waitKey(10) & 0xFF;
                if (k == 27) break;
            }
        }
    }

    if (choice == 3)
    {
        CannyImageSelection:
        std::printf("\n");
        std::printf("Choose image (\"All\" for all images): ");
        std::cout << std::flush;

        std::filesystem::path imagePath;
        std::cin >> imagePath;

        if (imagePath != "All")
        {
            cv::Mat image = cv::imread(std::filesystem::path("images") / imagePath, cv::IMREAD_GRAYSCALE);
            std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.stem().string() + "_gt_binary.pgm");
            cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);

            if (image.empty() || reference.empty())
            {
                std::printf("Error: image or reference not found\n");
                goto CannyImageSelection;
            }
        }

        int threshold1 = 125;
        int threshold2 = 350;
        if (imagePath == "All")
        {
            CannyThreshold1Selection:
            std::printf("Choose first threshold (0 - 500): ");
            std::cout << std::flush;

            std::scanf("%d", &threshold1);

            if (threshold1 < 0 || threshold1 > 500)
            {
                std::printf("Error: invalid threshold\n");
                goto CannyThreshold1Selection;
            }

            CannyThreshold2Selection:
            std::printf("Choose second threshold (0 - 500): ");
            std::cout << std::flush;

            std::scanf("%d", &threshold2);

            if (threshold2 < 0 || threshold2 > 500)
            {
                std::printf("Error: invalid threshold\n");
                goto CannyThreshold2Selection;
            }
        }

        std::printf("\n");

        if (imagePath == "All")
        {
            float averagePerformance = 0.0f;
            float averageFalsePositiveRate = 0.0f;
            float averageFalseNegativeRate = 0.0f;
            std::vector<float> Performances;
            std::vector<float> FalsePositiveRates;
            std::vector<float> FalseNegativeRates;
            int imageCount = 0;
            float maxPerformance = -1.0f;
            std::string maxPerformanceImagePath;
            float maxFalsePositiveRate = -1.0f;
            std::string maxFalsePositiveImagePath;
            float maxFalseNegativeRate = -1.0f;
            std::string maxFalseNegativeImagePath;

            for (auto imagePath : std::filesystem::directory_iterator("images"))
            {
                if (imagePath.is_regular_file())
                {
                    cv::Mat image = cv::imread(imagePath.path(), cv::IMREAD_GRAYSCALE);
                    std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.path().stem().string() + "_gt_binary.pgm");
                    cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);

                    if (!reference.empty())
                    {
                        cv::Mat edges = CannyEdgeDetector::GetEdges(image, threshold1, threshold2);

                        EdgeDetectionStats::Statistics statistics = EdgeDetectionStats::GetStatistics(edges, reference);

                        float performance = statistics.f1Score;
                        float falsePositiveRate = EdgeDetectionStats::GetFalsePositiveRate(statistics);
                        float falseNegativeRate = EdgeDetectionStats::GetFalseNegativeRate(statistics);

                        averagePerformance += performance;
                        averageFalsePositiveRate += falsePositiveRate;
                        averageFalseNegativeRate += falseNegativeRate;
                        Performances.push_back(performance);
                        FalsePositiveRates.push_back(falsePositiveRate);
                        FalseNegativeRates.push_back(falseNegativeRate);
                        imageCount++;

                        if (performance > maxPerformance)
                        {
                            maxPerformance = performance;
                            maxPerformanceImagePath = imagePath.path().string();
                        }
                        if (falsePositiveRate > maxFalsePositiveRate)
                        {
                            maxFalsePositiveRate = falsePositiveRate;
                            maxFalsePositiveImagePath = imagePath.path().string();
                        }
                        if (falseNegativeRate > maxFalseNegativeRate)
                        {
                            maxFalseNegativeRate = falseNegativeRate;
                            maxFalseNegativeImagePath = imagePath.path().string();
                        }
                    }
                }
            }

            averagePerformance /= imageCount;
            averageFalsePositiveRate /= imageCount;
            averageFalseNegativeRate /= imageCount;

            std::printf("Image count: %d\n", imageCount);
            std::printf("Average performance: %f\n", averagePerformance);
            std::printf("Average false positive rate: %f\n", averageFalsePositiveRate);
            std::printf("Average false negative rate: %f\n", averageFalseNegativeRate);
            std::printf("Median performance: %f\n", Performances[Performances.size() / 2]);
            std::printf("Median false positive rate: %f\n", FalsePositiveRates[FalsePositiveRates.size() / 2]);
            std::printf("Median false negative rate: %f\n", FalseNegativeRates[FalseNegativeRates.size() / 2]);
            std::printf("Max performance: %f\n", maxPerformance);
            std::printf("Max performance image path: %s\n", maxPerformanceImagePath.c_str());
            std::printf("Max false positive rate: %f\n", maxFalsePositiveRate);
            std::printf("Max false positive rate image path: %s\n", maxFalsePositiveImagePath.c_str());
            std::printf("Max false negative rate: %f\n", maxFalseNegativeRate);
            std::printf("Max false negative rate image path: %s\n", maxFalseNegativeImagePath.c_str());
            std::cout << std::flush;
        }
        else
        {
            cv::Mat image = cv::imread(std::filesystem::path("images") / imagePath, cv::IMREAD_GRAYSCALE);
            std::filesystem::path imageReferencePath = std::filesystem::path("images") / "gt" / (imagePath.stem().string() + "_gt_binary.pgm");
            cv::Mat reference = cv::imread(imageReferencePath, cv::IMREAD_GRAYSCALE);
            cv::Mat edges = CannyEdgeDetector::GetEdges(image, threshold1, threshold2);

            cv::namedWindow("Image", cv::WINDOW_NORMAL);
            cv::imshow("Image", image);

            cv::namedWindow("Reference", cv::WINDOW_NORMAL);
            cv::imshow("Reference", reference);

            cv::namedWindow("Edges", cv::WINDOW_NORMAL);
            cv::imshow("Edges", edges);
            cv::createTrackbar("Threshold 1", "Edges", nullptr, 500);
            cv::createTrackbar("Threshold 2", "Edges", nullptr, 500);

            while (true)
            {
                int newThreshold1 = cv::getTrackbarPos("Threshold 1", "Edges");
                int newThreshold2 = cv::getTrackbarPos("Threshold 2", "Edges");
                if (newThreshold1 != threshold1 || newThreshold2 != threshold2)
                {
                    threshold1 = newThreshold1;
                    threshold2 = newThreshold2;
                    edges = CannyEdgeDetector::GetEdges(image, threshold1, threshold2);

                    EdgeDetectionStats::Statistics statistics = EdgeDetectionStats::GetStatistics(edges, reference);

                    float performance = statistics.f1Score;
                    float falsePositiveRate = EdgeDetectionStats::GetFalsePositiveRate(statistics);
                    float falseNegativeRate = EdgeDetectionStats::GetFalseNegativeRate(statistics);

                    std::cout << "Performance: " << performance << std::endl;
                    std::cout << "False Positive Rate: " << falsePositiveRate << std::endl;
                    std::cout << "False Negative Rate: " << falseNegativeRate << std::endl;

                    cv::putText(edges, "Performance: " + std::to_string(performance), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
                    cv::imshow("Edges", edges);
                }

                int k = cv::waitKey(10) & 0xFF;
                if (k == 27) break;
            }
        }
    }

    return 0;
}
