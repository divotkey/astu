#pragma once

// Local includes
#include "TokenType.h"
#include "Script/Scanner.h"

// C++ Standard Library includes
#include <istream>
#include <memory>

namespace velox {

    /**
     * Represents a piece of source code and can scan and tokenize it.
     */
    class Source {
    public:

        /**
         * Constructor.
         */
        Source();

        /**
         * Virtual destructor.
         */
        virtual ~Source() {}

        TokenType GetNextTokenType();
        TokenType GetCurrentTokenType();
        const std::string &GetStringValue() const;
        int GetIntegerValue() const;
        double GetRealValue() const;

        std::string TokenTypeToString(TokenType type) const;

    protected:
        virtual std::shared_ptr<std::istream> GetStream() = 0;

    private:
        std::unique_ptr<astu::Scanner> scanner;
        bool scanningStarted;

        static std::unique_ptr<astu::Scanner> BuildScanner();
    };

} // end of namespace
