/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Extensions/InstantBuilder.h"
#include "Velox/Parser/SourceString.h"

// C++ Standard Library includes
#include <stdexcept>

using namespace velox;
using namespace std;

namespace astu {

    InstantBuilder::InstantBuilder()
    {
        Reset();
    }

    bool InstantBuilder::HasInstant(const string &name) const
    {
        for (auto & instant : instants) {
            if (instant->GetTypeName() == name)
                return true;
        }

        return false;
    }


    InstantBuilder &InstantBuilder::AddInstant(const string &instantSource)
    {
        // TODO implement
        return *this;
        //try {
        //    Parser parser;
        //    SourceString src(instantSource);
        //    auto instant = parser.ParseInstantDefinition(src);
        //
        //    if (HasInstant(instant->GetTypeName())) {
        //        throw std::logic_error("Unable to add instant to builder, ambiguous instant name '" + instant->GetTypeName() + "'");
        //    }
        //    instants.push_back(instant);
        //    return *this;
        //} catch (const ParserError & e) {
        //    throw std::logic_error(std::string("Error parsing instant source: ") + e.what() + ", at " + to_string(e.GetLineNumber()));
        //}
    }

    InstantBuilder &InstantBuilder::Reset()
    {
        instants.clear();
        return *this;
    }

    void InstantBuilder::Build(velox::Interpreter &interpreter)
    {
        for (auto &instant : instants) {
            interpreter.AddInstant(*instant);
        }
    }

} // end of namespace
