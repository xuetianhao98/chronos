cmake_minimum_required(VERSION 3.10)
project(chronos)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(chronos STATIC
        src/chronometer_manager.cpp
)

target_include_directories(chronos
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_executable(example examples/example.cpp)

target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(example PRIVATE chronos)