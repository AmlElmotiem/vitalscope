cmake_minimum_required(VERSION 3.20)
project(vitalscope_analysis_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(vitalscope_lib
    src/stats.cpp
    src/anomaly.cpp
    src/news2.cpp
    src/analysis.cpp
)
target_include_directories(vitalscope_lib PUBLIC include)

option(VITALSCOPE_BUILD_TESTS "Build the vitalscope unit tests" ON)
if(VITALSCOPE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
