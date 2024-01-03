cmake_minimum_required(VERSION 3.10)
project(rdup VERSION 1.0 LANGUAGES CXX)
option(WITH_PYTHON "Build Python Bindings" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
      FORCE)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WITH_PYTHON)
add_subdirectory(vendor/pybind11)
add_definitions("-DVERSION_INFO=${VERSION_INFO}")
pybind11_add_module(fastshuffle fastshuffle.cpp)
install(TARGETS fastshuffle LIBRARY DESTINATION lib)
endif()

add_executable(shuffle main.cpp shuffle.cpp)
target_link_libraries(shuffle)
install(TARGETS shuffle RUNTIME DESTINATION bin)

