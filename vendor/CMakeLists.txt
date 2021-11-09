cmake_minimum_required (VERSION 3.16)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)



#GLAD
set(GLAD_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glad" FORCE)
set(GLAD_INSTALL OFF CACHE BOOL "" FORCE)
set(GLAD_GENERATOR "c-debug" CACHE STRING "glad language" FORCE)
set(GLAD_API "gl=4.3" CACHE STRING "glad api version" FORCE)
add_subdirectory("glad")

#GLFW
add_subdirectory("glfw")

#ENTT
add_subdirectory("entt")

