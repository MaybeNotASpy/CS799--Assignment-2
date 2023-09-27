cmake_minimum_required(VERSION 3.20.5...3.27)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# Include Functions folder
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Functions)
include_directories(${CMAKE_SOURCE_DIR}/Algorithms)
project(
    GormanAssignment2
    VERSION 0.1
    DESCRIPTION "Assignment 2 for CS 776: Genetic Algorithms"
    LANGUAGES CXX
)

#add_compile_definitions(ELEMENTS=${N})
#add_compile_definitions(MAX_ITER=${MAX_ITER})
#add_compile_definitions(SAMPLE_SIZE=${SAMPLE_SIZE})
#add_compile_definitions(EVAL_MODULE="${EVAL_MODULE}")

set(CMAKE_CXX_STANDARD 23)
aux_source_directory(Functions Functions)
add_executable(Assignment2 Functions/dejong.cpp Algorithms/chc.cpp Algorithms/simple_ga.cpp parameter_search.cpp ga_performance.cpp chc_performance.cpp main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Assignment2 PUBLIC OpenMP::OpenMP_CXX)
endif()

add_compile_options(-fsanitize=address -fopenmp)
add_link_options(-fsanitize=address -fopenmp)

target_compile_options(Assignment2
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wconversion -Wsign-conversion -Wpedantic -Wshadow -Werror -pthread -fopenmp -Wno-shadow -Wno-unused-parameter>
    $<$<CONFIG:RELEASE>:-Ofast>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-ggdb3>
)


add_compile_definitions(
    $<$<CONFIG:RELEASE>:NDEBUG>
    $<$<CONFIG:RELEASE>:BOOST_DISABLE_ASSERTS>
)
