set(LIBRARY_SOURCES
    "CircleDetectorLib.cpp")
set(LIBRARY_HEADERS
    "../include/CircleDetectorLib.h"
    "../include/DataTypes.h"
    )
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
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBS}
    PolyFit
    )

if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP active: ${LIBRARY_NAME}")
    target_link_libraries(${LIBRARY_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

#target_include_directories(${LIBRARY_NAME} PUBLIC ${LIBRARY_INCLUDES})
