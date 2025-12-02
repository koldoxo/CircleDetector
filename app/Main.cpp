#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

#include "CircleDetector.h"

int main(void)
{
    std::cout << "Hi Circle Detection" << std::endl;

    std::ifstream ifs("C:\\Git\\koldoxo\\ZTask\\app\\config.json");
    nlohmann::json file = nlohmann::json::parse(ifs);

    std::cout << file["inputfile"]  << std::endl;
    std::cout << file["outputfile"] << std::endl;
  

    return EXIT_SUCCESS;
}