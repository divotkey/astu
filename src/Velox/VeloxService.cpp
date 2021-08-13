/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
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
        , cs(make_shared<FileCharStream>())
    {
        // Intentionally left empty.
    }

    VeloxService::~VeloxService()
    {
        // Intentionally left empty.
    }


    void VeloxService::RunScript(const std::string& filename)
    {
        cs->Open(filename);
        auto node = parser->parse(cs);
        cs->Close();

        auto script = VeloxScript::create(node, filename);
        script->execute();
    }

} // end of namespace