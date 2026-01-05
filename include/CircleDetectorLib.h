#include <memory>



namespace ZTask
{

    class CircleDetector
    {
    public:

        class Parameter
        {


        };// class Parameter

        using ParameterPtr = std::shared_ptr<Parameter>;

        class Operator
        {
        public:

            int calculate(ParameterPtr);

        }; //class Operator
    };// class CircleDetector
}// namespace ZTask


