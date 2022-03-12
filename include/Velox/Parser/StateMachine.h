// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Trigger.h"

// C++ Standard Library includes
#include <functional>
#include <stdexcept>
#include <memory>
#include <vector>
#include <cassert>
#include <map>

template<typename T>
class StateMachine {
public:

    StateMachine() : curState(nullptr), startState(nullptr) { }

    size_t NumOfStates() const {
        return states.size();
    }

    std::string GetUniqueStateName() const {
        for (size_t i = 0; ;++i) {
            std::string name = std::to_string(states.size() + i);
            if (!HasState(name)) {
                return name;
            }
        }
    }

    bool Process(const T &value) {
        assert(curState);

        const Transition *triggered = curState->GetFirstTrigger(value);
        if (triggered) {
            curState->Exit(value);
            curState = triggered->target;
            curState->Enter(value);
            return true;
        }

        return false;
    }

    void SetFlags(const std::string &name, unsigned int bitmask) {
        GetState(name).flags |= bitmask;
    }

    void ClearFlags(const std::string &name, unsigned int bitmask) {
        GetState(name).flags &= ~bitmask;
    }

    bool IsSet(const std::string& name, unsigned int bitmask) {
        return GetState(name).flags & bitmask;
    }

    unsigned int GetFlags(const std::string &name) {
        return GetState(name).flags;
    }


    void SetCurrentState(const std::string &name) {
        VerifyState(name);
        curState = &states[name];
    }

    const std::string& GetCurrentState() const {
        assert(curState);
        return curState->name;
    }

    void SetStartState(const std::string & name) {
        VerifyState(name);
        startState = &states[name];
    }

    const std::string& GetStartState() const {
        if (!startState) {
            throw std::logic_error("No start state defined");
        }
        return startState->name;
    }

    void Restart() {
        curState = startState;
    }

    void AddState(const std::string& name, std::function<void (const T &input)> enterFunc = nullptr) {
        if (HasState(name)) {
            throw std::logic_error("Ambiguous state name '" + name + "'");
        }

        auto &state = states[name];
        state.name = name;
        state.enterFunc = enterFunc;
    }

    bool HasState(const std::string& name) const {
        return states.find(name) != states.end();
    }

    void SetEnterFunc(const std::string& name, std::function<void (const T &input)> func) {
        VerifyState(name);
        states[name].enterFunc = func;
    }

    void SetExitFunc(const std::string& name, std::function<void (const T &input)> func) {
        VerifyState(name);
        states[name].exitFunc = func;
    }

    void AddTransition(const std::string &from, const std::string &to, std::shared_ptr<Trigger<T>> trigger) {
        GetState(from).transitions.push_back(Transition(trigger, &GetState(to)));
    }

    bool HasTransition(const std::string &stateName, const T &value) {
        auto &state = GetState(stateName);
        return state.GetFirstTrigger(value) != nullptr;
    }

    size_t NumTransitions(const std::string& stateName) const {
        return GetState(stateName).transitions.size();
    }

    std::shared_ptr<Trigger<T>> GetTrigger(const std::string& stateName, size_t idx) {
        return GetState(stateName).transitions.at(idx).trigger;
    }

    bool IsLeaf(const std::string &stateName) const {
        auto &state = GetState(stateName);
        return state.transitions.empty();
    }

    bool IsLeaf() const {
        assert(curState);
        return curState->transitions.empty();
    }

    const std::string& GetTarget(const std::string &stateName, const T &value) {
        auto &state = GetState(stateName);
        const Transition *triggered = state.GetFirstTrigger(value);
        if (!triggered) {
            return state.name;
        } else {
            return triggered->target->name;
        }
    }

private:

    // Forward declaration
    struct State;

    struct Transition {
        Transition(std::shared_ptr<Trigger<T>> trigger, State *target)
            : trigger(trigger), target(target) {}

        std::shared_ptr<Trigger<T>> trigger;

        State *target;
    };

    struct State {
        std::function<void (const T &input)> enterFunc;
        std::function<void (const T &input)> exitFunc;
        std::vector<Transition> transitions;
        std::string name;
        unsigned int flags;

        State() : flags(0) {}

        void Enter(const T& value) {
            if (enterFunc) {
                enterFunc(value);
            }
        }

        void Exit(const T& value) {
            if (exitFunc) {
                exitFunc(value);
            }
        }

        const Transition *GetFirstTrigger(const T &value) {
            for (const auto &t : transitions) {
                if (t.trigger->Evaluate(value)) {
                    return &t;
                }
            }
            return nullptr;
        }

    };

    std::map<std::string, State> states;
    State *curState;
    State* startState;

    State &GetState(const std::string &name) {
        auto it = states.find(name);
        if (it == states.end()) {
            throw std::logic_error("Unknown state '" + name + "'");
        }

        return it->second;
    }

    const State &GetState(const std::string &name) const {
        auto it = states.find(name);
        if (it == states.end()) {
            throw std::logic_error("Unknown state '" + name + "'");
        }

        return it->second;
    }


    void VerifyState(const std::string& name) const {
        if (!HasState(name)) {
            throw std::logic_error("Unknown state '" + name + "'");
        }
    }
};


