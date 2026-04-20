find_package(
  Boost
  CONFIG
  COMPONENTS log
  REQUIRED)

set(lib_name "plugin")

add_library(${lib_name} SHARED)
add_library(tc::plugin ALIAS ${lib_name})

target_sources(${lib_name} PRIVATE src/plugin.cpp)

target_compile_definitions(${lib_name} PRIVATE PLUGIN_BUILDING)

target_link_libraries(${lib_name} PRIVATE 
    tc::common
    Boost::log
)

install(TARGETS ${lib_name}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
