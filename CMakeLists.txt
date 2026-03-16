cmake_minimum_required(VERSION 3.10)
project(chronos)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(chronos STATIC
        src/chronometer.cpp
        src/chronotimer.cpp
        include/threadpool.h
        src/threadpool.cpp
)

target_include_directories(chronos
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_executable(chronometer_example examples/chronometer_example.cpp)

target_include_directories(chronometer_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(chronometer_example PRIVATE chronos)

add_executable(chronotimer_example examples/chronotimer_example.cpp)

target_include_directories(chronotimer_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(chronotimer_example PRIVATE chronos)