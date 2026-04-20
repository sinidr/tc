
find_package(GTest REQUIRED)


###############
# Plugin test #
###############

set(test_name "plugin_test")

add_executable(${test_name})

target_sources(
    ${test_name} 
    PRIVATE 
        "test_plugin.cpp"
)

target_link_libraries(${test_name} 
    PRIVATE
        tc::common
        GTest::gtest_main
)

add_test(
    NAME ${test_name}
    COMMAND ${test_name}
)

set_tests_properties(${test_name} PROPERTIES
    ENVIRONMENT "PLUGIN_PATH=$<TARGET_FILE:tc::plugin>"
)


#######################
# Crash reporter test #
#######################

find_package(
    Boost 
    CONFIG 
    COMPONENTS process filesystem
    REQUIRED
)

set(test_name "crash_reporter_test")

set(crashing_app ${test_name}_app)

add_executable(${crashing_app})

target_sources(${crashing_app} 
    PRIVATE 
        test_crash_reporter_app.cpp
)

target_link_libraries(${crashing_app} PRIVATE
    tc::common  # necessary if building with asan
)

if (NOT MSVC)
    # a gcc warning prevents the segfault
    target_compile_options(${crashing_app} PRIVATE -w)
endif()

add_executable(${test_name})

target_sources(${test_name}
    PRIVATE
        "test_crash_reporter.cpp"
)

target_link_libraries(${test_name} 
    PRIVATE 
        tc::common
        GTest::gtest_main
        Boost::boost
        Boost::filesystem
)

add_test(
    NAME ${test_name}
    COMMAND ${test_name}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} # so we dont add the crash report in the user sources
)


set_tests_properties(${test_name} 
    PROPERTIES
        ENVIRONMENT "PLUGIN_PATH=$<TARGET_FILE:tc::crash_reporter>;APP_PATH=$<TARGET_FILE:${crashing_app}>"
        TIMEOUT 10
)
