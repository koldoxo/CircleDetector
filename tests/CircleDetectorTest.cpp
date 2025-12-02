#include <catch2/catch_test_macros.hpp>

#include "CircleDetector.h"

TEST_CASE("test_api")
{

    ZTask::CircleDetector::ParameterPtr parameter{};
    ZTask::CircleDetector::Operator op{};

    op.calculate(parameter);

    REQUIRE(0 == op.calculate(parameter));
}

