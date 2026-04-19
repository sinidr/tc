
set(bin_name app)

add_executable(${bin_name})

target_sources(${bin_name}
    PRIVATE
        src/main.cpp
)

target_link_libraries(${bin_name}
    PRIVATE
        tc::common
)

install(
    TARGETS ${bin_name}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
