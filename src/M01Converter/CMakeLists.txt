# CLI frontend subproject
cmake_minimum_required(VERSION 3.15)

add_executable(M01Converter
  main.cpp
)

target_include_directories(M01Converter
    PRIVATE 
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_SOURCE_DIR}/include/cxxopts
)

target_link_libraries(M01Converter
    PRIVATE M01Core
)

install(TARGETS M01Converter
        RUNTIME DESTINATION bin)