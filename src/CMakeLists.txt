add_library(libchen "")

# environment
set_target_properties(
    libchen PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    VERSION ${libchen_VERSION}
    OUTPUT_NAME libchen PREFIX ""
)

target_compile_definitions(
    libchen PUBLIC
    CHEN_VERSION_MAJOR=${libchen_VERSION_MAJOR}
    CHEN_VERSION_MINOR=${libchen_VERSION_MINOR}
    CHEN_VERSION_PATCH=${libchen_VERSION_PATCH}
    CHEN_VERSION="${libchen_VERSION}"
)

# include path
target_include_directories(libchen PUBLIC ${CMAKE_SOURCE_DIR}/include)  # todo maybe PROJECT_SOURCE_DIR

# source codes
file(GLOB_RECURSE CHEN_SRC *.cpp)  # I hate to list all the files

# static build
# use -DBUILD_SHARED_LIBS=ON to build a shared library
target_sources(libchen PRIVATE ${CHEN_SRC})

# install libs
include(GNUInstallDirs)

install(
    TARGETS libchen
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/chen DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# code coverage
option(CHEN_CODE_COVERAGE "Enable libchen code coverage." OFF)

if(CHEN_CODE_COVERAGE)
    target_compile_options(libchen PRIVATE -g -O0 --coverage)  # debug, no optimization, enable coverage
    target_link_libraries(libchen gcov)
endif()