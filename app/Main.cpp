#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <nlohmann/json.hpp>

#include "CircleDetector.h"

int main(void)
{
    std::cout << "Hi Circle Detection" << std::endl;

    std::ifstream ifs("C:\\Git\\koldoxo\\ZTask\\app\\config.json");
    nlohmann::json file = nlohmann::json::parse(ifs);

    std::cout << file["inputfile"]  << std::endl;
    std::cout << file["outputfile"] << std::endl;
  
    auto filex = std::string(file["inputfile"]);
    const char* file_ptr = filex.c_str();

    int xdim = 2464;
    int ydim = 2056;
    int depth = 8;
    int channels = 1;

    float* data = stbi_loadf(file_ptr, &xdim, &ydim, &depth, 0);

    for (int i = 0; i < xdim; i++) 
    { 
        if (data[i, 0] > 0.) { std::cout << std::to_string(data[i, 0]) << std::endl; }
    }


    return EXIT_SUCCESS;
}