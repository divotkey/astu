/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/SignalService.h"

// C++ Standard Library includes
#include <stdexcept>
#include <memory>
#include <vector>
#include <map>

namespace astu {

    /**
     * Base class for substates to be managed by the SubstateService.
     *
     * @tparam T the context type of the substate
     * @ingroup srv_group
     */
    template <typename T>
    class Substate {
    public:

        /**
         * Constructor.
         *
         * @param context   the context of this substate
         */
        Substate(T &context) : context(context) {}

        /**
         * Virtual destructor.
         */
        virtual ~Substate() = default;

        /**
         * Called when this substate is entered.
         */
        virtual void OnEnter() {}

        /**
         * Called when this substate is exited.
         */
        virtual void OnExit() {}

    protected:
        /** The context of this substate. */
        T &context;
    };


    /**
     * This base class for service that implement the state pattern.
     * This class manages substates that operate on a context of type T.
     *
     * @tparam T the type of the substate
     * @ingroup srv_group
     */
    template <typename T>
    class SubstateService : public virtual Service {
    public:

        /**
         * Constructor.
         */
        SubstateService() {

            AddStartupHook([this]() {
                SwitchSubstate(startState);
            });

            AddShutdownHook([this]() {
                if (!curState.empty()) {
                    GetSubstate(curState).OnExit();
                    curState.clear();
                }
            });

        }

        /**
         * Virtual destructor.
         */
        virtual ~SubstateService() {}

        /**
         * Sets the start state that gets activated when this service is started.
         *
         * @param name  the name of the start state
         */
        void SetStartState(const std::string &name) {
            startState = name;
        }

        /**
         * Returns the name of the start state.
         *
         * @return the name of the start state or an empty string, if no start state has been specified
         */
        const std::string &GetStartState() const {
            return startState;
        }

        /**
         * Returns the name of the current state.
         *
         * @return the name of the current state or an empty string, if no state is currently active.
         */
        const std::string &GetCurrentSubstate() const {
            return curState;
        }

        /**
         * Adds a substate.
         * @param name      the name of the substate to add
         * @param substate  the substate to add
         * @throws std::logic_error if the specified name is ambiguous or an empty string
         */
        void AddSubstate(const std::string &name, std::unique_ptr<T> substate) {

            if (name.empty()) {
                throw std::logic_error("Substate name must not be empty");
            }

            if (HasSubstate(name)) {
                throw std::logic_error("Unable to add substate, ambiguous substate name '" + name + "'");
            }

            stateMap[name] = std::move(substate);
        }

        /**
         * Tests whether a substate with a specific name hast already been added.
         *
         * @param name  the name that has been added
         * @return `true` if the substate has already been added
         */
        bool HasSubstate(const std::string &name) const {
            return stateMap.find(name) != stateMap.end();
        }

        /**
         * Switches to the specified state
         *
         * @param name  the name of the state
         */
        void SwitchSubstate(const std::string &name) {
            if (name.empty()) {
                if (!curState.empty()) {
                    GetSubstate(curState).OnExit();
                    curState.clear();
                }
            } else {
                auto &nextState = GetSubstate(name);
                if (!curState.empty()) {
                    GetSubstate(curState).OnExit();
                }
                curState = name;
                nextState.OnEnter();
            }
        }

        /**
         * Returns the substate with the specified name.
         *
         * @param name  the name of the substate
         * @return the requested substate
         * @throws std::logic_error in case the substate is unknown
         */
        T& GetSubstate(const std::string &name) {
            auto it = stateMap.find(name);
            if (it == stateMap.end()) {
                throw std::logic_error("Unknown substate '" + name + "'");
            }
            return *(it->second);
        }

    private:
        /** Associates substates with names. */
        std::map<std::string, std::unique_ptr<T>> stateMap;

        /** The name of the current state. */
        std::string curState;

        /** The name of the start state. */
        std::string startState;
    };

} // end of namespace