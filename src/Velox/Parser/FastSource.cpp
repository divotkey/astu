/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Parser/FastSource.h"
#include "Velox/Parser/ParserError.h"

// C++ Standard Library includes
#include <cassert>
#include <cctype>

using namespace std;

namespace velox {

    /////////////////////////////////////////////////
    /////// class FastSource
    /////////////////////////////////////////////////

    const std::map<std::string, TokenType> FastSource::keywords = {
            {"undefined", TokenType::UNDEFINED},
            {"function", TokenType::FUNCTION},
            {"if", TokenType::IF},
            {"else", TokenType::ELSE},
            {"else", TokenType::SWITCH},
            {"else", TokenType::CASE},
            {"white", TokenType::WHILE},
            {"do", TokenType::DO},
            {"loop", TokenType::LOOP},
            {"exit", TokenType::EXIT},
            {"return", TokenType::RETURN},
            {"continue", TokenType::CONTINUE},
            {"break", TokenType::BREAK},
            {"for", TokenType::FOR},
            {"class", TokenType::CLASS},
            {"new", TokenType::NEW},
            {"instant", TokenType::INSTANT},
            {"global", TokenType::GLOBAL},
            {"import", TokenType::IMPORT},
            {"include", TokenType::INCLUDE},
    };

    FastSource::FastSource()
        : curToken(TokenType::INVALID), curChar(-1), curPos(0, 0), curInteger(0), curReal(0)
    {
        // Intentionally left empty.
    }

    TokenType FastSource::PeekNextTokenType()
    {
        memento.Clear();
        Store(memento);

        GetNextTokenType();
        auto result = curToken;

        memento.Rewind();
        Restore(memento);

        return result;
    }

    bool FastSource::IsBlockStartFollowing() const
    {
        return curChar == '{';
    }

    TokenType FastSource::GetNextTokenType()
    {
        EatWhitespace();

        startPos = curPos;
        switch (curChar) {
            case -1:
                endPos = curPos;
                return curToken = TokenType::EOS;
            case ';':
                ReadChar();
                endPos = curPos;
                return curToken = TokenType::SEMICOLON;
            case ',':
                ReadChar();
                endPos = curPos;
                return curToken = TokenType::COMMA;
            case '.':
                ReadChar();
                if (isdigit(curChar)) {
                    curInteger = 0;
                    ReadReal();
                    endPos = curPos;
                    return curToken = TokenType::REAL;
                }
                endPos = curPos;
                return curToken = TokenType::MEMBER_ACCESS;
            case '(':
                ReadChar();
                endPos = curPos;
                return curToken = TokenType::LEFT_PARENTHESIS;
            case ')':
                ReadChar();
                endPos = curPos;
                return curToken = TokenType::RIGHT_PARENTHESIS;
            case '[':
                ReadChar();
                endPos = curPos;
                return curToken = TokenType::LEFT_BRACKET;
            case ']':
                ReadChar();
                endPos = curPos;
                return curToken = TokenType::RIGHT_BRACKET;
            case '{':
                ReadChar();
                endPos = curPos;
                return curToken = TokenType::BLOCK_START;
            case '}':
                ReadChar();
                endPos = curPos;
                return curToken = TokenType::BLOCK_END;
            case '=':
                ReadChar();
                if (curChar == '=') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::EQUAL;
                }
                endPos = curPos;
                return curToken = TokenType::ASSIGNMENT;
            case '<':
                ReadChar();
                if (curChar == '=') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::LESS_EQUAL;
                }
                endPos = curPos;
                return curToken = TokenType::LESS_THAN;
            case '>':
                ReadChar();
                if (curChar == '=') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::GREATER_EQUAL;
                }
                endPos = curPos;
                return curToken = TokenType::GREATER_THAN;
            case '!':
                ReadChar();
                if (curChar == '=') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::NOT_EQUAL;
                }
                endPos = curPos;
                return curToken = TokenType::NOT;
            case '+':
                ReadChar();
                switch (curChar) {
                    case '=':
                        ReadChar();
                        endPos = curPos;
                        return curToken = TokenType::ASSIGN_ADD;
                    case '+':
                        ReadChar();
                        endPos = curPos;
                        return curToken = TokenType::INCREMENT;
                    default:
                        endPos = curPos;
                        return curToken = TokenType::ADD;
                }
            case '-':
                ReadChar();
                if (curChar == '=') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::ASSIGN_SUB;
                }
                endPos = curPos;
                return curToken = TokenType::SUB;
            case '%':
                ReadChar();
                if (curChar == '=') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::ASSIGN_MOD;
                }
                endPos = curPos;
                return curToken = TokenType::MOD;
            case '*':
                ReadChar();
                if (curChar == '=') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::ASSIGN_MUL;
                }
                endPos = curPos;
                return curToken = TokenType::MUL;
            case '/':
                ReadChar();
                switch (curChar) {
                    case '=':
                        ReadChar();
                        endPos = curPos;
                        return curToken = TokenType::ASSIGN_DIV;

                    case '/':
                        EatLine();
                        return GetNextTokenType();

                    case '*': {
                        ReadChar();
                        EatBlockComment();
                        return GetNextTokenType();
                    }

                    default:
                        endPos = curPos;
                        return curToken = TokenType::DIV;
                }
            case '&':
                ReadChar();
                if (curChar == '&') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::LOG_AND;
                }
                endPos = curPos;
                return curToken = TokenType::BIN_AND;
            case '|':
                ReadChar();
                if (curChar == '|') {
                    ReadChar();
                    endPos = curPos;
                    return curToken = TokenType::LOG_OR;
                }
                endPos = curPos;
                return curToken = TokenType::BIN_OR;
            case '"':
                ReadString();
                endPos = curPos;
                return curToken = TokenType::STRING;
        }

        if (isdigit(curChar)) {
            if (ReadInteger() && curChar == '.') {
                ReadChar();
                ReadReal();
                endPos = curPos;
                return curToken = TokenType::REAL;
            }
            endPos = curPos;
            return curToken = TokenType::INTEGER;
        }

        if (isalpha(curChar) || curChar == '_') {
            ReadIdentifier();
            endPos = curPos;
            auto it = keywords.find(curString);
            if (it == keywords.end()) {
                return curToken = TokenType::IDENT;
            }

            return curToken = it->second;
        }

        return curToken = TokenType::INVALID;
    }

    void FastSource::EatWhitespace()
    {
        while (curChar != -1 && isspace(curChar)) {
            ReadChar();
        }
    }

    void FastSource::EatLine()
    {
        while (curChar != -1 && curChar != '\n') {
            ReadChar();
        }
    }

    void FastSource::EatBlockComment()
    {
        while (curChar != -1) {
            if (curChar == '*') {
                ReadChar();
                if (curChar == '/') {
                    ReadChar();
                    return;
                }
            }
            ReadChar();
        }

        throw ParserError("block comment not closed", startPos.y);
    }

    void FastSource::ReadString()
    {
        assert(curChar == '"');
        ReadChar();
        curString.clear();
        while (curChar != -1 && curChar != '"') {
            if (curChar == '\\') {
                ReadChar();
                switch (curChar) {
                    case 'n':
                        curString += '\n';
                        break;
                    case 'r':
                        curString += '\r';
                        break;
                    case 't':
                        curString += '\t';
                        break;
                    case 'b':
                        curString += '\b';
                        break;
                    case '\\':
                        curString += '\\';
                        break;
                    default:
                        throw ParserError("unknown escape sequence", curPos.y);
                }
            } else {
                curString += static_cast<char>(curChar);
            }

            ReadChar();
        }

        if (curChar != '"') {
            throw ParserError("string literal not closed", startPos.y);
        }
        ReadChar();
    }

    void FastSource::ReadIdentifier()
    {
        assert(isalpha(curChar) || curChar == '_');
        curString = static_cast<char>(curChar);
        ReadChar();

        while (curChar != -1 && (isalnum(curChar) || curChar == '_')) {
            curString += curChar;
            ReadChar();
        }
    }

    bool FastSource::ReadInteger()
    {
        assert(isdigit(curChar));
        curInteger = curChar - '0';
        ReadChar();

        if (curInteger == 0 && (curChar == 'x' || curChar == 'X')) {
            ReadChar();
            ReadHexInteger();
            return false;
        }

        while (curChar != -1 && isdigit(curChar)) {
            curInteger *= 10;
            curInteger += curChar - '0';
            ReadChar();
        }

        return true;
    }

    void FastSource::ReadHexInteger()
    {
        while (curChar != -1 && isxdigit(curChar)) {
            curInteger *= 16;
            if (curChar >= '0' && curChar <= '9') {
                curInteger += curChar - '0';
            } else if (curChar >= 'A' && curChar <= 'F') {
                curInteger += 10 + curChar - 'A';
            }  else if (curChar >= 'a' && curChar <= 'f') {
                curInteger += 10 + curChar - 'a';
            } else {
                assert(false);
            }
            ReadChar();
        }
    }

    void FastSource::ReadReal()
    {
        double value = 0.0;
        double i = 1.0;

        while (curChar != -1 && isdigit(curChar)) {
            i *= 10;
            value += (curChar - '0') / i;
            ReadChar();
        }
        curReal = curInteger + value;
        if (curChar == 'e' || curChar == 'E') {
            ReadChar();
            curReal = std::pow(curReal, ReadExponent());
        }
    }


    double FastSource::ReadExponent()
    {
        int value = 0;
        bool negative  = false;
        if (curChar == '+' || curChar == '-') {
            negative  = curChar == '-' ? true : false;
            ReadChar();
        }

        if (!isdigit(curChar)) {
            throw ParserError("invalid scientific notation", curPos.y);
        }

        while (curChar != -1 && isdigit(curChar)) {
            value *= 10;
            value += curChar - '0';
            ReadChar();
        }

        return negative ? -value : value;
    }

    void FastSource::ReadChar()
    {
        // Consider the current character for position tracking.
        if (curChar == '\n') {
            curPos.x = 1;
            ++curPos.y;
        } else {
            ++curPos.x;
        }

        curChar = static_cast<char>(NextChar());

        // Skip carriage return, which would mess up position tracking.
        if (curChar == '\r') {
            curChar = static_cast<char>(NextChar());
        }

    }

    void FastSource::Reset()
    {
        // Counting rows and columns in text is a bit weird. The first character is at position 1, 1
        // and because the first call to ReadChar() will increase the column position, the reset
        // position 1, 0.
        startPos = endPos = curPos.Set(0, 1);

        // Let's set recognized values (tokens, strings, identifiers, numbers) to default values.
        curToken = TokenType::INVALID;
        curString.clear();
        curReal = curInteger = 0;

        // Set the current character to the first character in the source script.
        ReadChar();

        // Detect first token.
        GetNextTokenType();
    }

    void FastSource::Store(astu::Memento &inMemento)
    {
        inMemento << static_cast<int>(curToken) << curChar << curString << curInteger << curReal << curPos << startPos << endPos;
    }

    void FastSource::Restore(const astu::Memento &inMemento)
    {
        int token;
        curString.clear();
        inMemento >> token >> curChar >> curString >> curInteger >> curReal >> curPos >> startPos >> endPos;
        curToken = static_cast<TokenType>(token);
    }

    astu::Tuple2i FastSource::GetPos() const
    {
        return endPos;
    }

    TokenType FastSource::GetCurrentTokenType() const
    {
        return curToken;
    }

    const std::string &FastSource::GetStringValue() const
    {
        return curString;
    }

    int FastSource::GetIntegerValue() const
    {
        return curInteger;
    }

    double FastSource::GetRealValue() const
    {
        return curReal;
    }

    /////////////////////////////////////////////////
    /////// class FastFileSource
    /////////////////////////////////////////////////

    FastFileSource::FastFileSource(const std::string &inFilepath)
    {
        Reset(inFilepath);
    }

    void FastFileSource::Reset(const std::string &inFilepath)
    {
        auto newSource = make_unique<ifstream>(inFilepath, ios::in | ios::binary);
        if (!*newSource) {
            throw std::runtime_error("Unable to open source file '" + inFilepath + "' for reading.");
        }
        filepath = inFilepath;
        source = std::move(newSource);
        FastSource::Reset();
    }

    void FastFileSource::Reset()
    {
        if (!filepath.empty()) {
            Reset(filepath);
        }
        FastSource::Reset();
    }

    int FastFileSource::NextChar()
    {
        if (!source->good()) {
            return -1;
        }
        return source->get();
    }

    void FastFileSource::Store(astu::Memento &memento)
    {
        int64_t pos = source->tellg();
        memento << pos;
        FastSource::Store(memento);
    }

    void FastFileSource::Restore(const astu::Memento &memento)
    {
        int64_t pos;
        memento >> pos;
        source->seekg(pos);
        FastSource::Restore(memento);
    }

    std::string FastFileSource::GetFilepath() const
    {
        return filepath;
    }


    /////////////////////////////////////////////////
    /////// class FastStringSource
    /////////////////////////////////////////////////

    FastStringSource::FastStringSource(const string &sourceCode)
    {
        Reset(sourceCode);
    }

    void FastStringSource::Reset(const string &inSourceCode)
    {
        sourceCode = inSourceCode;
        pos = 0;
        FastSource::Reset();
    }

    void FastStringSource::Reset()
    {
        pos = 0;
        FastSource::Reset();
    }

    void FastStringSource::Store(astu::Memento &memento)
    {
        int64_t convertedPos = static_cast<int64_t>(pos);
        memento << convertedPos;
        FastSource::Store(memento);
    }

    void FastStringSource::Restore(const astu::Memento &memento)
    {
        int64_t storedPos;
        memento >> storedPos;
        pos = static_cast<size_t>(storedPos);
        FastSource::Restore(memento);
    }

    int FastStringSource::NextChar()
    {
        if (pos >= sourceCode.size()) {
            return -1;
        }
        return sourceCode[pos++];
    }

    std::string FastStringSource::GetFilepath() const
    {
        return "";
    }


} // end of namespace