cmake_minimum_required(VERSION 3.1)

# set the project name
project(GameBoilerplate)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

# add executable
file(GLOB SOURCES "*.cpp")
add_executable(GameBoilerplate ${SOURCES})

# specify required libraries
target_link_libraries(GameBoilerplate astu astu_box2d)
