cmake_minimum_required(VERSION 3.22)

project(ZTASK VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBRARY_NAME "CircleDetectorLib")
set(EXECUTABLE_NAME "CircleDetector")

message(STATUS "Using VCPKG")
include(${CMAKE_SOURCE_DIR}/pkg/vcpkg/scripts/buildsystems/vcpkg.cmake)

# nlohman_json
find_package(nlohmann_json REQUIRED)

#OpenCV 4
set(OpenCV_ROOT "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
find_package(OpenCV REQUIRED)

find_package(OpenMP REQUIRED)

add_subdirectory(src)
add_subdirectory(app)

### TESTS ###
if(ENABLE_TESTING)
    find_package(Catch2 REQUIRED)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()
