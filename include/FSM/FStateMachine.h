// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// C++ Standard Library
#include <set>
#include <map>
#include <vector>
#include <functional>

namespace astu {

    class FStateMachine {
    public:
        static const size_t INVALID_HANDLE;

        FStateMachine();

        size_t NumStates() const;
        std::set<char> DetermineSymbols() const;

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
        size_t SetTransition(char symbol, size_t targetState);
        size_t GetTransition(char symbol) const;
        size_t GetTransition(size_t state, char symbol) const;
        void SetEnterFunc(std::function<void (char, void*)> func);
        bool HasTransition(char symbol) const;
        bool HasTransition(size_t state, char symbol) const;
        void Clear();

        void SetFlag(unsigned int bitmask);
        void AddFlag(unsigned int bitmask);
        void ClearFlag(unsigned int bitmask);
        bool IsFlagSet(unsigned int bitmask) const;
        unsigned int GetFlags() const;
        unsigned int GetFlags(size_t state) const;

        void Reset();
        bool Process(char ch, void *context = nullptr);

    private:
        struct State {

            /** Constructor. */
            State() : accepting(false) {}

            /** Whether this state is an accepting state. */
            bool accepting;

            /** The flags of this state. */
            unsigned int flags;

            /** Called when this state is entered. */
            std::function<void (char, void*)> enterFunc;

            bool Enter(char ch, void *context) {
                if (enterFunc)
                    enterFunc(ch, context);

                return accepting;
            }
        };

        std::vector<State> states;
        size_t curState;
        size_t startState;

        std::vector<std::map<char, size_t>> transitionTable;

        void EnsureState() const;
        void EnsureState(size_t state) const;
    };

} // end of namespace

