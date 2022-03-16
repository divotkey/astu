// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "DStateMachine.h"
#include "NStateMachine.h"

namespace astu {

    class PowersetBuilder {
    public:

        std::unique_ptr<NStateMachine> Build(DStateMachine &dsm);

    private:

    };

} // end of namespace



