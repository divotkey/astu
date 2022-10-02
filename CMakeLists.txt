# CMake file for AST-Utilities Version 0.11.1
# Generated Sun Oct 02 14:01:12 CEST 2022 with SWORD Interpreter 1.9.0

# Define required CMake version
cmake_minimum_required(VERSION 3.1)

# Add version information
project(ASTU VERSION 0.11.1)

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
	src/AstUtils0.cpp 
	src/AstUtils1.cpp 
	include/AstIO.h 
	include/Astu.h 
	include/AstuApi.h 
	include/AstuAudio.h 
	include/AstuECS.h 
	include/AstuGraphics.h 
	include/AstuInput.h 
	include/AstuLogging.h 
	include/AstuMath.h 
	include/AstuMisc.h 
	include/AstuNetwork.h 
	include/AstuServices.h 
	include/AstuSuite2D.h 
	include/AstuSuiteJack.h 
	include/AstuSuiteSDL.h 
	include/AstUtils.h 
	include/AstUtils0.h 
	include/AstUtils1.h 

	# Module: Util
	src/Util/Buffer.cpp 
	src/Util/Controllable.cpp 
	src/Util/Controller.cpp 
	src/Util/Memento.cpp 
	src/Util/MessageBox.cpp 
	src/Util/SpinLockSleep.cpp 
	src/Util/SpinLockSleep.h 
	src/Util/StateMachine.cpp 
	src/Util/StdSleep.cpp 
	src/Util/StdSleep.h 
	src/Util/StringUtils.cpp 
	src/Util/Timer.cpp 
	src/Util/VersionInfo.cpp 
	include/Util/Buffer.h 
	include/Util/CommandQueue.h 
	include/Util/Controllable.h 
	include/Util/Controller.h 
	include/Util/IThreadSleep.h 
	include/Util/ListenerManager.h 
	include/Util/Memento.h 
	include/Util/MessageBox.h 
	include/Util/Pooled.h 
	include/Util/StateMachine.h 
	include/Util/StringUtils.h 
	include/Util/Timer.h 
	include/Util/VersionInfo.h 

	# Module: Math
	src/Math/MathUtils.cpp 
	src/Math/Random.cpp 
	include/Math/Interpolator.h 
	include/Math/Line2.h 
	include/Math/MathUtils.h 
	include/Math/Matrix3.h 
	include/Math/Matrix4.h 
	include/Math/Polygon.h 
	include/Math/Quaternion.h 
	include/Math/Random.h 
	include/Math/Ray2.h 
	include/Math/Ray3.h 
	include/Math/Segment1.h 
	include/Math/Segment2.h 
	include/Math/SphericalCoordinates.h 
	include/Math/Transform2.h 
	include/Math/Vector2.h 
	include/Math/Vector3.h 

	# Module: Graphics
	src/Graphics/AstuGraphics.cpp 
	src/Graphics/BmpCodec.cpp 
	src/Graphics/BmpCodec.h 
	src/Graphics/BoundingBox.cpp 
	src/Graphics/HdrCodec.cpp 
	src/Graphics/HdrCodec.h 
	src/Graphics/Image.cpp 
	src/Graphics/ImageRenderer.cpp 
	src/Graphics/Palette.cpp 
	src/Graphics/Pattern.cpp 
	src/Graphics/PatternRenderer.cpp 
	src/Graphics/PatternRenderer.h 
	src/Graphics/Quadtree.cpp 
	include/Graphics/BoundingBox.h 
	include/Graphics/Color.h 
	include/Graphics/ColorHsv.h 
	include/Graphics/Image.h 
	include/Graphics/ImageRenderer.h 
	include/Graphics/Palette.h 
	include/Graphics/Pattern.h 
	include/Graphics/Quadtree.h 
	include/Graphics/RalColors.h 
	include/Graphics/RenderQuality.h 
	include/Graphics/Texture.h 
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

	# Module: Network
	src/Network/BasicNetworkService.cpp 
	src/Network/Network.cpp 
	src/Network/Peer.cpp 
	include/Network/BasicNetworkService.h 
	include/Network/IInetSocketAddress.h 
	include/Network/IpMode.h 
	include/Network/Network.h 
	include/Network/Peer.h 
	include/Network/Socket.h 

	# Module: Logging
	src/Logging/ConsoleLoggingService.cpp 
	include/Logging/ConsoleLoggingService.h 
	include/Logging/LogLevel.h 

	# Module: Service
	src/Service/ActionSignalService.cpp 
	src/Service/ConsoleApplication.cpp 
	src/Service/ConsoleTimeService.cpp 
	src/Service/InteractiveApplication.cpp 
	src/Service/Service.cpp 
	src/Service/ServiceManager.cpp 
	src/Service/StateService.cpp 
	src/Service/Tasks.cpp 
	src/Service/TaskService.cpp 
	src/Service/UpdateService.cpp 
	include/Service/ActionSignalService.h 
	include/Service/ConsoleApplication.h 
	include/Service/ConsoleTimeService.h 
	include/Service/InteractiveApplication.h 
	include/Service/LoggingService.h 
	include/Service/NetworkService.h 
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

	# Module: FSM
	src/FSM/DStateMachine.cpp 
	src/FSM/DStateMachineBuilder.cpp 
	src/FSM/FStateMachine.cpp 
	src/FSM/NStateMachine.cpp 
	src/FSM/PowersetBuilder.cpp 
	include/FSM/DStateMachine.h 
	include/FSM/DStateMachineBuilder.h 
	include/FSM/FStateMachine.h 
	include/FSM/NStateMachine.h 
	include/FSM/PowersetBuilder.h 

	# Module: Script
	src/Script/Scanner.cpp 
	src/Script/ScannerBuilder.cpp 
	include/Script/Scanner.h 
	include/Script/ScannerBuilder.h 
	include/Script/ScannerError.h 

	# Module: Velox
	src/Velox/Interpreter/Interpreter.cpp 
	src/Velox/Interpreter/InterpreterActualParameterList.cpp 
	src/Velox/Interpreter/InterpreterArithmeticOperation.cpp 
	src/Velox/Interpreter/InterpreterAssignmentOperator.cpp 
	src/Velox/Interpreter/InterpreterClass.cpp 
	src/Velox/Interpreter/InterpreterClassDefinition.cpp 
	src/Velox/Interpreter/InterpreterColor.cpp 
	src/Velox/Interpreter/InterpreterConstructorCall.cpp 
	src/Velox/Interpreter/InterpreterExpression.cpp 
	src/Velox/Interpreter/InterpreterExpressionAssignment.cpp 
	src/Velox/Interpreter/InterpreterExpressionListAccess.cpp 
	src/Velox/Interpreter/InterpreterExpressionMemberAccess.cpp 
	src/Velox/Interpreter/InterpreterExpressionNew.cpp 
	src/Velox/Interpreter/InterpreterExpressionSimpleName.cpp 
	src/Velox/Interpreter/InterpreterExpressionUnaryMinus.cpp 
	src/Velox/Interpreter/InterpreterExpressionUndefined.cpp 
	src/Velox/Interpreter/InterpreterFormalParameterList.cpp 
	src/Velox/Interpreter/InterpreterFunction.cpp 
	src/Velox/Interpreter/InterpreterFunctionCall.cpp 
	src/Velox/Interpreter/InterpreterFunctionDefinition.cpp 
	src/Velox/Interpreter/InterpreterFunctionNoParameter.cpp 
	src/Velox/Interpreter/InterpreterFunctionOneParameter.cpp 
	src/Velox/Interpreter/InterpreterFunctionPrint.cpp 
	src/Velox/Interpreter/InterpreterFunctionScript.cpp 
	src/Velox/Interpreter/InterpreterFunctionThreeParameter.cpp 
	src/Velox/Interpreter/InterpreterFunctionTwoParameter.cpp 
	src/Velox/Interpreter/InterpreterInstantDefinition.cpp 
	src/Velox/Interpreter/InterpreterInstantRealization.cpp 
	src/Velox/Interpreter/InterpreterLiteralBoolean.cpp 
	src/Velox/Interpreter/InterpreterLiteralColor.cpp 
	src/Velox/Interpreter/InterpreterLiteralInteger.cpp 
	src/Velox/Interpreter/InterpreterLiteralList.cpp 
	src/Velox/Interpreter/InterpreterLiteralReal.cpp 
	src/Velox/Interpreter/InterpreterLiteralString.cpp 
	src/Velox/Interpreter/InterpreterLogicalOperation.cpp 
	src/Velox/Interpreter/InterpreterPostIncrement.cpp 
	src/Velox/Interpreter/InterpreterPreIncrement.cpp 
	src/Velox/Interpreter/InterpreterRelationalOperation.cpp 
	src/Velox/Interpreter/InterpreterReturnStatement.cpp 
	src/Velox/Interpreter/InterpreterStatementBlock.cpp 
	src/Velox/Interpreter/InterpreterStatementBreak.cpp 
	src/Velox/Interpreter/InterpreterStatementContinue.cpp 
	src/Velox/Interpreter/InterpreterStatementDoWhile.cpp 
	src/Velox/Interpreter/InterpreterStatementFor.cpp 
	src/Velox/Interpreter/InterpreterStatementGlobal.cpp 
	src/Velox/Interpreter/InterpreterStatementIf.cpp 
	src/Velox/Interpreter/InterpreterStatementLoop.cpp 
	src/Velox/Interpreter/InterpreterStatementNop.cpp 
	src/Velox/Interpreter/InterpreterStatementWhile.cpp 
	src/Velox/Interpreter/InterpreterVector.cpp 
	src/Velox/Interpreter/Item.cpp 
	src/Velox/Interpreter/ItemArithmeticOperator.cpp 
	src/Velox/Interpreter/ItemRelationalOperator.cpp 
	src/Velox/Interpreter/ItemState.cpp 
	src/Velox/Interpreter/ItemStateBool.cpp 
	src/Velox/Interpreter/ItemStateColor.cpp 
	src/Velox/Interpreter/ItemStateFunction.cpp 
	src/Velox/Interpreter/ItemStateInteger.cpp 
	src/Velox/Interpreter/ItemStateList.cpp 
	src/Velox/Interpreter/ItemStateObject.cpp 
	src/Velox/Interpreter/ItemStateReal.cpp 
	src/Velox/Interpreter/ItemStateReference.cpp 
	src/Velox/Interpreter/ItemStateString.cpp 
	src/Velox/Interpreter/ItemStateUndefined.cpp 
	src/Velox/Interpreter/ItemStateVector2.cpp 
	src/Velox/Interpreter/ItemStateWithSubItems.cpp 
	src/Velox/Interpreter/MemoryManager.cpp 
	src/Velox/Interpreter/ObjectType.cpp 
	src/Velox/Interpreter/Scope.cpp 
	src/Velox/Interpreter/ScriptContext.cpp 
	src/Velox/Interpreter/SimpleMemoryManager.cpp 
	src/Velox/Interpreter/VeloxImage.cpp 
	src/Velox/Parser/Parser.cpp 
	src/Velox/Parser/Source.cpp 
	src/Velox/Parser/SourceFile.cpp 
	src/Velox/Parser/SourceString.cpp 
	src/Velox/Parser/SourceString.h 
	src/Velox/Parser/Token.cpp 
	src/Velox/Parser/VeloxScanner.cpp 
	src/Velox/Parser/VeloxScanner.h 
	include/Velox/Interpreter/IMemoryManager.h 
	include/Velox/Interpreter/Interpreter.h 
	include/Velox/Interpreter/InterpreterActualParameterList.h 
	include/Velox/Interpreter/InterpreterArithmeticOperation.h 
	include/Velox/Interpreter/InterpreterAssignmentOperator.h 
	include/Velox/Interpreter/InterpreterClass.h 
	include/Velox/Interpreter/InterpreterClassDefinition.h 
	include/Velox/Interpreter/InterpreterColor.h 
	include/Velox/Interpreter/InterpreterConstructorCall.h 
	include/Velox/Interpreter/InterpreterError.h 
	include/Velox/Interpreter/InterpreterExpression.h 
	include/Velox/Interpreter/InterpreterExpressionAssignment.h 
	include/Velox/Interpreter/InterpreterExpressionListAccess.h 
	include/Velox/Interpreter/InterpreterExpressionMemberAccess.h 
	include/Velox/Interpreter/InterpreterExpressionNew.h 
	include/Velox/Interpreter/InterpreterExpressionSimpleName.h 
	include/Velox/Interpreter/InterpreterExpressionUnaryMinus.h 
	include/Velox/Interpreter/InterpreterExpressionUndefined.h 
	include/Velox/Interpreter/InterpreterFormalParameterList.h 
	include/Velox/Interpreter/InterpreterFunction.h 
	include/Velox/Interpreter/InterpreterFunctionCall.h 
	include/Velox/Interpreter/InterpreterFunctionDefinition.h 
	include/Velox/Interpreter/InterpreterFunctionNoParameter.h 
	include/Velox/Interpreter/InterpreterFunctionOneParameter.h 
	include/Velox/Interpreter/InterpreterFunctionPrint.h 
	include/Velox/Interpreter/InterpreterFunctionScript.h 
	include/Velox/Interpreter/InterpreterFunctionThreeParameter.h 
	include/Velox/Interpreter/InterpreterFunctionTwoParameter.h 
	include/Velox/Interpreter/InterpreterInstantDefinition.h 
	include/Velox/Interpreter/InterpreterInstantRealization.h 
	include/Velox/Interpreter/InterpreterLiteralBoolean.h 
	include/Velox/Interpreter/InterpreterLiteralColor.h 
	include/Velox/Interpreter/InterpreterLiteralInteger.h 
	include/Velox/Interpreter/InterpreterLiteralList.h 
	include/Velox/Interpreter/InterpreterLiteralReal.h 
	include/Velox/Interpreter/InterpreterLiteralString.h 
	include/Velox/Interpreter/InterpreterLogicalOperation.h 
	include/Velox/Interpreter/InterpreterPostIncrement.h 
	include/Velox/Interpreter/InterpreterPreIncrement.h 
	include/Velox/Interpreter/InterpreterRelationalOperation.h 
	include/Velox/Interpreter/InterpreterReturnStatement.h 
	include/Velox/Interpreter/InterpreterStatement.h 
	include/Velox/Interpreter/InterpreterStatementBlock.h 
	include/Velox/Interpreter/InterpreterStatementBreak.h 
	include/Velox/Interpreter/InterpreterStatementContinue.h 
	include/Velox/Interpreter/InterpreterStatementDoWhile.h 
	include/Velox/Interpreter/InterpreterStatementFor.h 
	include/Velox/Interpreter/InterpreterStatementGlobal.h 
	include/Velox/Interpreter/InterpreterStatementIf.h 
	include/Velox/Interpreter/InterpreterStatementLoop.h 
	include/Velox/Interpreter/InterpreterStatementNop.h 
	include/Velox/Interpreter/InterpreterStatementWhile.h 
	include/Velox/Interpreter/InterpreterVector.h 
	include/Velox/Interpreter/Item.h 
	include/Velox/Interpreter/ItemState.h 
	include/Velox/Interpreter/ItemStateBool.h 
	include/Velox/Interpreter/ItemStateColor.h 
	include/Velox/Interpreter/ItemStateFunction.h 
	include/Velox/Interpreter/ItemStateInteger.h 
	include/Velox/Interpreter/ItemStateList.h 
	include/Velox/Interpreter/ItemStateObject.h 
	include/Velox/Interpreter/ItemStateReal.h 
	include/Velox/Interpreter/ItemStateRealMember.h 
	include/Velox/Interpreter/ItemStateReference.h 
	include/Velox/Interpreter/ItemStateString.h 
	include/Velox/Interpreter/ItemStateUndefined.h 
	include/Velox/Interpreter/ItemStateVector2.h 
	include/Velox/Interpreter/ItemStateWithSubItems.h 
	include/Velox/Interpreter/ItemType.h 
	include/Velox/Interpreter/MemoryManager.h 
	include/Velox/Interpreter/ObjectType.h 
	include/Velox/Interpreter/Operators.h 
	include/Velox/Interpreter/Scope.h 
	include/Velox/Interpreter/ScriptContext.h 
	include/Velox/Interpreter/SimpleMemoryManager.h 
	include/Velox/Interpreter/VeloxImage.h 
	include/Velox/Parser/Parser.h 
	include/Velox/Parser/ParserError.h 
	include/Velox/Parser/Source.h 
	include/Velox/Parser/SourceFile.h 
	include/Velox/Parser/Token.h 
	include/Velox/Parser/TokenType.h 

	)

#################################################
####### Optional Support for Sdl
#################################################

OPTION(ASTU_USE_SDL "Build the ASTU library with support for SDL2" OFF)
if(ASTU_USE_SDL)
  	  list(APPEND astu_SOURCES 

		# Module: SuiteSDL
		src/SuiteSDL/ApplicationImpl.cpp 
		src/SuiteSDL/ApplicationImpl.h 
		src/SuiteSDL/ISdlEventListener.h 
		src/SuiteSDL/SdlApplication.cpp 
		src/SuiteSDL/SdlApplication0.cpp 
		src/SuiteSDL/SdlApplication1.cpp 
		src/SuiteSDL/SdlApplicationDummy.cpp 
		src/SuiteSDL/SdlAudioService.cpp 
		src/SuiteSDL/SdlEventService.cpp 
		src/SuiteSDL/SdlJoystickService.cpp 
		src/SuiteSDL/SdlKeyTable.cpp 
		src/SuiteSDL/SdlKeyTable.h 
		src/SuiteSDL/SdlLineRenderer.cpp 
		src/SuiteSDL/SdlRecordingSceneRenderer2D.cpp 
		src/SuiteSDL/SdlRecordingSceneRenderer2D.h 
		src/SuiteSDL/SdlRenderService.cpp 
		src/SuiteSDL/SdlSceneGraph2D.cpp 
		src/SuiteSDL/SdlSceneRenderer2D.cpp 
		src/SuiteSDL/SdlSceneRenderer2D.h 
		src/SuiteSDL/SdlService.cpp 
		src/SuiteSDL/SdlTexture.cpp 
		src/SuiteSDL/SdlTexture.h 
		src/SuiteSDL/SdlTimeService.cpp 
		src/SuiteSDL/SdlVideoService.cpp 
		include/SuiteSDL/ISdlApplication1Listener.h 
		include/SuiteSDL/ISdlResizeListener.h 
		include/SuiteSDL/SdlApplication.h 
		include/SuiteSDL/SdlApplication0.h 
		include/SuiteSDL/SdlApplication1.h 
		include/SuiteSDL/SdlAudioService.h 
		include/SuiteSDL/SdlEventService.h 
		include/SuiteSDL/SdlJoystickService.h 
		include/SuiteSDL/SdlLineRenderer.h 
		include/SuiteSDL/SdlRenderService.h 
		include/SuiteSDL/SdlSceneGraph2D.h 
		include/SuiteSDL/SdlService.h 
		include/SuiteSDL/SdlTimeService.h 
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
		src/SuiteJack/JackErrorUtil.cpp 
		src/SuiteJack/JackErrorUtil.h 
		include/SuiteJack/JackClient.h 
	
      )
endif(ASTU_USE_JACK)    
             
#################################################
####### Operating System Specific
#################################################

IF (WIN32)
	# Windows Specific
	list(APPEND astu_SOURCES
		src/System/Windows/DeepSleep.cpp 
		src/System/Windows/DeepSleep.h 
		src/System/Windows/NetworkImpl.cpp 
		src/System/Windows/NetworkImpl.h 
	)
ELSEIF(APPLE)
	# Apple Specific
ELSE()
	# Linux Specific
	list(APPEND astu_SOURCES
		src/System/Linux/AddrInfo.cpp 
		src/System/Linux/AddrInfo.h 
		src/System/Linux/DeepSleep.cpp 
		src/System/Linux/DeepSleep.h 
		src/System/Linux/InetSocketAddress.cpp 
		src/System/Linux/InetSocketAddress.h 
		src/System/Linux/NetworkImpl.cpp 
		src/System/Linux/NetworkImpl.h 
		src/System/Linux/SocketImpl.cpp 
		src/System/Linux/SocketImpl.h 
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
             