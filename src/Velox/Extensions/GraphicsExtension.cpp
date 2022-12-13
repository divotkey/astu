/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Extensions/GraphicsExtensions.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/InterpreterFunction.h"
#include "Velox/Extensions/ExtensionFunctionNoParameter.h"
#include "Velox/Extensions/ExtensionFunctionOneParameter.h"
#include "Velox/Extensions/ExtensionFunctionTwoParameter.h"
#include "Velox/Extensions/ExtensionConstructorNoParameter.h"
#include "Velox/Extensions/ExtensionConstructorOneParameter.h"
#include "Velox/Extensions/ExtensionConstructorTwoParameter.h"
#include "Graphics/Pattern.h"
#include "Graphics/UnionPattern.h"
#include "Graphics/UnicolorPattern.h"
#include "Graphics/RectanglePattern.h"
#include "Graphics/CirclePattern.h"
#include "Graphics/SimplePatternRenderer.h"
#include "Graphics/AntiAliasingPatternRenderer.h"
#include "Graphics/Image.h"

// C++ Standard Library includes
#include <cassert>
#include <functional>
#include <memory>

using namespace velox;
using namespace std;

#define UNICOLOR_TYPE                   "UnicolorPattern"
#define SIMPLE_PATTERN_RENDERER_TYPE    "SimplePatternRenderer"

namespace astu {

    /////////////////////////////////////////////////
    /////// Extension classes
    /////////////////////////////////////////////////

    // Special cases requires dedicated classes instead of using the generic constructor and function templates.
    // These classes will be placed here.


    /////////////////////////////////////////////////
    /////// Graphics Extension
    /////////////////////////////////////////////////

    GraphicsExtension::GraphicsExtension()
            : VeloxExtension("Graphics Extension")
    {
        // Intentionally left empty.
    }

    void GraphicsExtension::InjectExtension(Interpreter &interpreter) const
    {
        ObjectTypeBuilder builder;

        builder.TypeName("UnionPattern")
                .Constructor(ExtensionConstructorOneParameter<UnionPattern>::CreateItem(
                        [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
                            return make_shared<UnionPattern>();
                        }
                ))
                .AddFunction("AddPattern", ExtensionFunctionOneParameter<CompoundPattern>::CreateItem(
                        [](ScriptContext &sc, CompoundPattern &pattern, Item &param, unsigned int lineNumber) {
                            auto childPattern = dynamic_pointer_cast<Pattern>(param.GetData());
                            if (!childPattern) {
                                throw InterpreterError("AddPatter requires pattern as parameter", lineNumber);
                            }
                            pattern.Add(childPattern);

                            return Item::CreateUndefined();
                        }
                ))
                ;

        AddCommonPatternFunctions(builder);
        builder.Build(interpreter);


        builder.TypeName("UnicolorPattern")
            .Constructor(ExtensionConstructorOneParameter<UnicolorPattern>::CreateItem(
                    [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
                        if (param.IsUndefined()) {
                            return make_shared<UnicolorPattern>();
                        } else {
                            return make_shared<UnicolorPattern>(param.GetColorValue());
                        }
                    }
                ))
            .AddFunction("GetColor", ExtensionFunctionNoParameter<UnicolorPattern>::CreateItem(
                    [](ScriptContext &sc, UnicolorPattern &pattern, unsigned int lineNumber) {
                        return Item::CreateColor(pattern.GetColor());
                    }
                ))
            .AddFunction("SetColor", ExtensionFunctionOneParameter<UnicolorPattern>::CreateItem(
                    [](ScriptContext &sc, UnicolorPattern &pattern, Item &param, unsigned int lineNumber) {
                        pattern.SetColor(param.GetColorValue());
                        return Item::CreateUndefined();
                    }
                ))
                ;

        AddCommonPatternFunctions(builder);
        builder.Build(interpreter);

        builder.TypeName("CirclePattern")
                .Constructor(ExtensionConstructorOneParameter<CirclePattern>::CreateItem(
                        [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
                            if (param.IsUndefined()) {
                                return make_shared<CirclePattern>();
                            } else {
                                return make_shared<CirclePattern>(param.GetRealValue(lineNumber));
                            }
                        }
                ))
                .AddFunction("GetRadius", ExtensionFunctionNoParameter<CirclePattern>::CreateItem(
                        [](ScriptContext &sc, CirclePattern &pattern, unsigned int lineNumber) {
                            return Item::CreateReal(pattern.GetRadius());
                        }
                ))
                .AddFunction("SetRadius", ExtensionFunctionOneParameter<CirclePattern>::CreateItem(
                        [](ScriptContext &sc, CirclePattern &pattern, Item &param, unsigned int lineNumber) {
                            pattern.SetRadius(param.GetRealValue(lineNumber));
                            return Item::CreateUndefined();
                        }
                    ))
                .AddFunction("SetPattern", ExtensionFunctionOneParameter<CirclePattern>::CreateItem(
                        [](ScriptContext &sc, CirclePattern &pattern, Item &param, unsigned int lineNumber) {
                            if (param.IsUndefined()) {
                                pattern.SetPattern(nullptr);
                            } else {
                                auto drawPattern = dynamic_pointer_cast<Pattern>(param.GetData());
                                if (!drawPattern) {
                                    throw InterpreterError("SetPattern requires pattern as parameter", lineNumber);
                                }
                                pattern.SetPattern(drawPattern);
                            }

                            return Item::CreateUndefined();
                        }
                    ))
                ;

        AddCommonPatternFunctions(builder);
        builder.Build(interpreter);

        builder.TypeName("RectanglePattern")
                .Constructor(ExtensionConstructorTwoParameter<RectanglePattern>::CreateItem(
                        [](ScriptContext &sc, Item &param1, Item &param2, unsigned int lineNumber) {
                            if (param1.IsUndefined()) {
                                return make_shared<RectanglePattern>();
                            } else if (param2.IsUndefined()) {
                                auto s = param1.GetRealValue(lineNumber);
                                return make_shared<RectanglePattern>(s, s);
                            } else {
                                return make_shared<RectanglePattern>(param1.GetRealValue(lineNumber),
                                                                     param2.GetRealValue(lineNumber));
                            }
                        }
                ))
                .AddFunction("GetWidth", ExtensionFunctionNoParameter<RectanglePattern>::CreateItem(
                        [](ScriptContext &sc, RectanglePattern &pattern, unsigned int lineNumber) {
                            return Item::CreateReal(pattern.GetWidth());
                        }
                ))
                .AddFunction("GetHeight", ExtensionFunctionNoParameter<RectanglePattern>::CreateItem(
                        [](ScriptContext &sc, RectanglePattern &pattern, unsigned int lineNumber) {
                            return Item::CreateReal(pattern.GetHeight());
                        }
                ))
                .AddFunction("SetWidth", ExtensionFunctionOneParameter<RectanglePattern>::CreateItem(
                        [](ScriptContext &sc, RectanglePattern &pattern, Item &param, unsigned int lineNumber) {
                            pattern.SetWidth(param.GetRealValue(lineNumber));
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("SetHeight", ExtensionFunctionOneParameter<RectanglePattern>::CreateItem(
                        [](ScriptContext &sc, RectanglePattern &pattern, Item &param, unsigned int lineNumber) {
                            pattern.SetHeight(param.GetRealValue(lineNumber));
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("SetPattern", ExtensionFunctionOneParameter<RectanglePattern>::CreateItem(
                        [](ScriptContext &sc, RectanglePattern &pattern, Item &param, unsigned int lineNumber) {
                            if (param.IsUndefined()) {
                                pattern.SetPattern(nullptr);
                            } else {
                                auto drawPattern = dynamic_pointer_cast<Pattern>(param.GetData());
                                if (!drawPattern) {
                                    throw InterpreterError("SetPattern requires pattern as parameter", lineNumber);
                                }
                                pattern.SetPattern(drawPattern);
                            }

                            return Item::CreateUndefined();
                        }
                ))
                ;

        AddCommonPatternFunctions(builder);
        builder.Build(interpreter);


        builder.TypeName("SimplePatternRenderer")
                .Constructor(ExtensionConstructorNoParameter<SimplePatternRenderer>::CreateItem(
                        [](ScriptContext &sc, unsigned int lineNumber) {
                            return make_shared<SimplePatternRenderer>();
                        }
                    ))
                .AddFunction("Render", ExtensionFunctionTwoParameter<SimplePatternRenderer>::CreateItem(
                        [](ScriptContext &sc, SimplePatternRenderer &renderer, Item &param1, Item &param2, unsigned int lineNumber) {
                            auto pattern = dynamic_pointer_cast<Pattern>(param1.GetData());
                            if (!pattern) {
                                throw InterpreterError("a pattern is required as first parameter", lineNumber);
                            }

                            auto image = dynamic_pointer_cast<Image>(param2.GetData());
                            if (!image) {
                                throw InterpreterError("a image is required as second parameter", lineNumber);
                            }

                            renderer.Render(*pattern, *image);

                            return Item::CreateUndefined();
                        }
                    ))
                .Build(interpreter);

        builder.TypeName("PatternRenderer")
                .Constructor(ExtensionConstructorNoParameter<AntiAliasingPatternRenderer>::CreateItem(
                        [](ScriptContext &sc, unsigned int lineNumber) {
                            return make_shared<AntiAliasingPatternRenderer>();
                        }
                ))
                .AddFunction("Render", ExtensionFunctionTwoParameter<IPatternRenderer>::CreateItem(
                        [](ScriptContext &sc, IPatternRenderer &renderer, Item &param1, Item &param2, unsigned int lineNumber) {
                            auto pattern = dynamic_pointer_cast<Pattern>(param1.GetData());
                            if (!pattern) {
                                throw InterpreterError("a pattern is required as first parameter", lineNumber);
                            }

                            auto image = dynamic_pointer_cast<Image>(param2.GetData());
                            if (!image) {
                                throw InterpreterError("a image is required as second parameter", lineNumber);
                            }

                            renderer.Render(*pattern, *image);

                            return Item::CreateUndefined();
                        }
                ))
                .Build(interpreter);
    }

    void GraphicsExtension::AddCommonPatternFunctions(ObjectTypeBuilder &builder) const
    {
        builder
            .AddFunction("Translate", ExtensionFunctionOneParameter<Pattern>::CreateItem(
                    [](ScriptContext &sc, Pattern &pattern, Item &param, unsigned int lineNumber) {
                        pattern.Translate(param.GetVector2Value());
                        return Item::CreateUndefined();
                    }
                ))
                .AddFunction("Scale", ExtensionFunctionOneParameter<Pattern>::CreateItem(
                        [](ScriptContext &sc, Pattern &pattern, Item &param, unsigned int lineNumber) {
                            pattern.Scale(param.GetVector2Value());
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("Rotate", ExtensionFunctionOneParameter<Pattern>::CreateItem(
                        [](ScriptContext &sc, Pattern &pattern, Item &param, unsigned int lineNumber) {
                            pattern.Rotate(param.GetRealValue(lineNumber));
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("RotateDeg", ExtensionFunctionOneParameter<Pattern>::CreateItem(
                        [](ScriptContext &sc, Pattern &pattern, Item &param, unsigned int lineNumber) {
                            pattern.Rotate(MathUtils::ToRadians(param.GetRealValue(lineNumber)));
                            return Item::CreateUndefined();
                        }
                ))
            ;


    }

} // end of namespace