
find_package(GTest REQUIRED)

set(test_name "plugin_test")

add_executable(${test_name})

target_sources(${test_name} PRIVATE "test_plugin.cpp")

target_link_libraries(${test_name} PRIVATE
    tc::common
    tc::plugin
    GTest::gtest_main
)

add_test(
    NAME ${test_name}
    COMMAND ${test_name}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
