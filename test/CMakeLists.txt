# source codes
file(GLOB_RECURSE INC_TEST src/*.hpp)
file(GLOB_RECURSE SRC_TEST src/*.cpp)

# generate app
add_executable(libchen_test ${SRC_TEST})

# properties
set_target_properties(
        libchen_test PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
)

if(UNIX)
    target_compile_options(libchen_test PRIVATE -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers)
endif()

# include path
target_include_directories(libchen_test PRIVATE lib/googletest/googletest/include)

# link library
target_link_libraries(libchen_test libchen gtest)

# googletest
set(gtest_force_shared_crt ON CACHE BOOL "Use /MD in googletest")
add_subdirectory(lib/googletest/googletest)