// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "FStateMachine.h"
#include "NStateMachine.h"

// C++ Standard Library
#include <memory>
#include <vector>
#include <set>
#include <map>

namespace astu {

    class PowersetBuilder {
    public:

        std::unique_ptr<FStateMachine> Build(NStateMachine &nsm);

    private:

        std::map<size_t, std::set<size_t>> subStatesMap;

        void AddSubState(size_t state, size_t subState);
        bool FindState(size_t &state, const std::set<size_t> &subStates);
        std::function<void (char, void *)> BuildEnterFunc(NStateMachine &nsm, const std::set<size_t> &substates);
        bool IsAccepting(NStateMachine &nsm, const std::set<size_t> &substates) const;

        /**
         * Returns the combined flags of all substates.
         *
         * @param nsm           the non-deterministic state machine that owns the states
         * @param substates     the substates
         * @return              the combined flags
         */
        unsigned int CombineFlags(NStateMachine &nsm, const std::set<size_t> &substates) const;
    };

} // end of namespace



