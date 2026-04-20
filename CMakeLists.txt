cmake_minimum_required(VERSION 3.21)

# tc for 'Technical Challenge'
project(tc VERSION 1.0 LANGUAGES CXX)

###########
# options #
###########
option(ENABLE_SANITIZERS "Build with Address Sanitizer" OFF)

####################
# generic settings #
####################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
include(GNUInstallDirs)

################################
# build common settings target #
################################
add_library(common INTERFACE)
add_library(tc::common ALIAS common)

# c++ 20
target_compile_features(common INTERFACE cxx_std_20)

# common include directories
target_include_directories(common INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# warnings
if (MSVC)
    target_compile_options(common INTERFACE /W4 /WX)
else()
    target_compile_options(common INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

if (ENABLE_SANITIZERS)
    target_compile_options(common INTERFACE "-fsanitize=address")
    target_link_options(common INTERFACE "-fsanitize=address")
endif()

###########
# sources #
###########
add_subdirectory(app)
add_subdirectory(plugins)

#########
# tests #
#########
include(CTest)
add_subdirectory(tests)

########################
# some install targets #
########################

install(DIRECTORY include/plugin DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS common
    EXPORT tcTargets
)

install(EXPORT tcTargets
    NAMESPACE tc::
    DESTINATION lib/cmake/common
)
