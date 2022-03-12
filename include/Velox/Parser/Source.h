#pragma once

// Local includes
#include "Token.h"
#include "StateMachine.h"

// C++ Standard Library includes
#include <string>
#include <vector>
#include <memory>
#include <map>

namespace velox {

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

        const Token &GetFirstToken();

        const Token &GetCurrentToken() const;

        /**
         * Attempts to recognize the next token starting with the current character.
         *
         * @return the recognized token
         */
        const Token &GetNextToken();

        virtual char GetCurrentChar() const = 0;
        virtual bool IsEndOfFile() const = 0;
        virtual size_t GetCharPosition() const = 0;

    protected:
        virtual char GetFirstChar() = 0;
        virtual char GetNextChar() = 0;

    private:

        /** The string representing the current token. */
        std::string tokenString;

        /** The current token. */
        Token curToken;

        /** The position where the current token started. */
        size_t tokenStart;

        /** The state machine used to recognise tokens. */
        StateMachine<char> sm;

        void AddToken(const std::string &sequence, TokenType type);

        void AddIdentTermination(const std::string &state, TokenType type);

        std::string AddTokenStartState(const std::string &curState, char ch);
        std::string AddTokenState(const std::string &curState, char ch);
        std::string AddTokenEndState(const std::string &curState, TokenType type, bool useIdent);

        bool IsIdentStart(char ch);

        void PrintDebug(char ch);
    };

} // end of namespace
