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

cv::RNG rng(12345);

void curvature(cv::Mat& src, cv::Mat& dst, double sigma, std::filesystem::path outputpath, std::string stem);

void save_image(cv::Mat& im, std::filesystem::path outputpath, std::string stem, std::string sufix);

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
  
    // read data
    cv::Mat source = cv::imread(jsonfile["inputfile"], cv::IMREAD_GRAYSCALE);

    cv::Mat binary; 
    {
        cv::threshold(source, binary, 50.f, 255.f, cv::THRESH_BINARY);
        std::filesystem::path outputbinary = outputpath / (inputfile.stem().string() + "_bin.png");
        cv::imwrite(outputbinary.string(), binary);
    }
    
    // cv::Mat destination = cv::Mat::zeros(source.size(), CV_32F); // CV_16U 
    
    // compute curvatue
    // curvature(binary, destination, 255, outputpath, inputfile.stem().string());

    // save image
    save_image(binary, outputpath, inputfile.stem().string(), "_bin");

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    
    std::float_t sigma = 1.f;
    cv::Mat blurr;
    int size = std::floor(2 * sigma * 2 + 1); // 95% gauge
    {
        cv::GaussianBlur(binary, blurr, cv::Size(size, size), sigma, sigma, cv::BORDER_CONSTANT);
        save_image(blurr, outputpath, inputfile.stem().string(), "_blurr");
    }
    
    cv::findContours(blurr, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );

    cv::Mat drawing = cv::Mat::zeros(binary.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
    }

    save_image(drawing, outputpath, inputfile.stem().string(), "_contours");

    // show result
    //cv::imshow("curvature", destination);
    //cv::waitKey();

    return EXIT_SUCCESS;
}

void save_image(cv::Mat& im, std::filesystem::path outputpath, std::string stem, std::string sufix)
{
    std::filesystem::path outputfile = outputpath / (stem + sufix + ".png");
    cv::imwrite(outputfile.string(), im);
}

void curvature(cv::Mat& src, cv::Mat& dst, double sigma, std::filesystem::path outputpath, std::string stem)
{
    cv::Mat temp;

    int size = std::floor( 2*sigma*2 + 1 ); // 95% gauge
    {
        cv::GaussianBlur(src, temp, cv::Size(size, size), sigma, sigma, cv::BORDER_CONSTANT);
        save_image(temp, outputpath, stem, "_blurr");
    }
}