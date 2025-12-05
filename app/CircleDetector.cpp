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

    cv::Mat binary;
    cv::threshold(source, binary, 50, 255, cv::THRESH_BINARY);
  
    std::filesystem::path outputfile = outputpath / ( inputfile.stem().string() + "_bin.png");
    
    std::cout << "Writing to... " << outputfile << std::endl;

    cv::imwrite( outputfile.string(), binary);

    cv::imshow("binary", binary);

    cv::waitKey();   

    return EXIT_SUCCESS;
}