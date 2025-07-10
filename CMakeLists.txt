cmake_minimum_required(VERSION 3.31)
project(Interpreter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(interpreter
        main.cpp
        interpreter.cpp
        functions.cpp
        variable.cpp
)

target_compile_options(interpreter PRIVATE -Wall -Wextra)