// STD
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <memory>
#include <vector>
#include <chrono>

// [External] nlohmann_json
#include <nlohmann/json.hpp>

// [External] OpenCV 4
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// [Local] 
#include "CircleDetectorLib.h"

cv::RNG rng(12345);

void save_image(cv::Mat& im, std::filesystem::path outputpath, std::string stem, std::string sufix);

void unpack_parameters(nlohmann::json inputjson, std::map<std::string, std::float_t>& params);

void check_parameters(const std::map<std::string, std::float_t>& params);

void pre_processing(cv::Mat& in, cv::Mat& out, std::map<std::string, std::float_t>& params, bool debug = false);

int main(int argc, char* argv[])
{
    std::cout << "Circle Detection..." << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
    // check config file
    // std::filesystem::path inputconfig("C:\\Git\\koldoxo\\CircleDetector\\app\\config.json");

    if (argc < 1)
    {
        throw std::invalid_argument("Provide a path to config.json");
    }

    std::string inputstr = argv[1];
    std::filesystem::path inputconfig = inputstr;

    if (!std::filesystem::exists(inputconfig))
    {
        throw std::invalid_argument("config.json file does not exist");
    }

    // parse json
    std::ifstream ifs(inputconfig);
    nlohmann::json jsonfile = nlohmann::json::parse(ifs);

    // check input image
    std::string inputfilestr = jsonfile["input_file"];
    std::filesystem::path inputfile(inputfilestr);
   
    if (!std::filesystem::exists(inputfile))
    {
        throw std::invalid_argument("Input file entry does not exist");
    }

    std::cout << "Reading from " << inputfile << std::endl;

    //check output folder
    std::string outputpathstr = jsonfile["output_path"];
    std::filesystem::path outputpath(outputpathstr);

    if (!std::filesystem::is_directory(outputpath))
    {
        throw std::invalid_argument("Ouput path entry does not exist");
    }

    std::cout << "Writing to " << outputpath << std::endl;
  
    // unpack parameters
    std::map<std::string, std::float_t> parameters;
    {
        std::cout << "Unpacking Parameters" << std::endl;
        unpack_parameters(jsonfile, parameters);
    }
    
    // check parameters
    check_parameters(parameters);

    auto debug = bool(parameters["debug_mode"]);

    // read data
    cv::Mat source = cv::imread(jsonfile["input_file"], cv::IMREAD_GRAYSCALE);

    // call pre_processing
    cv::Mat preproc;
    {
        std::cout << "Calling Preprocessing" << std::endl;
        pre_processing(source, preproc, parameters, debug);
    }
    
    // get contours
    std::shared_ptr<std::vector<std::vector<cv::Point> >> contours = std::make_shared<std::vector<std::vector<cv::Point> >>();
    std::vector<cv::Vec4i> hierarchy;
    {
        std::cout << "Finding Contours" << std::endl;
        cv::findContours(preproc, *contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    }
    
    cv::Mat drawing = cv::Mat::zeros(source.size(), CV_8UC3);
    
    {
        std::cout << "Printing found Contours" << std::endl;
        #pragma omp parallel for
        for (std::int64_t i = 0; i < contours->size(); i++)
        {
            cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            cv::drawContours(drawing, *contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
        }
        save_image(drawing, outputpath, inputfile.stem().string(), "_contours");
    }

    // calling circle-detector
	std::shared_ptr<std::vector<std::pair<std::float_t, cv::Point>>> detectedCircles;
    {
        std::cout << "Calling CircleDetector" << std::endl;
        ZTask::CircleDetector::ParameterPtr param = std::make_shared<ZTask::CircleDetector::Parameter>();
        param->inputContours         = contours;
		param->inputWindowSize       = parameters["window_size"];
        param->inputWindowRatio      = parameters["window_ratio"];
		param->inputMinContourLength = parameters["min_length"];
		param->inputMinRadius        = parameters["min_radius"];
		param->inputMaxRadius        = parameters["max_radius"];
		param->inputDebugMode        = debug;
        ZTask::CircleDetector::Operator op;
        op.calculate(param);
		detectedCircles = param->outputCircles;
    }
    
    cv::Mat output = cv::Mat::zeros(source.size(), CV_8UC3);
    for (std::int64_t i = 0; i < detectedCircles->size(); i++)
    {
        auto circle = detectedCircles->at(i);
        if (circle.first > 0.f)
        {
            cv::circle(output, circle.second, static_cast<int>(circle.first), cv::Scalar(0, 255, 0), 2); //circle.second
        }
	}

	save_image(output, outputpath, inputfile.stem().string(), "_circles");

    // show result
    // cv::imshow("curvature", destination);
    // cv::waitKey();
    auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Processing Time (ms): " << duration.count() << std::endl;

    return EXIT_SUCCESS;
}

void pre_processing(cv::Mat& in, cv::Mat& out, std::map<std::string, std::float_t>& params, bool debug)
{
    cv::Mat bin;
    std::float_t t = params.find("threshold")->second;
    {
        cv::threshold(in, bin, t, 255.f, cv::THRESH_BINARY);
    }
    
    if (debug) { save_image(bin, "./", "debug", "bin"); }


    cv::Mat close;
    {
        auto kernel_size = static_cast<int>(params.find("close_size")->second);
		if (kernel_size % 2 == 0) { kernel_size += 1; } // make odd
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernel_size, kernel_size));
        cv::morphologyEx(bin, close, cv::MORPH_CLOSE, kernel);
    }
    
    if (debug) { save_image(close, "./", "debug", "close"); }

    std::float_t s = params.find("sigma")->second;
    int size = std::floor(2 * s * 2 + 1); // 95% gauge
    {
        cv::GaussianBlur(close, out, cv::Size(size, size), s, s, cv::BORDER_CONSTANT);
    }

    if (debug) { save_image(out, "./", "debug", "blur"); }
}


void unpack_parameters(nlohmann::json jsonfile, std::map<std::string, std::float_t>& params)
{
    std::string thresholdstr = jsonfile["threshold"];
    std::float_t threshold = std::atof(thresholdstr.c_str());
    params["threshold"] = threshold;

    std::string sigmastr = jsonfile["sigma"];
    std::float_t sigma = std::atof(sigmastr.c_str());
    params["sigma"] = sigma;

    std::string close_sizestr = jsonfile["close_size"];
    std::float_t close_size = std::atof(close_sizestr.c_str());
    params["close_size"] = close_size;

    std::string min_lengthstr = jsonfile["min_length"];
    std::float_t min_length = std::atof(min_lengthstr.c_str());
    params["min_length"] = min_length;

    std::string window_ratiostr = jsonfile["window_ratio"];
    std::float_t window_ratio = std::atof(window_ratiostr.c_str());
    params["window_ratio"] = window_ratio;

    std::string window_sizestr = jsonfile["window_size"];
    std::float_t window_size = std::atof(window_sizestr.c_str());
    params["window_size"] = window_size;

    std::string min_radiusstr = jsonfile["min_radius"];
    std::float_t min_radius = std::atof(min_radiusstr.c_str());
    params["min_radius"] = min_radius;

    std::string max_radiusstr = jsonfile["max_radius"];
    std::float_t max_radius = std::atof(max_radiusstr.c_str());
    params["max_radius"] = max_radius;

    std::string debug_modestr = jsonfile["debug_mode"];
    std::int64_t debug_mode = std::atoi(debug_modestr.c_str());
    params["debug_mode"] = debug_mode;

}

void check_parameters(const std::map<std::string, std::float_t>& params)
{
    for (const auto& [key, value] : params)
    {
        if (value < 0.f && key != "window_size")
        {
            throw std::invalid_argument("At least one parameter is lesser than zero");
        }
    }
}

void save_image(cv::Mat& im, std::filesystem::path outputpath, std::string stem, std::string sufix)
{
    std::filesystem::path outputfile = outputpath / (stem + sufix + ".png");
    cv::imwrite(outputfile.string(), im);
}
 