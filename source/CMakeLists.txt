cmake_minimum_required (VERSION 2.6)
project (KabukiToolkit)
# The version number.
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 9)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/chinese_room/include/module.h.in"
  "${PROJECT_BINARY_DIR}/chinese_room/include/module.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable
add_executable(KabukiToolkit tutorial.cc)

add_library(KabukiToolkit mysqrt.cc)