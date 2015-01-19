cmake_minimum_required (VERSION 2.8)

set (PROJECT "vkmmapi")
project (${PROJECT})

file(GLOB_RECURSE API_SRC "src/*.cpp")
file(GLOB_RECURSE API_H "src/*.h")

# Tell CMake to create the helloworld executable
add_library(${PROJECT} STATIC ${API_H} ${API_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT} qjson Qt5::Core Qt5::Network)

qt5_use_modules(${PROJECT} Core)
qt5_use_modules(${PROJECT} Network)
