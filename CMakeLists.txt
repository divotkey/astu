cmake_minimum_required(VERSION 3.1)

project(ASTU VERSION 0.11.0)

configure_file(src/AstUtilsConfig.h.in AstUtilsConfig.h)
set(CMAKE_CXX_STANDARD 17)

set(astulib_SOURCES)
list(APPEND astulib_SOURCES

	# Global files
	src/AstUtils0.cpp 
	src/AstUtils1.cpp 
	include/AstIO.h 
	include/Astu.h 
	include/AstuAudio.h 
	include/AstuECS.h 
	include/AstuGraphics.h 
	include/AstuInput.h 
	include/AstuMath.h 
	include/AstuMisc.h 
	include/AstuServices.h 
	include/AstuSuite2D.h 
	include/AstuSuiteJack.h 
	include/AstuSuiteSDL.h 
	include/AstUtils.h 
	include/AstUtils0.h 
	include/AstUtils1.h 

	# Module: Util
	src/Util/Controllable.cpp 
	src/Util/Controller.cpp 
	src/Util/DefaultMessageBox.cpp 
	src/Util/Memento.cpp 
	src/Util/StringUtils.cpp 
	src/Util/VersionInfo.cpp 
	include/Util/CommandQueue.h 
	include/Util/Controllable.h 
	include/Util/Controller.h 
	include/Util/ListenerManager.h 
	include/Util/Memento.h 
	include/Util/MessageBox.h 
	include/Util/Pooled.h 
	include/Util/StringUtils.h 
	include/Util/VersionInfo.h 

	# Module: Math
	src/Math/MathUtils.cpp 
	src/Math/Random.cpp 
	include/Math/Interpolator.h 
	include/Math/Line2.h 
	include/Math/MathUtils.h 
	include/Math/Matrix3.h 
	include/Math/Polygon.h 
	include/Math/Random.h 
	include/Math/Ray2.h 
	include/Math/Segment1.h 
	include/Math/Segment2.h 
	include/Math/Transform2.h 
	include/Math/Vector2.h 
	include/Math/Vector3.h 

	# Module: Graphics
	src/Graphics/AstuGraphics.cpp 
	src/Graphics/BmpCodec.cpp 
	src/Graphics/BmpCodec.h 
	src/Graphics/BoundingBox.cpp 
	src/Graphics/BoundingBox.h 
	src/Graphics/Image.cpp 
	src/Graphics/ImageRenderer.cpp 
	src/Graphics/Palette.cpp 
	src/Graphics/Pattern.cpp 
	src/Graphics/Pattern.h 
	src/Graphics/PatternRenderer.cpp 
	src/Graphics/PatternRenderer.h 
	src/Graphics/Quadtree.cpp 
	src/Graphics/Quadtree.h 
	include/Graphics/Color.h 
	include/Graphics/ColorHsv.h 
	include/Graphics/Image.h 
	include/Graphics/ImageRenderer.h 
	include/Graphics/Palette.h 
	include/Graphics/RalColors.h 
	include/Graphics/RenderQuality.h 
	include/Graphics/VertexBuffer2.h 
	include/Graphics/WebColors.h 

	# Module: Audio
	src/Audio/AstuAudio.cpp 
	src/Audio/AudioBuffer.cpp 
	src/Audio/AudioBuffer.h 
	src/Audio/WaveCodec.cpp 
	src/Audio/WaveCodec.h 

	# Module: ECS
	src/ECS/AutoDestructSystem.cpp 
	src/ECS/EntityFactoryService.cpp 
	src/ECS/EntityService.cpp 
	include/ECS/AutoDestructSystem.h 
	include/ECS/CAutoDestruct.h 
	include/ECS/EntityFactoryService.h 
	include/ECS/EntityService.h 
	include/ECS/EntitySystems.h 

	# Module: Input
	src/Input/AstuIO.cpp 
	src/Input/InputMappingService.cpp 
	src/Input/Keyboard.cpp 
	src/Input/Keys.cpp 
	src/Input/Mouse.cpp 
	include/Input/InputMappingService.h 
	include/Input/InputSignals.h 
	include/Input/Keyboard.h 
	include/Input/Keys.h 
	include/Input/Mouse.h 

	# Module: Service
	src/Service/ActionSignalService.cpp 
	src/Service/InteractiveApplication.cpp 
	src/Service/Service.cpp 
	src/Service/ServiceManager.cpp 
	src/Service/StateService.cpp 
	src/Service/Tasks.cpp 
	src/Service/TaskService.cpp 
	src/Service/UpdateService.cpp 
	include/Service/ActionSignalService.h 
	include/Service/InteractiveApplication.h 
	include/Service/Priority.h 
	include/Service/RenderService.h 
	include/Service/Resolution.h 
	include/Service/Service.h 
	include/Service/ServiceManager.h 
	include/Service/SignalService.h 
	include/Service/StateService.h 
	include/Service/Tasks.h 
	include/Service/TaskService.h 
	include/Service/TimeService.h 
	include/Service/UpdateService.h 
	include/Service/WindowService.h 

	# Module: Suite2D
	src/Suite2D/AutoRotateSystem.cpp 
	src/Suite2D/CameraControlService.cpp 
	src/Suite2D/CameraService.cpp 
	src/Suite2D/CBody.cpp 
	src/Suite2D/CColliders.cpp 
	src/Suite2D/Scene.cpp 
	src/Suite2D/SceneSystem.cpp 
	src/Suite2D/ShapeGenerator.cpp 
	include/Suite2D/AutoRotateSystem.h 
	include/Suite2D/CameraControlService.h 
	include/Suite2D/CameraService.h 
	include/Suite2D/CAutoRotate.h 
	include/Suite2D/CBody.h 
	include/Suite2D/CColliders.h 
	include/Suite2D/CollisionSignal.h 
	include/Suite2D/CPose.h 
	include/Suite2D/CScene.h 
	include/Suite2D/LineRenderer.h 
	include/Suite2D/PhysicsSystem.h 
	include/Suite2D/PolygonVertexBuffer.h 
	include/Suite2D/Scene.h 
	include/Suite2D/SceneSystem.h 
	include/Suite2D/ShapeGenerator.h 

	# Module: Ai
	src/Ai/QTable.cpp 
	src/Ai/Quantizer.cpp 
	src/Ai/StateUtil.cpp 
	include/Ai/QTable.h 
	include/Ai/Quantizer.h 
	include/Ai/StateUtil.h 

	# Module: Velox
	src/Velox/FileCharStream.cpp 
	src/Velox/FileCharStream.h 
	src/Velox/IAttachable.h 
	src/Velox/ICharStream.h 
	src/Velox/IVeloxImporter.h 
	src/Velox/StandardVeloxHooks.cpp 
	src/Velox/StandardVeloxHooks.h 
	src/Velox/StringCharStream.cpp 
	src/Velox/VeloxAst.cpp 
	src/Velox/VeloxAst.h 
	src/Velox/VeloxDragAndDropService.cpp 
	src/Velox/VeloxHook.cpp 
	src/Velox/VeloxHook.h 
	src/Velox/VeloxInterpreter.cpp 
	src/Velox/VeloxInterpreter.h 
	src/Velox/VeloxInterpreterException.h 
	src/Velox/VeloxItem.cpp 
	src/Velox/VeloxItem.h 
	src/Velox/VeloxParser.cpp 
	src/Velox/VeloxParser.h 
	src/Velox/VeloxScanner.cpp 
	src/Velox/VeloxScanner.h 
	src/Velox/VeloxScope.cpp 
	src/Velox/VeloxScope.h 
	src/Velox/VeloxScript.cpp 
	src/Velox/VeloxScript.h 
	src/Velox/VeloxService.cpp 
	src/Velox/VeloxTypes.h 
	include/Velox/FileCharStream.h 
	include/Velox/ICharStream.h 
	include/Velox/StringCharStream.h 
	include/Velox/VeloxDragAndDropService.h 
	include/Velox/VeloxService.h 
	)
    
	#Uncomment for debuggin purpose.
	#message(STATUS "core files: " "${astulib_SOURCES}")

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