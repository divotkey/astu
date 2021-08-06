/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include <Util/Memento.h>

// C++ Standard Library includes
#include <cassert>
#include <memory>
#include <vector>

namespace astu {

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
		 */
		size_t Update(size_t state, size_t action, double reward, size_t nextState);

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
		 * @throws std::invalid_argument in case the learning rate is less than zero or greater than one
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

		// DBG
		void UpdateStats();

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
		unsigned int maxVisits;

		/** The minimum visits of entries. */
		unsigned int minVisits;

		/** The average visits of entries. */
		double avgVisits;

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