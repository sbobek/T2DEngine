CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)

Project(game)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#Using c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL    REQUIRED)
find_package(GLEW      REQUIRED)
find_package(sid_preprocessor REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/../../engine/source/cmake/preprocessor.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../engine/source/cmake/util.cmake)

# GRAPHICS_SYSTEM
find_path(GFX_INCLUDE_DIR 
          NAMES texture_2d.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/gfx/source/src)

# UTILITY
find_path(UTILITY_INCLUDE_DIR 
          NAMES Timer.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/utility/source/src)

# MATH
find_path(MATH_INCLUDE_DIR 
          NAMES vec2.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/math/source/src)

# MEM
find_path(MEM_INCLUDE_DIR 
          NAMES memory_allocator.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/mem/source/src)

# RMS
find_path(RMS_INCLUDE_DIR 
	  NAMES Resource.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/rms/source/src)

# INPUT SYSTEM
find_path(IO_INCLUDE_DIR 
	  NAMES Input_manager.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/io/source/src)

# TILE MAP
find_path(TMAP_INCLUDE_DIR 
	  NAMES Tile.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/tmap/source/src)

# PHYSISCS_SYSTEM
find_path(PHY_2D_INCLUDE_DIR 
	  NAMES Body_2d.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/phy_2d/source/src)

# GAME OBJECT MODEL
find_path(GOM_INCLUDE_DIR 
          NAMES game_object.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/gom/source/src)

# UI
find_path(UI_INCLUDE_DIR
          NAMES Widget.hpp
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/ui/source/src)

# LEVEL MANAGEMENT 
find_path(LEVEL_MANAGEMENT_INCLUDE_DIR
          NAMES Level_manager.hpp
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/level_management/source/src)

#ENGINE
find_path(ENGINE_INCLUDE_DIR 
	  NAMES engine.hpp 
	  HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/source/src)

if(WIN32)
	find_library(ENGINE_LIBRARY NAMES Engine   HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/Build/Debug)
elseif(APPLE OR UNIX)
	find_library(ENGINE_LIBRARY NAMES libengine.a   HINTS ${CMAKE_CURRENT_LIST_DIR}/../../engine/Build)
endif()

if(APPLE)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(COREVIDEO_LIBRARY CoreVideo)
endif()

set(source_dir src)

set(include_files src/Hover_robot.hpp src/Hover_robot_creator.hpp
                  src/Player.hpp src/Player_creator.hpp src/Projectile_creator.hpp 
		  src/Player_idle_state.hpp src/Player_ducking_state.hpp src/Player_running_state.hpp
		  src/Player_jumping_state.hpp src/Player_climbing_state.hpp 
		  src/Player_taking_hit_state.hpp src/game.hpp)

set(unprocessed_source_files Hover_robot.cpp  Hover_robot_creator.cpp Player.cpp
                             Player_creator.cpp Projectile_creator.cpp Player_idle_state.cpp
		             Player_ducking_state.cpp Player_running_state.cpp 
			     Player_jumping_state.cpp Player_climbing_state.cpp 
			     Player_taking_hit_state.cpp game.cpp)

set(preprocessed_files "")
preprocess_source_files("${unprocessed_source_files}" preprocessed_files
                        "${CMAKE_CURRENT_LIST_DIR}/${source_dir}" "${CMAKE_CURRENT_BINARY_DIR}")

source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}" PREFIX "Preprocessed Source Files"
             FILES ${preprocessed_files})

# add the directory to each source file name
set(full_unprocessed_sources_name "")
prepend_string_to_list("${source_dir}/" "${unprocessed_source_files}" full_unprocessed_sources_name)

# dont compile the unprocessed files
set_source_files_properties(${full_unprocessed_sources_name} PROPERTIES HEADER_FILE_ONLY TRUE)



add_executable(game ${include_files} ${preprocessed_files} 
                    ${full_unprocessed_sources_name})

#Copy the resources folder to the executable folder
add_custom_command(TARGET game 
                   POST_BUILD 
		   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../resources $<TARGET_FILE_DIR:game>/resources)

if(APPLE)
	target_link_libraries(game ${ENGINE_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY} 
	                           ${GLEW_LIBRARY} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} 
				   ${COREVIDEO_LIBRARY})
elseif(WIN32 OR UNIX)	
	target_link_libraries(game ${ENGINE_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY} 
	                           ${GLEW_LIBRARY})
endif()

target_include_directories(game PUBLIC "${CMAKE_CURRENT_LIST_DIR}/${source_dir}" 
                                        ${GFX_INCLUDE_DIR}  ${MATH_INCLUDE_DIR} 
					${MEM_INCLUDE_DIR}  ${RMS_INCLUDE_DIR} 
					${IO_INCLUDE_DIR}   ${UTILITY_INCLUDE_DIR} 
					${TMAP_INCLUDE_DIR} ${PHY_2D_INCLUDE_DIR} 
					${GOM_INCLUDE_DIR}  ${UI_INCLUDE_DIR}
					${LEVEL_MANAGEMENT_INCLUDE_DIR}
					${ENGINE_INCLUDE_DIR})

