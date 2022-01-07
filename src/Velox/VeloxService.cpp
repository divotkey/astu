/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/VeloxService.h"
#include "Velox/VeloxParser.h"
#include "Velox/FileCharStream.h"
#include "VeloxScript.h"

using namespace std;

namespace astu {

    VeloxService::VeloxService()
        : Service("Velox Service")
        , parser(make_unique<VeloxParser>())
        , fcs(make_shared<FileCharStream>())
        , scs(make_shared<StringCharStream>())
    {
        // Intentionally left empty.
    }

    VeloxService::~VeloxService()
    {
        // Intentionally left empty.
    }


    void VeloxService::RunScriptFromFile(const std::string& filename)
    {
        std::shared_ptr<VeloxNode> node;
        try {
            fcs->Open(filename);
            node = parser->parse(fcs);
            fcs->Close();
        } catch (...) {
            fcs->Close();
            throw;
        }

        auto script = VeloxScript::create(node, filename);
        script->execute();
    }

    void VeloxService::RunScriptFromString(const std::string& scriptString)
    {
        std::shared_ptr<VeloxNode> node;
        try {
            scs->reset(scriptString);
            node = parser->parse(scs);
            scs->reset();
        } catch (...) {
            scs->reset();
            throw;
        }

        auto script = VeloxScript::create(node);
        script->execute();
    }

} // end of namespace