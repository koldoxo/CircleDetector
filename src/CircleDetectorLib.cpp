#include "CircleDetectorLib.h"

int ZTask::CircleDetector::Operator::calculate(ParameterPtr parameter)
{

	std::shared_ptr<std::vector<std::vector<cv::Point>>> contours = parameter->inputContours;

	return 0;
}
