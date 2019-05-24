cmake_minimum_required(VERSION 3.10)

project(absent LANGUAGES CXX)

# Definition

add_library(${PROJECT_NAME} INTERFACE)

add_library(rvarago::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        INTERFACE
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(${PROJECT_NAME}
        INTERFACE
            cxx_std_17
)

# Installation

include(GNUInstallDirs)

# Generate the config file with the target
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Config
)
install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Make the target be importable from the installation directory
install(EXPORT ${PROJECT_NAME}Config
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE rvarago::
)

# Make the target be importable from the build directory via package registry
export(EXPORT ${PROJECT_NAME}Config
       NAMESPACE rvarago::
)
export(PACKAGE ${PROJECT_NAME})

# Tests

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()