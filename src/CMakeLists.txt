# source codes
file(GLOB_RECURSE INC_CHEN ${CMAKE_SOURCE_DIR}/include/chen/*.hpp)  # todo maybe PROJECT_SOURCE_DIR
file(GLOB_RECURSE SRC_CHEN *.cpp)

# static build
# use -DBUILD_SHARED_LIBS=ON to build a shared library
add_library(libchen ${SRC_CHEN})

# properties
set_target_properties(
        libchen PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        OUTPUT_NAME libchen PREFIX ""
)

# include path
target_include_directories(libchen PUBLIC ${CMAKE_SOURCE_DIR}/include)

# code coverage
option(CHEN_CODE_COVERAGE "Enable libchen code coverage." OFF)

if(CHEN_CODE_COVERAGE)
    target_compile_options(libchen PUBLIC -g -O0 --coverage)  # debug, no optimization, enable coverage
endif()


#include(cmake/ChenInit.cmake)
#include(cmake/ChenUtil.cmake)
#
## group files in the IDE(e.g: Xcode and Visual Studio)
#chen_group_files(${CMAKE_CURRENT_SOURCE_DIR} "${INC_CHEN}")
#chen_group_files(${CMAKE_CURRENT_SOURCE_DIR} "${SRC_CHEN}")