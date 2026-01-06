include(Catch)

set(TEST_MAIN "CircleDetectorTest")
set(TEST_SOURCES "CircleDetectorTest.cpp")
set(TEST_INCLUDES "./" "../include/")

add_executable(${TEST_MAIN} ${TEST_SOURCES})
target_include_directories(${TEST_MAIN} PUBLIC ${TEST_INCLUDES})
target_link_libraries(${TEST_MAIN} PUBLIC ${LIBRARY_NAME} ${OpenCV_LIBS} Catch2::Catch2WithMain)

catch_discover_tests(${TEST_MAIN})
