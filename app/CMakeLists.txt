
add_executable(app)

target_sources(
    app
    PRIVATE
        src/main.cpp
)

target_link_libraries(
    app
    PRIVATE
        tc::common
)
