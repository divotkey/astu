/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Service/StateService.h"

// C++ Standard Library includes
#include <cassert>
#include <stdexcept>
#include <algorithm>

using namespace std;

namespace astu {

    StateService::StateService()
        : BaseService("State Service")
        , reenter(true)
    {
        // Intentionally left empty.
    }

    void StateService::OnStartup() 
    {
        // Uncomment, when start states do work.
        // if (!startState.empty()) {
        //     SwitchState(startState);
        // }
    }

    void StateService::OnShutdown() 
    {
        // Intentionally left empty.
    }

    StateService& StateService::EnableReenterMode(bool b)
    {
        reenter = b;
        return *this;
    }

    StateService& StateService::AddService(const string & state, shared_ptr<Service> srv)
    {
        if (srv == nullptr) {
            throw logic_error("Unable to add service to state, pointer to service is null");
        }

        if (HasService(state, srv)) {
            throw logic_error("Unable to add service to state, service already added");
        }

        GetOrCreateState(state).push_back(srv);
        return *this;
    }

    bool StateService::HasService(const string & state, shared_ptr<Service> srv) const
    {
        auto it = stateMap.find(state);
        if (it == stateMap.end()) {
            return false;
        }

        auto states = it->second;
        return find(states.begin(), states.end(), srv) != states.end();
    }

    bool StateService::HasState(const string& state) const
    {
        return stateMap.find(state) != stateMap.end();
    }

    StateService& StateService::CreateState(const string & state)
    {
        ValidateStateName(state);
        if (HasState(state)) {
            throw logic_error("Unable to create new state, state name is ambiguous");
        }

        stateMap[state] = State();
        return *this;
    }

    void StateService::SwitchState(const string & state)
    {
        if (!HasState(state)) {
            throw logic_error("Unknown state '" + state + "'");
        }

        if (!reenter && curState == state) {
            return;
        }

        if (HasState(curState)) {
            RemoveServices(stateMap[curState]);
        }

        curState = state;
        AddServices(stateMap[curState]);
    }

    void StateService::ValidateStateName(const string & state) const
    {
        if (state.empty()) {
            throw logic_error("Empty state name not allowed");
        }
    }

    StateService::State & StateService::GetOrCreateState(const string & state)
    {
        if (!HasState(state)) {
            CreateState(state);
        }

        return stateMap[state];
    }

    void StateService::RemoveServices(State & services)
    {
        auto & sm = ASTU_SERVICE_MANAGER();

        // Remove services in reverse order.
        for(auto it = services.rbegin(); it != services.rend(); ++it) {
            sm.RemoveService(*it);
        }
    }

    void StateService::AddServices(State & services)
    {
        auto & sm = ASTU_SERVICE_MANAGER();

        for (auto & srv : services) {
            sm.AddService(srv);
        }
    }

    const string & StateService::GetCurrentState() const
    {
        return curState;
    }

    // void StateService::SetStartState(const string& state)
    // {
    //     if (!HasState(state)) {
    //         throw logic_error("Unknown startup state: " + state);
    //     }

    //     startState = state;
    // }

    // const string& StateService::GetStartState() const
    // {
    //     return startState;
    // }

    bool StateService::OnSignal(const std::string & signal)
    {
        if ( HasState(signal) ) {
            SwitchState(signal);
            return true;
        }

        return false;
    }

} // end of namespace