cmake_minimum_required(VERSION 3.1)

project(ASTU VERSION 0.11.0)

configure_file(src/AstUtilsConfig.h.in AstUtilsConfig.h)
set(CMAKE_CXX_STANDARD 17)

set(astulib_SOURCES )
list(APPEND astulib_SOURCES
                    src/AstUtils0.cpp
                    src/Util/VersionInfo.cpp
                    src/Util/Controllable.cpp
                    src/Util/Controller.cpp
                    src/Util/Memento.cpp
                    src/Util/StringUtils.cpp
                    src/Math/Random.cpp
                    src/Math/MathUtils.cpp

                    src/Graphics/AstuGraphics.cpp

                    src/Graphics/Palette.cpp
                    src/Graphics/Image.cpp 
                    src/Graphics/BmpCodec.cpp
                    src/Graphics/Pattern.cpp
                    src/Graphics/BoundingBox.cpp
                    src/Graphics/Quadtree.cpp
                    src/Graphics/PatternRenderer.cpp
                    src/Graphics/ImageRenderer.cpp

                    src/Audio/AudioBuffer.cpp
                    src/Audio/WaveCodec.cpp
                    src/Audio/AstuAudio.cpp

                    src/ECS/EntityService.cpp
                    src/ECS/EntityFactoryService.cpp
                    src/ECS/AutoDestructSystem.cpp
                    src/Input/InputMappingService.cpp

                    src/Input/AstuIO.cpp
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
                    src/Service/InteractiveApplication.cpp

                    src/Suite2D/Scene.cpp
                    src/Suite2D/CameraService.cpp
                    src/Suite2D/CameraControlService.cpp
                    src/Suite2D/SceneSystem.cpp
                    src/Suite2D/AutoRotateSystem.cpp
                    src/Suite2D/AutoRotateSystem.cpp
                    src/Suite2D/ShapeGenerator.cpp
                    src/Suite2D/CBody.cpp
                    src/Suite2D/CColliders.cpp

                    src/Ai/Quantizer.cpp
                    src/Ai/StateUtil.cpp
                    src/Ai/QTable.cpp

                    src/Velox/VeloxScanner.cpp
                    src/Velox/VeloxAst.cpp
                    src/Velox/VeloxItem.cpp
                    src/Velox/VeloxScope.cpp
                    src/Velox/VeloxParser.cpp
                    src/Velox/VeloxInterpreter.cpp
                    src/Velox/StringCharStream.cpp
                    src/Velox/VeloxHook.cpp
                    src/Velox/VeloxScript.cpp
                    src/Velox/VeloxService.cpp
                    src/Velox/StandardVeloxHooks.cpp
                    src/Velox/VeloxDragAndDropService.cpp
                    src/Velox/FileCharStream.cpp

                    src/AstUtils1.cpp)

OPTION(ASTU_USE_JACK "Build the project with support for JACK Audio Connection Kit" OFF)
if (ASTU_USE_JACK)
    list(APPEND astulib_SOURCES 
        src/SuiteJack/JackErrorUtil.cpp
        src/SuiteJack/JackClient.cpp
        )
endif(ASTU_USE_JACK) 

OPTION(ASTU_USE_SDL "Build the project with support for SDL2" OFF)

if(ASTU_USE_SDL)

    list(APPEND astulib_SOURCES 
        src/SuiteSDL/SdlApplication0.cpp
        src/SuiteSDL/Application.cpp
        src/SuiteSDL/ApplicationImpl.cpp
        
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
        src/SuiteSDL/SdlRecordingSceneRenderer2D.cpp
        src/SuiteSDL/SdlApplication.cpp
        src/SuiteSDL/SdlMessageBox.cpp
        )
else(ASTU_USE_SDL)
    list(APPEND astulib_SOURCES src/SuiteSDL/SdlApplicationDummy.cpp src/Util/DefaultMessageBox.cpp)
endif(ASTU_USE_SDL)                 

add_library(astu ${astulib_SOURCES})

set(astulib_INCLUDES )
list(APPEND astulib_INCLUDES
        astu PRIVATE "${PROJECT_BINARY_DIR}" 
        astu PRIVATE "${PROJECT_SOURCE_DIR}/src"
        astu PUBLIC ${PROJECT_SOURCE_DIR}/include)
      
if(ASTU_USE_SDL)
    IF (WIN32)
        target_link_libraries(astu $ENV{SDL2_HOME}/lib/x64/SDL2.lib)
        list(APPEND astulib_INCLUDES astu PRIVATE $ENV{SDL2_HOME})
    ELSEIF(APPLE)
        #list(APPEND astulib_INCLUDES astu PRIVATE /Library/Frameworks/SDL2.framework/Headers)
        target_link_libraries(astu /Library/Frameworks/SDL2.framework/Versions/A/SDL2)
    ELSE()
        target_link_libraries(astu SDL2)
    ENDIF()
endif(ASTU_USE_SDL)                 

if(ASTU_USE_JACK)
    IF (WIN32)
        target_include_directories(astu PRIVATE $ENV{JACK2_HOME}/include)
        target_link_libraries(astu $ENV{JACK2_HOME}/lib/libjack64.lib)
    ELSE()
        target_link_libraries(astu jack)    
    ENDIF()
endif(ASTU_USE_JACK)                 

target_include_directories(${astulib_INCLUDES})