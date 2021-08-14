/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Velox/VeloxDragAndDropService.h"
#include "Velox/VeloxService.h"
#include "Velox/VeloxScanner.h"
#include "Util/MessageBox.h"

using namespace std;

namespace astu {

    VeloxDragAndDropService::VeloxDragAndDropService()
        : Service("Velox Drag and Drop Service")
        , fileExtensions({".vlx", ".VLX", ".velox"})
        , reloadKey(Keyboard::KEY_F5)
    {
        // Intentionally left empty.
    }

    VeloxDragAndDropService::~VeloxDragAndDropService()
    {
        // Intentionally left empty.
    }

    void VeloxDragAndDropService::OnStartup()
    {
        veloxSrv = ASTU_GET_SERVICE(VeloxService);
        lastFile.clear();
    }

    void VeloxDragAndDropService::OnShutdown()
    {
        veloxSrv = nullptr;
    }

    bool VeloxDragAndDropService::OnSignal(const DropSignal& signal)
    {
        try {
            switch (signal.type) {

            case DropSignal::Text:
                veloxSrv->RunScriptFromString(signal.content);                
                return true;

            case DropSignal::File:
                if (IsVeloxFile(signal.content)) {
                    lastFile = signal.content;
                    veloxSrv->RunScriptFromFile(signal.content);
                    return true;
                }
                return false;
            }
        } catch (const VeloxCompilerException& e) {
            MessageBox::ShowErrorMessage(
                e.getMessage() + " at line " + to_string(e.getLineNumber()), 
                "Velox Error");

            return true;
        } catch (const runtime_error & e) {
            MessageBox::ShowErrorMessage(
                e.what(), "Velox Error");
        }

        return false;
    }

    bool VeloxDragAndDropService::IsVeloxFile(const std::string& filePath)
    {
        auto pos = filePath.rfind('.');
        if (pos == string::npos) {
            return false;
        }
        auto ext = filePath.substr(pos);
        return fileExtensions.count(ext) != 0;
    }

    bool VeloxDragAndDropService::OnKeyReleased(int keycode)
    {
        if (keycode != reloadKey || lastFile.empty())
            return false;

        OnSignal(DropSignal(DropSignal::File, lastFile));
        return true;        
    }

} // end of namespace