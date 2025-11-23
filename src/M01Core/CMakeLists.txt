add_library(M01Core
  M01Core.cpp
  Song.cpp
  Measure.cpp
  InstrumentHelper.cpp
  SaveFile.cpp

  midifile/Binasc.cpp
  midifile/MidiEvent.cpp
  midifile/MidiEventList.cpp
  midifile/MidiFile.cpp
  midifile/MidiMessage.cpp
)

target_include_directories(M01Core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/midifile
)

include(FetchContent)
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 2f86d13
)
FetchContent_MakeAvailable(yaml-cpp)
target_link_libraries(M01Core PUBLIC yaml-cpp::yaml-cpp)