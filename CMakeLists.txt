# Global settings.
cmake_minimum_required(VERSION 2.8)
project(libUtil)

# The version info.
set(libUtil_VERSION_MAJOR 0)
set(libUtil_VERSION_MINOR 0)
set(libUtil_VERSION_PATCH 4)
configure_file(
  "${PROJECT_SOURCE_DIR}/Util/config.h.in"
  "${PROJECT_SOURCE_DIR}/Util/config.h"
)

set(CMAKE_VERBOSE_MAKEFILE 1)

# Set up the install stuff.
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}")

# Set up the uninstall.  Copied verbatim from
# http://www.vtk.org/Wiki/CMake_FAQ#Installation_questions.  This same
# page points out that
#
#   xargs rm < install_manifest.txt
#
# works too.
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# Add the real source directory.
add_subdirectory(Util)
