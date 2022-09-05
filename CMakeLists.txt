cmake_minimum_required(VERSION 3.24)
project(gravity)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

add_executable(gravity main.cpp)
# add_executable(gravity WIN32 main.cpp)


set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
add_subdirectory("deps/SFML")
target_link_libraries(gravity sfml-system sfml-window sfml-graphics)

# if(${WIN32})
#     target_link_libraries(gravity sfml-main)
# endif()
cm