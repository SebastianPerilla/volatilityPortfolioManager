cmake_minimum_required(VERSION 3.20)
project(ExpressionProblems)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Explicitly set output directories
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#
# # Include FetchContent
include(FetchContent)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Fetch nlohmann_json;
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
#
FetchContent_MakeAvailable(json)
#
# # Find CURL
find_package(CURL REQUIRED)

# Enable testing before adding test subdirectory
# enable_testing()

# Add subdirectories
add_subdirectory(src)
# add_subdirectory(tests)
