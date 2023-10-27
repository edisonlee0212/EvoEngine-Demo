cmake_minimum_required(VERSION 3.17)
project(
		EvoEngine-Demo
		VERSION 0.1
		DESCRIPTION "An interactive application developed for my research purposes based on EvoEngine"
)

option(BUILD_WITH_RAYTRACER "Build with Ray Tracer" ON)
option(BUILD_WITH_PHYSICS "Build with Physics" ON)

# Set a default build type if none was specified
set(default_build_type "Release")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
			STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
			"Debug" "Release")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (WIN32)
	# Compiler settings for Windows platform
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif (UNIX AND NOT APPLE)
	if (${CMAKE_BUILD_TYPE} STREQUAL Release)
		set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	elseif (${CMAKE_BUILD_TYPE} STREQUAL Debug)
		set(CMAKE_CXX_FLAGS_DEBUG "-g")
	endif ()
elseif (APPLE)
endif ()

# ------------------------------------------------------------------
# 3rd Party libraries
# ------------------------------------------------------------------
set(EVOENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../EvoEngine)

include_directories(${EVOENGINE_INCLUDES})

if (NOT TARGET evoengine)
	if (NOT TARGET evoengine)
		add_subdirectory(${EVOENGINE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/EvoEngine)
	endif()
endif ()
if(BUILD_WITH_RAYTRACER) 
	set(EVOENGINE_RAYTRACER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../EvoEngine-RayTracer)
	if (NOT TARGET evoengine-raytracer)
		add_subdirectory(${EVOENGINE_RAYTRACER_DIR} ${CMAKE_CURRENT_BINARY_DIR}/EvoEngine-RayTracer)
	endif ()
endif()
if(BUILD_WITH_PHYSICS) 
	set(EVOENGINE_PHYSICS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../EvoEngine-Physics)
	if (NOT TARGET evoengine-physics)
		add_subdirectory(${EVOENGINE_PHYSICS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/EvoEngine-Physics)
	endif ()
endif()

set(LOCAL_EVOENGINE_DEMO_INCLUDES
	${EVOENGINE_INCLUDES}
	${CMAKE_CURRENT_BINARY_DIR}
	)
set(LOCAL_EVOENGINE_DEMO_PCH
	${EVOENGINE_PCH}
	${CMAKE_CURRENT_SOURCE_DIR}/include/lib/evoengine_demo-pch.hpp
	)

# ------------------------------------------------------------------
# Example program
# ------------------------------------------------------------------

file(GLOB PLANET_SOURCES_LOCAL "src/core/Planet/*.cpp")
file(GLOB STAR_CLUSTER_SOURCES_LOCAL "src/core/StarCluster/*.cpp")
add_library(examples-core
	STATIC
	${PLANET_SOURCES_LOCAL}
	${STAR_CLUSTER_SOURCES_LOCAL}
	)
target_precompile_headers(examples-core
	PRIVATE
	${EVOENGINE_PCH}
	)
target_include_directories(examples-core
	PUBLIC
	${EVOENGINE_INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}/include/core
	)
target_link_libraries(examples-core
	evoengine
	)


add_executable(evoengine_demo_app
		"src/app/Application.cpp")

target_include_directories(evoengine_demo_app
	PRIVATE
	${LOCAL_EVOENGINE_DEMO_INCLUDES}
	)

target_precompile_headers(evoengine_demo_app
	PRIVATE
	${LOCAL_EVOENGINE_DEMO_PCH}
	)

target_link_libraries(evoengine_demo_app
	PRIVATE
	examples-core
	)

target_compile_definitions(evoengine_demo_app
	PRIVATE
	NOMINMAX
	)

if (BUILD_WITH_RAYTRACER)
	target_include_directories(evoengine_demo_app
		PRIVATE
		${EVOENGINE_RAYTRACER_INCLUDES}
	)
	target_link_libraries(evoengine_demo_app
		PRIVATE
		evoengine-raytracer
		)
	target_compile_definitions(evoengine_demo_app
		PRIVATE
		BUILD_WITH_RAYTRACER
		)
endif ()

if (BUILD_WITH_PHYSICS)
	target_include_directories(evoengine_demo_app
		PRIVATE
		${EVOENGINE_PHYSICS_INCLUDES}
	)
	target_link_libraries(evoengine_demo_app
		PRIVATE
		evoengine-physics
		)
	target_compile_definitions(evoengine_demo_app
		PRIVATE
		BUILD_WITH_PHYSICS
		)
endif ()

# ------------------------------------------------------------------
# Copy Internal resources
# ------------------------------------------------------------------
add_custom_command(TARGET evoengine_demo_app POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/src/Internals
		${CMAKE_BINARY_DIR})

file(COPY src/app/imgui.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})