set(LIBRARY_SOURCES
    "CircleDetector.cpp")
set(LIBRARY_HEADERS
    "../include/CircleDetector.h")
set(LIBRARY_INCLUDES
    "./"
    "../include/"
    "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME} PUBLIC
    ${LIBRARY_INCLUDES})
target_link_libraries(${LIBRARY_NAME} PUBLIC
    nlohmann_json::nlohmann_json)
target_include_directories(${LIBRARY_NAME} PUBLIC
    ${LIBRARY_INCLUDES})
