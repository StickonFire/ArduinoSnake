cmake_minimum_required(VERSION 4.0.3)

project(ArduinoSnake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/c67de117379f4d1c889c7581a0a76aa0979c2083.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    ArduinoSnakeTest
    snakeModel/snakeModel.cpp
    controller/controller.cpp
    test/snakeModelTest.cpp
    test/controllerTest.cpp
)

target_link_libraries(
    ArduinoSnakeTest
    GTest::gtest_main
    GTest::gmock_main
)

target_include_directories(
    ArduinoSnakeTest
    PUBLIC include
)

include(GoogleTest)
gtest_discover_tests(ArduinoSnakeTest)