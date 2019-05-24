cmake_minimum_required(VERSION 3.10)

project(absent LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

add_library(rvarago::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        INTERFACE
            include
)

target_compile_features(${PROJECT_NAME}
        INTERFACE
            cxx_std_17
)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()