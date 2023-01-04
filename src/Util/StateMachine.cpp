/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Util/StateMachine.h"
#include "Util/StringUtils.h"

// C++ Standard Library includes
#include <stdexcept>

using namespace std;

namespace astu {

    void StateMachine::BuildDeterministic(StateMachine &outSm) const {
        outSm.ClearStates();
        outSm.ClearInputSymbols();
        outSm.AddInputSymbols(GetInputSymbols());

        outSm.CreateState().substates.insert(&GetStartState());

        size_t numStates;
        size_t numTransitions;
        do {
            numStates = outSm.NumOfStates();
            numTransitions = outSm.NumTransitions();

            std::vector<State*> workList;
            for (auto & state : outSm.states) {
                workList.push_back(state.get());
            }

            for (auto state : workList) {
                for (const char ch : inputSymbols) {
                    std::set<const State*> newState;
                    for (const auto & substate : state->substates) {
                        for (size_t i = 0; i < substate->GetNumTransitions(ch); ++i) {
                            newState.insert(&substate->GetTarget(ch, i));
                        }
                    }

                    if (!outSm.HasState(newState)) {
                        auto &s = outSm.CreateState();
                        s.substates = newState;
                        if (!state->HasTransition(ch, s)) {
                            state->AddTransition(ch, s);
                        }
                    } else {
                        auto &s = outSm.GetState(newState);
                        if (!state->HasTransition(ch, s)) {
                            state->AddTransition(ch, s);
                        }
                    }
                }
            }
        } while(numStates != outSm.NumOfStates() || numTransitions != outSm.NumTransitions());

        // Clean up
        for (auto &state : outSm.states) {

            state->SetTerminal(false);
            for (auto substate : state->substates) {
                state->origins.push_back(substate->GetName());
                if (substate->IsTerminal()) {
                    state->SetTerminal(true);
                }
                state->enterFuncs.insert(state->enterFuncs.end(), substate->enterFuncs.begin(), substate->enterFuncs.end());
            }
            state->substates.clear();
        }
    }

    StateMachine::StateMachine() : startState(nullptr), curState(nullptr) {
        // Intentionally left empty.
    }

    size_t StateMachine::NumOfStates() const {
        return states.size();
    }

    StateMachine::State &StateMachine::CreateState() {
        return CreateState("q" + to_string(NumOfStates()));
    }

    StateMachine::State &StateMachine::CreateState(const string &name) {
        if (HasState(name)) {
            throw std::logic_error("Ambiguous state name '" + name + "'");
        }

        State *newState = new State(name);
        states.push_back(unique_ptr<State>(newState));
        if (!startState) {
            startState = newState;
        }

        return *newState;
    }

    StateMachine::State &StateMachine::GetState(size_t idx) {
        return *states.at(idx);
    }

    const StateMachine::State &StateMachine::GetState(size_t idx) const {
        return *states.at(idx);
    }

    void StateMachine::AddInputSymbol(char symbol) {
        inputSymbols.insert(symbol);
    }

    void StateMachine::AddInputSymbols(const std::set<char> symbols) {
        inputSymbols.insert(symbols.begin(), symbols.end());
    }

    bool StateMachine::HasInputSymbol(char symbol) const {
        return inputSymbols.find(symbol) != inputSymbols.end();
    }

    const std::set<char> &StateMachine::GetInputSymbols() const {
        return inputSymbols;
    }

    void StateMachine::ClearStates() {
        states.clear();
        startState = nullptr;
    }

    void StateMachine::ClearInputSymbols() {
        inputSymbols.clear();
    }

    void StateMachine::SetStartState(StateMachine::State &state) {
        startState = &state;
    }

    StateMachine::State &StateMachine::GetStartState() {
        if (!startState)
            throw std::logic_error("No start state defined");

        return *startState;
    }

    const StateMachine::State &StateMachine::GetStartState() const {
        if (!startState)
            throw std::logic_error("No start state defined");

        return *startState;
    }

    bool StateMachine::HasState(const string &name) const {
        for (auto & state : states) {
            if (state->GetName() == name)
                return true;
        }

        return false;
    }

    size_t StateMachine::NumTransitions() const {
        size_t sum = 0;
        for (const auto &state : states) {
            sum += state->GetNumTransitions();
        }
        return sum;
    }

    bool StateMachine::HasState(const std::set<const State*> &substates) const {
        for (const auto &state: states) {
            if (state->substates == substates) {
                return true;
            }
        }

        return false;
    }

    StateMachine::State &StateMachine::GetState(const set<const State *> &substates) {
        for (const auto &state: states) {
            if (state->substates == substates) {
                return *state;
            }
        }

        throw std::logic_error("No state with specified sub states found");
    }

    void StateMachine::Restart() {
        curState = &GetStartState();
    }

    bool StateMachine::Process(char input) {
        if (!curState) {
            Restart();
        }

        size_t n = curState->GetNumTransitions(input);
        if (n > 1) {
            throw std::runtime_error("Non deterministic state " + curState->GetName() + " with input '" + input + "'");
        } else if (n == 0) {
            return false;
        }

        curState = &curState->GetTarget(input, 0);
        curState->Enter(input);

        return true;
    }

    const StateMachine::State &StateMachine::GetCurrentState() const {
        return *curState;
    }

    void StateMachine::DetermineInputSymbols() {
        inputSymbols.clear();

        for (auto &state : states) {
            for (auto it : state->transitions) {
                inputSymbols.insert(it.first);
            }
        }
    }

    /////////////////////////////////////////////////
    /////// Inner class State
    /////////////////////////////////////////////////

    void StateMachine::State::AddTransition(char input, State &target) {
        if (HasTransition(input, target)) {
            throw std::logic_error("Transition already added");
        }

        transitions[input].push_back(&target);
    }

    bool StateMachine::State::HasTransition(char input, State& target) const {
        auto it = transitions.find(input);

        if (it == transitions.end())
            return false;

        for (State *targetState : it->second) {
            if (targetState == &target) {
                return true;
            }
        }

        return false;
    }

    size_t StateMachine::State::GetNumTransitions(char input) const {
        auto it = transitions.find(input);

        if (it == transitions.end())
            return 0;

        return it->second.size();
    }

    size_t StateMachine::State::GetNumTransitions() const {
        size_t sum = 0;
        for (const auto & lst : transitions) {
            sum += lst.second.size();
        }

        return sum;
    }

    StateMachine::State &StateMachine::State::GetTarget(char symbol, size_t idx) {
        auto it = transitions.find(symbol);
        if (it == transitions.end()) {
            throw std::out_of_range("No transitions for this symbol");
        }

        return *it->second.at(idx);
    }

    const StateMachine::State &StateMachine::State::GetTarget(char symbol, size_t idx) const {
        auto it = transitions.find(symbol);
        if (it == transitions.end()) {
            throw std::out_of_range("No transitions for this symbol");
        }

        return *it->second.at(idx);
    }

    string StateMachine::State::GetTargetSetAsString(char symbol) const {
        string result("{");
        auto it = transitions.find(symbol);

        if (it != transitions.end()) {
            for (auto it2 = it->second.cbegin(), end = it->second.cend(); it2 != end;) {
                result += (**it2).GetName();
                if (++it2 != end) {
                    result += ", ";
                }
            }
        }

        result += '}';
        return result;
    }

} // end of namespace