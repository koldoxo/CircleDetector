// STD
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>


// [External] nlohmann_json
#include <nlohmann/json.hpp>

// [External] OpenCV 4
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// [Local] 
#include "CircleDetectorLib.h"


void curvature(cv::Mat& src, cv::Mat& dst, double sigma);

int main(void)
{
    std::cout << "Circle Detection..." << std::endl;

    // check congi file
    std::filesystem::path inputconfig("C:\\Git\\koldoxo\\CircleDetector\\app\\config.json");

    if (!std::filesystem::exists(inputconfig))
    {
        throw std::invalid_argument("config.json file does not exist");
    }

    // parse json
    std::ifstream ifs(inputconfig);
    nlohmann::json jsonfile = nlohmann::json::parse(ifs);

    // check input image
    std::string inputfilestr = jsonfile["inputfile"];
    std::filesystem::path inputfile(inputfilestr);
   
    if (!std::filesystem::exists(inputfile))
    {
        throw std::invalid_argument("Input file entry does not exist");
    }

    std::cout << "Reading in... " << inputfile << std::endl;

    //check output folder

    std::string outputpathstr = jsonfile["outputpath"];
    std::filesystem::path outputpath(outputpathstr);

    if (!std::filesystem::is_directory(outputpath))
    {
        throw std::invalid_argument("Ouput path entry does not exist");
    }

    std::cout << "Writing to... " << outputpath << std::endl;
  
    cv::Mat source = cv::imread(jsonfile["inputfile"], cv::IMREAD_GRAYSCALE);
    cv::Mat destination = cv::Mat::zeros(source.size(), CV_16U);
    
    curvature(source, destination, 10);

    std::filesystem::path outputfile = outputpath / (inputfile.stem().string() + "_curv.png");
    cv::imwrite(outputfile.string(), destination);

    //cv::GaussianBlur(source, destination, cv::Size(5, 5), 1, 1, cv::BORDER_CONSTANT);

    cv::imshow("curvature", destination);

    cv::waitKey();

    return EXIT_SUCCESS;
}

void curvature(cv::Mat& src, cv::Mat& dst, double sigma)
{
    cv::Mat temp; 
    int size = std::floor( 2*sigma*2 + 1 );
    cv::GaussianBlur(src , temp, cv::Size(size,size), sigma, sigma, cv::BORDER_CONSTANT);

    cv::Mat Fx;
    cv::Sobel(temp, Fx, CV_32F, 1, 0, 3);

    cv::Mat Fy;
    cv::Sobel(temp, Fy, CV_32F, 0, 1, 3);

    cv::Mat F2x = Fx * Fx;
    cv::Mat F2y = Fy * Fy;

    cv::Mat Fxx;
    cv::Sobel(temp, Fxx, CV_32F, 2, 0, 3);

    cv::Mat Fyy;
    cv::Sobel(temp, Fyy, CV_32F, 0, 2, 3);

    cv::Mat Fxy;
    cv::Sobel(Fx, Fxy, CV_32F, 0, 1, 3);

    // curvature = (F2y * Fxx + F2x * Fyy - 2 * Fx * Fy * Fxy) / ( F2x + F2y)^(3 / 2);
    
    cv::Mat term1;
    cv::multiply(F2y, Fxx, term1);
    
    cv::Mat term2;
    cv::multiply(F2x, Fyy, term2);

    cv::Mat term3;
    cv::multiply(Fx, Fy, term3, -2.);

    cv::Mat numerator = cv::Mat::zeros(src.size(), CV_32F);
    cv::add(term1, term2, numerator);
    cv::add(numerator, term3, numerator);

    numerator = cv::abs(numerator);

    cv::Mat denominator = cv::Mat::zeros(src.size(), CV_16U);
    cv::add(F2x, F2y, denominator);
    cv::pow(denominator, 1.5, denominator);

    cv::divide(numerator, denominator, dst);
}