// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "FSM/PowersetBuilder.h"

// C++ Standard Library
#include <cassert>
#include <stdexcept>

using namespace std;

namespace astu {

    std::unique_ptr<FStateMachine> PowersetBuilder::Build(NStateMachine &nsm) {
        if (nsm.GetStartState() == NStateMachine::INVALID_HANDLE) {
            throw logic_error("Non-deterministic state machine does not have a defined start state");
        }
        auto dsm = make_unique<FStateMachine>();

        // Initialize state set with a state containing only the start state.
        AddSubState(dsm->BeginState(), nsm.GetStartState());
        dsm->SetStartState(true);
        dsm->SetAccepting(nsm.IsAccepting(nsm.GetStartState()));
        dsm->SetEnterFunc(nsm.GetEnterFunc(nsm.GetStartState()));
        dsm->EndState();

        set<size_t> ss;
        std::vector<size_t> workList;
        size_t addedStates, addedTransitions;

        do {
            addedStates = addedTransitions = 0;

            // Create copy of current states.
            for (auto it: subStatesMap) {
                workList.push_back(it.first);
            }

            // Let's do this for all states have created so far.
            for (auto curState: workList) {
                // Let's do this for each symbol.
                for (char ch: nsm.GetSymbols()) {

                    // Let's do this for all sub-states of the current state.
                    ss.clear();
                    for (auto subState: subStatesMap[curState]) {
                        // Query targets of subState based on current char.
                        const auto &s = nsm.GetTransitions(subState, ch);
                        ss.insert(s.begin(), s.end());
                    }

                    size_t targetState;
                    if (!FindState(targetState, ss)) {
                        targetState = dsm->BeginState();
                        dsm->SetAccepting(IsAccepting(nsm, ss));
                        dsm->SetEnterFunc(BuildEnterFunc(nsm, ss));
                        dsm->SetFlag(CombineFlags(nsm, ss));
                        ++addedStates;
                        subStatesMap[targetState] = ss;
                        dsm->EndState();
                    }

                    if (dsm->HasTransition(curState, ch)) {
                        if (dsm->GetTransition(curState, ch) != targetState) {
                            throw std::logic_error("Unable to create deterministic state machine");
                        }
                    } else {
                        dsm->BeginState(curState);
                        dsm->SetTransition(ch, targetState);
                        dsm->EndState();
                        ++addedTransitions;
                    }
                }
            }
        } while(addedTransitions > 0 || addedStates > 0);

        subStatesMap.clear();
        return dsm;
    }

    void PowersetBuilder::AddSubState(size_t state, size_t subState) {
        subStatesMap[state].insert(subState);
    }

    bool PowersetBuilder::FindState(size_t &state, const set<size_t> &subStates) {
        for (auto it : subStatesMap) {
            if (it.second == subStates) {
                state = it.first;
                return true;
            }
        }
        return false;
    }

    std::function<void(char, void*)> PowersetBuilder::BuildEnterFunc(NStateMachine &nsm, const set<size_t> &substates) {
        size_t n = 0;
        for (auto state: substates) {
            if (nsm.HasEnterFunc(state))
                ++n;
        }

        switch (n) {
            case 0:
                return nullptr;

            case 1:
                for (auto state: substates) {
                    if (nsm.HasEnterFunc(state))
                        return nsm.GetEnterFunc(state);
                }
                assert(false);
                return nullptr;

            default:
                std::vector<std::function<void(char, void*)>> funcList;
                for (auto state: substates) {
                    if (nsm.HasEnterFunc(state))
                        funcList.push_back(nsm.GetEnterFunc(state));
                }

                return [funcList](char ch, void * ctx){
                    for (auto &f : funcList)
                        f(ch, ctx);
                };
        }
    }

    bool PowersetBuilder::IsAccepting(NStateMachine &nsm, const set<size_t> &substates) const {
        for (auto state: substates) {
            if (nsm.IsAccepting(state))
                return true;
        }
        return false;
    }

    unsigned int PowersetBuilder::CombineFlags(NStateMachine &nsm, const set<size_t> &substates) const {
        unsigned int flags = 0;
        for (auto state : substates) {
            flags |= nsm.GetFlags(state);
        }

        return flags;
    }

} // end of namespace