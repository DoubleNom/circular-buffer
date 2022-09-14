cmake_minimum_required(VERSION 3.14)
project(CircularBuffer)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_compile_definitions(CIRCULAR_BUFFER_TEST)

add_executable(
        CircularBuffer_test
        circular_buffer_test.cpp
)

target_link_libraries(
        CircularBuffer_test
        gtest_main
)

gtest_discover_tests(CircularBuffer_test)
