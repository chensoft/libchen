cmake_minimum_required(VERSION 3.1)
project(libchen VERSION 1.1.1 LANGUAGES CXX)

include(CTest)
include(GNUInstallDirs)

add_subdirectory(src)
add_subdirectory(test)