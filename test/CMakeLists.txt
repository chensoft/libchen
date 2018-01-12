add_executable(libchen-test EXCLUDE_FROM_ALL "")  # use make test to run it

# environment
set_target_properties(
    libchen-test PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
)

if(UNIX)
    target_compile_options(libchen-test PRIVATE -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers)
endif()

# include path
target_include_directories(libchen-test PRIVATE lib/googletest/googletest/include)

# source codes
file(GLOB_RECURSE CHEN_TEST_SRC src/*.cpp)

# generate app
target_sources(libchen-test PRIVATE ${CHEN_TEST_SRC})

# googletest
set(gtest_force_shared_crt ON CACHE BOOL "Use /MD in googletest")
add_subdirectory(lib/googletest/googletest)

# link library
target_link_libraries(libchen-test libchen gtest)