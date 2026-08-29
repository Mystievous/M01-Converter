include(FetchContent)
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 2f86d13
)
FetchContent_MakeAvailable(yaml-cpp)

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
        FileBytes.cpp
        SongDecoder.cpp
        MidiExport.cpp
)

target_compile_options(M01Core PRIVATE -Wall -Wextra)
target_link_libraries(M01Core PRIVATE yaml-cpp::yaml-cpp midifile)
target_include_directories(M01Core PUBLIC ${midifile_SOURCE_DIR}/include)
target_include_directories(M01Core
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/M01Core
)