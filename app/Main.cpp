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
  
    cv::Mat image = cv::imread(file["inputfile"]);

    cv::imshow("source image", image);
    cv::waitKey();

    return EXIT_SUCCESS;
}