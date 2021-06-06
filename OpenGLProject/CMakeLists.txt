# CMakeList.txt : CMake project for OpenGLProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0076 NEW)
 
# Add source to this project's executable.
set(SOURCE_FILES 
	
	"input/input.cpp" 
	"window.cpp"
	
	"util/uuid.cpp"
	"application.cpp"

	"input/input.h"
	"window.h"
	 
	
	"util/uuid.h"
	"application.h"
	
	
	
	"util/debug.h"
	"util/errors.h"
	"module.h"

	"main.cpp" "util/worker_pool.h" "util/linked_list.h" "core_modules/render_module.h" "core_modules/event_module.h" "core_modules/update_module.h" "core_modules/threading_module.h"   "core_modules/render_module.cpp")
	



# specify executable
add_executable(OpenGLProject ${SOURCE_FILES})

#add libs
target_link_libraries(OpenGLProject PRIVATE glad)
target_link_libraries(OpenGLProject PRIVATE glfw)
find_package(OpenGL REQUIRED)
if(UNIX AND NOT APPLE)
	find_package(LibUUID REQUIRED) 
endif()


# add subfolders



# TODO: Add tests and install targets if needed.
