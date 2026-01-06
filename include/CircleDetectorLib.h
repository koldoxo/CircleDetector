#pragma once

#include <memory>
#include <vector>

// [External] OpenCV 4
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

namespace ZTask
{

    class CircleDetector
    {
    public:

        class Parameter
        {
        public:
            std::shared_ptr<std::vector<std::vector<cv::Point>>> inputContours;

        };// class Parameter

        using ParameterPtr = std::shared_ptr<Parameter>;

        class Operator
        {
        public:

            int calculate(ParameterPtr);

        }; //class Operator
    };// class CircleDetector
}// namespace ZTask


