#include "CircleDetectorLib.h"

int ZTask::CircleDetector::Operator::calculate(ParameterPtr parameter)
{

	std::shared_ptr<ContourArray> contours = parameter->inputContours;

	return EXIT_SUCCESS;
}

std::vector<std::float_t> ZTask::CircleDetector::Operator::get_curvature_profile(const ContourType& contour, std::int64_t windowSize, std::int64_t windowRatio)
{
	std::vector<std::float_t> profile;
	profile.resize(contour.size(), 0.f);

	auto profileIt = profile.data();
	for (auto& it = contour.begin(); it != contour.end(); it++ ) 
	{
		*profileIt = get_local_curvature(static_cast<std::uint64_t>(std::distance(contour.begin(), it)), contour, windowSize, false);
		profileIt++;
	}

	return profile;
}


std::float_t ZTask::CircleDetector::Operator::get_local_curvature(std::uint64_t index, const ContourType& contour, std::uint64_t windowSize, bool debug = false)
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
		// splitting and translating
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

	// compute angle of middle point
	std::float_t angle;
	{
		auto dy = y[central_index + 1] - y[central_index - 1];
		auto dx = x[central_index + 1] - x[central_index - 1];
		angle = std::atan2(dy, dx);
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

	auto coeffs = ZTask::polyfit<std::float_t>(x, y, 2);

	std::float_t curvature = 2 * coeffs[2]; // curvature = 2a for y = ax^2 + bx + c

	return curvature;
}
