#include "CircleDetectorLib.h"

int ZTask::CircleDetector::Operator::calculate(ParameterPtr parameter)
{

	std::shared_ptr<ContourArray> contours = parameter->inputContours;

	return 0;
}
