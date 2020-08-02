cmake_minimum_required(VERSION 3.1)

project(ASTU VERSION 1.0.0)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CONFIG_STRING "DEBUG")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CONFIG_STRING "RELEASE")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CONFIG_STRING "SIZE")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CONFIG_STRING "RELEASE WITH DEBUG-INFO")
else()
    set(CONFIG_STRING "UNKNOWN CONFIGURATION")
endif ()

configure_file(src/AstUtilsConfig.h.in AstUtilsConfig.h)
set(CMAKE_CXX_STANDARD 17)

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

target_include_directories(
                        astu PRIVATE "${PROJECT_BINARY_DIR}"
                        astu PRIVATE "${PROJECT_SOURCE_DIR}/src"
                        astu PUBLIC ${PROJECT_SOURCE_DIR}/include
                        )

