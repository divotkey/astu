/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Extensions/GraphicsExtensions.h"
#include "Velox/Extensions/InstantBuilder.h"
#include "Velox/Extensions/FibTokens.h"
#include "Velox/Parser/MementoSource.h"
#include "Velox/Parser/Parser.h"
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
#include "Graphics/BlurPattern.h"
#include "Graphics/UnicolorPattern.h"
#include "Graphics/RectanglePattern.h"
#include "Graphics/CirclePattern.h"
#include "Graphics/QuadtreePattern.h"
#include "Graphics/SimplePatternRenderer.h"
#include "Graphics/AntiAliasingPatternRenderer.h"
#include "Graphics/Image.h"
#include "Graphics/Palette.h"
#include "Util/StringUtils.h"

// C++ Standard Library includes
#include <memory>

using namespace velox;
using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// Extension classes
    /////////////////////////////////////////////////

    // Special cases requires dedicated classes instead of using the generic constructor and function templates.
    // These classes will be placed here.

    /////////////////////////////////////////////////
    /////// Utility functions
    /////////////////////////////////////////////////

    PatternRenderer::TransformMode ParamToMode(ScriptContext &sc, const Item &param, unsigned int lineNumber) {
        if (param.IsInteger()) {
            switch (param.GetIntegerValue(lineNumber)) {
                case 0:
                    return PatternRenderer::TransformMode::RAW;

                case 1:
                    return PatternRenderer::TransformMode::CENTERED;

                case 2:
                    return PatternRenderer::TransformMode::H_NORMALIZED;

                case 3:
                    return PatternRenderer::TransformMode::V_NORMALIZED;

                case 4:
                    return PatternRenderer::TransformMode::NORMALIZED;

                default:
                    throw InterpreterError("Invalid transform mode: " + to_string(param.GetIntegerValue()), lineNumber);
            }
        } else if (param.IsString()) {
            auto s = param.GetStringValue(sc);
            StringUtils::ToUpperCase(s);

            if (s == "RAW") {
                return PatternRenderer::TransformMode::RAW;
            }
            else if (s == "CENTERED") {
                return PatternRenderer::TransformMode::CENTERED;
            }
            else if (s == "H_NORMALIZED") {
                return PatternRenderer::TransformMode::H_NORMALIZED;
            }
            else if (s == "V_NORMALIZED") {
                return PatternRenderer::TransformMode::V_NORMALIZED;
            }
            else if (s == "NORMALIZED") {
                return PatternRenderer::TransformMode::NORMALIZED;
            } else {
                throw InterpreterError("Invalid transform mode: '" + param.GetStringValue(sc) + "'", lineNumber);
            }
        }

        throw InterpreterError("Invalid parameter type specified for transform mode", lineNumber);
    }

    AntialiasingLevel ParamToAaLevel(ScriptContext &sc, const Item &param, unsigned int lineNumber) {
        if (param.IsUndefined()) {
            return AntialiasingLevel::Good;
        } else if (param.IsInteger()) {
            switch (param.GetIntegerValue(lineNumber)) {
                case 0:
                    return AntialiasingLevel::Simple;

                case 1:
                    return AntialiasingLevel::Good;

                case 2:
                    return AntialiasingLevel::Beautiful;

                case 3:
                    return AntialiasingLevel::Insane;

                default:
                    throw InterpreterError("Invalid anti-aliasing level: " + std::to_string(param.GetIntegerValue()), lineNumber);
            }
        } else if (param.IsString()) {
            auto s = param.GetStringValue(sc);
            StringUtils::ToUpperCase(s);

            if (s == "SIMPLE") {
                return AntialiasingLevel::Simple;
            }
            else if (s == "GOOD") {
                return AntialiasingLevel::Good;
            }
            else if (s == "BEAUTIFUL") {
                return AntialiasingLevel::Beautiful;
            }
            else if (s == "INSANE") {
                return AntialiasingLevel::Insane;
            } else {
                throw InterpreterError("Invalid anti-aliasing level: '" + param.GetStringValue(sc) + "'", lineNumber);
            }
        }

        throw InterpreterError("Invalid parameter type specified for anti-aliasing level", lineNumber);
    }


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

        builder.Reset()
            .TypeName("UnionPattern")
            .Constructor(ExtensionConstructorNoParameter<UnionPattern>::CreateItem(
                    [](ScriptContext &sc, unsigned int lineNumber) {
                        return make_shared<UnionPattern>();
                    }
            ))
            .AddFunction("AddPattern", ExtensionFunctionOneParameter<CompoundPattern>::CreateItem(
                    [](ScriptContext &sc, CompoundPattern &pattern, Item &param, unsigned int lineNumber) {
                        auto childPattern = dynamic_pointer_cast<Pattern>(param.GetData());
                        if (!childPattern) {
                            throw InterpreterError("AddPatter requires pattern as parameter", lineNumber);
                        }
                        pattern.AddPattern(childPattern);

                        return Item::CreateUndefined();
                    }
            ))
            ;

        AddCommonPatternFunctions(builder);
        builder.Build(interpreter);

        builder.Reset()
            .TypeName("BlurPattern")
            .Constructor(ExtensionConstructorTwoParameter<BlurPattern>::CreateItem(
                    [](ScriptContext &sc, Item &param1, Item &param2, unsigned int lineNumber) {
                        if (param1.IsUndefined()) {
                            return make_shared<BlurPattern>();
                        } else if (param2.IsUndefined()) {
                            return make_shared<BlurPattern>(param1.GetIntegerValue(lineNumber));
                        } else {
                            return make_shared<BlurPattern>(param1.GetIntegerValue(lineNumber), param2.GetIntegerValue(lineNumber));
                        }
                    }
            ))
            .AddFunction("AddPattern", ExtensionFunctionOneParameter<CompoundPattern>::CreateItem(
                    [](ScriptContext &sc, CompoundPattern &pattern, Item &param, unsigned int lineNumber) {
                        auto childPattern = dynamic_pointer_cast<Pattern>(param.GetData());
                        if (!childPattern) {
                            throw InterpreterError("AddPatter requires pattern as parameter", lineNumber);
                        }
                        pattern.AddPattern(childPattern);

                        return Item::CreateUndefined();
                    }
            ))
            .AddFunction("GetRadius", ExtensionFunctionNoParameter<BlurPattern>::CreateItem(
                    [](ScriptContext &sc, BlurPattern &pattern, unsigned int lineNumber) {
                        return Item::CreateReal(pattern.GetRadius());
                    }
            ))
            .AddFunction("SetRadius", ExtensionFunctionOneParameter<BlurPattern>::CreateItem(
                    [](ScriptContext &sc, BlurPattern &pattern, Item &param, unsigned int lineNumber) {
                        pattern.SetRadius(param.GetRealValue(lineNumber));
                        return Item::CreateUndefined();
                    }
            ))
            .AddFunction("GetNumSamples", ExtensionFunctionNoParameter<BlurPattern>::CreateItem(
                    [](ScriptContext &sc, BlurPattern &pattern, unsigned int lineNumber) {
                        return Item::CreateInteger(static_cast<unsigned int>(pattern.GetNumSamples()));
                    }
            ))
            .AddFunction("SetNumSamples", ExtensionFunctionOneParameter<BlurPattern>::CreateItem(
                    [](ScriptContext &sc, BlurPattern &pattern, Item &param, unsigned int lineNumber) {
                        int n = param.GetIntegerValue(lineNumber);
                        if (n < 0) {
                            throw InterpreterError("Invalid number of samples " + to_string(n), lineNumber);
                        }
                        pattern.SetNumSamples(static_cast<unsigned int>(n));
                        return Item::CreateUndefined();
                    }
            ))
            ;

        AddCommonPatternFunctions(builder);
        builder.Build(interpreter);

        builder
                .Reset()
                .TypeName("QuadtreePattern")
                .Constructor(ExtensionConstructorTwoParameter<QuadtreePattern>::CreateItem(
                        [](ScriptContext &sc, Item &param1, Item &param2, unsigned int lineNumber) {
                            if (param1.IsUndefined()) {
                                return make_shared<QuadtreePattern>();
                            } else if (param2.IsUndefined()) {
                                return make_shared<QuadtreePattern>(param1.GetIntegerValue(lineNumber));
                            } else {
                                return make_shared<QuadtreePattern>(param1.GetIntegerValue(lineNumber), param2.GetIntegerValue(lineNumber));
                            }
                        }
                ))
                .AddFunction("AddPattern", ExtensionFunctionOneParameter<CompoundPattern>::CreateItem(
                        [](ScriptContext &sc, CompoundPattern &pattern, Item &param, unsigned int lineNumber) {
                            auto childPattern = dynamic_pointer_cast<Pattern>(param.GetData());
                            if (!childPattern) {
                                throw InterpreterError("AddPatter requires pattern as parameter", lineNumber);
                            }
                            pattern.AddPattern(childPattern);

                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("BuildTree", ExtensionFunctionNoParameter<QuadtreePattern>::CreateItem(
                        [](ScriptContext &sc, QuadtreePattern &quadtree, unsigned int lineNumber) {
                            quadtree.BuildTree();
                            return Item::CreateUndefined();
                        }
                ))
                ;

        AddCommonPatternFunctions(builder);
        builder.Build(interpreter);

        builder
            .Reset()
            .TypeName("UnicolorPattern")
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

        builder
            .Reset()
            .TypeName("CirclePattern")
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

        builder
            .Reset().TypeName("RectanglePattern")
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


        builder
            .Reset()
            .TypeName("SimplePatternRenderer")
            .Constructor(ExtensionConstructorOneParameter<SimplePatternRenderer>::CreateItem(
                    [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
                        return make_shared<SimplePatternRenderer>(ParamToMode(sc, param, lineNumber));
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

        builder
            .Reset()
            .TypeName("PatternRenderer")
            .Constructor(ExtensionConstructorTwoParameter<AntiAliasingPatternRenderer>::CreateItem(
                    [](ScriptContext &sc, Item &param1, Item &param2, unsigned int lineNumber) {
                        return make_shared<AntiAliasingPatternRenderer>(
                                ParamToMode(sc, param1, lineNumber), ParamToAaLevel(sc, param2, lineNumber));
                    }
            ))
            .AddFunction("Render", ExtensionFunctionTwoParameter<AntiAliasingPatternRenderer>::CreateItem(
                    [](ScriptContext &sc, AntiAliasingPatternRenderer &renderer, Item &param1, Item &param2, unsigned int lineNumber) {
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

        AddPalette(interpreter);
        AddWebColors(interpreter);
        AddRalColors(interpreter);
        AddPatternInstants(interpreter);
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

    void GraphicsExtension::AddPalette(Interpreter &interpreter) const
    {
        ObjectTypeBuilder builder;

        builder.Reset()
                .TypeName("Palette")
                .Constructor(ExtensionConstructorTwoParameter<Palette>::CreateItem(
                        [](ScriptContext &sc, Item &param1, Item &param2, unsigned int lineNumber) {

                            if (param1.IsUndefined()) {
                                return make_shared<Palette>();
                            } else if (param2.IsUndefined()) {
                                return make_shared<Palette>(param1.GetColorValue());
                            } else {
                                return make_shared<Palette>(param1.GetColorValue(), param2.GetColorValue());
                            }
                        }
                ))
                .AddFunction("AddColor", ExtensionFunctionTwoParameter<Palette>::CreateItem(
                        [](ScriptContext &sc, Palette &palette, Item &param1, Item &param2, unsigned int lineNumber) {
                            palette.AddColor(param1.GetColorValue(), param2.GetRealValue(lineNumber));
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("SetStartColor", ExtensionFunctionOneParameter<Palette>::CreateItem(
                        [](ScriptContext &sc, Palette &palette, Item &param1, unsigned int lineNumber) {
                            palette.SetStartColor(param1.GetColorValue());
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("SetEndColor", ExtensionFunctionOneParameter<Palette>::CreateItem(
                        [](ScriptContext &sc, Palette &palette, Item &param1, unsigned int lineNumber) {
                            palette.SetEndColor(param1.GetColorValue());
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("GetColor", ExtensionFunctionOneParameter<Palette>::CreateItem(
                        [](ScriptContext &sc, Palette &palette, Item &param, unsigned int lineNumber) {
                            return Item::CreateColor(palette.GetColor(param.GetRealValue(lineNumber)));
                        }
                ))
                .Build(interpreter)
                ;

    }

    void GraphicsExtension::AddPatternInstants(Interpreter &interpreter) const
    {
        auto source = MementoSource(FibTokens::GetMemento());
        Parser parser;
        auto script = parser.Parse(source);
        interpreter.Execute(*script);
    }

    void GraphicsExtension::AddWebColors(Interpreter &interpreter) const
    {
        auto item = Item::CreateObject();

        // Category: Pink colors
        item->AddItem("MediumVioletRed", Item::CreateColor(0xc71585));
        item->AddItem("DeepPink", Item::CreateColor(0xff1493));
        item->AddItem("PaleVioletRed", Item::CreateColor(0xdb7093));
        item->AddItem("HotPink", Item::CreateColor(0xff69b4));
        item->AddItem("LightPink", Item::CreateColor(0xffb6c1));
        item->AddItem("Pink", Item::CreateColor(0xffc0cb));

        // Category: Red colors
        item->AddItem("DarkRed", Item::CreateColor(0x8b0000));
        item->AddItem("Red", Item::CreateColor(0xff0000));
        item->AddItem("Firebrick", Item::CreateColor(0xb22222));
        item->AddItem("Crimson", Item::CreateColor(0xdc143c));
        item->AddItem("IndianRed", Item::CreateColor(0xcd5c5c));
        item->AddItem("LightCoral", Item::CreateColor(0xf08080));
        item->AddItem("Salmon", Item::CreateColor(0xfa8072));
        item->AddItem("DarkSalmon", Item::CreateColor(0xe9967a));
        item->AddItem("LightSalmon", Item::CreateColor(0xffa07a));

        // Category: Orange colors
        item->AddItem("OrangeRed", Item::CreateColor(0xff4500));
        item->AddItem("Tomato", Item::CreateColor(0xff6347));
        item->AddItem("DarkOrange", Item::CreateColor(0xff8c00));
        item->AddItem("Coral", Item::CreateColor(0xff7f50));
        item->AddItem("Orange", Item::CreateColor(0xffa500));

        // Category: Yellow colors
        item->AddItem("DarkKhaki", Item::CreateColor(0xbdb76b));
        item->AddItem("Gold", Item::CreateColor(0xffd700));
        item->AddItem("Khaki", Item::CreateColor(0xf0e68c));
        item->AddItem("PeachPuff", Item::CreateColor(0xffdab9));
        item->AddItem("Yellow", Item::CreateColor(0xffff00));
        item->AddItem("PaleGoldenrod", Item::CreateColor(0xeee8aa));
        item->AddItem("Moccasin", Item::CreateColor(0xffe4b5));
        item->AddItem("PapayaWhip", Item::CreateColor(0xffefd5));
        item->AddItem("LightGoldenrodYellow", Item::CreateColor(0xfafad2));
        item->AddItem("LemonChiffon", Item::CreateColor(0xfffacd));
        item->AddItem("LightYellow", Item::CreateColor(0xffffe0));

        // Category: Brown colors
        item->AddItem("Maroon", Item::CreateColor(0x800000));
        item->AddItem("Brown", Item::CreateColor(0xa52a2a));
        item->AddItem("SaddleBrown", Item::CreateColor(0x8b4513));
        item->AddItem("Sienna", Item::CreateColor(0xa0522d));
        item->AddItem("Chocolate", Item::CreateColor(0xd2691e));
        item->AddItem("DarkGoldenrod", Item::CreateColor(0xb8860b));
        item->AddItem("Peru", Item::CreateColor(0xcd853f));
        item->AddItem("RosyBrown", Item::CreateColor(0xbc8f8f));
        item->AddItem("Goldenrod", Item::CreateColor(0xdaa520));
        item->AddItem("SandyBrown", Item::CreateColor(0xf4a460));
        item->AddItem("Tan", Item::CreateColor(0xd2b48c));
        item->AddItem("Burlywood", Item::CreateColor(0xdeb887));
        item->AddItem("Wheat", Item::CreateColor(0xf5deb3));
        item->AddItem("NavajoWhite", Item::CreateColor(0xffdead));
        item->AddItem("Bisque", Item::CreateColor(0xffe4c4));
        item->AddItem("BlanchedAlmond", Item::CreateColor(0xffebcd));
        item->AddItem("Cornsilk", Item::CreateColor(0xfff8dc));

        // Category: Green Colors
        item->AddItem("DarkGreen", Item::CreateColor(0x006400));
        item->AddItem("Green", Item::CreateColor(0x008000));
        item->AddItem("DarkOliveGreen", Item::CreateColor(0x556b2f));
        item->AddItem("ForestGreen", Item::CreateColor(0x228b22));
        item->AddItem("SeaGreen", Item::CreateColor(0x2e8b57));
        item->AddItem("Olive", Item::CreateColor(0x808000));
        item->AddItem("OliveDrab", Item::CreateColor(0x6b8e23));
        item->AddItem("MediumSeaGreen", Item::CreateColor(0x3cb371));
        item->AddItem("LimeGreen", Item::CreateColor(0x32cd32));
        item->AddItem("Lime", Item::CreateColor(0x00ff00));
        item->AddItem("SpringGreen", Item::CreateColor(0x00ff7f));
        item->AddItem("MediumSpringGreen", Item::CreateColor(0x00fa9a));
        item->AddItem("DarkSeaGreen", Item::CreateColor(0x8fbc8f));
        item->AddItem("MediumAquamarine", Item::CreateColor(0x66cdaa));
        item->AddItem("YellowGreen", Item::CreateColor(0x9acd32));
        item->AddItem("LawnGreen", Item::CreateColor(0x7cfc00));
        item->AddItem("Chartreuse", Item::CreateColor(0x7fff00));
        item->AddItem("LightGreen", Item::CreateColor(0x90ee90));
        item->AddItem("GreenYellow", Item::CreateColor(0xadff2f));
        item->AddItem("PaleGreen", Item::CreateColor(0x98fb98));

        // Category: Cyan colors
        item->AddItem("Teal", Item::CreateColor(0x008080));
        item->AddItem("DarkCyan", Item::CreateColor(0x008b8b));
        item->AddItem("LightSeaGreen", Item::CreateColor(0x20b2aa));
        item->AddItem("CadetBlue", Item::CreateColor(0x5f9ea0));
        item->AddItem("DarkTurquoise", Item::CreateColor(0x00ced1));
        item->AddItem("MediumTurquoise", Item::CreateColor(0x48d1cc));
        item->AddItem("Turquoise", Item::CreateColor(0x40e0d0));
        item->AddItem("Aqua", Item::CreateColor(0x00ffff));
        item->AddItem("Cyan", Item::CreateColor(0x00ffff));
        item->AddItem("Aquamarine", Item::CreateColor(0x7fffd4));
        item->AddItem("PaleTurquoise", Item::CreateColor(0xafeeee));
        item->AddItem("LightCyan", Item::CreateColor(0xe0ffff));

        // Category: Blue colors
        item->AddItem("Navy", Item::CreateColor(0x000080));
        item->AddItem("DarkBlue", Item::CreateColor(0x00008b));
        item->AddItem("MediumBlue", Item::CreateColor(0x0000cd));
        item->AddItem("Blue", Item::CreateColor(0x0000ff));
        item->AddItem("MidnightBlue", Item::CreateColor(0x191970));
        item->AddItem("RoyalBlue", Item::CreateColor(0x4169e1));
        item->AddItem("SteelBlue", Item::CreateColor(0x4682b4));
        item->AddItem("DodgerBlue", Item::CreateColor(0x1e90ff));
        item->AddItem("DeepSkyBlue", Item::CreateColor(0x00bfff));
        item->AddItem("CornflowerBlue", Item::CreateColor(0x6495ed));
        item->AddItem("SkyBlue", Item::CreateColor(0x87ceeb));
        item->AddItem("LightSkyBlue", Item::CreateColor(0x87cefa));
        item->AddItem("LightSteelBlue", Item::CreateColor(0xb0c4de));
        item->AddItem("LightBlue", Item::CreateColor(0xadd8e6));
        item->AddItem("PowderBlue", Item::CreateColor(0xb0e0e6));

        // Category: Purple, violet, and magenta colors
        item->AddItem("Indigo", Item::CreateColor(0x4b0082));
        item->AddItem("Purple", Item::CreateColor(0x800080));
        item->AddItem("DarkMagenta", Item::CreateColor(0x8b008b));
        item->AddItem("DarkViolet", Item::CreateColor(0x9400d3));
        item->AddItem("DarkSlateBlue", Item::CreateColor(0x483d8b));
        item->AddItem("BlueViolet", Item::CreateColor(0x8a2be2));
        item->AddItem("DarkOrchid", Item::CreateColor(0x9932cc));
        item->AddItem("Fuchsia", Item::CreateColor(0xff00ff));
        item->AddItem("Magenta", Item::CreateColor(0xff00ff));
        item->AddItem("SlateBlue", Item::CreateColor(0x6a5acd));
        item->AddItem("MediumSlateBlue", Item::CreateColor(0x7b68ee));
        item->AddItem("MediumOrchid", Item::CreateColor(0xba55d3));
        item->AddItem("MediumPurple", Item::CreateColor(0x9370db));
        item->AddItem("Orchid", Item::CreateColor(0xda70d6));
        item->AddItem("Violet", Item::CreateColor(0xee82ee));
        item->AddItem("Plum", Item::CreateColor(0xdda0dd));
        item->AddItem("Thistle", Item::CreateColor(0xd8bfd8));
        item->AddItem("Lavender", Item::CreateColor(0xe6e6fa));

        // Category: White colors
        item->AddItem("MistyRose", Item::CreateColor(0xffe4e1));
        item->AddItem("AntiqueWhite", Item::CreateColor(0xfaebd7));
        item->AddItem("Linen", Item::CreateColor(0xfaf0e6));
        item->AddItem("Beige", Item::CreateColor(0xf5f5dc));
        item->AddItem("WhiteSmoke", Item::CreateColor(0xf5f5f5));
        item->AddItem("LavenderBlush", Item::CreateColor(0xfff0f5));
        item->AddItem("OldLace", Item::CreateColor(0xfdf5e6));
        item->AddItem("AliceBlue", Item::CreateColor(0xf0f8ff));
        item->AddItem("Seashell", Item::CreateColor(0xfff5ee));
        item->AddItem("GhostWhite", Item::CreateColor(0xf8f8ff));
        item->AddItem("Honeydew", Item::CreateColor(0xf0fff0));
        item->AddItem("FloralWhite", Item::CreateColor(0xfffaf0));
        item->AddItem("Azure", Item::CreateColor(0xf0ffff));
        item->AddItem("MintCream", Item::CreateColor(0xf5fffa));
        item->AddItem("Snow", Item::CreateColor(0xfffafa));
        item->AddItem("Ivory", Item::CreateColor(0xfffff0));
        item->AddItem("White", Item::CreateColor(0xffffff));

        // Category: Gray and black colors
        item->AddItem("Black", Item::CreateColor(0x000000));
        item->AddItem("DarkSlateGray", Item::CreateColor(0x2f4f4f));
        item->AddItem("DimGray", Item::CreateColor(0x696969));
        item->AddItem("SlateGray", Item::CreateColor(0x708090));
        item->AddItem("Gray", Item::CreateColor(0x808080));
        item->AddItem("LightSlateGray", Item::CreateColor(0x778899));
        item->AddItem("DarkGray", Item::CreateColor(0xa9a9a9));
        item->AddItem("Silver", Item::CreateColor(0xc0c0c0));
        item->AddItem("LightGray", Item::CreateColor(0xd3d3d3));
        item->AddItem("Gainsboro", Item::CreateColor(0xdcdcdc));

        interpreter.AddGlobal("WebColors", item);
    }

    void GraphicsExtension::AddRalColors(Interpreter &interpreter) const
    {
        auto item = Item::CreateObject();

        item->AddItem("GreenBeige", Item::CreateColor(0xBEBD7F));
        item->AddItem("Beige", Item::CreateColor(0xC2B078));
        item->AddItem("SandYellow", Item::CreateColor(0xC6A664));
        item->AddItem("SignalYellow", Item::CreateColor(0xE5BE01));
        item->AddItem("GoldenYellow", Item::CreateColor(0xCDA434));
        item->AddItem("HoneyYellow", Item::CreateColor(0xA98307));
        item->AddItem("MaizeYellow", Item::CreateColor(0xE4A010));
        item->AddItem("DaffodilYellow", Item::CreateColor(0xDC9D00));
        item->AddItem("BrownBeige", Item::CreateColor(0x8A6642));
        item->AddItem("LemonYellow", Item::CreateColor(0xC7B446));
        item->AddItem("OysterWhite", Item::CreateColor(0xEAE6CA));
        item->AddItem("Ivory", Item::CreateColor(0xE1CC4F));
        item->AddItem("LightIvory", Item::CreateColor(0xE6D690));
        item->AddItem("SulfurYellow", Item::CreateColor(0xEDFF21));
        item->AddItem("SaffronYellow", Item::CreateColor(0xF5D033));
        item->AddItem("ZincYellow", Item::CreateColor(0xF8F32B));
        item->AddItem("GreyBeige", Item::CreateColor(0x9E9764));
        item->AddItem("OliveYellow", Item::CreateColor(0x999950));
        item->AddItem("RapeYellow", Item::CreateColor(0xF3DA0B));
        item->AddItem("TrafficYellow", Item::CreateColor(0xFAD201));
        item->AddItem("OchreYellow", Item::CreateColor(0xAEA04B));
        item->AddItem("LuminousYellow", Item::CreateColor(0xFFFF00));
        item->AddItem("Curry", Item::CreateColor(0x9D9101));
        item->AddItem("MelonYellow", Item::CreateColor(0xF4A900));
        item->AddItem("BroomYellow", Item::CreateColor(0xD6AE01));
        item->AddItem("DahliaYellow", Item::CreateColor(0xF3A505));
        item->AddItem("PastelYellow", Item::CreateColor(0xEFA94A));
        item->AddItem("PearlBeige", Item::CreateColor(0x6A5D4D));
        item->AddItem("PearlGold", Item::CreateColor(0x705335));
        item->AddItem("SunYellow", Item::CreateColor(0xF39F18));
        item->AddItem("YellowOrange", Item::CreateColor(0xED760E));
        item->AddItem("RedOrange", Item::CreateColor(0xC93C20));
        item->AddItem("Vermilion", Item::CreateColor(0xCB2821));
        item->AddItem("PastelOrange", Item::CreateColor(0xFF7514));
        item->AddItem("PureOrange", Item::CreateColor(0xF44611));
        item->AddItem("LuminousOrange", Item::CreateColor(0xFF2301));
        item->AddItem("LuminousBrightOrange", Item::CreateColor(0xFFA420));
        item->AddItem("BrightRedOrange", Item::CreateColor(0xF75E25));
        item->AddItem("TrafficOrange", Item::CreateColor(0xF54021));
        item->AddItem("SignalOrange", Item::CreateColor(0xD84B20));
        item->AddItem("DeepOrange", Item::CreateColor(0xEC7C26));
        item->AddItem("SalmonRange", Item::CreateColor(0xE55137));
        item->AddItem("PearlOrange", Item::CreateColor(0xC35831));
        item->AddItem("FlameRed", Item::CreateColor(0xAF2B1E));
        item->AddItem("SignalRed", Item::CreateColor(0xA52019));
        item->AddItem("CarmineRed", Item::CreateColor(0xA2231D));
        item->AddItem("RubyRed", Item::CreateColor(0x9B111E));
        item->AddItem("PurpleRed", Item::CreateColor(0x75151E));
        item->AddItem("WineRed", Item::CreateColor(0x5E2129));
        item->AddItem("BlackRed", Item::CreateColor(0x412227));
        item->AddItem("OxideRed", Item::CreateColor(0x642424));
        item->AddItem("BrownRed", Item::CreateColor(0x781F19));
        item->AddItem("BeigeRed", Item::CreateColor(0xC1876B));
        item->AddItem("TomatoRed", Item::CreateColor(0xA12312));
        item->AddItem("AntiquePink", Item::CreateColor(0xD36E70));
        item->AddItem("LightPink", Item::CreateColor(0xEA899A));
        item->AddItem("CoralRed", Item::CreateColor(0xB32821));
        item->AddItem("Rose", Item::CreateColor(0xE63244));
        item->AddItem("StrawberryRed", Item::CreateColor(0xD53032));
        item->AddItem("TrafficRed", Item::CreateColor(0xCC0605));
        item->AddItem("SalmonPink", Item::CreateColor(0xD95030));
        item->AddItem("LuminousRed", Item::CreateColor(0xF80000));
        item->AddItem("LuminousBrightRed", Item::CreateColor(0xFE0000));
        item->AddItem("RaspberryRed", Item::CreateColor(0xC51D34));
        item->AddItem("PureRed", Item::CreateColor(0xCB3234));
        item->AddItem("OrientRed", Item::CreateColor(0xB32428));
        item->AddItem("PearlRubyRed", Item::CreateColor(0x721422));
        item->AddItem("PearlPink", Item::CreateColor(0xB44C43));
        item->AddItem("RedLilac", Item::CreateColor(0x6D3F5B));
        item->AddItem("RedViolet", Item::CreateColor(0x922B3E));
        item->AddItem("HeatherViolet", Item::CreateColor(0xDE4C8A));
        item->AddItem("ClaretViolet", Item::CreateColor(0x641C34));
        item->AddItem("BlueLilac", Item::CreateColor(0x6C4675));
        item->AddItem("TrafficPurple", Item::CreateColor(0xA03472));
        item->AddItem("PurpleViolet", Item::CreateColor(0x4A192C));
        item->AddItem("SignalViolet", Item::CreateColor(0x924E7D));
        item->AddItem("PastelViolet", Item::CreateColor(0xA18594));
        item->AddItem("Telemagenta", Item::CreateColor(0xCF3476));
        item->AddItem("PearlViolet", Item::CreateColor(0x8673A1));
        item->AddItem("PearlBlackBerry", Item::CreateColor(0x6C6874));
        item->AddItem("VioletBlue", Item::CreateColor(0x354D73));
        item->AddItem("GreenBlue", Item::CreateColor(0x1F3438));
        item->AddItem("UltramarineBlue", Item::CreateColor(0x20214F));
        item->AddItem("SaphireBlue", Item::CreateColor(0x1D1E33));
        item->AddItem("BlackBlue", Item::CreateColor(0x18171C));
        item->AddItem("SignalBlue", Item::CreateColor(0x1E2460));
        item->AddItem("BrillantBlue", Item::CreateColor(0x3E5F8A));
        item->AddItem("GreyBlue", Item::CreateColor(0x26252D));
        item->AddItem("AzureBlue", Item::CreateColor(0x025669));
        item->AddItem("GentianBlue", Item::CreateColor(0x0E294B));
        item->AddItem("SteelBlue", Item::CreateColor(0x231A24));
        item->AddItem("LightBlue", Item::CreateColor(0x3B83BD));
        item->AddItem("CobaltBlue", Item::CreateColor(0x1E213D));
        item->AddItem("PigeonBlue", Item::CreateColor(0x606E8C));
        item->AddItem("SkyBlue", Item::CreateColor(0x2271B3));
        item->AddItem("TrafficBlue", Item::CreateColor(0x063971));
        item->AddItem("TurquoiseBlue", Item::CreateColor(0x3F888F));
        item->AddItem("CapriBlue", Item::CreateColor(0x1B5583));
        item->AddItem("OceanBlue", Item::CreateColor(0x1D334A));
        item->AddItem("WaterBlue", Item::CreateColor(0x256D7B));
        item->AddItem("NightBlue", Item::CreateColor(0x252850));
        item->AddItem("DistantBlue", Item::CreateColor(0x49678D));
        item->AddItem("PastelBlue", Item::CreateColor(0x5D9B9B));
        item->AddItem("PearlGentianBlue", Item::CreateColor(0x2A6478));
        item->AddItem("PearlNightBlue", Item::CreateColor(0x102C54));
        item->AddItem("PatinaGreen", Item::CreateColor(0x316650));
        item->AddItem("EmeraldGreen", Item::CreateColor(0x287233));
        item->AddItem("LeafGreen", Item::CreateColor(0x2D572C));
        item->AddItem("OliveGreen", Item::CreateColor(0x424632));
        item->AddItem("BlueGreen", Item::CreateColor(0x1F3A3D));
        item->AddItem("MossGreen", Item::CreateColor(0x2F4538));
        item->AddItem("GreyOlive", Item::CreateColor(0x3E3B32));
        item->AddItem("BottleGreen", Item::CreateColor(0x343B29));
        item->AddItem("BrownGreen", Item::CreateColor(0x39352A));
        item->AddItem("FirGreen", Item::CreateColor(0x31372B));
        item->AddItem("GrassGreen", Item::CreateColor(0x35682D));
        item->AddItem("ResedaGreen", Item::CreateColor(0x587246));
        item->AddItem("BlackGreen", Item::CreateColor(0x343E40));
        item->AddItem("ReedGreen", Item::CreateColor(0x6C7156));
        item->AddItem("YellowOlive", Item::CreateColor(0x47402E));
        item->AddItem("BlackOlive", Item::CreateColor(0x3B3C36));
        item->AddItem("TurquoiseGreen", Item::CreateColor(0x1E5945));
        item->AddItem("MayGreen", Item::CreateColor(0x4C9141));
        item->AddItem("YellowGreen", Item::CreateColor(0x57A639));
        item->AddItem("PastelGreen", Item::CreateColor(0xBDECB6));
        item->AddItem("ChromeGreen", Item::CreateColor(0x2E3A23));
        item->AddItem("PaleGreen", Item::CreateColor(0x89AC76));
        item->AddItem("OliveDrab", Item::CreateColor(0x25221B));
        item->AddItem("TrafficGreen", Item::CreateColor(0x308446));
        item->AddItem("FernGreen", Item::CreateColor(0x3D642D));
        item->AddItem("OpalGreen", Item::CreateColor(0x015D52));
        item->AddItem("LightGreen", Item::CreateColor(0x84C3BE));
        item->AddItem("PineGreen", Item::CreateColor(0x2C5545));
        item->AddItem("MintGreen", Item::CreateColor(0x20603D));
        item->AddItem("SignalGreen", Item::CreateColor(0x317F43));
        item->AddItem("MintTurquoise", Item::CreateColor(0x497E76));
        item->AddItem("PastelTurquoise", Item::CreateColor(0x7FB5B5));
        item->AddItem("PearlGreen", Item::CreateColor(0x1C542D));
        item->AddItem("PearlOpalGreen", Item::CreateColor(0x193737));
        item->AddItem("PureGreen", Item::CreateColor(0x008F39));
        item->AddItem("LuminousGreen", Item::CreateColor(0x00BB2D));
        item->AddItem("SquirrelGrey", Item::CreateColor(0x78858B));
        item->AddItem("SilverGrey", Item::CreateColor(0x8A9597));
        item->AddItem("OliveGrey", Item::CreateColor(0x7E7B52));
        item->AddItem("MossGrey", Item::CreateColor(0x6C7059));
        item->AddItem("SignalGrey", Item::CreateColor(0x969992));
        item->AddItem("MouseGrey", Item::CreateColor(0x646B63));
        item->AddItem("BeigeGrey", Item::CreateColor(0x6D6552));
        item->AddItem("KhakiGrey", Item::CreateColor(0x6A5F31));
        item->AddItem("GreenGrey", Item::CreateColor(0x4D5645));
        item->AddItem("TarpaulinGrey", Item::CreateColor(0x4C514A));
        item->AddItem("IronGrey", Item::CreateColor(0x434B4D));
        item->AddItem("BasaltGrey", Item::CreateColor(0x4E5754));
        item->AddItem("BrownGrey", Item::CreateColor(0x464531));
        item->AddItem("SlateGrey", Item::CreateColor(0x434750));
        item->AddItem("AnthraciteGrey", Item::CreateColor(0x293133));
        item->AddItem("BlackGrey", Item::CreateColor(0x23282B));
        item->AddItem("UmbraGrey", Item::CreateColor(0x332F2C));
        item->AddItem("ConcreteGrey", Item::CreateColor(0x686C5E));
        item->AddItem("GraphiteGrey", Item::CreateColor(0x474A51));
        item->AddItem("GraniteGrey", Item::CreateColor(0x2F353B));
        item->AddItem("StoneGrey", Item::CreateColor(0x8B8C7A));
        item->AddItem("BlueGrey", Item::CreateColor(0x474B4E));
        item->AddItem("PebbleGrey", Item::CreateColor(0xB8B799));
        item->AddItem("CementGrey", Item::CreateColor(0x7D8471));
        item->AddItem("YellowGrey", Item::CreateColor(0x8F8B66));
        item->AddItem("LightGrey", Item::CreateColor(0xCBD0CC));
        item->AddItem("PlatinumGrey", Item::CreateColor(0x7F7679));
        item->AddItem("DustyGrey", Item::CreateColor(0x7D7F7D));
        item->AddItem("AgateGrey", Item::CreateColor(0xB5B8B1));
        item->AddItem("QuartzGrey", Item::CreateColor(0x6C6960));
        item->AddItem("WindowGrey", Item::CreateColor(0x9DA1AA));
        item->AddItem("TrafficGreyA", Item::CreateColor(0x8D948D));
        item->AddItem("TrafficGreyB", Item::CreateColor(0x4E5452));
        item->AddItem("SilkGrey", Item::CreateColor(0xCAC4B0));
        item->AddItem("Telegrey1", Item::CreateColor(0x909090));
        item->AddItem("Telegrey2", Item::CreateColor(0x82898F));
        item->AddItem("Telegrey4", Item::CreateColor(0xD0D0D0));
        item->AddItem("PearlMouseGrey", Item::CreateColor(0x898176));
        item->AddItem("GreenBrown", Item::CreateColor(0x826C34));
        item->AddItem("OchreBrown", Item::CreateColor(0x955F20));
        item->AddItem("SignalBrown", Item::CreateColor(0x6C3B2A));
        item->AddItem("ClayBrown", Item::CreateColor(0x734222));
        item->AddItem("CopperBrown", Item::CreateColor(0x8E402A));
        item->AddItem("FawnBrown", Item::CreateColor(0x59351F));
        item->AddItem("OliveBrown", Item::CreateColor(0x6F4F28));
        item->AddItem("NutBrown", Item::CreateColor(0x5B3A29));
        item->AddItem("RedBrown", Item::CreateColor(0x592321));
        item->AddItem("SepiaBrown", Item::CreateColor(0x382C1E));
        item->AddItem("ChestnutBrown", Item::CreateColor(0x633A34));
        item->AddItem("MahoganyBrown", Item::CreateColor(0x4C2F27));
        item->AddItem("ChocolateBrown", Item::CreateColor(0x45322E));
        item->AddItem("GreyBrown", Item::CreateColor(0x403A3A));
        item->AddItem("BlackBrown", Item::CreateColor(0x212121));
        item->AddItem("OrangeBrown", Item::CreateColor(0xA65E2E));
        item->AddItem("BeigeBrown", Item::CreateColor(0x79553D));
        item->AddItem("PaleBrown", Item::CreateColor(0x755C48));
        item->AddItem("TerraBrown", Item::CreateColor(0x4E3B31));
        item->AddItem("PearlCopper", Item::CreateColor(0x763C28));
        item->AddItem("Cream", Item::CreateColor(0xFDF4E3));
        item->AddItem("GreyWhite", Item::CreateColor(0xE7EBDA));
        item->AddItem("SignalWhite", Item::CreateColor(0xF4F4F4));
        item->AddItem("SignalBlack", Item::CreateColor(0x282828));
        item->AddItem("JetBlack", Item::CreateColor(0x0A0A0A));
        item->AddItem("WhiteAluminium", Item::CreateColor(0xA5A5A5));
        item->AddItem("GreyAluminium", Item::CreateColor(0x8F8F8F));
        item->AddItem("PureWhite", Item::CreateColor(0xFFFFFF));
        item->AddItem("GraphiteBlack", Item::CreateColor(0x1C1C1C));
        item->AddItem("TrafficWhite", Item::CreateColor(0xF6F6F6));
        item->AddItem("TrafficBlack", Item::CreateColor(0x1E1E1E));
        item->AddItem("PapyrusWhite", Item::CreateColor(0xCFD3CD));
        item->AddItem("PearlLightGrey", Item::CreateColor(0x9C9C9C));
        item->AddItem("PearlDarkGrey", Item::CreateColor(0x828282));

        interpreter.AddGlobal("RalColors", item);
    }


} // end of namespace