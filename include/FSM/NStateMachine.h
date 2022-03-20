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
#include <string>
#include <functional>

namespace astu {

    class NStateMachine {
    public:
        static const size_t INVALID_HANDLE;
        static const std::set<size_t> EMPTY_SET;

        NStateMachine();
        const std::set<char> &GetSymbols() const;

        size_t NumStates() const;

        size_t CreateState();
        size_t BeginState();
        size_t BeginState(size_t state);
        void EndState();
        void SetAccepting(bool b);
        bool IsAccepting() const;
        bool IsAccepting(size_t state) const;
        void SetStartState(bool b);
        bool IsStartState() const;
        size_t GetStartState() const;
        void AddTransition(char symbol, size_t targetState);
        const std::set<size_t> &GetTransitions(char ch) const;
        const std::set<size_t> &GetTransitions(size_t state, char ch) const;
        void SetEnterFunc(std::function<void (char, void *)> func);
        std::function<void (char, void *)> GetEnterFunc();
        std::function<void (char, void *)> GetEnterFunc(size_t state) const;
        bool HasEnterFunc(size_t state) const;
        bool HasTransition(char symbol, size_t targetState) const;

        size_t GetState(const std::string &name) const;
        bool HasState(const std::string &name) const;
        void SetName(const std::string &name);

        void SetFlag(unsigned int bitmask);
        void AddFlag(unsigned int bitmask);
        void ClearFlag(unsigned int bitmask);
        bool IsFlagSet(unsigned int bitmask) const;
        unsigned int GetFlags() const;
        unsigned int GetFlags(size_t state) const;

        void Clear();
        void Reset();

    private:
        struct State {

            /** Constructor. */
            State() : accepting(false), flags(0) {}

            /** Whether this state is an accepting state. */
            bool accepting;

            /** The flags of this state. */
            unsigned int flags;

            /** Called when this state is entered. */
            std::function<void (char, void*)> enterFunc;
        };

        std::set<char> symbols;
        std::vector<State> states;
        size_t curState;
        size_t startState;

        std::vector<std::map<char, std::set<size_t>>> transitionTable;

        /** Stores names of named states. */
        std::map<std::string, size_t> stateNames;

        void EnsureState() const;
        void EnsureState(size_t state) const;
    };

} // end of namespace


