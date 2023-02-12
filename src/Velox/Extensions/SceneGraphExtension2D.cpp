/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Extensions/SceneGraphExtension2D.h"
#include "Velox/Extensions/InstantBuilder.h"
#include "Velox/Extensions/GraphicsInstants.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/InterpreterFunction.h"
#include "Velox/Interpreter/InterpreterFunctionNoParameter.h"
#include "Velox/Interpreter/InterpreterFunctionOneParameter.h"
#include "Velox/Interpreter/InterpreterFunctionTwoParameter.h"
#include "Velox/Extensions/ExtensionFunctionNoParameter.h"
#include "Velox/Extensions/ExtensionFunctionOneParameter.h"
#include "Velox/Extensions/ExtensionFunctionTwoParameter.h"
#include "Velox/Extensions/ExtensionConstructorNoParameter.h"
#include "Velox/Extensions/ExtensionConstructorOneParameter.h"
#include "Velox/Extensions/ExtensionConstructorTwoParameter.h"
#include "Suite2D/Scene.h"
#include "Suite2D/CommandService2D.h"

// C++ Standard Library includes
#include <memory>

#define SPATIAL_2D_TYPE                 "Spatial2D"
#define NODE_2D_TYPE                    "Node2D"
#define POLYLINE_2D_TYPE                "Polyline2D"
#define TEXTURE_2D_TYPE                 "Texture2D"
#define SPRITE_2D_TYPE                  "Sprite2D"
#define VERTEX_BUFFER_BUILDER_2D_TYPE   "VertexBufferBuilder2D"
#define VERTEX_BUFFER_2D_TYPE           "VertexBuffer2D"

#define F_VECTOR(a) Vector2f(static_cast<float>(a.x), static_cast<float>(a.y))

using namespace astu::suite2d;
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


    /////////////////////////////////////////////////
    /////// Graphics Extension
    /////////////////////////////////////////////////

    SceneGraphExtension2D::SceneGraphExtension2D()
            : VeloxExtension("Scene Graph Extension (2D)")
    {
        // Intentionally left empty.
    }

    void SceneGraphExtension2D::InjectExtension(Interpreter &interpreter) const
    {
        if (!cmdSrv) {
            cmdSrv = ASTU_GET_SERVICE(CommandService2D);
        }

        if (!sceneGraph) {
            sceneGraph = ASTU_GET_SERVICE(SceneGraph);
        }

        if (!vtxBufferBuilder) {
            vtxBufferBuilder = ASTU_GET_SERVICE(VertexBufferBuilder2f);
        }

        if (!progressSignalSrv) {
            progressSignalSrv = ASTU_GET_SERVICE(ProgressSignalService);
        }

        AddCameraFunctions(interpreter);

        interpreter.AddGlobalFunction("AttachSpatial2D", make_shared<InterpreterFunctionTwoParameter>(
                [this](ScriptContext &sc, std::shared_ptr<Item> param1, std::shared_ptr<Item> param2, unsigned int lineNumber) -> shared_ptr<Item> {
                    auto spatial = dynamic_pointer_cast<Spatial>(param1->GetData());
                    if (!spatial) {
                        throw InterpreterError("AttachSpatial2D requires a spatial as first parameter", lineNumber);
                    }

                    if (param2->IsUndefined()) {
                        cmdSrv->AttachToScene(spatial->Clone());
                    } else {
                        cmdSrv->AttachToScene(spatial, param2->GetStringValue(sc));
                    }
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("SetProgress", make_shared<InterpreterFunctionOneParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> shared_ptr<Item> {
                    progressSignalSrv->QueueSignalTs(ProgressSignal(param->GetRealValue(lineNumber)));
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("RemoveSpatial2D", make_shared<InterpreterFunctionOneParameter>(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->RemoveSpatial(param->GetStringValue(sc));
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("ClearScene2D", make_shared<InterpreterFunctionOneParameter>(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->ClearScene();
                    return Item::CreateUndefined();
                }));

        ObjectTypeBuilder builder;

        builder
                .Reset()
                .TypeName(SPATIAL_2D_TYPE)
                .Constructor(ExtensionConstructorNoParameter<Spatial>::CreateItem(
                        [](ScriptContext &sc, unsigned int lineNumber) {
                            throw InterpreterError("Base class Spatial2D cannot be created directly, create Node2D, "
                                                   "Polyline2D, Sprite2D, etc. instead.");
                            return nullptr;
                        }
                ))
                ;

        AddCommonSpatialFunctions(builder);
        builder.Build(interpreter);

        builder
                .Reset()
                .TypeName(NODE_2D_TYPE)
                .Constructor(ExtensionConstructorNoParameter<Node>::CreateItem(
                        [](ScriptContext &sc, unsigned int lineNumber) {
                            return make_shared<Node>();
                        }
                ))
                .AddFunction("DetachAll", ExtensionFunctionNoParameter<Node>::CreateItem(
                        [](ScriptContext &sc, Node &node, unsigned int lineNumber) {
                            node.DetachAll();
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("AttachChild", ExtensionFunctionOneParameter<Node>::CreateItem(
                        [](ScriptContext &sc, Node &node, Item &param, unsigned int lineNumber) {
                            auto child = dynamic_pointer_cast<Spatial>(param.GetData());
                            if (!child) {
                                throw InterpreterError("Spatial2D expected as parameter for AttachChild", lineNumber);
                            }
                            node.AttachChild(child);
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("DetachChild", ExtensionFunctionOneParameter<Node>::CreateItem(
                        [](ScriptContext &sc, Node &node, Item &param, unsigned int lineNumber) {
                            auto child = dynamic_pointer_cast<Spatial>(param.GetData());
                            if (!child) {
                                throw InterpreterError("Spatial2D expected as parameter for DetachChild", lineNumber);
                            }
                            node.DetachChild(child);
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("FindChild", ExtensionFunctionOneParameter<Node>::CreateItem(
                        [](ScriptContext &sc, Node &node, Item &param, unsigned int lineNumber) {
                            auto child = node.FindChildOrNull(param.GetStringValue(sc));
                            if (!child) {
                                throw InterpreterError("No child node '" + param.GetStringValue(sc) + "' found");
                            }
                            auto result = sc.FindObjectType(SPATIAL_2D_TYPE)->CreateObject(sc);
                            result->SetData(child);
                            return result;
                        }
                ))
                ;

        AddCommonSpatialFunctions(builder);
        builder.Build(interpreter);

        builder
                .Reset()
                .TypeName(POLYLINE_2D_TYPE)
                .Constructor(ExtensionConstructorOneParameter<Polyline>::CreateItem(
                        [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
                            auto vtxBuffer = dynamic_pointer_cast<VertexBuffer2f>(param.GetData());
                            if (!vtxBuffer) {
                                throw InterpreterError("VertexBuffer2D expected as parameter", lineNumber);
                            }
                            return make_shared<Polyline>(vtxBuffer);
                        }
                ))
                .AddFunction("SetColor", ExtensionFunctionOneParameter<Polyline>::CreateItem(
                        [](ScriptContext &sc, Polyline &polyline, Item &param, unsigned int lineNumber) {
                            auto c = param.GetColorValue();
                            polyline.SetColor(Color4f(
                                    static_cast<float>(c.r),
                                    static_cast<float>(c.g),
                                    static_cast<float>(c.b),
                                    static_cast<float>(c.a)));

                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("GetColor", ExtensionFunctionNoParameter<Polyline>::CreateItem(
                        [](ScriptContext &sc, Polyline &polyline, unsigned int lineNumber) {
                            auto c = polyline.GetColor();
                            return Item::CreateColor(
                                    Color4d(static_cast<double>(c.r),
                                            static_cast<double>(c.g),
                                            static_cast<double>(c.b),
                                            static_cast<double>(c.a)));
                        }
                ))
                .AddFunction("IsClosed", ExtensionFunctionNoParameter<Polyline>::CreateItem(
                        [](ScriptContext &sc, Polyline &polyline, unsigned int lineNumber) {
                            return Item::CreateBoolean(polyline.IsClosed());
                        }
                ))
                .AddFunction("SetClosed", ExtensionFunctionOneParameter<Polyline>::CreateItem(
                        [](ScriptContext &sc, Polyline &polyline, Item &param, unsigned int lineNumber) {
                            polyline.SetClosed(param.GetBooleanValue(lineNumber));
                            return Item::CreateUndefined();
                        }
                ))
                ;

        AddCommonSpatialFunctions(builder);
        builder.Build(interpreter);

        //builder
        //        .Reset()
        //        .TypeName(TEXTURE_2D_TYPE)
        //        .Constructor(ExtensionConstructorOneParameter<Sprite>::CreateItem(
        //                [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
        //                    //auto vtxBuffer = dynamic_pointer_cast<VertexBuffer2f>(param.GetData());
        //                    //if (!vtxBuffer) {
        //                    //    throw InterpreterError("VertexBuffer2D expected as parameter", lineNumber);
        //                    //}
        //                    return make_shared<Sprite>(nullptr);
        //                }
        //        ))
        //        .AddFunction("GetWidth", ExtensionFunctionNoParameter<Texture>::CreateItem(
        //                [](ScriptContext &sc, Texture &texture, unsigned int lineNumber) {
        //                    return Item::CreateInteger(texture.GetWidth());
        //                }
        //        ))
        //        .AddFunction("GetHeight", ExtensionFunctionNoParameter<Texture>::CreateItem(
        //                [](ScriptContext &sc, Texture &texture, unsigned int lineNumber) {
        //                    return Item::CreateInteger(texture.GetHeight());
        //                }
        //        ))
        //        .Build(interpreter);

        //builder
        //        .Reset()
        //        .TypeName(SPRITE_2D_TYPE)
        //        .Constructor(ExtensionConstructorOneParameter<Sprite>::CreateItem(
        //                [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
        //                    //auto vtxBuffer = dynamic_pointer_cast<VertexBuffer2f>(param.GetData());
        //                    //if (!vtxBuffer) {
        //                    //    throw InterpreterError("VertexBuffer2D expected as parameter", lineNumber);
        //                    //}
        //                    return make_shared<Sprite>(nullptr);
        //                }
        //        ))
        //        .AddFunction("SetWidth", ExtensionFunctionOneParameter<Sprite>::CreateItem(
        //                [](ScriptContext &sc, Sprite &sprite, Item &param, unsigned int lineNumber) {
        //                    auto w = param.GetRealValue(lineNumber);
        //                    if ( w <= 0) {
        //                        throw InterpreterError("Width of sprite must be greater zero, got " + to_string(w), lineNumber);
        //                    }
        //                    sprite.SetWidth(static_cast<float>(w));
        //                    return Item::CreateUndefined();
        //                }
        //        ))
        //        .AddFunction("SetHeight", ExtensionFunctionOneParameter<Sprite>::CreateItem(
        //                [](ScriptContext &sc, Sprite &sprite, Item &param, unsigned int lineNumber) {
        //                    auto h = param.GetRealValue(lineNumber);
        //                    if ( h <= 0) {
        //                        throw InterpreterError("Height of sprite must be greater zero, got " + to_string(h), lineNumber);
        //                    }
        //                    sprite.SetHeight(static_cast<float>(h));
        //                    return Item::CreateUndefined();
        //                }
        //        ))
        //        ;
        //
        //AddCommonSpatialFunctions(builder);
        //builder.Build(interpreter);

        builder
                .Reset()
                .TypeName(VERTEX_BUFFER_2D_TYPE)
                .Constructor(ExtensionConstructorNoParameter<VertexBuffer2f>::CreateItem(
                        [](ScriptContext &sc, unsigned int lineNumber) {
                            throw InterpreterError("VertexBuffer2D objects cannot be created directly, use "
                                                   "VertexBufferBuilder2D instead", lineNumber);

                            return nullptr;
                        }
                ))
                .AddFunction("NumVertices", ExtensionFunctionNoParameter<VertexBuffer2f>::CreateItem(
                        [](ScriptContext &sc, VertexBuffer2f &vtxBuffer, unsigned int lineNumber) {
                            return Item::CreateInteger(static_cast<int>(vtxBuffer.NumVertices()));
                        }
                ))
                .Build(interpreter);

        builder
                .Reset()
                .TypeName(VERTEX_BUFFER_BUILDER_2D_TYPE)
                .Constructor(ExtensionConstructorNoParameter<VertexBufferBuilder2f>::CreateItem(
                        [this](ScriptContext &sc, unsigned int lineNumber) {
                            vtxBufferBuilder->Reset();
                            return vtxBufferBuilder;
                        }
                ))
                .AddFunction("AddVertex", ExtensionFunctionTwoParameter<VertexBufferBuilder2f>::CreateItem(
                        [](ScriptContext &sc, VertexBufferBuilder2f &vtxBuilder, Item &param1, Item &param2, unsigned int lineNumber) {
                            if (param2.IsUndefined()) {
                                auto v = param1.GetVector2Value();
                                vtxBuilder.AddVertex(static_cast<float>(v.x), static_cast<float>(v.y));
                            } else {
                                vtxBuilder.AddVertex(
                                        static_cast<float>(param1.GetRealValue(lineNumber)),
                                        static_cast<float>(param2.GetRealValue(lineNumber)));
                            }
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("Reset", ExtensionFunctionNoParameter<VertexBufferBuilder2f>::CreateItem(
                        [](ScriptContext &sc, VertexBufferBuilder2f &vtxBuilder, unsigned int lineNumber) {
                            vtxBuilder.Reset();
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("Build", ExtensionFunctionNoParameter<VertexBufferBuilder2f>::CreateItem(
                        [](ScriptContext &sc, VertexBufferBuilder2f &vtxBuilder, unsigned int lineNumber) {
                            auto result = sc.FindObjectType(VERTEX_BUFFER_2D_TYPE)->CreateObject(sc);
                            result->SetData(vtxBuilder.Build());
                            return result;
                        }
                ))
                .Build(interpreter);
                ;
    }

    void SceneGraphExtension2D::AddCommonSpatialFunctions(ObjectTypeBuilder &builder) const
    {
        builder
            .AddFunction("GetName", ExtensionFunctionNoParameter<Spatial>::CreateItem(
                    [](ScriptContext &sc, Spatial &spatial, unsigned int lineNumber) {
                        return Item::CreateString(spatial.GetName());
                    }
            ))
            .AddFunction("SetName", ExtensionFunctionOneParameter<Spatial>::CreateItem(
                    [](ScriptContext &sc, Spatial &spatial, Item &param, unsigned int lineNumber) {
                        spatial.SetName(param.GetStringValue(sc));
                        return Item::CreateUndefined();
                    }
            ))
            .AddFunction("SetVisible", ExtensionFunctionOneParameter<Spatial>::CreateItem(
                    [](ScriptContext &sc, Spatial &spatial, Item &param, unsigned int lineNumber) {
                        spatial.SetVisible(param.GetBooleanValue(lineNumber));
                        return Item::CreateUndefined();
                    }
            ))
            .AddFunction("IsVisible", ExtensionFunctionNoParameter<Spatial>::CreateItem(
                    [](ScriptContext &sc, Spatial &spatial, unsigned int lineNumber) {
                        return Item::CreateBoolean(spatial.IsVisible());
                    }
            ))
            .AddFunction("GetTransparency", ExtensionFunctionNoParameter<Spatial>::CreateItem(
                    [](ScriptContext &sc, Spatial &spatial, unsigned int lineNumber) {
                        return Item::CreateReal(spatial.GetTransparency());
                    }
            ))
            .AddFunction("SetTransparency", ExtensionFunctionOneParameter<Spatial>::CreateItem(
                    [](ScriptContext &sc, Spatial &spatial, Item &param, unsigned int lineNumber) {
                        spatial.SetTransparency(static_cast<float>(param.GetRealValue(lineNumber)));
                        return Item::CreateUndefined();
                    }
            ))
            ;
    }

    void SceneGraphExtension2D::AddCameraFunctions(Interpreter &interpreter) const
    {
        interpreter.AddGlobalFunction("SetCameraPos", make_shared<InterpreterFunctionTwoParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> shared_ptr<Item> {
                    if (param2->IsUndefined()) {
                        cmdSrv->SetCameraPos(F_VECTOR(param1->GetVector2Value()));
                    } else {
                        cmdSrv->SetCameraPos(
                                Vector2f(static_cast<float>(param1->GetRealValue(lineNumber)),
                                         static_cast<float>(param2->GetRealValue(lineNumber))));
                    }
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("SetCameraHomePos", make_shared<InterpreterFunctionTwoParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> shared_ptr<Item> {
                    if (param2->IsUndefined()) {
                        cmdSrv->SetCameraHomePos(F_VECTOR(param1->GetVector2Value()));
                    } else {
                        cmdSrv->SetCameraHomePos(
                                Vector2f(static_cast<float>(param1->GetRealValue(lineNumber)),
                                         static_cast<float>(param2->GetRealValue(lineNumber))));
                    }
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("SetCameraZoom", make_shared<InterpreterFunctionOneParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->SetCameraZoom(static_cast<float>(param->GetRealValue(lineNumber)));
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("SetCameraHomeZoom", make_shared<InterpreterFunctionOneParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->SetCameraHomeZoom(static_cast<float>(param->GetRealValue(lineNumber)));
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("ShowFixedWidth", make_shared<InterpreterFunctionOneParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->ShowFixedWidth(static_cast<float>(param->GetRealValue(lineNumber)));
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("ShowFixedHeight", make_shared<InterpreterFunctionOneParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->ShowFixedHeight(static_cast<float>(param->GetRealValue(lineNumber)));
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("ShowScreenSpace", make_shared<InterpreterFunctionNoParameter>(
                [this](ScriptContext &sc, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->ShowScreenSpace();
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("ShowFitting", make_shared<InterpreterFunctionTwoParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->ShowFitting(static_cast<float>(param1->GetRealValue(lineNumber)), static_cast<float>(param2->GetRealValue(lineNumber)));
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("ShowFilling", make_shared<InterpreterFunctionTwoParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->ShowFilling(static_cast<float>(param1->GetRealValue(lineNumber)), static_cast<float>(param2->GetRealValue(lineNumber)));
                    return Item::CreateUndefined();
                }));

        interpreter.AddGlobalFunction("ShowStretched", make_shared<InterpreterFunctionTwoParameter>(
                [this](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> shared_ptr<Item> {
                    cmdSrv->ShowStretched(static_cast<float>(param1->GetRealValue(lineNumber)), static_cast<float>(param2->GetRealValue(lineNumber)));
                    return Item::CreateUndefined();
                }));
    }

} // end of namespace