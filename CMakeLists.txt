cmake_minimum_required(VERSION 3.15)
project(M01-Converter VERSION 1.1.0 LANGUAGES CXX)

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

# Platform label for release archive: linux, macos, or windows
string(TOLOWER "${CMAKE_SYSTEM_NAME}" M01_OS)
if (M01_OS STREQUAL "darwin")
    set(M01_OS "macos")
endif ()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    set(M01_ARCH "x86_64")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
    set(M01_ARCH "arm64")
else ()
    set(M01_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# A macOS build targeting more than one architecture is universal.
list(LENGTH CMAKE_OSX_ARCHITECTURES M01_OSX_ARCH_COUNT)
if (M01_OSX_ARCH_COUNT GREATER 1)
    set(M01_ARCH "universal")
endif ()

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${PROJECT_VERSION}_${M01_OS}_${M01_ARCH}")

if (M01_OS STREQUAL "linux")
    set(CPACK_GENERATOR "TGZ")
else ()
    set(CPACK_GENERATOR "ZIP")
endif ()

set(CPACK_COMPONENTS_ALL runtime)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_ARCHIVE_RUNTIME_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")

include(CPack)