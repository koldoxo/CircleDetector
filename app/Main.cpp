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

    cv::imshow("resized image", resizedIm);

    cv::waitKey();

    return EXIT_SUCCESS;
}