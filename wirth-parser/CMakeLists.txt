cmake_minimum_required (VERSION 3.16)

project (WirthParser)

include_directories ("${PROJECT_SOURCE_DIR}/include")
set (CMAKE_BUILD_TYPE Release)

add_executable (wirth-parser src/main.cpp src/parser.cpp src/ast.cpp)
target_compile_options (
    wirth-parser
    PUBLIC
    -std=c++20
    -O3
    -Wall
    -Werror
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wno-old-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion
    -Wformat=2
    -Wno-unused-variable
    -Wno-ignored-attributes
    -pedantic
)
