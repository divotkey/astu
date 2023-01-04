/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "DStateMachine.h"

// C++ Standard Library
#include <string>
#include <set>

namespace astu {

    class DStateMachineBuilder {
    public:

        DStateMachineBuilder();

        DStateMachineBuilder& BeginState(const std::string& state);
        DStateMachineBuilder& EndState();

        DStateMachineBuilder& AddTransition(const std::string& state, char sym, const std::string& target) {
            BeginState(state);
            AddTransition(sym, target);
            EndState();
            return *this;
        }

        DStateMachineBuilder& AddTransition(char sym, const std::string& target);
        DStateMachineBuilder& Accepting(bool enable);

        DStateMachineBuilder& Reset();

        std::unique_ptr<DStateMachine> Build();

    private:
        using StateTransitions = std::map<char, std::string>;

        std::set<char> symbols;
        std::set<std::string> states;
        std::set<std::string> accepting;
        std::map<std::string, StateTransitions> transitions;
        StateTransitions *curState;
        std::string curStateName;

        void InitializeState(DStateMachine::State &state);
    };

} // end of namespace


