# CrabsTK
# @file    CMakeLists.txt
# @author  Cale McCollough <https://cookingwithcale.org>
# @license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
# reserved (R). This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at <https://mozilla.org/MPL/2.0/>.
cmake_minimum_required (VERSION 3.16)
project (PROJECT)

# ASCIICrabs is expected as a sibling or parent directory.
# Adjust this path to match your actual layout.
set (ASCIICRABS_DIR "../../ASCIICrabs" CACHE PATH "Path to ASCIICrabs")

include_directories("${ASCIICRABS_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/_Seams")

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Single-TU rule: one .cpp = one .exe. The entry lives in _Seams/_Main.hxx;
# the generated .cpp wrapper (build tree) is the only source of the exe.
file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")
file (WRITE "${CMAKE_BINARY_DIR}/generated/_Main.cpp"
  "#include \"${PROJECT_SOURCE_DIR}/_Seams/_Main.hxx\"\n")
add_executable(PROJECT "${CMAKE_BINARY_DIR}/generated/_Main.cpp")
