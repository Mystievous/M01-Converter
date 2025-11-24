include(FetchContent)
set(BUILD_MIDILIBRARY_ONLY ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  midifile
  GIT_REPOSITORY https://github.com/Mystievous/M01-midifile.git
  GIT_TAG 191c334
)
FetchContent_MakeAvailable(midifile)

add_library(M01Core
  M01Core.cpp
  Song.cpp
  Measure.cpp
  InstrumentHelper.cpp
  SaveFile.cpp
)

target_link_libraries(M01Core PRIVATE midifile)
target_include_directories(M01Core PUBLIC ${midifile_SOURCE_DIR}/include)

target_include_directories(M01Core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/M01Core
)