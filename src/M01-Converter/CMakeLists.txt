# CLI frontend subproject
cmake_minimum_required(VERSION 3.15)

add_executable(M01-Converter
  main.cpp
)

target_include_directories(M01-Converter
    PRIVATE 
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_SOURCE_DIR}/include/cxxopts
)

target_link_libraries(M01-Converter
    PRIVATE M01Core
)

install(TARGETS M01-Converter
        RUNTIME DESTINATION bin)