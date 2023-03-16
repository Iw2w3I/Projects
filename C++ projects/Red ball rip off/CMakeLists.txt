cmake_minimum_required(VERSION 3.0)
project(SFML3)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

add_executable(SFML3 main.cpp)

target_link_libraries(SFML3 sfml-graphics sfml-audio sfml-window sfml-system)
