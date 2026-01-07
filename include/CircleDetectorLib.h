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
            std::uint64_t                 inputWindowRatio;
            //std::uint64_t               inputWindowSize;
            std::uint64_t                 inputMinContourLength;
            
        };// class Parameter

        using ParameterPtr = std::shared_ptr<Parameter>;

        class Operator
        {
        public:

            int calculate(ParameterPtr);

        }; //class Operator
    };// class CircleDetector
}// namespace ZTask


