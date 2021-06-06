# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0076 NEW)

project ("OpenGLProject")

# Include sub-projects.
add_subdirectory("vendor")
add_subdirectory ("OpenGLProject")
