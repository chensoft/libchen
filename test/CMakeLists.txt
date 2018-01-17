add_executable(libchen-test "")

# environment
set_target_properties(
    libchen-test PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
)

# include path
target_include_directories(libchen-test PRIVATE src)

# source codes
file(GLOB_RECURSE CHEN_TEST_INC src/*.hpp)
file(GLOB_RECURSE CHEN_TEST_SRC src/*.cpp)

# generate app
target_sources(libchen-test PRIVATE ${CHEN_TEST_INC} ${CHEN_TEST_SRC})

# link library
target_link_libraries(libchen-test libchen)

# make test
add_test(libchen-test libchen-test)