/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */
#pragma once

// C++ Standard Library includes
#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include <functional>

namespace astu {

    class StateMachine {
    public:

        /** Constructor. */
        StateMachine();

        /** Deleted copy-constructor. */
        StateMachine(const StateMachine &other) = delete;

        /** Deleted assignment operator. */
        StateMachine &operator=(const StateMachine &) = delete;

        /**
         * Builds a deterministic state machine based on this state machine.
         *
         * @param outSm receives the deterministic state machine
         */
        void BuildDeterministic(StateMachine& outSm) const;

        /**
         * Represents a state within this state machine.
         */
        class State {
        public:

            /**
             * Returns the name of this state.
             *
             * @return this state's name
             */
            const std::string &GetName() const {
                return name;
            }

            bool IsTerminal() const {
                return terminal;
            }

            void SetTerminal(bool t) {
                terminal = t;
            }

            /**
             * Adda a new transition
             *
             * @param input     the input symbol
             * @param target    the target state
             * @throws std::logic_error in case the transition already exists
             */
            void AddTransition(char input, State& target);

            /**
             * Tests whether a transition already exists.
             *
             * @param input     the input symbol
             * @param target    the target state
             * @return `true` if this transition has already been added
             */
            bool HasTransition(char input, State& target) const;

            /**
             * Returns the number of transitions triggered by the specified input.
             *
             * @param input the input symbol
             * @return the number of transitions
             */
            size_t GetNumTransitions(char input) const;

            /**
             * Returns the total number of transitions.
             *
             * @return this state's total number of transitions
             */
            size_t GetNumTransitions() const;

            /**
             * Returns the target state for the specified symbol.
             *
             * @param symbol    the symbol which triggers the transition
             * @param idx       the index of the transition
             * @return the requested transition
             * @throws std::out_of_range exception in case the specified index is invalid
             */
            State& GetTarget(char symbol, size_t idx = 0);

            /**
             * Returns the target state for the specified symbol.
             *
             * @param symbol    the symbol which triggers the transition
             * @param idx       the index of the transition
             * @return the requested transition
             * @throws std::out_of_range exception in case the specified index is invalid
             */
            const State& GetTarget(char symbol, size_t idx = 0) const;

            /**
             * Returns the set of target states for the specified symbol as string.
             *
             * @param symbol    the symbol for the target set.
             * @return the string containing the set of target states
             */
            std::string GetTargetSetAsString(char symbol) const;

            const std::vector<std::string> &GetOrigins() const {
                return origins;
            }

            void SetEnterFunc(std::function<void (char ch)> f) {
                enterFuncs.clear();
                enterFuncs.push_back(f);
            }

        private:
            /** The name of this state. */
            std::string name;

            /** The transitions of this state. */
            std::map<char, std::vector<State*>> transitions;

            /** Used to create deterministic state machines. */
            std::set<const State*> substates;

            /** The names of the substates, used to create this state. */
            std::vector<std::string> origins;

            /** Private constructor. */
            State(const std::string& name) : name(name), terminal(false) {}

            bool terminal;

            std::vector<std::function<void (char ch)>> enterFuncs;

            void Enter(char ch) {
                for (auto & f : enterFuncs) {
                    f(ch);
                }
            }

            friend class StateMachine;
        };

        void SetStartState(State& state);

        State& GetStartState();

        const State& GetStartState() const;
        const State& GetCurrentState() const;

        bool Process(char input);
        void Restart();

        /**
         * Removes all states from this state machine.
         */
        void ClearStates();

        /**
         * Returns the number of states.
         *
         * @return the number of states.
         */
        size_t NumOfStates() const;

        /**
         * Returns to total number of transitions.
         *
         * @return sum of total transitions of all states
         */
        size_t NumTransitions() const;

        /**
         * Returns the state with the specified index.
         *
         * @param idx   the index of the state to return
         * @return the requested state.
         * @throws std::out_of_range exception in case the specified index is invalid
         */
        State& GetState(size_t idx);

        /**
         * Returns the state with the specified index.
         *
         * @param idx   the index of the state to return
         * @return the requested state.
         * @throws std::out_of_range exception in case the specified index is invalid
         */
        const State& GetState(size_t idx) const;

        /**
         * Creates a new state.
         *
         * @return the newly created state.
         */
        State& CreateState();

        /**
         * Creates a new state.
         *
         * @param name  the unique name of the new state
         * @return the newly created state
         */
        State& CreateState(const std::string &name);

        /**
         * Tests whether a state with the specified name exists.
         *
         * @param name  the name to test
         * @return `true` if the a state with this name exists
         */
        bool HasState(const std::string &name) const;

        /**
         * Adds the specified symbol to the set of accepted input symbols.
         *
         * @param symbol    the input symbol to add
         */
        void AddInputSymbol(char symbol);

        /**
         * Adds the specified set of symbols to the set of accepted input symbols.
         *
         * @param symbols   the input symbols to add
         */
        void AddInputSymbols(const std::set<char> symbols);

        void DetermineInputSymbols();

        /**
         * Tests whether a certain input symbol is in the set of accepted input symbols.
         *
         * @param symbol    the inputSymbol
         * @return `true` if the symbol is in the set of input symbols
         */
        bool HasInputSymbol(char symbol) const;

        /**
         * Returns the set of accepted input symbols.
         *
         * @return the set of input symbols
         */
        const std::set<char> &GetInputSymbols() const;

        /**
         * Removes all input symbols.
         */
        void ClearInputSymbols();

    private:
        /** The states of this state machine. */
        std::vector<std::unique_ptr<State>> states;

        /** The start state of this state machine. */
        State *startState;

        /** The current State of this state machine. */
        State *curState;

        /** the finite set of input symbols. */
        std::set<char> inputSymbols;

        bool HasState(const std::set<const State*> &substates) const;
        State& GetState(const std::set<const State*> &substates);
    };

} // end of namespace


