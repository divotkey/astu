// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "FSM/FStateMachine.h"
#include "Util/StringUtils.h"

// C++ Standard Library
#include <stdexcept>
#include <cassert>
#include <limits>
#include <string>

using namespace std;

namespace astu {

    const size_t FStateMachine::INVALID_HANDLE = numeric_limits<size_t>::max();

    FStateMachine::FStateMachine() : curState(INVALID_HANDLE), startState(INVALID_HANDLE) {
        // Intentionally left empty.
    }

    size_t FStateMachine::CreateState() {
        assert(states.size() == transitionTable.size());

        states.resize(states.size() + 1);
        transitionTable.resize(transitionTable.size() + 1);

        assert(states.size() == transitionTable.size());

        return states.size() - 1;
    }

    size_t FStateMachine::BeginState() {
        return curState = CreateState();
    }

    void FStateMachine::EndState() {
        curState = INVALID_HANDLE;
    }

    size_t FStateMachine::BeginState(size_t state) {
        if (state >= states.size()) {
            throw std::logic_error("Unable to begin state, invalid state handle: " + to_string(state));
        }

        size_t prevState = curState;
        curState = state;
        return prevState;
    }

    void FStateMachine::Clear() {
        states.clear();
        transitionTable.clear();
        curState = startState = INVALID_HANDLE;
    }

    void FStateMachine::SetAccepting(bool b) {
        EnsureState();
        states[curState].accepting = b;
    }

    bool FStateMachine::IsAccepting() const {
        EnsureState();
        return states[curState].accepting;
    }

    void FStateMachine::EnsureState() const {
        if (curState == INVALID_HANDLE) {
            throw std::logic_error("Unable to process operation, no active state");
        }
    }

    size_t FStateMachine::SetTransition(char symbol, size_t targetState) {
        EnsureState();

        auto &transitions = transitionTable[curState];
        auto it = transitions.find(symbol);
        size_t prev = it == transitions.end() ? INVALID_HANDLE : it->second;
        transitions[symbol] = targetState;

        return prev;
    }

    size_t FStateMachine::GetTransition(char symbol) const {
        EnsureState();
        const auto &transitions = transitionTable[curState];
        auto it = transitions.find(symbol);

        if (it == transitions.end())
            return INVALID_HANDLE;

        return it->second;
    }

    bool FStateMachine::HasTransition(char symbol) const {
        return GetTransition(symbol) != INVALID_HANDLE;
    }

    bool FStateMachine::HasTransition(size_t state, char symbol) const {
        return GetTransition(state, symbol) != INVALID_HANDLE;
    }

    size_t FStateMachine::NumStates() const {
        return states.size();
    }

    void FStateMachine::SetEnterFunc(std::function<void(char, void *)> func) {
        EnsureState();
        states[curState].enterFunc = func;
    }

    void FStateMachine::Reset() {
        curState = startState;
    }

    bool FStateMachine::Process(char ch, void *context) {
        assert(curState < states.size());

        auto &transitions = transitionTable[curState];
        auto it = transitions.find(ch);
        if (it == transitions.end()) {
            throw std::logic_error("No transition found for character " + StringUtils::ToPrintable(ch));
        }

        curState = it->second;
        assert(curState < states.size());
        return states[curState].Enter(ch, context);
    }

    size_t FStateMachine::GetStartState() const {
        return startState;
    }

    void FStateMachine::SetStartState(bool b) {
        EnsureState();
        startState = curState;
    }

    bool FStateMachine::IsStartState() const {
        EnsureState();
        return startState == curState;
    }

    std::set<char> FStateMachine::DetermineSymbols() const {
        set<char> symbols;
        for (const auto &transitions : transitionTable) {
            for (auto it : transitions) {
                symbols.insert(it.first);
            }
        }

        return symbols;
    }

    bool FStateMachine::IsAccepting(size_t state) const {
        EnsureState(state);
        return states[state].accepting;
    }

    void FStateMachine::EnsureState(size_t state) const {
        if (state >= states.size()) {
            throw std::logic_error("Invalid state " + to_string(state));
        }
    }

    size_t FStateMachine::GetTransition(size_t state, char symbol) const {
        EnsureState(state);
        auto &transitions = transitionTable[state];
        auto it = transitions.find(symbol);
        if (it == transitions.end()) {
            return INVALID_HANDLE;
        }
        return it->second;
    }

    void FStateMachine::SetFlag(unsigned int bitmask) {
        EnsureState();
        states[curState].flags = bitmask;
    }

    void FStateMachine::AddFlag(unsigned int bitmask) {
        EnsureState();
        states[curState].flags |= bitmask;
    }

    void FStateMachine::ClearFlag(unsigned int bitmask) {
        EnsureState();
        states[curState].flags |= ~bitmask;
    }

    bool FStateMachine::IsFlagSet(unsigned int bitmask) const {
        return GetFlags() & bitmask;
    }

    unsigned int FStateMachine::GetFlags() const {
        EnsureState();
        return GetFlags(curState);
    }

    unsigned int FStateMachine::GetFlags(size_t state) const {
        EnsureState(state);
        return states[state].flags;
    }


} // end of namespace


