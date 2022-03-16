// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// C++ Standard Library
#include <string>
#include <vector>
#include <memory>
#include <map>

namespace astu {

    class DStateMachine {
    public:

        DStateMachine(size_t numStates);

        void Reset();
        bool Process(char ch);
        size_t NumStates() const;
        size_t NumSymbols() const;
        char GetSymbol(size_t idx) const;
        const std::string& GetName(size_t idx) const;
        bool IsAccepting(size_t idx) const;
        size_t GetIndex(const std::string & stateName) const;
        size_t GetTarget(size_t idx, char sym) const;

    private:

        struct State {
            std::string name;
            bool accept;
            std::unique_ptr<State*[]> trx;
        };

        std::map<char, size_t> symbolMap;

        std::vector<State> states;
        State *curState;
        State *startState;

        size_t GetSymbolIndex(char sym) const;
        size_t GetIndex(State* state) const;

        friend class DStateMachineBuilder;
    };

} // end of namespace


