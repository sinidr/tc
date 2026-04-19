cmake_minimum_required(VERSION 3.21)

# tc for 'Technical Challenge'
project(tc VERSION 1.0 LANGUAGES CXX)

####################
# generic settings #
####################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

################################
# build common settings target #
################################
add_library(tc_common_settings INTERFACE)
add_library(tc::common ALIAS tc_common_settings)

# c++ 20
target_compile_features(tc_common_settings INTERFACE cxx_std_20)

# common include directories
target_include_directories(tc_common_settings INTERFACE "plugin/include")

###########
# sources #
###########
add_subdirectory(app)
add_subdirectory(plugin)
add_subdirectory(crash_reporter)

#########
# tests #
#########
include(CTest)
add_subdirectory(tests)
