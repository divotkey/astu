/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Parser/ISource.h"
#include "Util/Memento.h"

namespace astu {

    class MementoSource : public velox::ISource {
    public:

        /**
         * Constructor.
         *
         * @param memento   the memento that contains the source data
         */
        MementoSource(const Memento &memento);

        /**
         * Stores the tokenized source into the specified memento.
         *
         * @param source    the source to store
         * @param memento   receives the source tokens and data
         */
        static void SourceToMemento(velox::ISource &source, Memento &memento);

        // Inherited via FastSource;
        void Reset() override;
        std::string GetFilepath() const override;
        velox::TokenType GetCurrentTokenType() const override;
        velox::TokenType GetNextTokenType() override;
        const std::string &GetStringValue() const override;
        int GetIntegerValue() const override;
        double GetRealValue() const override;
        Tuple2i GetPos() const override;
        velox::TokenType PeekNextTokenType() override;
        bool IsBlockStartFollowing() const override;

    private:
        /** The original filename of the source. */
        std::string sourceFile;

        /** The memento that contains the source data. */
        Memento memento;

        /** The position of the token. */
        Tuple2i pos;

        /** The last recognized token type. */
        velox::TokenType curToken;

        /** Stores last recognized string value or identifier names. */
        std::string curString;

        /** Stores the last recognized integer value. */
        int curInteger;

        /** Stores the last recognized floating-point value. */
        double curReal;

        static void StoreToken(const ISource &source, Memento &memento);
    };

} // end of namespace

