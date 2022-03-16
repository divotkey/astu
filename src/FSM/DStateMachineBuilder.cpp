/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "FSM/DStateMachineBuilder.h"
#include "Util/StringUtils.h"

// C++ Standard Library
#include <cassert>

using namespace std;

namespace astu {

    DStateMachineBuilder::DStateMachineBuilder() {
        Reset();
    }

    DStateMachineBuilder &DStateMachineBuilder::Reset() {
        curState = nullptr;
        transitions.clear();
        symbols.clear();
        states.clear();
        return *this;
    }

    DStateMachineBuilder & DStateMachineBuilder::AddTransition(char sym, const string &target) {
        if (!curState) {
            throw std::logic_error("Attempt to add transition without BeginState()");
        }

        auto it = curState->find(sym);
        if (it != curState->end()) {
            throw std::logic_error("Transition already added");
        }
        (*curState)[sym] = target;

        symbols.insert(sym);
        states.insert(target);
        return *this;
    }

    DStateMachineBuilder &DStateMachineBuilder::BeginState(const string &state) {
        if (curState) {
            throw std::logic_error("BeginState() without EndState()");
        }

        auto it = transitions.find(state);
        if (it == transitions.end()) {
            curState = &transitions[state];
        } else {
            curState = &it->second;
        }
        states.insert(state);
        curStateName = state;
        return *this;
    }

    DStateMachineBuilder &DStateMachineBuilder::EndState() {
        curState = nullptr;
        curStateName.clear();
        return *this;
    }

    DStateMachineBuilder &DStateMachineBuilder::Accepting(bool enable) {
        if (!curState) {
            throw std::logic_error("Attempt to set state to accepting without BeginState()");
        }
        assert(!curStateName.empty());
        accepting.insert(curStateName);
        return *this;
    }

    std::unique_ptr<DStateMachine> DStateMachineBuilder::Build() {
        auto result = make_unique<DStateMachine>(states.size());

        // Initialize symbol to index map.
        size_t idx = 0;
        for (char sym : symbols) {
            result->symbolMap[sym] = idx++;
        }

        // Initialize state names.
        size_t i = 0;
        for (auto &stateName : states) {
            result->states[i++].name = stateName;
        }

        // Initialize transition tables.
        for (auto &state : result->states) {
            assert(transitions.find(state.name) != transitions.end());
            std::map<char, std::string> symMap = transitions[state.name];

            state.trx = make_unique<DStateMachine::State*[]>(symbols.size());
            for (char symbol : symbols) {
                auto it = symMap.find(symbol);
                if (it == symMap.end()) {
                    throw std::logic_error("State '" + state.name + "' is missing transition for symbol " +
                                           StringUtils::ToPrintable(symbol));
                }
                state.trx[result->GetSymbolIndex(symbol)] = &result->states[result->GetIndex(it->second)];
            }

            if (accepting.find(state.name) != accepting.end()) {
                state.accept = true;
            }
        }

        return result;
    }

} // end of namespace