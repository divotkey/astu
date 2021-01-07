cmake_minimum_required(VERSION 3.1)

project(ASTU VERSION 0.5.3)

configure_file(src/AstUtilsConfig.h.in AstUtilsConfig.h)
set(CMAKE_CXX_STANDARD 17)

set(astulib_SOURCES )
list(APPEND astulib_SOURCES
                    src/AstUtils.cpp 
                    src/AstUtils0.cpp
                    src/misc/VersionInfo.cpp
                    src/math/Random.cpp

                    src/gfx/AstGraphics.cpp

                    src/gfx/Color.cpp 
                    src/gfx/Palette.cpp
                    src/gfx/Image.cpp 
                    src/gfx/BmpCodec.cpp
                    src/gfx/Pattern.cpp
                    src/gfx/BoundingBox.cpp
                    src/gfx/Quadtree.cpp
                    src/gfx/PatternRenderer.cpp
                    src/gfx/Turtle.cpp

                    src/io/AstIO.cpp

                    src/audio/AudioBuffer.cpp
                    src/audio/WaveCodec.cpp
                    src/audio/AstAudio.cpp

                    src/srv/ServiceManager.cpp
                    src/srv/Service.cpp
                    src/srv/UpdateService.cpp
                    src/srv/StateService.cpp
                    src/srv/EntityService.cpp
                    src/srv/IteratingEntitySystem.cpp
                    src/srv/Mouse.cpp
                    src/srv/Keyboard.cpp

                    src/AstUtils1.cpp)

set(WITH_JACK false CACHE BOOL "Use JACK2")
if (WITH_JACK)
    list(APPEND astulib_SOURCES 
        src/audio/JackErrorUtil.cpp
        src/audio/JackClient.cpp
        )
endif(WITH_JACK) 

set(WITH_SDL false CACHE BOOL "Use SDL")

if(WITH_SDL)
    list(APPEND astulib_SOURCES 
        src/sdl/SdlApplication.cpp
        src/sdl/Application.cpp
        src/sdl/ApplicationImpl.cpp
        
        src/sdl/SdlService.cpp
        src/sdl/SdlEventService.cpp
        src/sdl/SdlVideoService.cpp
        src/sdl/SdlRenderService.cpp
        src/sdl/SdlAudioService.cpp
        src/sdl/SdlTimeService.cpp
        )
else(WITH_SDL)
    list(APPEND astulib_SOURCES src/sdl/SdlApplicationDummy.cpp)
endif(WITH_SDL)                 

add_library(astu ${astulib_SOURCES})

set(astulib_INCLUDES )
list(APPEND astulib_INCLUDES
        astu PRIVATE "${PROJECT_BINARY_DIR}" 
        astu PRIVATE "${PROJECT_SOURCE_DIR}/src"
        astu PUBLIC ${PROJECT_SOURCE_DIR}/include)
      
if(WITH_SDL)
    IF (WIN32)
        target_link_libraries(astu $ENV{SDL2_HOME}/lib/x64/SDL2.lib)
        list(APPEND astulib_INCLUDES astu PRIVATE $ENV{SDL2_HOME})
    ELSEIF(APPLE)
        #list(APPEND astulib_INCLUDES astu PRIVATE /Library/Frameworks/SDL2.framework/Headers)
        target_link_libraries(astu /Library/Frameworks/SDL2.framework/Versions/A/SDL2)
    ELSE()
        target_link_libraries(astu SDL2)
    ENDIF()
endif(WITH_SDL)                 

if(WITH_JACK)
    IF (WIN32)
        target_include_directories(astu PRIVATE $ENV{JACK2_HOME}/include)
        target_link_libraries(astu $ENV{JACK2_HOME}/lib/libjack64.lib)
    ELSE()
        target_link_libraries(astu jack)    
    ENDIF()
endif(WITH_JACK)                 

target_include_directories(${astulib_INCLUDES})