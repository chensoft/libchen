# source codes
file(GLOB_RECURSE INC_CHEN ${CMAKE_SOURCE_DIR}/include/chen/*.hpp)  # todo maybe PROJECT_SOURCE_DIR
file(GLOB_RECURSE SRC_CHEN *.cpp)

# static build
# use -DBUILD_SHARED_LIBS=ON to build a shared library
add_library(libchen ${SRC_CHEN})

# versions
target_compile_definitions(
        libchen PUBLIC
        CHEN_VERSION_MAJOR=${libchen_VERSION_MAJOR}
        CHEN_VERSION_MINOR=${libchen_VERSION_MINOR}
        CHEN_VERSION_PATCH=${libchen_VERSION_PATCH}
        CHEN_VERSION="${libchen_VERSION}"
)

# properties
set_target_properties(
        libchen PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        VERSION ${libchen_VERSION}
        OUTPUT_NAME libchen PREFIX ""
)

# include path
target_include_directories(libchen PUBLIC ${CMAKE_SOURCE_DIR}/include)

# install
include(GNUInstallDirs)
install(TARGETS libchen
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/chen DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# code coverage
option(CHEN_CODE_COVERAGE "Enable libchen code coverage." OFF)

if(CHEN_CODE_COVERAGE)
    target_compile_options(libchen PRIVATE -g -O0 --coverage)  # debug, no optimization, enable coverage
    target_link_libraries(libchen gcov)
endif()