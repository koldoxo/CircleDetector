set(EXE_SOURCES
    "CircleDetector.cpp")

set(EXE_INCLUDES
    "./"
    "../include/"
    )

add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})

target_include_directories(
    ${EXECUTABLE_NAME} 
    PUBLIC
    ${EXE_INCLUDES})

target_link_libraries(
    ${EXECUTABLE_NAME} 
    PUBLIC
    ${LIBRARY_NAME}
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBS})