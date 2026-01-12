#include "CircleDetectorLib.h"

int ZTask::CircleDetector::Operator::calculate(ParameterPtr parameter)
{

	// unpack parameters
	std::shared_ptr<ContourArray> contours		   = parameter->inputContours;
	std::int64_t				  minContourLength = parameter->inputMinContourLength;
	std::float_t				  windowRatio	   = parameter->inputWindowRatio;
	std::int64_t				  windowSize	   = parameter->inputWindowSize;

	bool debug = parameter->inputDebugMode;

	if (contours->size() == 0)
	{
		throw std::invalid_argument("No contours provided");
	}

	#pragma omp parallel for
	for (int n = 0; n < contours->size() ; n++ )
	{
		if ((*contours)[n].size() < minContourLength)
		{
			continue;
		}

		if (windowSize == -1)
		{
			windowSize = static_cast<std::int64_t>(std::round(static_cast<std::float_t>((*contours)[n].size()) * windowRatio /100.f));
			if (windowSize % 2 == 0) { windowSize += 1; }
		}
		
		auto circle = get_circle((*contours)[n], windowSize, debug);
		
	}


	return EXIT_SUCCESS;
}

std::pair<std::float_t, cv::Point> ZTask::CircleDetector::Operator::get_circle(const ContourType& contour, std::int64_t windowSize, bool debug = false)
{
	std::vector<std::float_t> curvatures;
	std::vector<cv::Point2f>  centers;

	curvatures.resize(contour.size(), 0.f);
	centers.resize(contour.size(), cv::Point2f());

	auto curvatureIt = curvatures.data();
	auto centersIt	 = centers.data();
	for (auto& it = contour.begin(); it != contour.end(); it++ ) 
	{
		auto curvatureAndCenter = get_local_curvature(static_cast<std::uint64_t>(std::distance(contour.begin(), it)), contour, windowSize, false);
		
		*curvatureIt = curvatureAndCenter.first;
		curvatureIt++;
		
		*centersIt = curvatureAndCenter.second;
		centersIt++;
	}

	if (debug)
	{
		std::ofstream outputFile("./curvatures.txt");
		for (int n = 0; n < curvatures.size(); n++) { outputFile << curvatures[n] << std::endl; }
	}

	// mean curvature and std-dev
	std::float_t meanCurvature = std::accumulate(curvatures.begin(), curvatures.end(), 0.0);
	meanCurvature /= curvatures.size();

	std::float_t stdDevCurvature = 0.0;
	for (auto curv : curvatures) { stdDevCurvature += (curv - meanCurvature) * (curv - meanCurvature);}
	stdDevCurvature = std::sqrt(stdDevCurvature / float(curvatures.size()));

	// compute histogram of ccurvatures
	int count = 0;
	for (auto curv : curvatures) { if (curv > (meanCurvature - stdDevCurvature) && curv < (meanCurvature + stdDevCurvature)) { count++; } }
	std::float_t percentOfCurvatures = static_cast<std::float_t>(count) / static_cast<std::float_t>(curvatures.size()) * 100.f;

	// if the curvature is too low and not enough points agree on it, discard
	if (std::abs(meanCurvature) < 0.000000001 && percentOfCurvatures < 66.) { return  std::pair<std::float_t, cv::Point>(); }

	cv::Point2f meanCenter;
	meanCenter.x = 0.f;
	meanCenter.y = 0.f;
	for (auto center: centers) 
	{
		meanCenter.x += center.x;
		meanCenter.y += center.y;
	}
		
	meanCenter.x /= static_cast<std::float_t>(centers.size());
	meanCenter.y /= static_cast<std::float_t>(centers.size());

	std::pair<std::float_t, cv::Point> circle;
	circle.first  = 1. / meanCurvature;
	circle.second = cv::Point(static_cast<int>(meanCenter.x), static_cast<int>(meanCenter.y));

	return circle;
}


std::pair<std::float_t, cv::Point2f> ZTask::CircleDetector::Operator::get_local_curvature(std::uint64_t index, const ContourType& contour, std::uint64_t windowSize, bool debug = false)
{
	if (debug)
	{
		std::ofstream outputFile("./contour.txt");
		for (int n = 0; n < contour.size(); n++) { outputFile << contour[n].x << "," << contour[n].x << std::endl; }
	}

	// set start and end neighbothood points
	int start = std::max(0, static_cast<int>(index) - static_cast<int>(windowSize / 2));
	int end	  = std::min(static_cast<int>(contour.size()), static_cast<int>(index) + static_cast<int>(windowSize / 2) + 1);

	// fill neigborhood
	auto beginIt = contour.begin() + start;
	auto endIt	 = contour.begin() + end;
	ContourType window(beginIt, endIt);

	//// split neighborhood into x and y and translate it to central point and comute angle
	std::vector<std::float_t> x;
	x.resize(window.size());
	std::vector<std::float_t> y;
	y.resize(window.size());

	for (int n=0; n < window.size(); n++) 
	{ 
		// casting to float
		x[n] = static_cast<std::float_t>( window[n].x );
		y[n] = static_cast<std::float_t>( window[n].y );
	}

	if (debug)
	{
		std::ofstream outputFile("./window.txt");
		for (int n = 0; n < x.size(); n++) { outputFile << x[n] << "," << y[n] << std::endl; }
	} 

	// translate to central point
	int central_index =  static_cast<int>(window.size() / 2) % 2 == 0 ? static_cast<int>(window.size() / 2) + 1: static_cast<int>(window.size() / 2);
	for (int n = 0; n < window.size(); n++)
	{  
	
		x[n] -= static_cast<std::float_t>(window[central_index].x);
		y[n] -= static_cast<std::float_t>(window[central_index].y);
	}


	if (debug)
	{
		std::ofstream outputFile("./translate.txt");
		for (int n = 0; n < x.size(); n++){outputFile << x[n] << "," << y[n] << std::endl;}
	}

	// soften 3-point moving average
	for (int n = 0; n < window.size(); n++)
	{
		if (n == 0)
		{
			x[n] = (x[n] + x[n + 1]) / 2.0f;
			y[n] = (y[n] + y[n + 1]) / 2.0f;
		}
		else if (n == window.size() - 1)
		{
			x[n] = (x[n - 1] + x[n]) / 2.0f;
			y[n] = (y[n - 1] + y[n]) / 2.0f;
		}
		else
		{
			x[n] = (x[n - 1] + x[n] + x[n + 1]) / 3.0f;
			y[n] = (y[n - 1] + y[n] + y[n + 1]) / 3.0f;
		}
	}

	if (debug)
	{
		std::ofstream outputFile("./soften.txt");
		for (int n = 0; n < x.size(); n++) { outputFile << x[n] << "," << y[n] << std::endl; }
	}

	// compute angle of middle point and tangent vector
	std::float_t angle;
	std::float_t nx, tx;
	std::float_t ny, ty;
	{
		auto dy = y[central_index + 1] - y[central_index - 1];
		auto dx = x[central_index + 1] - x[central_index - 1];
		angle = std::atan2(dy, dx);
		
		tx = dx * 1 / std::sqrt(dx * dx + dy * dy);
		ty = dy * 1 / std::sqrt(dx * dx + dy * dy);

		// clockwise		(x, y) → (-y, x)
		// anti-clockwise	(x, y) → (y, -x)

		nx =  ty;
		ny = -tx;
	}
	
	if (debug)
	{
		std::ofstream outputFile("./angle.txt");
		outputFile << angle << std::endl;
	}

	for(int n=0; n < x.size(); n++)
	{
		// clockwise rotation
		auto temp_x = x[n];
		x[n] = x[n]   * std::cos(-angle) - y[n] * std::sin(-angle);
		y[n] = temp_x * std::sin(-angle) + y[n] * std::cos(-angle);
	}

	if (debug)
	{
		std::ofstream outputFile("./rotate.txt");
		for (int n = 0; n < x.size(); n++) { outputFile << x[n] << "," << y[n] << std::endl; }
	}

	// fit polynomial 2nd degree
	auto coeffs = ZTask::polyfit<std::float_t>(x, y, 2);

	std::float_t curvature = 2 * coeffs[2]; // curvature = 2a for y = ax^2 + bx + c

	std::pair<std::float_t, cv::Point2f> result;
	result.first = curvature;

	cv::Point2f center;

	// we avoid division by zero
	if (std::abs(curvature) > 0.000000001)
	{
		center.x = (1. / curvature)*nx;
		center.y = (1. / curvature)*ny;
		//center = -center; // we need a vector from the point to the center (-1)

		auto temp_x = center.x;
		center.x = center.x * std::cos(-angle) + center.y * std::sin(-angle);
		center.y = -temp_x  * std::sin(-angle) + center.y * std::cos(-angle);

		center.x += static_cast<float>(window[central_index].x);
		center.y += static_cast<float>(window[central_index].y);

	}
	else
	{
		center.x = 0.;
		center.y = 0.;
	}

	result.second = center;

	return result;
}
