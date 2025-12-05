#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

#include "CircleDetector.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

int main(void)
{
    std::cout << "Circle Detection..." << std::endl;

    std::ifstream ifs("C:\\Git\\koldoxo\\ZTask\\app\\config.json");
    nlohmann::json file = nlohmann::json::parse(ifs);

    std::cout << file["inputfile"]  << std::endl;
    std::cout << file["outputfile"] << std::endl;
  
    cv::Mat sourceIm = cv::imread(file["inputfile"], cv::IMREAD_GRAYSCALE);

    //cv::imshow("source image", sourceIm);

    cv::Mat resizedIm;

    std::float_t ratio = static_cast<std::float_t>(sourceIm.rows) / static_cast<std::float_t>(sourceIm.cols);

    std::float_t newCols = sourceIm.cols * 0.3333;
    std::float_t newRows = ratio * newCols;

    cv::resize(sourceIm, resizedIm, cv::Size(newCols, newRows) );

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(resizedIm, circles, cv::HOUGH_GRADIENT, 1.5, 50, 300, 100, 0, 0);

    for (size_t i = 0; i < circles.size(); i++)
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        cv::circle(resizedIm, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        cv::circle(resizedIm, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
    }

    int count = 0;
    for (auto it = sourceIm.datastart ; it != sourceIm.dataend; it++)
    {
        ++count;
    }

    std::cout << "Image total size: " << sourceIm.cols * sourceIm.rows << "  " << std::endl;
    std::cout << "Pointer count: "    << count << "  " << std::endl;

    //cv::imshow("resized image", resizedIm);
    //cv::waitKey();

    return EXIT_SUCCESS;
}