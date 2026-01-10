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
            std::shared_ptr<ContourArray> outputContours;
            std::float_t                  inputWindowRatio;
            std::int64_t                  inputWindowSize;
            std::int64_t                  inputMinContourLength;
			bool                          inputDebugMode;
            
        };// class Parameter

        using ParameterPtr = std::shared_ptr<Parameter>;

        class Operator
        {
        public:

            int calculate(ParameterPtr parameter);

			static std::vector<std::float_t> get_curvature_profile(const ContourType& contour, std::int64_t windowSize, bool debug);

			static std::float_t              get_local_curvature(std::uint64_t index, const ContourType& contour, std::uint64_t windowSize, bool debug);

        }; //class Operator
    };// class CircleDetector
}// namespace ZTask


