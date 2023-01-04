/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Extensions/CompiledSource.h"
#include "Velox/Parser/Parser.h"
#include "Velox/Parser/MementoSource.h"

using namespace velox;

namespace astu {


    velox::InterpreterScript &CompiledSource::GetParsedSource() const
    {
        if (!parsedSource) {
            MementoSource source(Memento(GetData(), GetDataSize()));
            Parser parser;
            parsedSource = parser.Parse(source);
        }

        return *parsedSource;
    }

    void CompiledSource::Execute(velox::Interpreter &interpreter) const
    {
        interpreter.Execute(GetParsedSource());
    }

} // end of namespace