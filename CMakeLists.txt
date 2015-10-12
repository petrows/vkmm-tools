cmake_minimum_required (VERSION 2.8)

set (PROJECT "vkmm-tools")
project (${PROJECT})

cmake_policy(SET CMP0043 OLD)
cmake_policy(SET CMP0020 NEW)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (BUILD_SYSTEM_ID "")
# Detect & check OS
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set (BUILD_LINUX 1)
	set (BUILD_SYSTEM_ID "linux")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set (BUILD_WIN32 1)
	set (BUILD_SYSTEM_ID "win")
else()
	message(FATAL_ERROR "Unknown build target platform!")
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11")

if (BUILD_LINUX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

set(BUILD_SHARED_LIBS OFF)
set(ENABLE_STATIC ON)

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Network)

include_directories("vkmm-api/src/")
include_directories("qjson/include/")

include_directories("taglib/taglib/")
include_directories("taglib/taglib/toolkit")
include_directories("${CMAKE_BINARY_DIR}/taglib")

add_subdirectory(qjson)
add_subdirectory(taglib)
add_subdirectory(vkmm-api)

# Add all apps to build
file(GLOB APPS_LIST "apps/vkmm-tools-*")

foreach(APP_TOOL ${APPS_LIST})
	get_filename_component(APP_NAME "${APP_TOOL}" NAME)
	add_subdirectory("${APP_TOOL}")
endforeach()
