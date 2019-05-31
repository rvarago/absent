from conans import ConanFile

class AbsentConan(ConanFile):
    name        = "absent"
    version     = "0.1"
    description = "A simple library to compose nullable types in a declarative style for the modern C++ programmer"
    author      = "Rafael Varago (rvarago)"
    topics      = ("nullable-type", "composition", "monadic-interface", "declarative-programming")
    license     = "MIT"
    url         = "https://github.com/rvarago/absent"

    settings    = "os", "compiler", "arch", "build_type"
    requires    = "gtest/1.8.0@bincrafters/stable"
    generators  = "cmake_find_package"

