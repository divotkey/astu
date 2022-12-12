// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

namespace velox {

    /**
     * Base class for all custom data that might get attached to items.
     */
    class ItemData {
    public:

        /** Virtual destructor. */
        virtual ~ItemData() {}
    };

} // end of namespace