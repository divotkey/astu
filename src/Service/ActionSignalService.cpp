/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Service/ActionSignalService.h"

// C++ Standard Library includes
#include <cassert>
#include <algorithm>

using namespace std;

namespace astu {

    ActionSignalService::ActionSignalService()
        : Service("Action Signal Service")
    {
        // Intentionally left empty.
    }

    void ActionSignalService::OnStartup()
    {
        stringSignals = ASTU_GET_SERVICE(SignalService<string>);

        auto& inputMapping = ASTU_SERVICE(InputMappingService);
        for (auto it : actionToSignals) {
            bindings.push_back(
                inputMapping.BindAction(
                    it.first, [this](ActionBinding& binding) {
                    HandleAction(binding); 
                })
            );
        }
    }

    void ActionSignalService::OnShutdown() 
    {
        auto& inputMapping = ASTU_SERVICE(InputMappingService);
        for (auto & binding : bindings) {
            inputMapping.RemoveActionBinding(binding);
        }
        bindings.clear();

        stringSignals = nullptr;
    }

    void ActionSignalService::HandleAction(ActionBinding& binding)
    {
        if ( !binding.IsPressed() ) {
            return;
        }

        auto it = actionToSignals.find(binding.GetAction());
        if (it != actionToSignals.end()) {
            stringSignals->QueueSignal(it->second);
        }
    }

    void ActionSignalService::AddSignal(const string& action, const string& signal)
    {
        RemoveSignal(action, signal);

        actionToSignals[action] = signal;
        if ( IsStarted() ) {
            bindings.push_back(
                ASTU_SERVICE(InputMappingService).BindAction(
                    action, [this](ActionBinding& binding) {
                    HandleAction(binding); 
                }));
        }
    }

    void ActionSignalService::RemoveSignal(const std::string& action, const std::string& signal)
    {
        auto binding = FindActionBinding(action);
        if (binding) {
            bindings.erase(
                remove(bindings.begin(), bindings.end(), binding), 
                bindings.end()
                );

            assert(IsStarted());

            ASTU_SERVICE(InputMappingService).RemoveActionBinding(binding);
        }

        actionToSignals.erase(action);
    }

    shared_ptr<ActionBinding> ActionSignalService::FindActionBinding(const string &name)
    {
        for (auto binding : bindings) {
            if (binding->GetAction() == name) {
                return binding;
            }
        }

        return nullptr;
    }


} // end of namespace