project(absent_tests LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}
        attempt_test.cpp
        bind_test.cpp
        either_test.cpp
        eval_test.cpp
        fmap_test.cpp
        foreach_test.cpp
        main.cpp
)

target_compile_features(${PROJECT_NAME}
        PRIVATE
            cxx_std_17
)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME}
            PRIVATE
                -Wall -Wextra -Werror -ansi -pedantic
    )
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME}
            PRIVATE
                /Wall /W4
    )
else()
    message("Unknown compiler...skipping configuration for warnings")
endif()

find_package(Catch2 REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        rvarago::absent
        Catch2::Catch2
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
