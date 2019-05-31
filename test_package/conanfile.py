from conans import ConanFile, CMake
import os

class AbsentTestConan(ConanFile):
    name        = "absent_test_package"
    author      = "Rafael Varago (rvarago)"

    generators  = "cmake_find_package"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        self.run(".{}{}".format(os.sep, self.name))
