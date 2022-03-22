cmake_minimum_required(VERSION 3.21)
project(OperatingSystemsAssignment3)

set(CMAKE_CXX_STANDARD 11)

add_executable(OperatingSystemsAssignment3 main.cpp output_mode_helpers.cpp output_mode_helpers.h tracereader.cpp tracereader.h PageTable.cpp PageTable.h Level.cpp Level.h paging.cpp paging.h)
