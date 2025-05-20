cmake_minimum_required(VERSION 3.31)
project(M01D-Converter)

set(CMAKE_CXX_STANDARD 20)

add_executable(M01D-Converter
        src/main.cpp
        src/Song.cpp
        src/Measure.cpp
        src/midifile/Binasc.cpp
        src/midifile/MidiEvent.cpp
        src/midifile/MidiEventList.cpp
        src/midifile/MidiFile.cpp
        src/midifile/MidiMessage.cpp
        src/InstrumentHelper.cpp
        include/InstrumentHelper.h
)

include_directories(include include/midifile include/cxxopts)

include(FetchContent)
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 2f86d13
)
FetchContent_MakeAvailable(yaml-cpp)

target_link_libraries(M01D-Converter PUBLIC yaml-cpp::yaml-cpp)
