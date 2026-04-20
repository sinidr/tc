

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake
from conan.tools.build import cross_building

import os

class TestPackageConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeDeps", "VirtualRunEnv"
    test_type = "explicit"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def generate(self):
        tc = CMakeToolchain(self, generator="Ninja")
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if not cross_building(self):
            libdir = self.package_folder
            libname = "test_package"

            if self.settings.os == "Windows":
                filename = f"{libname}.dll"
                path_var = "%TC_PATH%"
            elif self.settings.os == "Macos":
                filename = f"lib{libname}.dylib"
                path_var = "$TC_PATH"
            else:
                filename = f"lib{libname}.so"
                path_var = "$TC_PATH"

            plugin_path = os.path.join(libdir, filename)
            self.run(f"{path_var}/bin/app {plugin_path}", env="conanrun")
