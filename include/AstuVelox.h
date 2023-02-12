/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

/**
 * This file defines public functions, templates and classes offered 
 * by Velox scripting system.
 */

#include "Velox/Parser/Parser.h"
#include "Velox/Parser/SourceFile.h"
#include "Velox/Parser/ParserError.h"
#include "Velox/Interpreter/Interpreter.h"
#include "Velox/Interpreter/InterpreterFunctionNoParameter.h"
#include "Velox/Interpreter/InterpreterFunctionOneParameter.h"
#include "Velox/Interpreter/InterpreterFunctionTwoParameter.h"
#include "Velox/Interpreter/InterpreterFunctionThreeParameter.h"
#include "Velox/Interpreter/ScriptContext.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Service/VeloxExecutionService.h"
#include "Velox/Service/VeloxExtensionService.h"
#include "Velox/Extensions/LoggingExtension.h"
#include "Velox/Extensions/AstuExtensions.h"
#include "Velox/Extensions/ImageExtension.h"
#include "Velox/Extensions/GraphicsExtension.h"
#include "Velox/Extensions/SceneGraphExtension2D.h"
#include "Velox/Extensions/ExtensionConstructorNoParameter.h"
#include "Velox/Extensions/ExtensionConstructorOneParameter.h"
#include "Velox/Extensions/ExtensionConstructorTwoParameter.h"
#include "Velox/Extensions/ExtensionFunctionNoParameter.h"
#include "Velox/Extensions/ExtensionFunctionOneParameter.h"
#include "Velox/Extensions/ExtensionFunctionTwoParameter.h"
#include "Velox/Extensions/ExtensionFunctionThreeParameter.h"

namespace astu {

    /**
     * @defgroup velox_group Entity component system
     * @brief This module contains templates, classes and functions dedicated to the Velox scripting system.
     * @{
     */

    /**@}*/

} // end of namespace
