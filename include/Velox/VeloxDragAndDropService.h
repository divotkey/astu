/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes.
#include "Service/Service.h"
#include "Input/InputSignals.h"

// C++ Standard Library includes
#include <string>
#include <set>

namespace astu {

    // Forward declaration.
    class VeloxService;

    class VeloxDragAndDropService 
        : public virtual Service
        , private SignalListener<DropSignal> 
        , private KeystrokeListener
    {
    public:

        /** Constructor. */
        VeloxDragAndDropService();

        /** Virtual destructor. */
        virtual ~VeloxDragAndDropService();

    private:
        /** Used to run velox scripts. */
        std::shared_ptr<VeloxService> veloxSrv;

        /** The file extensions used to recognize velox scripts. */
        std::set<std::string> fileExtensions;

        /** The path to the last Velox script. */
        std::string lastFile;

        /** The keycode used to reload the last script. */
        int reloadKey;

        bool IsVeloxFile(const std::string& filePath);

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via SignalListener<DropSignal> 
        virtual bool OnSignal(const DropSignal& signal) override;

        // Inherited via KeystrokeListener
        virtual bool OnKeyReleased(int keycode);
    };

} // end of namespace