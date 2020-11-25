/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <stdexcept>
#include <algorithm>
#include "StateService.h"

namespace astu {

    StateService::StateService()
        : BaseService("State")
    {
        // Intentionally left empty.
    }

    void StateService::OnStartup() 
    {
        // Intentionally left empty.
    }

    void StateService::OnShutdown() 
    {
        // Intentionally left empty.
    }

    void StateService::AddService(const std::string & state, std::shared_ptr<IService> srv)
    {
        if (srv == nullptr) {
            throw std::logic_error("Unable to add service to state, pointer to service is null");
        }

        if (HasService(state, srv)) {
            throw std::logic_error("Unable to add service to state, service allready added");
        }

        GetOrCreateState(state).push_back(srv);
    }

    bool StateService::HasService(const std::string & state, std::shared_ptr<IService> srv) const
    {
        auto it = stateMap.find(state);
        if (it == stateMap.end()) {
            return false;
        }

        auto states = it->second;
        return std::find(states.begin(), states.end(), srv) != states.end();
    }

    bool StateService::HasState(const std::string& state) const
    {
        return stateMap.find(state) != stateMap.end();
    }

    void StateService::CreateState(const std::string & state)
    {
        ValidateStateName(state);
        if (HasState(state)) {
            throw std::logic_error("Unable to create new state, state name is ambiguous");
        }

        stateMap[state] = State();
    }

    void StateService::SwitchState(const std::string & state)
    {
        if (!HasState(state)) {
            throw std::logic_error("Unknown state '" + state + "'");
        }

        if (HasState(curState)) {
            RemoveServices(stateMap[curState]);
        }

        curState = state;
        AddServices(stateMap[curState]);
    }

    void StateService::ValidateStateName(const std::string & state) const
    {
        if (state.empty()) {
            throw std::logic_error("Empty state name not allowed");
        }
    }

    StateService::State & StateService::GetOrCreateState(const std::string & state)
    {
        if (!HasState(state)) {
            CreateState(state);
        }

        return stateMap[state];
    }

    void StateService::RemoveServices(State & services)
    {
        auto & sm = GetSM();

        // Remove services in reverse order.
        for(auto it = services.rbegin(); it != services.rend(); ++it) {
            sm.RemoveService(*it);
        }
    }

    void StateService::AddServices(State & services)
    {
        auto & sm = GetSM();

        for (auto & srv : services) {
            sm.AddService(srv);
        }
    }

    const std::string & StateService::GetCurrentState() const
    {
        return curState;
    }

} // end of namespace