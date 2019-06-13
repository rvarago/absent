project(absent_tests LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}
        absent_tests.cpp
        bind_test.cpp
        boost_optional_test.cpp
        customnullable_test.cpp
        either_test.cpp
        fmap_test.cpp
        foreach_test.cpp
        unique_ptr_test.cpp
)

target_compile_features(${PROJECT_NAME}
        PRIVATE
            cxx_std_17
)

if (WITH_TESTS_FOR_BOOST)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WITH_ADAPTER_FOR_BOOST_OPTIONAL)
endif()

if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME}
            PRIVATE
                -Wall -Wextra -Werror -ansi -pedantic
    )
else()
    target_compile_options(${PROJECT_NAME}
            PRIVATE
                /Wall /W4
    )
endif()

find_package(gtest REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            gtest::gtest
            rvarago::absent
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})