# CMakeList.txt : CMake project for OpenGLProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)
cmake_policy(SET CMP0076 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source to this project's executable.
set(SOURCE_FILES 
	
	"main.cpp" 
	
	"application.cpp"
	"application.h"
	"window.cpp"
	"window.h"
	
	"core.h"
	"debug.h"
	"log.h"
	"assert.h"
	"profiling.h"
	"defer.h"
	
	"input/input.cpp" 
	"input/input.h"

	"util/time.h"
	"util/image.h" 
	"util/array_vector.h"
	"util/text_colours.h"
	"util/greedy_vector.h" 
	"util/fileio.h"
	"util/fileio.cpp"

	"util/stb_image.h"
	"util/stb_image.cpp"
	"util/stb_image_write.h" 
	"util/stb_custom_macros.h"  
	
	"scene/entity.h"  
	  
	"graphics/texture.h" 
	"graphics/context.h"
	"graphics/buffer.h"
	"graphics/vertex_array.h"
	"graphics/vertex_array.cpp"
	"graphics/2D/tex_coords.h"
	"graphics/2D/instance_renderer.h"	
	"graphics/2D/instance_renderer.cpp"
	"graphics/shader.h"
	"graphics/shader.cpp"
	 
	"math/vector.h" 
	"math/matrix.h"  
	"math/transform.h" 
	"math/funcs.h"
		 
	"assets/asset.h"
	"assets/asset_manager.h")
	
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "/Zc:alignedNew")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# specify executable
add_compile_definitions(OGL_DEBUG OGL_ENABLE_WARNINGS)
add_executable(OpenGLProject ${SOURCE_FILES}  "scene/entity.h")
include_directories(OpenGLProject PRIVATE "./")
# target_precompile_headers(OpenGLProject PRIVATE "oglpch.h")

get_target_property(cppversion OpenGLProject CXX_STANDARD)
message(STATUS "Compiling with C++ Version " ${cppversion})

#add libs
target_link_libraries(OpenGLProject PRIVATE glad)
target_link_libraries(OpenGLProject PRIVATE glfw)
target_link_libraries(OpenGLProject PRIVATE EnTT::EnTT)
find_package(OpenGL REQUIRED)
find_package(TBB QUIET)
if(${TBB_FOUND})
	message(STATUS "Found TBB")
	target_link_libraries(OpenGLProject PRIVATE TBB::tbb)
endif(${TBB_FOUND})

# add subfolders



# TODO: Add tests and install targets if needed.
