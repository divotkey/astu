// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "FSM/NStateMachine.h"

// C++ Standard Library
#include <stdexcept>
#include <cassert>
#include <limits>
#include <string>

using namespace std;

namespace astu {

    const size_t NStateMachine::INVALID_HANDLE = numeric_limits<size_t>::max();

    NStateMachine::NStateMachine() : curState(INVALID_HANDLE) {
        // Intentionally left empty.
    }

    size_t NStateMachine::CreateState() {
        assert(states.size() == transitionTable.size());

        states.resize(states.size() + 1);
        transitionTable.resize(transitionTable.size() + 1);

        assert(states.size() == transitionTable.size());

        return states.size() - 1;
    }

    size_t NStateMachine::BeginState() {
        return curState = CreateState();
    }

    void NStateMachine::EndState() {
        curState = INVALID_HANDLE;
    }

    size_t NStateMachine::BeginState(size_t state) {
        if (state >= states.size()) {
            throw std::logic_error("Unable to begin state, invalid state handle: " + to_string(state));
        }

        size_t prevState = curState;
        curState = state;
        return prevState;
    }

    void NStateMachine::Clear() {
        states.clear();
        transitionTable.clear();
        symbols.clear();
        curState = INVALID_HANDLE;
    }

    void NStateMachine::SetAccepting(bool b) {
        EnsureState();
        states[curState].accepting = b;
    }

    bool NStateMachine::IsAccepting() const {
        EnsureState();
        return states[curState].accepting;
    }

    void NStateMachine::EnsureState() const {
        if (curState == INVALID_HANDLE) {
            throw std::logic_error("Unable to process operation, no active state");
        }
    }

    void NStateMachine::AddTransition(char symbol, size_t targetState) {
        EnsureState();

        auto &transitions = transitionTable[curState];
        transitions[symbol].insert(targetState);
        symbols.insert(symbol);
    }

    bool NStateMachine::HasTransition(char symbol, size_t targetState) const {
        EnsureState();

        const auto &transitions = transitionTable[curState];
        auto it = transitions.find(symbol);
        if (it == transitions.end())
            return false;

        return it->second.find(targetState) != it->second.end();
    }

    const std::set<char> &NStateMachine::GetSymbols() {
        return symbols;
    }

    size_t NStateMachine::NumStates() {
        return states.size();
    }

    void NStateMachine::SetEnterFunc(std::function<void(char)> func) {
        EnsureState();
        states[curState].enterFunc = func;
    }

} // end of namespace