find_package(
  Boost
  CONFIG
  COMPONENTS log
  REQUIRED)

set(lib_name "plugin")

add_library(${lib_name} SHARED)
add_library(tc::plugin ALIAS ${lib_name})

target_sources(${lib_name} PRIVATE include/plugin/plugin.h src/plugin.cpp)

target_compile_definitions(${lib_name} PRIVATE PLUGIN_BUILDING)

target_link_libraries(${lib_name} PRIVATE 
    tc::common
    Boost::log
)
