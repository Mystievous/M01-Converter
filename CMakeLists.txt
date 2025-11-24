cmake_minimum_required(VERSION 3.15)
project(M01-Converter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Top-level orchestrator only: add the subprojects.
add_subdirectory(src/M01Core)
add_subdirectory(src/M01-Converter)