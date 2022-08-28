# CMake file for AST-Utilities Version 0.11.0
# Generated Sun Aug 28 09:51:24 CEST 2022 with SWORD Interpreter 1.9.1

# Define required CMake version
cmake_minimum_required(VERSION 3.1)

# Add version information
project(ASTU VERSION 0.11.0)

# Define C++ Standard
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# using GCC
	# Disable warning regarding Application Binary Interfaces (API) changes.
	set(CMAKE_CXX_FLAGS "-Wno-psabi")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
	# using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# using Visual Studio C++
endif()

#################################################
####### ASTU Core Files
#################################################

set(astu_SOURCES)
list(APPEND astu_SOURCES

	# Global files
	src/AstUtils1.cpp 
	src/AstUtils0.cpp 
	include/AstuApi.h 
	include/AstIO.h 
	include/AstUtils1.h 
	include/AstuGraphics.h 
	include/AstuMath.h 
	include/AstuInput.h 
	include/AstuECS.h 
	include/AstuNetwork.h 
	include/AstuMisc.h 
	include/AstuSuite2D.h 
	include/AstuAudio.h 
	include/AstUtils0.h 
	include/Astu.h 
	include/AstUtils.h 
	include/AstuSuiteJack.h 
	include/AstuSuiteSDL.h 
	include/AstuServices.h 

	# Module: Util
	src/Util/StateMachine.cpp 
	src/Util/Timer.cpp 
	src/Util/SpinLockSleep.h 
	src/Util/Controller.cpp 
	src/Util/Memento.cpp 
	src/Util/StdSleep.h 
	src/Util/SpinLockSleep.cpp 
	src/Util/Controllable.cpp 
	src/Util/MessageBox.cpp 
	src/Util/StringUtils.cpp 
	src/Util/VersionInfo.cpp 
	src/Util/StdSleep.cpp 
	include/Util/ListenerManager.h 
	include/Util/Controllable.h 
	include/Util/CommandQueue.h 
	include/Util/StringUtils.h 
	include/Util/VersionInfo.h 
	include/Util/Buffer.h 
	include/Util/Buffer.cpp 
	include/Util/Controller.h 
	include/Util/Timer.h 
	include/Util/Pooled.h 
	include/Util/StateMachine.h 
	include/Util/IThreadSleep.h 
	include/Util/Memento.h 
	include/Util/MessageBox.h 

	# Module: Math
	src/Math/MathUtils.cpp 
	src/Math/Random.cpp 
	include/Math/Vector2.h 
	include/Math/Matrix4.h 
	include/Math/Quaternion.h 
	include/Math/Ray3.h 
	include/Math/Matrix3.h 
	include/Math/Polygon.h 
	include/Math/Vector3.h 
	include/Math/Segment2.h 
	include/Math/MathUtils.h 
	include/Math/Random.h 
	include/Math/Segment1.h 
	include/Math/SphericalCoordinates.h 
	include/Math/Ray2.h 
	include/Math/Interpolator.h 
	include/Math/Line2.h 
	include/Math/Transform2.h 

	# Module: Graphics
	src/Graphics/Pattern.cpp 
	src/Graphics/Image.cpp 
	src/Graphics/Quadtree.cpp 
	src/Graphics/AstuGraphics.cpp 
	src/Graphics/HdrCodec.h 
	src/Graphics/BmpCodec.cpp 
	src/Graphics/Palette.cpp 
	src/Graphics/PatternRenderer.h 
	src/Graphics/BoundingBox.cpp 
	src/Graphics/HdrCodec.cpp 
	src/Graphics/ImageRenderer.cpp 
	src/Graphics/BmpCodec.h 
	src/Graphics/PatternRenderer.cpp 
	include/Graphics/Color.h 
	include/Graphics/RenderQuality.h 
	include/Graphics/Image.h 
	include/Graphics/WebColors.h 
	include/Graphics/VertexBuffer2.h 
	include/Graphics/BoundingBox.h 
	include/Graphics/Palette.h 
	include/Graphics/RalColors.h 
	include/Graphics/Quadtree.h 
	include/Graphics/Pattern.h 
	include/Graphics/ColorHsv.h 
	include/Graphics/ImageRenderer.h 

	# Module: Audio
	src/Audio/WaveCodec.h 
	src/Audio/WaveCodec.cpp 
	src/Audio/AstuAudio.cpp 
	src/Audio/AudioBuffer.cpp 
	src/Audio/AudioBuffer.h 

	# Module: ECS
	src/ECS/EntityFactoryService.cpp 
	src/ECS/EntityService.cpp 
	src/ECS/AutoDestructSystem.cpp 
	include/ECS/CAutoDestruct.h 
	include/ECS/EntityFactoryService.h 
	include/ECS/AutoDestructSystem.h 
	include/ECS/EntitySystems.h 
	include/ECS/EntityService.h 

	# Module: Input
	src/Input/Keys.cpp 
	src/Input/InputMappingService.cpp 
	src/Input/Keyboard.cpp 
	src/Input/AstuIO.cpp 
	src/Input/Mouse.cpp 
	include/Input/Keys.h 
	include/Input/InputMappingService.h 
	include/Input/InputSignals.h 
	include/Input/Keyboard.h 
	include/Input/Mouse.h 

	# Module: Network
	src/Network/Peer.cpp 
	src/Network/Network.cpp 
	include/Network/Socket.h 
	include/Network/Peer.h 
	include/Network/IpMode.h 
	include/Network/IInetSocketAddress.h 
	include/Network/Network.h 

	# Module: Service
	src/Service/UpdateService.cpp 
	src/Service/ConsoleApplication.cpp 
	src/Service/Service.cpp 
	src/Service/StateService.cpp 
	src/Service/Tasks.cpp 
	src/Service/TaskService.cpp 
	src/Service/ConsoleTimeService.cpp 
	src/Service/InteractiveApplication.cpp 
	src/Service/ActionSignalService.cpp 
	src/Service/ServiceManager.cpp 
	include/Service/TaskService.h 
	include/Service/Service.h 
	include/Service/ConsoleTimeService.h 
	include/Service/Resolution.h 
	include/Service/WindowService.h 
	include/Service/ConsoleApplication.h 
	include/Service/ActionSignalService.h 
	include/Service/UpdateService.h 
	include/Service/TimeService.h 
	include/Service/Tasks.h 
	include/Service/InteractiveApplication.h 
	include/Service/StateService.h 
	include/Service/ServiceManager.h 
	include/Service/SignalService.h 
	include/Service/RenderService.h 
	include/Service/Priority.h 

	# Module: Suite2D
	src/Suite2D/CameraControlService.cpp 
	src/Suite2D/Scene.cpp 
	src/Suite2D/CColliders.cpp 
	src/Suite2D/ShapeGenerator.cpp 
	src/Suite2D/CameraService.cpp 
	src/Suite2D/CBody.cpp 
	src/Suite2D/SceneSystem.cpp 
	src/Suite2D/AutoRotateSystem.cpp 
	include/Suite2D/PhysicsSystem.h 
	include/Suite2D/LineRenderer.h 
	include/Suite2D/Scene.h 
	include/Suite2D/CollisionSignal.h 
	include/Suite2D/CameraControlService.h 
	include/Suite2D/SceneSystem.h 
	include/Suite2D/CScene.h 
	include/Suite2D/PolygonVertexBuffer.h 
	include/Suite2D/CameraService.h 
	include/Suite2D/AutoRotateSystem.h 
	include/Suite2D/CColliders.h 
	include/Suite2D/CAutoRotate.h 
	include/Suite2D/ShapeGenerator.h 
	include/Suite2D/CBody.h 
	include/Suite2D/CPose.h 

	# Module: Ai
	src/Ai/StateUtil.cpp 
	src/Ai/Quantizer.cpp 
	src/Ai/QTable.cpp 
	include/Ai/Quantizer.h 
	include/Ai/StateUtil.h 
	include/Ai/QTable.h 

	# Module: FSM
	src/FSM/PowersetBuilder.cpp 
	src/FSM/FStateMachine.cpp 
	src/FSM/DStateMachine.cpp 
	src/FSM/NStateMachine.cpp 
	src/FSM/DStateMachineBuilder.cpp 
	include/FSM/FStateMachine.h 
	include/FSM/NStateMachine.h 
	include/FSM/DStateMachineBuilder.h 
	include/FSM/PowersetBuilder.h 
	include/FSM/DStateMachine.h 

	# Module: Script
	src/Script/Scanner.cpp 
	src/Script/ScannerBuilder.cpp 
	include/Script/ScannerBuilder.h 
	include/Script/Scanner.h 
	include/Script/ScannerError.h 

	# Module: Velox
	include/Velox/Parser/VeloxScanner.h 
	include/Velox/Parser/Source.h 
	include/Velox/Parser/SourceString.h 
	include/Velox/Parser/SourceFile.h 
	include/Velox/Parser/Parser.cpp 
	include/Velox/Parser/Source.cpp 
	include/Velox/Parser/SourceString.cpp 
	include/Velox/Parser/ParserError.h 
	include/Velox/Parser/VeloxScanner.cpp 
	include/Velox/Parser/Token.cpp 
	include/Velox/Parser/Token.h 
	include/Velox/Parser/TokenType.h 
	include/Velox/Parser/SourceFile.cpp 
	include/Velox/Parser/Parser.h 
	include/Velox/Interpreter/ItemStateReference.cpp 
	include/Velox/Interpreter/InterpreterStatementGlobal.h 
	include/Velox/Interpreter/InterpreterFunctionOneParameter.cpp 
	include/Velox/Interpreter/InterpreterStatement.h 
	include/Velox/Interpreter/ItemStateReference.h 
	include/Velox/Interpreter/ItemStateString.h 
	include/Velox/Interpreter/ItemStateVector2.h 
	include/Velox/Interpreter/ItemStateColor.h 
	include/Velox/Interpreter/Item.cpp 
	include/Velox/Interpreter/ItemStateRealMember.h 
	include/Velox/Interpreter/InterpreterExpression.cpp 
	include/Velox/Interpreter/InterpreterFunctionPrint.cpp 
	include/Velox/Interpreter/InterpreterLiteralString.h 
	include/Velox/Interpreter/InterpreterLiteralReal.cpp 
	include/Velox/Interpreter/InterpreterLiteralList.cpp 
	include/Velox/Interpreter/ItemStateUndefined.h 
	include/Velox/Interpreter/InterpreterStatementBreak.cpp 
	include/Velox/Interpreter/ItemStateBool.cpp 
	include/Velox/Interpreter/Operators.h 
	include/Velox/Interpreter/InterpreterExpressionSimpleName.h 
	include/Velox/Interpreter/InterpreterInstantRealization.h 
	include/Velox/Interpreter/ItemStateReal.cpp 
	include/Velox/Interpreter/SimpleMemoryManager.h 
	include/Velox/Interpreter/InterpreterExpressionUndefined.h 
	include/Velox/Interpreter/InterpreterFunctionScript.cpp 
	include/Velox/Interpreter/InterpreterColor.h 
	include/Velox/Interpreter/MemoryManager.h 
	include/Velox/Interpreter/InterpreterStatementFor.h 
	include/Velox/Interpreter/InterpreterLiteralInteger.h 
	include/Velox/Interpreter/InterpreterExpressionAssignment.cpp 
	include/Velox/Interpreter/InterpreterLogicalOperation.cpp 
	include/Velox/Interpreter/InterpreterError.h 
	include/Velox/Interpreter/InterpreterStatementBreak.h 
	include/Velox/Interpreter/Interpreter.h 
	include/Velox/Interpreter/InterpreterFunctionScript.h 
	include/Velox/Interpreter/ScriptContext.h 
	include/Velox/Interpreter/InterpreterExpressionUnaryMinus.cpp 
	include/Velox/Interpreter/ItemStateString.cpp 
	include/Velox/Interpreter/InterpreterStatementDoWhile.h 
	include/Velox/Interpreter/InterpreterFunctionTwoParameter.cpp 
	include/Velox/Interpreter/InterpreterLiteralInteger.cpp 
	include/Velox/Interpreter/InterpreterRelationalOperation.cpp 
	include/Velox/Interpreter/InterpreterPreIncrement.h 
	include/Velox/Interpreter/InterpreterConstructorCall.h 
	include/Velox/Interpreter/InterpreterFunctionThreeParameter.cpp 
	include/Velox/Interpreter/ItemRelationalOperator.cpp 
	include/Velox/Interpreter/InterpreterStatementIf.cpp 
	include/Velox/Interpreter/InterpreterExpressionUndefined.cpp 
	include/Velox/Interpreter/ItemType.h 
	include/Velox/Interpreter/ItemStateBool.h 
	include/Velox/Interpreter/InterpreterStatementNop.h 
	include/Velox/Interpreter/InterpreterExpressionMemberAccess.cpp 
	include/Velox/Interpreter/InterpreterLiteralList.h 
	include/Velox/Interpreter/Scope.h 
	include/Velox/Interpreter/InterpreterStatementNop.cpp 
	include/Velox/Interpreter/InterpreterReturnStatement.h 
	include/Velox/Interpreter/InterpreterStatementBlock.cpp 
	include/Velox/Interpreter/MemoryManager.cpp 
	include/Velox/Interpreter/InterpreterLiteralBoolean.h 
	include/Velox/Interpreter/InterpreterLogicalOperation.h 
	include/Velox/Interpreter/InterpreterAssignmentOperator.h 
	include/Velox/Interpreter/ItemArithmeticOperator.cpp 
	include/Velox/Interpreter/InterpreterStatementFor.cpp 
	include/Velox/Interpreter/ItemStateReal.h 
	include/Velox/Interpreter/ItemStateInteger.h 
	include/Velox/Interpreter/InterpreterExpressionListAccess.cpp 
	include/Velox/Interpreter/InterpreterExpressionNew.h 
	include/Velox/Interpreter/InterpreterFunctionDefinition.cpp 
	include/Velox/Interpreter/InterpreterPreIncrement.cpp 
	include/Velox/Interpreter/InterpreterFunction.cpp 
	include/Velox/Interpreter/InterpreterClassDefinition.h 
	include/Velox/Interpreter/ItemState.cpp 
	include/Velox/Interpreter/ItemStateFunction.cpp 
	include/Velox/Interpreter/InterpreterActualParameterList.h 
	include/Velox/Interpreter/InterpreterFormalParameterList.cpp 
	include/Velox/Interpreter/InterpreterFunctionDefinition.h 
	include/Velox/Interpreter/InterpreterFunctionThreeParameter.h 
	include/Velox/Interpreter/ItemStateUndefined.cpp 
	include/Velox/Interpreter/ItemStateObject.cpp 
	include/Velox/Interpreter/ItemStateVector2.cpp 
	include/Velox/Interpreter/InterpreterInstantDefinition.cpp 
	include/Velox/Interpreter/IMemoryManager.h 
	include/Velox/Interpreter/InterpreterFunctionNoParameter.h 
	include/Velox/Interpreter/InterpreterRelationalOperation.h 
	include/Velox/Interpreter/InterpreterStatementIf.h 
	include/Velox/Interpreter/InterpreterClass.h 
	include/Velox/Interpreter/InterpreterReturnStatement.cpp 
	include/Velox/Interpreter/Item.h 
	include/Velox/Interpreter/ItemStateList.cpp 
	include/Velox/Interpreter/InterpreterExpressionAssignment.h 
	include/Velox/Interpreter/InterpreterLiteralColor.cpp 
	include/Velox/Interpreter/InterpreterFunction.h 
	include/Velox/Interpreter/ObjectType.h 
	include/Velox/Interpreter/InterpreterArithmeticOperation.cpp 
	include/Velox/Interpreter/InterpreterAssignmentOperator.cpp 
	include/Velox/Interpreter/InterpreterPostIncrement.h 
	include/Velox/Interpreter/InterpreterStatementBlock.h 
	include/Velox/Interpreter/InterpreterLiteralBoolean.cpp 
	include/Velox/Interpreter/InterpreterStatementLoop.h 
	include/Velox/Interpreter/InterpreterFunctionPrint.h 
	include/Velox/Interpreter/ItemStateWithSubItems.cpp 
	include/Velox/Interpreter/InterpreterStatementContinue.h 
	include/Velox/Interpreter/InterpreterPostIncrement.cpp 
	include/Velox/Interpreter/InterpreterFormalParameterList.h 
	include/Velox/Interpreter/InterpreterFunctionOneParameter.h 
	include/Velox/Interpreter/InterpreterVector.h 
	include/Velox/Interpreter/InterpreterStatementGlobal.cpp 
	include/Velox/Interpreter/InterpreterLiteralReal.h 
	include/Velox/Interpreter/InterpreterClassDefinition.cpp 
	include/Velox/Interpreter/InterpreterStatementWhile.cpp 
	include/Velox/Interpreter/ItemStateList.h 
	include/Velox/Interpreter/InterpreterClass.cpp 
	include/Velox/Interpreter/InterpreterInstantDefinition.h 
	include/Velox/Interpreter/InterpreterExpressionUnaryMinus.h 
	include/Velox/Interpreter/InterpreterArithmeticOperation.h 
	include/Velox/Interpreter/ItemState.h 
	include/Velox/Interpreter/ItemStateWithSubItems.h 
	include/Velox/Interpreter/InterpreterFunctionTwoParameter.h 
	include/Velox/Interpreter/ItemStateInteger.cpp 
	include/Velox/Interpreter/InterpreterExpressionMemberAccess.h 
	include/Velox/Interpreter/ItemStateFunction.h 
	include/Velox/Interpreter/InterpreterFunctionCall.cpp 
	include/Velox/Interpreter/InterpreterExpressionSimpleName.cpp 
	include/Velox/Interpreter/ItemStateObject.h 
	include/Velox/Interpreter/InterpreterLiteralColor.h 
	include/Velox/Interpreter/Interpreter.cpp 
	include/Velox/Interpreter/InterpreterColor.cpp 
	include/Velox/Interpreter/ItemStateColor.cpp 
	include/Velox/Interpreter/ScriptContext.cpp 
	include/Velox/Interpreter/Scope.cpp 
	include/Velox/Interpreter/InterpreterStatementLoop.cpp 
	include/Velox/Interpreter/ObjectType.cpp 
	include/Velox/Interpreter/InterpreterStatementContinue.cpp 
	include/Velox/Interpreter/InterpreterVector.cpp 
	include/Velox/Interpreter/InterpreterFunctionNoParameter.cpp 
	include/Velox/Interpreter/InterpreterStatementDoWhile.cpp 
	include/Velox/Interpreter/InterpreterLiteralString.cpp 
	include/Velox/Interpreter/InterpreterStatementWhile.h 
	include/Velox/Interpreter/InterpreterExpression.h 
	include/Velox/Interpreter/InterpreterConstructorCall.cpp 
	include/Velox/Interpreter/InterpreterInstantRealization.cpp 
	include/Velox/Interpreter/InterpreterExpressionListAccess.h 
	include/Velox/Interpreter/InterpreterActualParameterList.cpp 
	include/Velox/Interpreter/InterpreterFunctionCall.h 
	include/Velox/Interpreter/InterpreterExpressionNew.cpp 
	include/Velox/Interpreter/SimpleMemoryManager.cpp 

	)

#################################################
####### Optional Support for Sdl
#################################################

OPTION(ASTU_USE_SDL "Build the ASTU library with support for SDL2" OFF)
if(ASTU_USE_SDL)
  	  list(APPEND astu_SOURCES 

		# Module: SuiteSDL
		src/SuiteSDL/SdlRecordingSceneRenderer2D.h 
		src/SuiteSDL/ApplicationImpl.h 
		src/SuiteSDL/ApplicationImpl.cpp 
		src/SuiteSDL/SdlKeyTable.cpp 
		src/SuiteSDL/SdlApplication0.cpp 
		src/SuiteSDL/SdlKeyTable.h 
		src/SuiteSDL/SdlSceneRenderer2D.h 
		src/SuiteSDL/SdlEventService.cpp 
		src/SuiteSDL/SdlAudioService.cpp 
		src/SuiteSDL/SdlApplication1.cpp 
		src/SuiteSDL/SdlService.cpp 
		src/SuiteSDL/SdlRenderService.cpp 
		src/SuiteSDL/SdlRecordingSceneRenderer2D.cpp 
		src/SuiteSDL/ISdlEventListener.h 
		src/SuiteSDL/SdlApplicationDummy.cpp 
		src/SuiteSDL/SdlJoystickService.cpp 
		src/SuiteSDL/SdlSceneGraph2D.cpp 
		src/SuiteSDL/SdlSceneRenderer2D.cpp 
		src/SuiteSDL/SdlApplication.cpp 
		src/SuiteSDL/SdlLineRenderer.cpp 
		src/SuiteSDL/SdlVideoService.cpp 
		src/SuiteSDL/SdlTimeService.cpp 
		include/SuiteSDL/SdlSceneGraph2D.h 
		include/SuiteSDL/SdlApplication.h 
		include/SuiteSDL/SdlJoystickService.h 
		include/SuiteSDL/SdlApplication0.h 
		include/SuiteSDL/SdlTimeService.h 
		include/SuiteSDL/SdlRenderService.h 
		include/SuiteSDL/SdlEventService.h 
		include/SuiteSDL/SdlLineRenderer.h 
		include/SuiteSDL/ISdlApplication1Listener.h 
		include/SuiteSDL/ISdlResizeListener.h 
		include/SuiteSDL/SdlService.h 
		include/SuiteSDL/SdlApplication1.h 
		include/SuiteSDL/SdlAudioService.h 
		include/SuiteSDL/SdlVideoService.h 
	
      )
endif(ASTU_USE_SDL)    
             
#################################################
####### Optional Support for Jack
#################################################

OPTION(ASTU_USE_JACK "Build the ASTU library with support for JACK Audio Connection Kit" OFF)
if(ASTU_USE_JACK)
  	  list(APPEND astu_SOURCES 

		# Module: SuiteJack
		src/SuiteJack/JackClient.cpp 
		src/SuiteJack/JackErrorUtil.h 
		src/SuiteJack/JackErrorUtil.cpp 
		include/SuiteJack/JackClient.h 
	
      )
endif(ASTU_USE_JACK)    
             
#################################################
####### Operating System Specific
#################################################

IF (WIN32)
	# Windows Specific
	list(APPEND astu_SOURCES
		src/System/Windows/DeepSleep.h 
		src/System/Windows/DeepSleep.cpp 
	)
ELSEIF(APPLE)
	# Apple Specific
ELSE()
	# Linux Specific
	list(APPEND astu_SOURCES
		src/System/Linux/NetworkImpl.cpp 
		src/System/Linux/DeepSleep.h 
		src/System/Linux/SocketImpl.h 
		src/System/Linux/AddrInfo.h 
		src/System/Linux/InetSocketAddress.h 
		src/System/Linux/AddrInfo.cpp 
		src/System/Linux/SocketImpl.cpp 
		src/System/Linux/InetSocketAddress.cpp 
		src/System/Linux/DeepSleep.cpp 
		src/System/Linux/NetworkImpl.h 
	)
ENDIF()

#################################################
####### Configuration File
#################################################

# Setup configuration file to promulgate version info, etc.
# Note: this needs to be done after definition of optional variables.
configure_file(src/AstUtilsConfig.h.in AstUtilsConfig.h)

#################################################
####### Static Library Target (astu)
#################################################

# Define static library target.
add_library(astu ${astu_SOURCES})

# Set include directories for target astu
target_include_directories(
		astu PRIVATE "${PROJECT_BINARY_DIR}"
		astu PRIVATE "${PROJECT_SOURCE_DIR}/src"
		astu PUBLIC "${PROJECT_SOURCE_DIR}/include"
	)

# Add dependencies for Operating System specific files and libraries
IF (WIN32)
	target_include_directories(astu PRIVATE "${PROJECT_SOURCE_DIR}/src/System/Windows")
ELSEIF(APPLE)
	# not implemented yet
ELSE()
	target_include_directories(astu PRIVATE "${PROJECT_SOURCE_DIR}/src/System/Linux")
ENDIF()

# Add dependencies for SDL
if(ASTU_USE_SDL)
	IF (WIN32)
		target_link_libraries(astu $ENV{SDL2_HOME}/lib/x64/SDL2.lib)
		list(APPEND astu_INCLUDES astu PRIVATE $ENV{SDL2_HOME})
		target_include_directories(astu PRIVATE $ENV{SDL2_HOME})
	ELSEIF(APPLE)
		#list(APPEND astu_INCLUDES astu PRIVATE /Library/Frameworks/SDL2.framework/Headers)
		target_link_libraries(astu /Library/Frameworks/SDL2.framework/Versions/A/SDL2)
	ELSE()
		target_link_libraries(astu SDL2)
	ENDIF()
endif(ASTU_USE_SDL)                 

# Add dependencies for Jack
if(ASTU_USE_JACK)
	IF (WIN32)
		target_include_directories(astu PRIVATE $ENV{JACK2_HOME}/include)
		target_link_libraries(astu $ENV{JACK2_HOME}/lib/libjack64.lib)
	ELSE()
		target_link_libraries(astu jack)    
	ENDIF()
endif(ASTU_USE_JACK)    
             
#################################################
####### Shared Library Target (astuShared)
#################################################

# Define shared library target.
add_library(astuShared SHARED ${astu_SOURCES})
IF (WIN32)
	target_compile_definitions(astuShared PRIVATE ASTU_EXPORT)       
ENDIF()

# Set include directories for target astuShared
target_include_directories(
		astuShared PRIVATE "${PROJECT_BINARY_DIR}"
		astuShared PRIVATE "${PROJECT_SOURCE_DIR}/src"
		astuShared PUBLIC "${PROJECT_SOURCE_DIR}/include"
	)

# Add dependencies for Operating System specific files and libraries
IF (WIN32)
	target_include_directories(astu PRIVATE "${PROJECT_SOURCE_DIR}/src/System/Windows")
ELSEIF(APPLE)
	# not implemented yet
ELSE()
	target_include_directories(astu PRIVATE "${PROJECT_SOURCE_DIR}/src/System/Linux")
ENDIF()

# Add dependencies for SDL
if(ASTU_USE_SDL)
	IF (WIN32)
		target_link_libraries(astuShared $ENV{SDL2_HOME}/lib/x64/SDL2.lib)
		list(APPEND astuShared_INCLUDES astuShared PRIVATE $ENV{SDL2_HOME})
		target_include_directories(astuShared PRIVATE $ENV{SDL2_HOME})
	ELSEIF(APPLE)
		#list(APPEND astuShared_INCLUDES astuShared PRIVATE /Library/Frameworks/SDL2.framework/Headers)
		target_link_libraries(astuShared /Library/Frameworks/SDL2.framework/Versions/A/SDL2)
	ELSE()
		target_link_libraries(astuShared SDL2)
	ENDIF()
endif(ASTU_USE_SDL)                 

# Add dependencies for Jack
if(ASTU_USE_JACK)
	IF (WIN32)
		target_include_directories(astuShared PRIVATE $ENV{JACK2_HOME}/include)
		target_link_libraries(astuShared $ENV{JACK2_HOME}/lib/libjack64.lib)
	ELSE()
		target_link_libraries(astuShared jack)    
	ENDIF()
endif(ASTU_USE_JACK)    
             