/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service.h"
#include "Graphics/Image.h"

// C++ Standard Library includes
#include <string>
#include <map>
#include <mutex>

namespace astu {

    //class BlackboardData {
    //private:
    //    std::mutex dataMutex;
    //    friend class BlackboardServiceTs;
    //};

    /**
     * A thread safe implementation of the blackboard communication concept.
     * 
     * @ingroup srv_group
     */
    class BlackboardServiceTs : virtual public Service {
    public:

        BlackboardServiceTs();

        bool HasString(const std::string &key) const;
        void SetString(const std::string &key, const std::string &value);
        const std::string GetString(const std::string &key) const;
        void ClearStrings();

        bool HasImage(const std::string &key) const;
        void SetImage(const std::string &key, std::shared_ptr<Image>);
        std::shared_ptr<Image> GetImage(const std::string &key) const;
        void ClearImages();

    protected:
        void OnStartup() override;
        void OnShutdown() override;

    private:

        std::map<std::string, std::string> stringMap;
        mutable std::mutex stringMapMutex;

        std::map<std::string, std::shared_ptr<Image>> imageMap;
        mutable std::mutex imageMapMutex;
    };

} // end of namespace