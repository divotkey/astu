/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Extensions/AudioExtension.h"
#include "Velox/Extensions/InstantBuilder.h"
#include "Velox/Extensions/GraphicsInstants.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/InterpreterFunction.h"
#include "Velox/Interpreter/InterpreterFunctionTwoParameter.h"
#include "Velox/Extensions/ExtensionFunctionNoParameter.h"
#include "Velox/Extensions/ExtensionFunctionOneParameter.h"
#include "Velox/Extensions/ExtensionFunctionTwoParameter.h"
#include "Velox/Extensions/ExtensionFunctionThreeParameter.h"
#include "Velox/Extensions/ExtensionConstructorNoParameter.h"
#include "Velox/Extensions/ExtensionConstructorOneParameter.h"
#include "Velox/Extensions/ExtensionConstructorTwoParameter.h"
#include "Audio/AudioBuffer.h"
#include "AstuAudio.h"

// C++ Standard Library includes
#include <memory>

#define AUDIO_SAMPLES_TYPE     "AudioSamples"

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

    AudioExtension::AudioExtension()
            : VeloxExtension("Graphics Extension")
    {
        // Intentionally left empty.
    }

    void AudioExtension::InjectExtension(Interpreter &interpreter) const
    {
        ObjectTypeBuilder builder;

        builder
                .Reset()
                .TypeName(AUDIO_SAMPLES_TYPE)
                .Constructor(ExtensionConstructorTwoParameter<AudioSamples>::CreateItem(
                        [](ScriptContext &sc, Item &param1, Item &param2, unsigned int lineNumber) {
                            return make_shared<AudioSamples>(param1.GetIntegerValue(lineNumber), param2.GetIntegerValue(lineNumber));
                        }
                ))
                .AddFunction("NumSamples", ExtensionFunctionNoParameter<AudioSamples>::CreateItem(
                        [](ScriptContext &sc, AudioSamples &samples, unsigned int lineNumber) {
                            return Item::CreateInteger(static_cast<int>(samples.NumOfSamples()));
                        }
                ))
                .AddFunction("NumFrames", ExtensionFunctionNoParameter<AudioSamples>::CreateItem(
                        [](ScriptContext &sc, AudioSamples &samples, unsigned int lineNumber) {
                            return Item::CreateInteger(static_cast<int>(samples.NumOfFrames()));
                        }
                ))
                .AddFunction("NumChannels", ExtensionFunctionNoParameter<AudioSamples>::CreateItem(
                        [](ScriptContext &sc, AudioSamples &samples, unsigned int lineNumber) {
                            return Item::CreateInteger(static_cast<int>(samples.NumOfChannels()));
                        }
                ))
                .AddFunction("GetSampleRate", ExtensionFunctionNoParameter<AudioSamples>::CreateItem(
                        [](ScriptContext &sc, AudioSamples &samples, unsigned int lineNumber) {
                            return Item::CreateReal(samples.GetSampleRate());
                        }
                ))
                .AddFunction("AddSample", ExtensionFunctionOneParameter<AudioSamples>::CreateItem(
                        [](ScriptContext &sc, AudioSamples &samples, Item &param, unsigned int lineNumber) {
                            samples.GetSamples().push_back(static_cast<float>(param.GetRealValue(lineNumber)));
                            return Item::CreateUndefined();
                        }
                ))
                .Build(interpreter);

        interpreter.AddGlobalFunction("StoreAudio", make_shared<InterpreterFunctionTwoParameter>(
                [](ScriptContext &sc, std::shared_ptr<Item> param1, std::shared_ptr<Item> param2, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    auto samples = std::dynamic_pointer_cast<AudioSamples>(param1->GetData());
                    if (samples == nullptr) {
                        throw InterpreterError("First parameter must be of type AudioSamples", lineNumber);
                    }

                    try {
                        StoreAudio(*samples, param2->GetStringValue(sc));
                        return Item::CreateUndefined();
                    } catch (std::runtime_error &e) {
                        throw InterpreterError(e.what(), lineNumber);
                    }
                }));
    }

} // end of namespace