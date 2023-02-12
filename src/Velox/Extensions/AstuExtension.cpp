/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Extensions/AstuExtensions.h"
#include "Velox/Extensions/InstantBuilder.h"
#include "Velox/Extensions/GraphicsInstants.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/InterpreterFunction.h"
#include "Velox/Extensions/ExtensionFunctionNoParameter.h"
#include "Velox/Extensions/ExtensionFunctionOneParameter.h"
#include "Velox/Extensions/ExtensionFunctionTwoParameter.h"
#include "Velox/Extensions/ExtensionConstructorNoParameter.h"
#include "Velox/Extensions/ExtensionConstructorOneParameter.h"
#include "Velox/Extensions/ExtensionConstructorTwoParameter.h"
#include "Util/StringUtils.h"
#include "Util/Timer.h"
#include "Graphics/ObjExporter.h"


// C++ Standard Library includes
#include <memory>
#include <fstream>

using namespace velox;
using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// Extension classes
    /////////////////////////////////////////////////

    // Special cases requires dedicated classes instead of using the generic constructor and function templates.
    // These classes will be placed here.


    class TextFileProxy : public velox::ItemData {
    public:

        TextFileProxy(const std::string& filename)
            : ofs(filename.c_str(), std::ios::out)
        {

        }

        std::ofstream ofs;
    };

    /////////////////////////////////////////////////
    /////// Utility functions
    /////////////////////////////////////////////////


    /////////////////////////////////////////////////
    /////// Graphics Extension
    /////////////////////////////////////////////////

    AstuExtension::AstuExtension()
            : VeloxExtension("ASTU Extension")
    {
        // Intentionally left empty.
    }

    void AstuExtension::InjectExtension(Interpreter &interpreter) const
    {
        ObjectTypeBuilder builder;

        builder
                .Reset()
                .TypeName("Timer")
                .Constructor(ExtensionConstructorOneParameter<Timer>::CreateItem(
                        [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
                            return make_shared<Timer>();
                        }
                ))
                .AddFunction("Start", ExtensionFunctionNoParameter<Timer>::CreateItem(
                        [](ScriptContext &sc, Timer &timer, unsigned int lineNumber) {
                            timer.Start();
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("Stop", ExtensionFunctionNoParameter<Timer>::CreateItem(
                        [](ScriptContext &sc, Timer &timer, unsigned int lineNumber) {
                            timer.Stop();
                            return Item::CreateUndefined();
                        }
                ))
                .AddFunction("IsRunning", ExtensionFunctionNoParameter<Timer>::CreateItem(
                        [](ScriptContext &sc, Timer &timer, unsigned int lineNumber) {
                            return Item::CreateBoolean(timer.IsRunning());
                        }
                ))
                .AddFunction("GetMilliseconds", ExtensionFunctionNoParameter<Timer>::CreateItem(
                        [](ScriptContext &sc, Timer &timer, unsigned int lineNumber) {
                            return Item::CreateInteger(static_cast<int>(timer.GetMilliseconds()));
                        }
                ))
                .AddFunction("GetMicroseconds", ExtensionFunctionNoParameter<Timer>::CreateItem(
                        [](ScriptContext &sc, Timer &timer, unsigned int lineNumber) {
                            return Item::CreateInteger(static_cast<int>(timer.GetMicroseconds()));
                        }
                ))
                .AddFunction("GetNanoseconds", ExtensionFunctionNoParameter<Timer>::CreateItem(
                        [](ScriptContext &sc, Timer &timer, unsigned int lineNumber) {
                            return Item::CreateInteger(static_cast<int>(timer.GetNanoseconds()));
                        }
                ))
                .AddFunction("ToString", ExtensionFunctionNoParameter<Timer>::CreateItem(
                        [](ScriptContext &sc, Timer &timer, unsigned int lineNumber) {
                            return Item::CreateString(StringUtils::DurationToString(timer.GetNanoseconds(), true));
                        }
                ))
                .Build(interpreter);

        builder
            .Reset()
            .TypeName("TextOutputFile")
            .Constructor(ExtensionConstructorOneParameter<TextFileProxy>::CreateItem(
                    [](ScriptContext &sc, Item &param, unsigned int lineNumber) {
                        return make_shared<TextFileProxy>(param.GetStringValue(sc));
                    }
            ))
            .AddFunction("Write", ExtensionFunctionOneParameter<TextFileProxy>::CreateItem(
                    [](ScriptContext &sc, TextFileProxy &proxy, Item &param, unsigned int lineNumber) {
                        proxy.ofs << param.GetStringValue(sc);
                        return Item::CreateUndefined();
                    }
            ))
            .AddFunction("WriteLine", ExtensionFunctionOneParameter<TextFileProxy>::CreateItem(
                    [](ScriptContext &sc, TextFileProxy &proxy, Item &param, unsigned int lineNumber) {
                        proxy.ofs << param.GetStringValue(sc) << std::endl;

                        return Item::CreateUndefined();
                    }
            ))
            .AddFunction("Close", ExtensionFunctionNoParameter<TextFileProxy>::CreateItem(
                    [](ScriptContext &sc, TextFileProxy &proxy, unsigned int lineNumber) {
                        proxy.ofs.close();
                        return Item::CreateUndefined();
                    }
            ))
            .Build(interpreter);
    }

} // end of namespace