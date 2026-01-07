#include "CircleDetectorLib.h"

int ZTask::CircleDetector::Operator::calculate(ParameterPtr parameter)
{

	std::shared_ptr<ContourArray> contours = parameter->inputContours;

	return EXIT_SUCCESS;
}

int ZTask::CircleDetector::Operator::get_curvature_profile(const ContourType& contour, std::vector<float>& profile, std::int64_t minContourLength, std::int64_t windowSize, std::int64_t windowRatio)
{


	return EXIT_SUCCESS;
}


std::float_t ZTask::CircleDetector::Operator::get_local_curvature(cv::Point& point, std::uint64_t index, const ContourType& contour, std::uint64_t windowSize)
{
	// set start and end neighbothood points
	int start = std::max(0, static_cast<int>(index) - static_cast<int>(windowSize / 2));
	int end   = std::min(static_cast<int>(contour.size()), static_cast<int>(index) + static_cast<int>(windowSize / 2) + 1);

	// fill neigborhood
	auto beginIt = contour.begin() + start;
	auto endIt = contour.begin() + end;
	ContourType neighborhood(beginIt, endIt);

	// split neighborhood into x and y and translate it to central point and comute angle
	std::vector<std::float_t> x;
	x.resize(neighborhood.size());
	std::vector<std::float_t> y;
	y.resize(neighborhood.size());
	std::float_t angle = 0.f;
	for (int n=0; n < neighborhood.size(); n++) 
	{ 
		// splitting and translating
		x[n] = static_cast<std::float_t>( neighborhood[n].x ) - static_cast<std::float_t>(point.x);
		y[n] = static_cast<std::float_t>( neighborhood[n].y ) - static_cast<std::float_t>(point.y);


		// gradient
		std::float_t dx = 0.0f;
		std::float_t dy = 0.0f;
		if (n == 0)
		{
			dx = x[n + 1] - x[n];
			dy = y[n + 1] - y[n];
		}
		else if (n == neighborhood.size() - 1)
		{
			dx = x[n] - x[n - 1];
			dy = y[n] - y[n - 1];
		}
		else
		{
			dx = (x[n + 1] - x[n - 1]) / 2.0f;
			dy = (y[n + 1] - y[n - 1]) / 2.0f;
		}

		angle += std::atan2(dy, dx);
		if (n == neighborhood.size()) { angle /= static_cast<std::float_t>(neighborhood.size()); }
	}


	// rotate points to align with x-axis
	for(int n=0; n < neighborhood.size(); n++)
	{
		x[n] = x[n] * std::cos(-angle) - y[n] * std::sin(-angle);
		y[n] = x[n] * std::sin(-angle) + y[n] * std::cos(-angle);
	}

	// fit polynomial (2nd order) to neighborhood
	auto coeffs2d = polyder1d(polyder1d(ZTask::polyfit<std::float_t>(x, y, 2)));
	
	std::float_t curvature = 0.f;
	for (int n = 0; n < x.size(); n++){ curvature += polyval(coeffs2d, x[n]); }
	curvature /= static_cast<std::float_t>(x.size());

	return curvature;
}
