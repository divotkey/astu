/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "FSM/NStateMachine.h"

// C++ Standard Library
#include <stdexcept>
#include <cassert>
#include <limits>
#include <string>

using namespace std;

namespace astu {

    const size_t NStateMachine::INVALID_HANDLE = numeric_limits<size_t>::max();
    const std::set<size_t> NStateMachine::EMPTY_SET;


    NStateMachine::NStateMachine() : curState(INVALID_HANDLE), startState(INVALID_HANDLE) {
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
        stateNames.clear();
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

    bool NStateMachine::IsAccepting(size_t state) const {
        EnsureState(state);
        return states[state].accepting;
    }

    void NStateMachine::EnsureState() const {
        if (curState == INVALID_HANDLE) {
            throw std::logic_error("Unable to process operation, no active state");
        }
    }

    void NStateMachine::EnsureState(size_t state) const {
        if (state >= states.size()) {
            throw std::logic_error("Invalid state " + to_string(state));
        }
    }

    void NStateMachine::AddTransition(char symbol, size_t targetState) {
        EnsureState();
        AddTransition(curState, symbol, targetState);
    }

    void NStateMachine::AddTransition(size_t sourceState, char symbol, size_t targetState) {
        EnsureState(sourceState);

        auto &transitions = transitionTable[sourceState];
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

    const std::set<char> &NStateMachine::GetSymbols() const {
        return symbols;
    }

    size_t NStateMachine::NumStates() const {
        return states.size();
    }

    void NStateMachine::SetEnterFunc(std::function<void(char, void*)> func) {
        EnsureState();
        states[curState].enterFunc = func;
    }

    size_t NStateMachine::GetStartState() const {
        return startState;
    }

    void NStateMachine::SetStartState(bool b) {
        EnsureState();
        startState = curState;
    }

    bool NStateMachine::IsStartState() const {
        EnsureState();
        return startState == curState;
    }

    const std::set<size_t> &NStateMachine::GetTransitions(char ch) const {
        EnsureState();
        return GetTransitions(curState, ch);
    }

    const std::set<size_t> &NStateMachine::GetTransitions(size_t state, char ch) const {
        EnsureState(state);
        auto &transitions = transitionTable[state];
        auto it = transitions.find(ch);
        if (it == transitions.end()) {
            return EMPTY_SET;
        }

        return it->second;
    }

    std::function<void(char, void *)> NStateMachine::GetEnterFunc(size_t state) const {
        EnsureState(state);
        return states[state].enterFunc;
    }

    std::function<void(char, void *)> NStateMachine::GetEnterFunc() {
        EnsureState();
        return states[curState].enterFunc;
    }

    bool NStateMachine::HasEnterFunc(size_t state) const {
        EnsureState(state);
        return states[state].enterFunc != nullptr;
    }

    bool NStateMachine::HasState(const string &name) const {
        return stateNames.find(name) != stateNames.end();
    }

    size_t NStateMachine::GetState(const string &name) const {
        auto it = stateNames.find(name);
        if (it == stateNames.end()) {
            throw std::logic_error("Unknown state '" + name + "'");
        }

        return it->second;
    }

    void NStateMachine::SetName(const string &name) {
        EnsureState();
        if (HasState(name) && GetState(name) != curState) {
            throw std::logic_error("Ambiguous state name ' + name + '");
        }
        stateNames[name] = curState;
    }

    void NStateMachine::SetFlag(unsigned int bitmask) {
        EnsureState();
        states[curState].flags = bitmask;
    }

    void NStateMachine::AddFlag(unsigned int bitmask) {
        EnsureState();
        states[curState].flags |= bitmask;
    }


    void NStateMachine::ClearFlag(unsigned int bitmask) {
        EnsureState();
        states[curState].flags |= ~bitmask;
    }

    bool NStateMachine::IsFlagSet(unsigned int bitmask) const {
        return GetFlags() & bitmask;
    }

    unsigned int NStateMachine::GetFlags() const {
        EnsureState();
        return GetFlags(curState);
    }

    unsigned int NStateMachine::GetFlags(size_t state) const {
        EnsureState(state);
        return states[state].flags;
    }

    size_t NStateMachine::GetCurrentState() const {
        EnsureState();
        return curState;
    }


} // end of namespace