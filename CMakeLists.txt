cmake_minimum_required(VERSION 4.1)
project(vector_vs_deque)
set(CMAKE_CXX_STANDARD 17)

add_executable(vector_vs_deque vector_vs_deque.cpp)

set(OPT_LEVEL "O1" CACHE STRING "Optimization level: O0, O1, O2, O3")
set_property(CACHE OPT_LEVEL PROPERTY STRINGS O0 O1 O2 O3)

if (OPT_LEVEL STREQUAL "O0")
  target_compile_options(vector_vs_deque PRIVATE -O0)
elseif(OPT_LEVEL STREQUAL "O1")
  target_compile_options(vector_vs_deque PRIVATE -O1)
elseif(OPT_LEVEL STREQUAL "O2")
  target_compile_options(vector_vs_deque PRIVATE -O2)
elseif(OPT_LEVEL STREQUAL "O3")
  target_compile_options(vector_vs_deque PRIVATE -O3)
endif()
