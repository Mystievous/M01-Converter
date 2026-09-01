cmake_minimum_required(VERSION 3.15)
project(M01-Converter VERSION 1.0.0 LANGUAGES CXX)

option(M01_FLAT_INSTALL "Install the executable and config.yml side by side." OFF)
include(GNUInstallDirs)

if (M01_FLAT_INSTALL)
    set(M01_INSTALL_BINDIR ".")
    set(M01_INSTALL_CONFIGDIR ".")
else ()
    set(M01_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}")
    set(M01_INSTALL_CONFIGDIR "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

# Top-level orchestrator only: add the subprojects.
add_subdirectory(src/M01Core)
add_subdirectory(src/M01-Converter)

install(FILES template/config.yml
        DESTINATION "${M01_INSTALL_CONFIGDIR}"
        COMPONENT runtime
)