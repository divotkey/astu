/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Ai/QTable.h"
#include "Math/Random.h"

// C++ Standard Library includes
#include <stdexcept>
#include <algorithm>
#include <string>
#include <limits>
#include <sstream>

namespace astu {

	QTable::QTable(size_t numStates, size_t numActions)
		: numStates(numStates)
		, numActions(numActions)
		, numUpdates(0)
		, minVisits(0)
		, maxVisits(0)
		, avgVisits(0)
		, alpha(0.05)
		, gamma(0.9)
		, statsDirty(true)
	{
		m_entries = std::make_unique<Entry[]>(numStates * numActions);
	}

	void QTable::Reset(double spread)
	{
		Entry * ptr = m_entries.get();
		size_t n = numStates * numActions;
		double range = spread * 2;
		for (size_t i = 0; i < n; ++i) {
			Entry & e = *ptr++;
			e.visited = false;
			e.delta = 0;
			e.value = -spread + Random::GetInstance().NextDouble() * range;
		}
	}

	size_t QTable::NumEntries() const
	{
		return numActions * numStates;
	}

	size_t QTable::GetUsedMemory() const
	{
		return NumEntries() * sizeof(Entry);
	}

	const QTable::Entry & QTable::GetEntry(size_t state, size_t action) const
	{
		assert(state < numStates);
		assert(action < numActions);
		return m_entries[GetIndex(state, action)];
	}

	void QTable::UpdateValue(size_t state, size_t action, double value)
	{
		auto & entry = m_entries[GetIndex(state, action)];
		entry.delta = value - entry.value;
		entry.value = value;
		++entry.visited;
		++numUpdates;
		statsDirty = true;
	}

	size_t QTable::Update(size_t state, size_t action, double reward, size_t nextState)
	{
		auto prevValue = GetValue(state, action);
		auto bestAction = GetMaxAction(nextState);
		auto nextValue = GetValue(nextState, bestAction);
		double newValue = (1 - alpha) * prevValue + alpha * (reward + gamma * nextValue);
		UpdateValue(state, action, newValue);

		return bestAction;
	}

	void QTable::Update(size_t state, size_t action, double reward)
	{
		auto prevValue = GetValue(state, action);
		double newValue = (1 - alpha) * prevValue + alpha * reward;
		UpdateValue(state, action, newValue);
	}

	size_t QTable::GetMaxAction(size_t state) const
	{
		QTable::Entry * entry = &m_entries[GetIndex(state, 0)];

		unsigned int result = 0;
		double max = (entry++)->value;
		for (unsigned int i = 1; i < numActions; ++i) {
			double v = (entry++)->value;
			if (v > max) {
				result = i;
				max = v;
			}
		}

		return result;
	}

	void QTable::SetLearningRate(double alpha)
	{
		if (alpha < 0 || alpha > 1) {
			throw std::invalid_argument("invalid learning rate: " + std::to_string(alpha));
		}
		alpha = alpha;
	}

	void QTable::SetDiscountFactor(double gamma)
	{
		gamma = gamma;
	}

	Memento& QTable::StoreToMemento(Memento & m) const
	{
		m << "QTable" << numActions << numStates << alpha << gamma << numUpdates;

		Entry * end = m_entries.get() + NumEntries();
		for (Entry *ptr = m_entries.get(); ptr < end; ++ptr) {
			ptr->StoreToMemento(m);
		}

		return m;
	}

	void QTable::RestoreFromMento(const Memento & m)
	{
		std::string id;
		m >> id;
		if (id != "QTable") {
			throw std::logic_error("Invalid QTable memento");
		}
		size_t nActions, nStates;
		m >> nActions >> nStates;
		if (nActions != numActions) {
			throw std::logic_error("Invalid QTable memento, number of actions mismatch");
		}
		if (nStates != numStates) {
			throw std::logic_error("Invalid QTable memento, number of states mismatch");
		}
		m >> alpha >> gamma >> numUpdates;

		Entry * end = m_entries.get() + NumEntries();
		for (Entry *ptr = m_entries.get(); ptr < end; ++ptr) {
			ptr->RestoreFromMemento(m);
		}

		statsDirty = true;
	}

	double QTable::GetAverageValue(size_t state) const
	{
		Entry* pEntry = &m_entries[GetIndex(state, 0)];

		size_t numUpdates = 0;
		double sum = 0.0;
		for (int i = 0; i < numActions; ++i) {
			sum += pEntry->value;
			++pEntry;
		}

		return sum / numActions;
	}

	double QTable::GetStandardDeviationValue(size_t state) const
	{
		double avg = GetAverageValue(state);

		Entry* pEntry = &m_entries[GetIndex(state, 0)];

		double deviation = 0.0;
		for (int i = 0; i < numActions; ++i) {
			double e = pEntry->value - avg;
			deviation += e * e;
			++pEntry;
		}

		return sqrt(deviation);
	}

	double QTable::GetMaxStandardDeviationValue() const
	{
		double result = 0;
		for (size_t i = 0; i < numStates; ++i) {
			double stdDev = GetStandardDeviationValue(i);
			if (stdDev > result) {
				result = stdDev;
			}
		}

		return result;
	}

	size_t QTable::GetStateUpdates(size_t state) const
	{
		Entry* pEntry = &m_entries[GetIndex(state, 0)];

		size_t numUpdates = 0;
		for (int i = 0; i < numActions; ++i) {
			numUpdates += pEntry->visited;
			++pEntry;
		}

		return numUpdates;
	}

	size_t QTable::GetMaxUpdates(size_t idx) const
	{
		if (statsDirty) {
			UpdateStats();
		}

		return updateList.at(idx).numUpdates;
	}

	size_t QTable::GetMinUpdates() const
	{
		if (statsDirty) {
			UpdateStats();
		}

		return minUpdates;
	}

	double QTable::GetAvgUpdates() const
	{
		return static_cast<double>(numUpdates) / numStates;
	}

	double QTable::GetMedianUpdates() const
	{
		if (statsDirty) {
			UpdateStats();
		}

		if (updateList.size() % 2 == 0) {
			size_t idx = updateList.size() / 2;
			return (static_cast<double>(updateList[idx - 1].numUpdates) + updateList[idx].numUpdates) / 2;
		} else {
			return static_cast<double>(updateList[updateList.size() / 2].numUpdates);
		}
	}

	size_t QTable::CountStatesWithLimitedUpdates(size_t maxUpdates) const
	{
		size_t result = 0;
		for (size_t i = 0; i < numStates; ++i) {
			size_t stateUpdates = GetStateUpdates(i);
			if (GetStateUpdates(i) <= maxUpdates) {
				++result;
			}
		}

		return result;
	}


	const std::vector<QTable::StateEntry>& QTable::GetUpdateDistribution() const
	{
		if (statsDirty) {
			UpdateStats();
		}

		return updateList;
	}

	size_t QTable::GetVisits(size_t entryIdx) const
	{
		if (entryIdx >= NumEntries()) {
			throw std::out_of_range("invalid entry index");
		}
		return m_entries[entryIdx].visited;
	}

	void QTable::UpdateStats() const
	{
		minVisits = std::numeric_limits<unsigned int>::max();
		maxVisits = 0;

		Entry* ptr = &m_entries[0];

		auto n = numActions * numStates;
		long sumVisits = 0;
		for (size_t i = 0; i < n; ++i) {
			Entry & e = *ptr++;
			if (e.visited < minVisits) {
				minVisits = e.visited;
			}
			if (e.visited > maxVisits) {
				maxVisits = e.visited;
			}
			sumVisits += e.visited;
		}
		avgVisits = static_cast<double>(sumVisits) / n;

		// Calculate statistics per state.
		maxUpdates = 0;
		minUpdates = std::numeric_limits<size_t>::max();
		updateList.clear();
		for (size_t i = 0; i < numStates; ++i) {
			size_t stateUpdates = GetStateUpdates(i);
			updateList.push_back(StateEntry(i, stateUpdates));

			if (stateUpdates > maxUpdates) {
				maxUpdates = stateUpdates;
			}

			if (stateUpdates < minUpdates) {
				minUpdates = stateUpdates;
			}
		}

		std::sort(updateList.begin(), updateList.end(), std::greater<>());
		statsDirty = false;
	}

	void QTable::Entry::StoreToMemento(Memento & m) const
	{
		m << value << delta << visited;
	}

	void QTable::Entry::RestoreFromMemento(const Memento & m)
	{
		m >> value >> delta >> visited;
	}


} // end of namespace