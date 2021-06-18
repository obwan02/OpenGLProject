# CMakeList.txt : CMake project for OpenGLProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0076 NEW)
 
# Add source to this project's executable.
set(SOURCE_FILES 
	
	"input/input.cpp" 
	"window.cpp"
	
	"application.cpp"

	"application.h"
	"util/errors.h"
	"layer.h"

	"main.cpp"  
	"util/linked_list.h" 
	 
	"events/event_dispatcher.h"  
	"events/event.h" 
	"util/util.h" 
	"events/events.h" 
	"graphics/vertex_array.h" 
	"graphics/buffer.h" "graphics/vertex_buffer.h" "graphics/vertex_array.cpp" "graphics/gl_data_types.h")

# specify executable
add_compile_definitions(OGL_DEBUG OGL_ENABLE_ASSERTS OGL_ENABLE_ERRORS OGL_ENABLE_WARNINGS)
add_executable(OpenGLProject ${SOURCE_FILES})
include_directories(OpenGLProject PRIVATE "./")
target_precompile_headers(OpenGLProject PRIVATE "oglpch.h")

get_target_property(cppversion OpenGLProject CXX_STANDARD)
message(STATUS "Compiling with C++ Version " ${cppversion})

#add libs
target_link_libraries(OpenGLProject PRIVATE glad)
target_link_libraries(OpenGLProject PRIVATE glfw)
find_package(OpenGL REQUIRED)


# add subfolders



# TODO: Add tests and install targets if needed.
