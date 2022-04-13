// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "ItemStateWithSubItems.h"

// C++ Standard Library includes
#include <map>

namespace velox {

    class ItemStateObject : public ItemStateWithSubItems {
    public:

        /** Constructor. */
        ItemStateObject();

        // Inherited via ItemState
        std::unique_ptr <ItemState> Copy() const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;
        void SetData(std::shared_ptr<ItemData> data) override;
        std::shared_ptr<ItemData> GetData() override;

    private:
        /** Additional data. */
        std::shared_ptr<ItemData> data;
    };

}


