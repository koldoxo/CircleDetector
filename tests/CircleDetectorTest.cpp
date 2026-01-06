#include <catch2/catch_test_macros.hpp>

#include "CircleDetectorLib.h"

TEST_CASE("CircleDetectorInstance")
{

    std::shared_ptr<std::vector<std::vector<cv::Point> >> contours = std::make_shared<std::vector<std::vector<cv::Point> >>();

    ZTask::CircleDetector::ParameterPtr parameter = std::make_shared<ZTask::CircleDetector::Parameter>();
    parameter->inputContours = contours;
    ZTask::CircleDetector::Operator op;

    op.calculate(parameter);

    REQUIRE(0 == op.calculate(parameter));
}

