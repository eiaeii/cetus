cmake_minimum_required(VERSION 2.8)

set( PROJ_NAME      "cetus" )
set( PROJ_PATH      ${CMAKE_SOURCE_DIR} )
set( PROJ_OUT_PATH  ${CMAKE_BINARY_DIR} )
set( PROJ_SOURCES   "")
set( PROJ_HEADERS   "")

set( PROJ_INCLUDES  "${PROJ_PATH}")

project(${PROJ_NAME})

# Default compiler args
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|.*Clang)")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++14")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "-g -Os -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
	# /FS - Allows multiple cl.exe processes to write to the same .pdb file
	# /DEBUG - Enable debug during linking
	# /Od - Disables optimization
	set(CMAKE_CXX_FLAGS_DEBUG "/Zi /FS /DEBUG /Od /MT")
	# /Ox - Full optimization
	set(CMAKE_CXX_FLAGS_RELEASE "/Ox -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ox /Zi /FS /DEBUG")
endif()

if (NOT CMAKE_BUILD_TYPE)
    message("-- Defaulting to release build (use -DCMAKE_BUILD_TYPE:STRING=Debug for debug build)")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Adds sources and headers to project
file( GLOB_RECURSE PROJ_SOURCES ./*.cpp ./*.hpp)

file( GLOB_RECURSE PROJ_HEADERS ./*.h ./*.inl)

include_directories(${PROJ_INCLUDES})
# library
add_library(cetus ${PROJ_SOURCES})

set(LIBRARY_OUTPUT_PATH "${PROJ_PATH}/")

