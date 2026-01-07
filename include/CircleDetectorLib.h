#pragma once

// STD
#include <iostream>
#include <memory>
#include <vector>

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
            std::shared_ptr<ContourArray> outputContours;
            std::int64_t                  inputWindowRatio;
            std::int64_t                  inputWindowSize;
            std::int64_t                  inputMinContourLength;
            
        };// class Parameter

        using ParameterPtr = std::shared_ptr<Parameter>;

        class Operator
        {
        public:

            int calculate(ParameterPtr parameter);

			int get_curvature_profile(const ContourType& contour, std::vector<float>& profile, std::int64_t minContourLength, std::int64_t windowSize, std::int64_t windowRatio);

			static std::float_t get_local_curvature(cv::Point& point, std::uint64_t index, const ContourType& contour, std::uint64_t windowSize);

        }; //class Operator
    };// class CircleDetector
}// namespace ZTask


