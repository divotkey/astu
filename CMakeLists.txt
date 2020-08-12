cmake_minimum_required(VERSION 3.1)

project(ASTU VERSION 1.0.0)

configure_file(src/AstUtilsConfig.h.in AstUtilsConfig.h)
set(CMAKE_CXX_STANDARD 17)

add_library(astu    src/AstUtils.cpp 
                    src/math/Random.cpp
                    src/gfx/Color.cpp 
                    src/gfx/Image.cpp 
                    src/gfx/BmpCodec.cpp
                    src/gfx/Pattern.cpp
                    src/gfx/BoundingBox.cpp
                    src/gfx/Quadtree.cpp
                    src/gfx/PatternRenderer.cpp
                    src/gfx/Turtle.cpp
                    src/AstUtils1.cpp)

target_include_directories(
                        astu PRIVATE "${PROJECT_BINARY_DIR}"
                        astu PRIVATE "${PROJECT_SOURCE_DIR}/src"
                        astu PUBLIC ${PROJECT_SOURCE_DIR}/include
                        )

