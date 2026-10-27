cmake_minimum_required (VERSION 2.6)
project (PROJECT)

# Single-TU rule: one .cpp = one .exe. The entry lives in _Seams/_Main.hxx;
# the generated .cpp wrapper (build tree) is the only source of the exe.
file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")
file (WRITE "${CMAKE_BINARY_DIR}/generated/_Main.cpp"
  "#include \"${PROJECT_SOURCE_DIR}/_Seams/_Main.hxx\"\n")
add_executable(PROJECT "${CMAKE_BINARY_DIR}/generated/_Main.cpp")
