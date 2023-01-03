/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service.h"

// C++ Standard Library includes
#include <string>
#include <map>
#include <mutex>

namespace astu {

    class BlackboardData {
    private:
        std::mutex dataMutex;
        friend class BlackboardServiceTs;
    };

    /**
     * A thread safe implementation of the blackboard communication concept.
     * 
     * @ingroup srv_group
     */
    class BlackboardServiceTs : virtual public Service {
    public:

        BlackboardServiceTs();

        bool HasStringTs(const std::string &key) const;
        void SetStringTs(const std::string &key, const std::string &value);
        const std::string GetStringTs(const std::string &key) const;

    private:

        std::map<std::string, std::string> stringMap;
        mutable std::mutex stringMapMutex;

    protected:
        void OnStartup() override;
        void OnShutdown() override;
    };

} // end of namespace