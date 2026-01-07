#include <catch2/catch_test_macros.hpp>

#include "CircleDetectorLib.h"

TEST_CASE("Instance", "CircleDetector")
{

    std::shared_ptr<ZTask::ContourArray> contours = std::make_shared<ZTask::ContourArray>();

    ZTask::CircleDetector::ParameterPtr parameter = std::make_shared<ZTask::CircleDetector::Parameter>();
    parameter->inputContours = contours;
    ZTask::CircleDetector::Operator op;

    op.calculate(parameter);

    REQUIRE(0 == op.calculate(parameter));
}

TEST_CASE("Curvature", "CircleDetector")
{

    std::shared_ptr<ZTask::ContourArray> contours = std::make_shared<ZTask::ContourArray>();

}

