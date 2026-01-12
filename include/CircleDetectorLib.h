#pragma once

// STD
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

// Eigen
#include <Eigen/Dense>

// [External] OpenCV 4
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// [Local] 
#include "DataTypes.h"
#include "./external/Polyfit.hpp"

namespace ZTask
{

    class CircleDetector
    {
    public:

        class Parameter
        {
        public:
            std::shared_ptr<ContourArray> inputContours;
            std::float_t                  inputWindowRatio;
            std::int64_t                  inputWindowSize;
            std::int64_t                  inputMinContourLength;
			std::float_t				  inputMinRadius;
			std::float_t                  inputMaxRadius;
			bool                          inputDebugMode;

            std::shared_ptr<std::vector<std::pair<std::float_t,cv::Point>>> outputCircles = std::make_shared<std::vector<std::pair<std::float_t, cv::Point>>>();
            
        };// class Parameter

        using ParameterPtr = std::shared_ptr<Parameter>;

        class Operator
        {
        public:

            int calculate(ParameterPtr parameter);

			static std::pair<std::float_t, cv::Point>   get_circle(const ContourType& contour, std::int64_t windowSize, bool debug);

			static std::pair<std::float_t, cv::Point2f> get_local_curvature(std::uint64_t index, const ContourType& contour, std::uint64_t windowSize, bool debug);

        }; //class Operator
    };// class CircleDetector
}// namespace ZTask


