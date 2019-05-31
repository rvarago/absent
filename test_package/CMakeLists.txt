cmake_minimum_required(VERSION 3.10)

project(absent_test_package LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(absent REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

enable_testing()
add_test(NAME ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND ${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            absent::absent
)

target_compile_features(${PROJECT_NAME}
        PRIVATE
            cxx_std_17
)
