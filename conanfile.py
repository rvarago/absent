from conans import ConanFile

class AbsentConan(ConanFile):
    name        = "absent"

    description = "A simple library to compose nullable types in a declarative style for the modern C++ programmer"
    author      = "Rafael Varago (rvarago)"
    topics      = ("nullable-type", "composition", "monadic-interface", "declarative-programming")
    license     = "MIT"
    homepage    = "https://github.com/rvarago/absent"
    url         = homepage
    exports     = ["README.md", "LICENSE"]

    build_requires  = "gtest/1.8.0@bincrafters/stable"
    generators      = "cmake_find_package"

    exports_sources = "include/*"
    no_copy_source = True

    def package(self):
        self.copy("*.h")

    def package_id(self):
        self.info.header_only()