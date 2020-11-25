/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include "Service.h"

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
    class StateService : public BaseService {
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
         * Adds a service to a state.
         * 
         * If the state does not yet exist, it will be created.
         * 
         * @param state the name of the state
         * @param srv   the service to add
         * @throws std::logic_error in case the service has already beed added
         */
        void AddService(const std::string & state, std::shared_ptr<IService> srv);

        /**
         * Tests whether a service has already been added to a state.
         * 
         * @param state the name of the state
         * @param srv   the service to add
         * @return `true` if the service has already been added
         */
        bool HasService(const std::string & state, std::shared_ptr<IService> srv) const;

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
         * @throws std::logic_error in case the state name is ambiguous
         */
        void CreateState(const std::string & state);

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

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        using State = std::vector<std::shared_ptr<IService>>;

        /** Associates names to states. */
        std::map<std::string, State> stateMap;

        /** The current state. */
        std::string curState;


        void ValidateStateName(const std::string & state) const;
        State & GetOrCreateState(const std::string & state);

        void RemoveServices(State & services);
        void AddServices(State & services);
    };

} // end of namespace