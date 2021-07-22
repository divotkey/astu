cmake_minimum_required(VERSION 3.1)

project(ASTU VERSION 0.10.0)

configure_file(src/AstUtilsConfig.h.in AstUtilsConfig.h)
set(CMAKE_CXX_STANDARD 17)

set(astulib_SOURCES )
list(APPEND astulib_SOURCES
                    src/AstUtils0.cpp
                    src/misc/VersionInfo.cpp
                    src/math/Random.cpp
                    src/math/MathUtils.cpp

                    src/gfx/AstGraphics.cpp
                    src/gfx/Turtle.cpp

                    src/Graphics/Palette.cpp
                    src/Graphics/Image.cpp 
                    src/Graphics/BmpCodec.cpp
                    src/Graphics/Pattern.cpp
                    src/Graphics/BoundingBox.cpp
                    src/Graphics/Quadtree.cpp
                    src/Graphics/PatternRenderer.cpp
                    src/Graphics/ImageRenderer.cpp

                    src/io/AstIO.cpp

                    src/audio/AudioBuffer.cpp
                    src/audio/WaveCodec.cpp
                    src/audio/AstAudio.cpp

                    src/srv/EntityService.cpp
                    src/srv/EntityFactoryService.cpp
                    src/srv/IteratingEntitySystem.cpp
                    src/srv/InputMappingService.cpp
                    src/srv/Controllable.cpp
                    src/srv/Controller.cpp

                    src/Input/Keys.cpp
                    src/Input/Mouse.cpp
                    src/Input/Keyboard.cpp

                    src/Service/ServiceManager.cpp
                    src/Service/Service.cpp
                    src/Service/UpdateService.cpp
                    src/Service/ActionSignalService.cpp
                    src/Service/StateService.cpp
                    src/Service/TaskService.cpp
                    src/Service/Tasks.cpp

                    src/Suite2D/Scene.cpp
                    src/Suite2D/CameraService.cpp
                    src/Suite2D/CameraControlService.cpp
                    src/Suite2D/SceneSystem.cpp
                    src/Suite2D/AutoRotateSystem.cpp
                    src/Suite2D/AutoRotateSystem.cpp
                    src/Suite2D/ShapeGenerator.cpp

                    src/AstUtils1.cpp)

set(WITH_JACK false CACHE BOOL "Use JACK2")
if (WITH_JACK)
    list(APPEND astulib_SOURCES 
        src/audio/JackErrorUtil.cpp
        src/audio/JackClient.cpp
        )
endif(WITH_JACK) 


#OPTION(USE_SDL "Build the project with support for SDL2" OFF)
set(WITH_SDL false CACHE BOOL "Use SDL")

if(WITH_SDL)
    list(APPEND astulib_SOURCES 
        src/SDL/SdlApplication.cpp
        src/SDL/Application.cpp
        src/SDL/ApplicationImpl.cpp
        
        src/SuiteSDL/SdlService.cpp
        src/SuiteSDL/SdlEventService.cpp
        src/SuiteSDL/SdlKeyTable.cpp
        src/SuiteSDL/SdlVideoService.cpp
        src/SuiteSDL/SdlRenderService.cpp
        src/SuiteSDL/SdlJoystickService.cpp
        src/SuiteSDL/SdlAudioService.cpp
        src/SuiteSDL/SdlTimeService.cpp
        src/SuiteSDL/SdlLineRenderer.cpp
        src/SuiteSDL/SdlSceneGraph2D.cpp
        src/SuiteSDL/SdlSceneRenderer2D.cpp
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