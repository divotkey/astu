/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "FSM/DStateMachine.h"

// C++ Standard Library
#include <cassert>
#include <vector>

using namespace std;

namespace astu {

    DStateMachine::DStateMachine(size_t numStates)
        : states(numStates)
        , curState(nullptr)
        , startState(nullptr)
    {
        // Intentionally left empty.
    }

    bool DStateMachine::Process(char sym) {
        size_t idx = GetSymbolIndex(sym);
        assert(curState && idx < symbolMap.size());
        curState = curState->trx[idx];
        assert(curState);
        return curState->accept;
    }

    size_t DStateMachine::GetSymbolIndex(char sym) const {
        auto it = symbolMap.find(sym);
        if (it == symbolMap.end()) {
            throw std::logic_error("Illegal symbol");
        }
        return it->second;
    }

    void DStateMachine::Reset() {
        assert(startState);
        curState = startState;
    }

    size_t DStateMachine::NumStates() const {
        return states.size();
    }

    const std::string &DStateMachine::GetName(size_t idx) const {
        return states.at(idx).name;
    }

    size_t DStateMachine::GetTarget(size_t idx, char sym) const {
        const State &state = states.at(idx);
        assert(symbolMap.size() > GetSymbolIndex(sym));
        return GetIndex(state.trx[GetSymbolIndex(sym)]);
    }

    size_t DStateMachine::GetIndex(DStateMachine::State *state) const {
        assert(state);

        for (auto it = states.cbegin(); it != states.cend(); ++it) {
            if (&*it == state)
                return it - states.cbegin();
        }

        assert(false);
        return states.size();
    }

    size_t DStateMachine::GetIndex(const string &stateName) const {
        size_t idx = 0;
        for (auto it = states.cbegin(); it != states.cend(); ++it) {
            if (it->name == stateName)
                return idx;
            ++idx;
        }

        throw std::out_of_range("Unable to retrieve index, unknown state: '" + stateName + "'");
    }

    size_t DStateMachine::NumSymbols() const {
        return symbolMap.size();
    }

    char DStateMachine::GetSymbol(size_t idx) const {
        size_t i = 0;
        for (auto it : symbolMap) {
            if (i == idx)
                return it.first;

            ++i;
        }

        throw std::out_of_range("invalid Symbol index " + to_string(idx));
    }

    bool DStateMachine::IsAccepting(size_t idx) const {
        return states.at(idx).accept;
    }

} // end of namespace
