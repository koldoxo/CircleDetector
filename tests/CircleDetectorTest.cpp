#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>

#include "CircleDetectorLib.h"

using namespace Catch::Matchers;

cv::RNG rng(12345);

TEST_CASE("InstantiateObjects", "CircleDetector")
{

    std::shared_ptr<ZTask::ContourArray> contours = std::make_shared<ZTask::ContourArray>();

    ZTask::CircleDetector::ParameterPtr parameter = std::make_shared<ZTask::CircleDetector::Parameter>();
    parameter->inputContours = contours;
    ZTask::CircleDetector::Operator op;

    op.calculate(parameter);

    REQUIRE(0 == op.calculate(parameter));
}

TEST_CASE("PassParameter", "CircleDetector")
{

    std::shared_ptr<ZTask::ContourArray> contours = std::make_shared<ZTask::ContourArray>();

}

TEST_CASE("FitPolynomial", "CircleDetector")
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
    
    cv::Point center(250, 250);
    std::uint64_t radius = 200;
    std::uint64_t tol    = 1;
    cv::Mat circle = cv::Mat::zeros(cv::Size(500, 500), CV_8UC1);

    {
        for (int i = 0; i < circle.cols; i++)
        {
            for (int j = 0; j < circle.rows; j++)
            {
				auto d = std::sqrt((i - center.x) * (i - center.x) + (j - center.y) * (j - center.y));

                if (d > (radius - tol) && d < (radius + tol) )
                {
				    circle.at<uchar>(j, i) = 255;
			    }
            }
        }
        //cv::imwrite("C:/Git/koldoxo/CircleDetector/tests/disk.png", circle);
    }

    cv::Mat blurr;
    {
        std::float_t s = 1.f;
        int size = std::floor(2 * s * 2 + 1); // 95% gauge
        cv::GaussianBlur(circle, blurr, cv::Size(size, size), s, s, cv::BORDER_CONSTANT);
		cv::imwrite("C:/Git/koldoxo/CircleDetector/tests/blurred.png", blurr);
    }
    

	std::shared_ptr<ZTask::ContourArray> contours = std::make_shared<ZTask::ContourArray>();
    std::vector<cv::Vec4i> hierarchy;
    {
        
        cv::findContours(blurr, *contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        cv::Mat drawing = cv::Mat::zeros(circle.size(), CV_8UC3);
        for (std::int64_t i = 0; i < contours->size(); i++)
        {
            cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            cv::drawContours(drawing, *contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
        }
	    cv::imwrite("C:/Git/koldoxo/CircleDetector/tests/contours.png", drawing);
    }

    int contour_index = 1;
    int point_index = int((*contours)[contour_index].size() / 2);
	auto& circle_contour = (*contours)[contour_index];
    auto& point = (*contours)[contour_index][point_index];
	std::int64_t window_size = int(circle_contour.size() * 0.025) % 2 == 0 ? int(circle_contour.size() * 0.025) + 1 : int(circle_contour.size() * 0.025);
    // we need 2.5% of the contour size and odd number
    // with 3.7% we get much better results but it increases the window 

    auto curvature = ZTask::CircleDetector::Operator::get_local_curvature(point_index, circle_contour, window_size, false);

    auto R = 1 / curvature;

    REQUIRE_THAT(R, WithinAbs(radius, 5));

}

TEST_CASE("GetProfile", "CircleDetector")
{

    cv::Point center(250, 250);
    std::uint64_t radius = 200;
    std::uint64_t tol = 1;
    cv::Mat circle = cv::Mat::zeros(cv::Size(500, 500), CV_8UC1);

    {
        for (int i = 0; i < circle.cols; i++)
        {
            for (int j = 0; j < circle.rows; j++)
            {
                auto d = std::sqrt((i - center.x) * (i - center.x) + (j - center.y) * (j - center.y));

                if (d > (radius - tol) && d < (radius + tol))
                {
                    circle.at<uchar>(j, i) = 255;
                }
            }
        }
        //cv::imwrite("C:/Git/koldoxo/CircleDetector/tests/disk.png", circle);
    }

    cv::Mat blurr;
    {
        std::float_t s = 1.f;
        int size = std::floor(2 * s * 2 + 1); // 95% gauge
        cv::GaussianBlur(circle, blurr, cv::Size(size, size), s, s, cv::BORDER_CONSTANT);
        //cv::imwrite("C:/Git/koldoxo/CircleDetector/tests/blurred.png", blurr);
    }


    std::shared_ptr<ZTask::ContourArray> contours = std::make_shared<ZTask::ContourArray>();
    std::vector<cv::Vec4i> hierarchy;
    {

        cv::findContours(blurr, *contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        cv::Mat drawing = cv::Mat::zeros(circle.size(), CV_8UC3);
        for (std::int64_t i = 0; i < contours->size(); i++)
        {
            cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            cv::drawContours(drawing, *contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
        }
       // cv::imwrite("C:/Git/koldoxo/CircleDetector/tests/contours.png", drawing);
    }

    int contour_index = 1;
    int point_index = int((*contours)[contour_index].size() / 2);
    auto& circle_contour = (*contours)[contour_index];
    auto& point = (*contours)[contour_index][point_index];

	std::int64_t window_size = int(circle_contour.size() * 0.037) % 2 == 0 ? int(circle_contour.size() * 0.037) + 1 : int(circle_contour.size() * 0.037);// we need 2.5% of the contour size and odd number for good results

    auto profile = ZTask::CircleDetector::Operator::get_curvature_profile(circle_contour, window_size, false);
    {
        //std::ofstream outputFile("C:\\Git\\koldoxo\\CircleDetector\\tests\\poc\\profile11.txt");
        //for (int n = 0; n < profile.size(); n++) { outputFile << profile[n] << std::endl; }
    }
    
    std::float_t curvature = std::accumulate(profile.begin(), profile.end(), 0.0);
    curvature /= profile.size();
    
    auto R = 1 / curvature;
    REQUIRE_THAT(R, WithinAbs(radius,5.0));

}