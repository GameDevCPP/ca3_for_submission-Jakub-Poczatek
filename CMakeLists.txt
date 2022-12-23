cmake_minimum_required(VERSION 3.9)
project(CA3)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
file(GLOB_RECURSE CMPNTS src/components/*.cpp src/components/*.h)
file(GLOB_RECURSE SCNS src/scenes/*.cpp src/scenes/*.h)
add_executable(CA3 ${SOURCES} ${RESOURCE_FILES} src/components/cmp_health_pickup.cpp src/components/cmp_health_pickup.h src/scenes/scene_test.cpp src/scenes/scene_test.h src/JsonData.h)
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
target_link_libraries(CA3 lib_engine)
set(EXECUTABLES ${EXECUTABLES} CA3)
add_dependencies(CA3 copy_resources)
set_target_properties(CA3
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
        )

#### setup Directories ####
#Main output directory
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
        -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
        )

#### Add External Dependencies ####

# SFML - graphics library
add_subdirectory("lib/sfml" EXCLUDE_FROM_ALL)
#include_directories("lib/sfml/include")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# B2D - Box2D phyics library
add_subdirectory("lib/b2d/Box2D" EXCLUDE_FROM_ALL)
#include_directories("lib/b2d/Box2D/")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Add Engine and our own library projects####

file(GLOB_RECURSE RESOURCE_FILES "res/*.*")
####
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(lib_engine lib_tile_level_loader_rgb lib_maths lib_ecm Box2D sfml-graphics sfml-audio lib_aStar lib_audio lib_json)

## Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" INTERFACE ${SFML_INCS})

## RGB tile loader lib ##
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader_rgb/*.cpp lib_tile_level_loader_rgb/*.h)
add_library(lib_tile_level_loader_rgb STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_tile_level_loader_rgb INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader_rgb/" )
target_link_libraries(lib_tile_level_loader_rgb lib_maths sfml-graphics lib_bmp_loader)

## BMP image loader lib ##
file(GLOB_RECURSE SOURCE_FILES lib_bmp_loader/*.cpp lib_bmp_loader/*.h)
add_library(lib_bmp_loader STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_bmp_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_bmp_loader/" )
target_link_libraries(lib_bmp_loader lib_maths sfml-graphics)

## A* lib ##
file(GLOB_RECURSE SOURCE_FILES lib_aStar/*.cpp lib_aStar/*.h)
add_library(lib_aStar STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_aStar INTERFACE "${CMAKE_SOURCE_DIR}/lib_aStar/" )

## Audio lib ##
file(GLOB_RECURSE SOURCE_FILES lib_audio/*.cpp lib_audio/*.h)
add_library(lib_audio STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_audio INTERFACE "${CMAKE_SOURCE_DIR}/lib_audio/" )
target_link_libraries(lib_audio sfml-audio)

## JSON lib ##
add_library(lib_json INTERFACE lib_json/json.h)
target_sources(lib_json INTERFACE "${CMAKE_SOURCE_DIR}/lib_json/json.h")
target_include_directories(lib_json INTERFACE "${CMAKE_SOURCE_DIR}/lib_json" INTERFACE ${SFML_INCS})

## ECM lib
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)

foreach(exe ${EXECUTABLES})
    set_target_properties(${exe} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            )
    #add all SFML dlls to the output directory
    add_custom_command(TARGET ${exe} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_BINARY_DIR}/lib/sfml/bin/"
            $<TARGET_FILE_DIR:${exe}>)
endforeach()

# Compiler flags
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
elseif(APPLE)
    message("hello apple")
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9 CACHE STRING "")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)