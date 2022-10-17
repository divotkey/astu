/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include <Util/Memento.h>

// C++ Standard Library includes
#include <cassert>
#include <memory>
#include <vector>

namespace astu {

    /**
     * Implementation of a classical Q-Table used for machine learning.
     *
     * @ingroup ai_group
     */
	class QTable {
	public:

		/**
		 * Constructor.
		 */
		QTable(size_t numStates, size_t numActions);

		/**
		 * Resets the entries of this table.
		 *
		 * @param spread	the range used for random values
		 */
		void Reset(double spread);

		/**
		 * Returns the number of states of this table.
		 *
		 * @return the number of states.
		 */
		size_t NumStates() const {
			return numStates;
		}

		/**
		 * Returns the number of actions per states.
		 *
		 * @return number of actions per state
		 */
		size_t NumActions() const {
			return numActions;
		}

		/**
		 * Returns the number of entries.
		 *
		 * @return the number of entries
		 */
		size_t NumEntries() const;

		/**
		 * Returns the required memory in bytes.
		 *
		 * @return the used memory in bytes
		 */
		size_t GetUsedMemory() const;

		/**
		 * Returns the value of the specified state and action.
		 *
		 * @param state		the state index
		 * @param action	the action index
		 * @return the requested value
		 */
		double GetValue(size_t state, size_t action) const {
			return GetEntry(state, action).value;
		}

		/**
		 * Updates the entry of the specified state and action.
		 *
		 * @param state		the state index
		 * @param action	the action index
		 * @param value		the new value
		 */
		void UpdateValue(size_t state, size_t action, double value);

		/**
		 * Updates this Q-Table.
		 *
		 * @param state		the state to update
		 * @param action	the action to update
		 * @param reward	the reward for the specified state and action
		 * @param nextState	the next state to following the specified state and action
		 * @return the selected best future action of teh next state
		 */
		size_t Update(size_t state, size_t action, double reward, size_t nextState);

		/**
		 * Updates this Q-Table (without next state).
		 * 
		 * Use this method for updating if there is no next state e.g., if the
		 * end of an episode has been reached.
		 * 
		 * @param state		the state to update
		 * @param action	the action to update
		 * @param reward	the reward for the specified state and action
		 */
		void Update(size_t state, size_t action, double reward);

		/**
		 * Returns the index of the action with the actual maximum value.
		 *
		 * @param state	the state index
		 * @return the index of the action of the specified state with the maximum value
		 */
		size_t GetMaxAction(size_t state) const;

		/**
		 * Sets the learning rate.
		 *
		 * @param alpha	the learning rate
		 * @throws std::invalid_argument in case the learning rate is less than
		 * 		zero or greater than one
		 */
		void SetLearningRate(double alpha);

		/**
		 * Returns the learning rate.
		 *
		 * @return the learning rate
		 */
		double GetLearningRate() const {
			return alpha;
		}

		/**
		 * Sets the discount factor
		 *
		 * @param gamma	the discount factor
		 */
		void SetDiscountFactor(double gamma);

		/**
		 * Returns the discount factor.
		 *
		 * @return the discount factor
		 */
		double GetDiscountFactor() const {
			return gamma;
		}

		/**
		 * Stores the current state of this table to the specified memento.
		 *
		 * @param m	the memento used to store the state
		 * @return reference to the specified memento
		 */
		Memento & StoreToMemento( Memento & m) const;

		/**
		 * Restores the state of this table from the specified memento.
		 *
		 * @param m	the memento used to restore the state
		 */
		void RestoreFromMento(const  Memento & m);

		/**
		 * Returns the number of updates a particular state has received so far.
		 * 
		 * @param state	the state index
		 * @return the number of updates
		 */
		size_t GetStateUpdates(size_t state) const;

		/**
		 * Returns the average value for all actions of a state.
		 * 
		 * @param state	the state index
		 * @return the average value of all actions
		 */
		double GetAverageValue(size_t state) const;

		/**
		 * Returns the standard deviation for the values of all actions.
		 * 
		 * @param state	the state index
		 * @return the standard deviation of all actions
		 */
		double GetStandardDeviationValue(size_t state) const;

		/**
		 * Return the maximum standard deviation value of all states.
		 * 
		 * @param the maximum standard deviation value.
		 */
		double GetMaxStandardDeviationValue() const;

		/**
		 * Returns the maximum number of updates a state has received.
		 * 
		 * @param idx	the index in the sorted update count list
		 * @return the maximum number of updates
		 */
		size_t GetMaxUpdates(size_t idx = 0) const;

		/**
		 * Returns the minimum number of updates a state has received.
		 * 
		 * @return the minimum number of updates
		 */
		size_t GetMinUpdates() const;

		/**
		 * Returns the average number of updates states have received.
		 * 
		 * @return the average number of updates
		 */
		double GetAvgUpdates() const;

		/**
		 * Returns the median number of updates states have received.
		 * 
		 * @return the median number of updates
		 */
		double GetMedianUpdates() const;

		/**
		 * Number of states which have less or equal the specified amount of updates.
		 * 
		 * @param maxUpdates	the maximum number of updates
		 * @return the number of states that satisfy the condition
		 */
		size_t CountStatesWithLimitedUpdates(size_t maxUpdates = 0) const;

		struct StateEntry {
			size_t state;
			size_t numUpdates;

			StateEntry(size_t state, size_t numUpdates)
				: state(state), numUpdates(numUpdates) {}

			bool operator >(const StateEntry& rhs) {
				return numUpdates > rhs.numUpdates;
			}
		};

		/**
		 * Returns a list of states sorted by the number of their updates.
		 * 
		 * The states are sorted in descending order, which means that the
		 * first state in the list has received the most updates so far.		 
		 */
		const std::vector<StateEntry>& GetUpdateDistribution() const;


		// DBG
		size_t GetVisits(size_t entryIdx) const;

		/**
		 * Updates the statistical data.
		 */
		void UpdateStats() const;

		unsigned int GetMaxVisits() const {
			return maxVisits;
		}

		unsigned int GetMinVisits() const {
			return minVisits;
		}

		double GetAvgVisits() const {
			return avgVisits;
		}

		/**
		 * Returns the number of updates this table has received.
		 *
		 * @return the number of updates
		 */
		size_t NumUpdates() const {
			return numUpdates;
		}

	private:
		/**
		 * An entry within this table.
		 * Each state stored in this table contains number of
		 * actions per state entries.
		 */
		struct Entry {
			double value;
			double delta;
			unsigned int visited;

			void StoreToMemento( Memento & m) const;
			void RestoreFromMemento(const  Memento & m);
		};


		/** The entries of this table. */
		std::unique_ptr<Entry[]> m_entries;

		/** The number of states represented by this table. */
		const size_t numStates;

		/** The number of actions per state. */
		const size_t numActions;

		/** The maximum visits of entries. */
		mutable unsigned int maxVisits;

		/** The minimum visits of entries. */
		mutable unsigned int minVisits;

		/** The average visits of entries. */
		mutable double avgVisits;

		/** The maximum number of updates a state has received so far. */
		mutable size_t maxUpdates;

		/** The maximum number of updates a state has received so far. */
		mutable size_t minUpdates;

		/** Sorted list of states by number of updates. */
		mutable std::vector<StateEntry> updateList;

		/** Indicates that statistical data require updates. */
		mutable bool statsDirty;

		/** The number of updates this table has revived. */
		size_t numUpdates;

		/** The learning rate. */
		double alpha;

		/** The discount factor. */
		double gamma;


		/**
		 * Calculates the index of the entry.
		 *
		 * @param state		the state index
		 * @param action	the action index
		 * @return the index of the entry
		 */
		size_t GetIndex(size_t state, size_t action) const {
			assert(state < numStates && action < numActions);
			return state * numActions + action;
		}

		/**
		 * Returns the entry of the specified state and action.
		 *
		 * @param state		the state index
		 * @param action	the action index
		 * @return the requested table entry
		 */
		 const Entry & GetEntry(size_t state, size_t action) const;
	};

} // end of namespace