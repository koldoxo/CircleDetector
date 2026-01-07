#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>

#include "CircleDetectorLib.h"

using namespace Catch::Matchers;

TEST_CASE("Instance", "CircleDetector")
{

    std::shared_ptr<ZTask::ContourArray> contours = std::make_shared<ZTask::ContourArray>();

    ZTask::CircleDetector::ParameterPtr parameter = std::make_shared<ZTask::CircleDetector::Parameter>();
    parameter->inputContours = contours;
    ZTask::CircleDetector::Operator op;

    op.calculate(parameter);

     REQUIRE(0 == op.calculate(parameter));
}

TEST_CASE("Parameters", "CircleDetector")
{

    std::shared_ptr<ZTask::ContourArray> contours = std::make_shared<ZTask::ContourArray>();

}

TEST_CASE("Polynomial", "CircleDetector")
{
    std::vector<std::float_t> x = { -5.f,-4.f,-3.f,-2.f,-1.f,0.f,1.f,2.f,3.f,4.f,5.f };
    std::vector<std::float_t> y(x.data(), x.data() + x.size());
    for (auto& aux : y) { aux *= aux; }

    auto coeffs = ZTask::polyfit<std::float_t>(x, y, 2);
	auto value  = ZTask::polyval<std::float_t>(coeffs, 3.f);

    auto coeffsd = ZTask::polyder1d<std::float_t>(coeffs);
    auto valued  = ZTask::polyval<std::float_t>(coeffsd, 4.f);

    REQUIRE_THAT(coeffs[0], WithinAbs(0.0, 0.001));
    REQUIRE_THAT(coeffs[1], WithinAbs(0.0, 0.001));
    REQUIRE_THAT(coeffs[2], WithinAbs(1.0, 0.001));
    REQUIRE_THAT(value,     WithinAbs(9.0, 0.001));

    REQUIRE_THAT(coeffsd[0], WithinAbs(0.0, 0.001));
    REQUIRE_THAT(coeffsd[1], WithinAbs(2.0, 0.001));
    REQUIRE_THAT(valued,     WithinAbs(8.0, 0.001));

}

TEST_CASE("LocalCurvature", "CircleDetector")
{
    
    ZTask::ContourType circle(360, cv::Point(0,0));

	float target = 10.f;
    for (int angle = 0; angle < 360; angle++)
    {
        float rad = static_cast<float>(angle) * static_cast<float>(CV_PI) / 180.0f;
        circle[angle].x = static_cast<int>(target * std::cos(rad));
        circle[angle].y = static_cast<int>(target * std::sin(rad));
	}

	std::int64_t window_size = 50;
	int index = 100;

    auto curvature = ZTask::CircleDetector::Operator::get_local_curvature(circle[index], index, circle, window_size);

    auto radii = 1 / curvature;

    REQUIRE_THAT(radii, WithinAbs(target, 0.1));


}