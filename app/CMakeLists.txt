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

# Add your main executable target first
# add_executable(MyApp main.cpp)

# Add a custom command to run after 'MyApp' is built
# add_custom_command(
#     TARGET MyApp                 # The target the command depends on
#     POST_BUILD                  # Specifies the command runs after the build
#     COMMAND $<TARGET_FILE:MyApp> # The command to execute (the full path to the built executable)
#     ARGS arg1 arg2 ...          # Optional arguments for the executable
#     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} # Optional: sets the working directory for the command
#     COMMENT "Running the built MyApp executable..." # Optional: message displayed during the build
# )

