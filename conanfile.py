from conans import ConanFile

class AbsentConan(ConanFile):
    build_requires  = "catch2/2.11.3"
    generators      = "cmake_find_package"
