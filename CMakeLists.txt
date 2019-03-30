include(ExternalProject)

cmake_minimum_required (VERSION 3.9.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()


# Detect 64bit Build
# See: https://stackoverflow.com/questions/39258250/how-to-detect-if-64-bit-msvc-with-cmake
set(USE_64 ON)
if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)" 
	AND "${CMAKE_SIZEOF_VOID_P}" STREQUAL "4" 
	AND NOT CMAKE_CL_64)
    set(USE_64 OFF)
endif()


# Detect Multi Config Generator 
# See: https://gitlab.kitware.com/cmake/cmake/issues/17645
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (isMultiConfig)
    set(propagate_build_type --config $<CONFIG> -DCMAKE_BUILD_TYPE=$<CONFIG>)
else()
    set(propagate_build_type -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
endif()

# List Subdirs
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()


# Config
project (CH_NOISE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(INSTALL_DIRECTORY ${CMAKE_BINARY_DIR}/install)

# See: http://www.brianlheim.com/2018/04/09/cmake-cheat-sheet.html
set(CMAKE_CXX_STANDARD 17)

# External Libraries
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

# Work Offline
option(BUILD_OFFLINE "Build offline" OFF)

if (${BUILD_OFFLINE})
    set_property(DIRECTORY ${CMAKE_BINARY_DIR}
                 PROPERTY EP_UPDATE_DISCONNECTED 1)
    message("Working offline, not pulling external projects")
endif()

# IMPORTANT: VS doesn't care about this
# See: https://stackoverflow.com/questions/19024259/how-to-change-the-build-type-to-release-mode-in-cmake?rq=1
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID} CONFIG=${CONFIG}")

# C and C++ files

file(GLOB_RECURSE CH_NOISE_SRC
    "src/*.c"
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
)

include_directories(include/)
add_executable(CH_NOISE ${CH_NOISE_SRC})

set_target_properties(CH_NOISE PROPERTIES
      ENABLE_EXPORTS 1)

link_directories($<TARGET_LINKER_FILE_DIR:CH_NOISE>)

install(TARGETS CH_NOISE
            RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin
            LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib
            ARCHIVE DESTINATION ${INSTALL_DIRECTORY}/lib/static)

