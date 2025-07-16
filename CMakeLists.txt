cmake_minimum_required(VERSION 3.18)
project(Tesi LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(FF_HEADER_ONLY)

add_executable(test_queue_simple
  src/test_queue_simple.cpp
)

target_include_directories(test_queue_simple PRIVATE
  ${CMAKE_SOURCE_DIR}/external/fastflow
  ${CMAKE_SOURCE_DIR}/include
)