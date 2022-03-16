/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library
#include <set>
#include <map>
#include <vector>
#include <functional>

namespace astu {

    class NStateMachine {
    public:
        static const size_t INVALID_HANDLE;

        NStateMachine();
        const std::set<char> &GetSymbols();

        size_t NumStates();

        size_t CreateState();
        size_t BeginState();
        size_t BeginState(size_t state);
        void EndState();
        void SetAccepting(bool b);
        bool IsAccepting() const;
        void AddTransition(char symbol, size_t targetState);
        void SetEnterFunc(std::function<void (char ch)> func);
        bool HasTransition(char symbol, size_t targetState) const;
        void Clear();
        void Reset();

    private:
        struct State {
            State() : accepting(false) {}
            bool accepting;
            std::function<void (char ch)> enterFunc;
        };

        std::set<char> symbols;
        std::vector<State> states;
        size_t curState;

        std::vector<std::map<char, std::set<size_t>>> transitionTable;

        void EnsureState() const;
    };

} // end of namespace


