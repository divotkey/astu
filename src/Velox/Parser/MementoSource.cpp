/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Parser/MementoSource.h"
#include "Velox/Parser/TokenType.h"

// C++ Standard Library includes
#include <cassert>

using namespace velox;

namespace astu {

    MementoSource::MementoSource(const Memento &memento)
        : memento(memento)
    {
        Reset();
    }

    void MementoSource::SourceToMemento(velox::ISource &source, Memento &memento)
    {
        memento << source.GetFilepath();

        do {
            StoreToken(source, memento);
            source.GetNextTokenType();
        }
        while (source.GetCurrentTokenType() != TokenType::EOS);
        assert(source.GetCurrentTokenType() == TokenType::EOS);
        StoreToken(source, memento);
    }

    void MementoSource::StoreToken(const ISource &source, Memento &memento)
    {
        //memento << static_cast<int>(source.GetCurrentTokenType());
        memento << static_cast<unsigned char>(source.GetCurrentTokenType());
        memento << source.GetPos();

        switch (source.GetCurrentTokenType()) {
            case TokenType::IDENT:
                [[fallthrough]];

            case TokenType::STRING:
                memento << source.GetStringValue();
                break;

            case TokenType::INTEGER:
                memento << source.GetIntegerValue();
                break;

            case TokenType::REAL:
                memento << source.GetRealValue();
                break;

            default:
                // Ignore.
                break;
        }
    }

    void MementoSource::Reset()
    {
        memento.Rewind();
        memento >> sourceFile;
        curToken = TokenType::INVALID;
        pos = {0, 0};
        curString.clear();
        curReal = curInteger = 0;
        GetNextTokenType();
    }

    std::string MementoSource::GetFilepath() const
    {
        return sourceFile;
    }

    velox::TokenType MementoSource::GetCurrentTokenType() const
    {
        return curToken;
    }

    velox::TokenType MementoSource::GetNextTokenType()
    {
        unsigned char type;
        memento >> type;
        curToken = static_cast<TokenType>(type);
        memento >> pos;

        switch (curToken) {
            case TokenType::IDENT:
                [[fallthrough]];

            case TokenType::STRING:
                curString.clear();
                memento >> curString;
                break;

            case TokenType::INTEGER:
                memento >> curInteger;
                break;

            case TokenType::REAL:
                memento >> curReal;
                break;

            default:
                // Ignore.
                break;

        }

        return curToken;
    }

    const std::string &MementoSource::GetStringValue() const
    {
        return curString;
    }

    int MementoSource::GetIntegerValue() const
    {
        return curInteger;
    }

    double MementoSource::GetRealValue() const
    {
        return curReal;
    }

    Tuple2i MementoSource::GetPos() const
    {
        return pos;
    }

    velox::TokenType MementoSource::PeekNextTokenType()
    {
        memento.Mark();
        int type;
        memento >> type;
        memento.Reset();
        return static_cast<TokenType>(type);
    }

    bool MementoSource::IsBlockStartFollowing() const
    {
        memento.Mark();
        int type;
        memento >> type;
        memento.Reset();
        return static_cast<TokenType>(type) == TokenType::BLOCK_START;
    }

} // end of namespace
