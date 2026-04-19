
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class TcConan(ConanFile):

    name = "tc"
    version = "0.1"
    settings = "os", "arch", "compiler", "build_type"

    default_options = {
        "boost/*:without_log": False,
        "boost/*:without_log_setup": False,
    }

    def build_requirements(self):
        #self.tool_requires("cmake/3.22.0")
        #self.tool_requires("ninja/1.13.2")
        self.test_requires("gtest/1.17.0")

    def requirements(self):
        self.requires("boost/1.84.0", visible=False)

    def layout(self):
        compiler = str(self.settings.compiler)
        build_type = str(self.settings.build_type).lower()

        self.folders.build = f"build/{compiler}-{build_type}"
        self.folders.generators = f"{self.folders.build}/generators"

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        toolchain = CMakeToolchain(self)
        toolchain.presets_prefix = str(self.settings.compiler)
        toolchain.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()


