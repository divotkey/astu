cmake_minimum_required(VERSION 3.1)

project(astu)
set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

add_library(astu 
    src/AstUtils.cpp 
    src/math/Random.cpp
    src/gfx/Color.cpp 
    src/gfx/Image.cpp 
    src/gfx/BmpCodec.cpp
    src/gfx/Pattern.cpp
    src/gfx/BoundingBox.cpp
    src/gfx/Quadtree.cpp
    src/gfx/PatternRenderer.cpp
    src/gfx/Turtle.cpp
    )
