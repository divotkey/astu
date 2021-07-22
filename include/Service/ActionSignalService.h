/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service.h"
#include "SignalService.h"
#include "InputMappingService.h"

// C++ Standard Library includes
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace astu {

    class ActionSignalService : public astu::BaseService
    {
    public:

        /**
         * Constructor.
         */
        ActionSignalService();

        /**
         * Adds a signal to be sent when a certain action is triggered.
         * 
         * @param action    the action that triggers the signal
         * @param signal    the signal to be sent
         */
        void AddSignal(const std::string& action, const std::string& signal);

        /**
         * Removes the signal to be sent.
         * @param action    the action that triggers the signal
         * @param signal    the signal to be sent
         */
        void RemoveSignal(const std::string& action, const std::string& signal);

    private:
        /** Used to emit signals based on triggered actions. */
        std::shared_ptr<SignalService<std::string>> stringSignals;

        /** Maps actions names to string signals. */
        std::map<std::string, std::string> actionToSignals;

        /** The Action bindings. */
        std::vector<std::shared_ptr<ActionBinding>> bindings;

        /***
         * Handles incoming actions.
         * 
         * @binding the triggered action
         */
        void HandleAction(ActionBinding& binding);

        std::shared_ptr<ActionBinding> FindActionBinding(const std::string &name);

        // Inherited via BaseService
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
    };

} // end of namespace
