cmake_minimum_required (VERSION 2.8)

set (PROJECT "vkmm-tools")
project (${PROJECT})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wall")

set(BUILD_SHARED_LIBS OFF)

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Network)

include_directories("vkmm-api/src/")
include_directories("qjson/include/")

add_subdirectory(qjson)
add_subdirectory(vkmm-api)

# Add all apps to build
file(GLOB APPS_LIST "apps/vkmm-tools-*")

foreach(APP_TOOL ${APPS_LIST})
	add_subdirectory("${APP_TOOL}")
endforeach()
