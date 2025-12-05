#include <catch2/catch_test_macros.hpp>

#include "CircleDetectorLib.h"

TEST_CASE("CircleDetectorInstance")
{

    ZTask::CircleDetector::ParameterPtr parameter{};
    ZTask::CircleDetector::Operator op{};

    op.calculate(parameter);

    REQUIRE(0 == op.calculate(parameter));
}


TEST_CASE("CircleDetectorInstance")
{

    ZTask::CircleDetector::ParameterPtr parameter{};
    ZTask::CircleDetector::Operator op{};




    op.calculate(parameter);

    REQUIRE(0 == op.calculate(parameter));
}
