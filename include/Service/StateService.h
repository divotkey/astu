/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "SignalService.h"

// C++ Standard Library includes
#include <memory>
#include <vector>
#include <string>
#include <map>

#define ASTU_ADD_TO_STATE(state, srv) ASTU_SERVICE(StateService).AddService(state, srv)
#define ASTU_CREATE_AND_ADD_TO_STATE(state, srvType, ...) ASTU_ADD_TO_STATE(state, std::make_shared<srvType>(__VA_ARGS__) )

namespace astu {

    /**
     * The state service manages a set of states.
     * 
     * Each of these states contains a list of services that are added to the
     * active services when the state is activated. As soon as the state is
     * changed, the services of the current state are removed and those of
     * the new state are added accordingly.
     * 
     * @ingroup srv_group
     */
    class StateService final 
        : virtual public Service
        , private SignalListener<std::string>
    {
    public:

        /**
         * Constructor.
         */
        StateService();

        /**
         * Virtual destructor.
         */
        ~StateService() {}

        /**
         * Enables or disables reenter mode.
         * 
         * If reenter mode is enabled, state switches to the current state
         * will exit and re-enter the state. If reenter mode is disabled,
         * state switches the to current state will be ignored.
         * 
         * @param b whether to enable reenter mode
         * @return a reference to this service for method chaining
         */
        StateService& EnableReenterMode(bool b);

        /**
         * Returns whether reenter mode is enabled.
         * 
         * @return `true` if reenter mode is enabled
         */
        bool IsReenterMode() const {
            return reenter;
        }

        /**
         * Adds a service to a state.
         * 
         * If the state does not yet exist, it will be created.
         * 
         * @param state the name of the state
         * @param srv   the service to add
         * @return a reference to this service for method chaining
         * @throws std::logic_error in case the service has already been added
         */
        StateService& AddService(const std::string & state, std::shared_ptr<Service> srv);

        /**
         * Tests whether a service has already been added to a state.
         * 
         * @param state the name of the state
         * @param srv   the service to add
         * @return `true` if the service has already been added
         */
        bool HasService(const std::string & state, std::shared_ptr<Service> srv) const;

        /**
         * Tests whether a state exists.
         * 
         * @param state the name of the state
         * @return `true` if the state exists
         */
        bool HasState(const std::string& state) const;

        /**
         * Creates a new empty state.
         * 
         * @param state the name of the state
         * @return a reference to this service for method chaining
         * @throws std::logic_error in case the state name is ambiguous
         */
        StateService& CreateState(const std::string & state);

        /**
         * Switches to a certain state.
         * 
         * @param state the name of the state to switch
         * @throws std::logic_error in case the state is unknown
         */
        void SwitchState(const std::string & state);

        /**
         * Returns the name of the current state.
         * 
         * @return the current state name
         */
        const std::string & GetCurrentState() const;

        // Due to the current implementation of ServiceManager, start-states
        // do not work.

        // /*
        //  * Specifies a state to be used on startup.
        //  * 
        //  * @param state the name of the startup state
        //  */
        // void SetStartState(const std::string& state);

        // /*
        //  * Returns the state to be used on startup.
        //  * 
        //  * @return the name of the startup state
        //  */
        // const std::string& GetStartState() const;

    protected:

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        using State = std::vector<std::shared_ptr<Service>>;

        /** Associates names to states. */
        std::map<std::string, State> stateMap;

        /** The current state. */
        std::string curState;

        /** The state to be used on startup. */
        std::string startState;

        /** Whether switching to current state will re-enter the state. */
        bool reenter;


        void ValidateStateName(const std::string & state) const;
        State & GetOrCreateState(const std::string & state);

        void RemoveServices(State & services);
        void AddServices(State & services);

        // Inherited via SignalListener
        virtual bool OnSignal(const std::string & signal) override;         
    };

} // end of namespace