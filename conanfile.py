
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy

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
        self.requires("cpptrace/1.0.4", visible=False)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        compiler = str(self.settings.compiler)
        build_type = str(self.settings.build_type).lower()

        toolchain = CMakeToolchain(self)
        toolchain.presets_prefix = compiler
        toolchain.cache_variables["CMAKE_INSTALL_PREFIX"] = f"install/{compiler}-{build_type}"
        toolchain.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
 
    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["challenge_project"]

    def export_sources(self):
        include_patterns = [
            "app/*", "crash_reporter/*", "plugin/*", "tests/*",
            "CMakeLists.txt"
        ]

        for export_source_pattern in include_patterns:
            copy(self, export_source_pattern, self.recipe_folder, self.export_sources_folder)
