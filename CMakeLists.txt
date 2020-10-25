cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}/CMake" p)
list(APPEND CMAKE_MODULE_PATH "${p}") #For old Imagine++
list(APPEND CMAKE_SYSTEM_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?
find_package(Imagine REQUIRED)

set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.12.9/clang_64)

project(TP4)

add_executable(TP4 quadtree/quadtree.h tools.h main.cpp)
ImagineUseModules(TP4 Graphics)
