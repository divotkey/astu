// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Script/Scanner.h"
#include "Script/ScannerError.h"

// C++ Standard Library
#include <cassert>
#include <algorithm>

using namespace std;

namespace astu {

    const char Scanner::END_OF_SOURCE                       = -1;
    const unsigned int Scanner::ADD_STRING_VALUE_CHAR_FLAG  = (1 << 1);
    const unsigned int Scanner::MARK_TOKEN_START_FLAG       = (1 << 2);

    Scanner::Scanner() {
        // Intentionally left empty.
    }

    void Scanner::Reset(std::shared_ptr<std::istream> inSource) {
        source = inSource;
        position = 0;
        newLines.clear();
    }

    size_t Scanner::NumStates() const {
        if (sm) {
            return sm->NumStates();
        }
        return 0;
    }

    void Scanner::GetNextToken() {
        ScanForNextTokenWithComments();

        while (IsIgnoreToken(tokenType)) {
            ScanForNextTokenWithComments();
        }
    }

    void Scanner::ScanForNextTokenWithComments() {
        ScanForNextToken(*sm);
        while (tokenType == blockCommentStart || tokenType == lineCommentStart) {
            if (tokenType == blockCommentStart) {
                assert(blockCommentSm);
                ScanForNextToken(*blockCommentSm);
            }
            else {
                assert(tokenType == lineCommentStart);
                assert(blockCommentSm);
                ScanForNextToken(*lineCommentSm);
            }

            ScanForNextToken(*sm);
        }
    }

    void Scanner::ScanForNextToken(FStateMachine& sm) {
        sm.Reset();
        intValue = 0;
        realFactor = 1;
        realValue = 0;
        stringValue.clear();
        tokenType = 0; // TODO set to invalid token
        tokenPos = 0;

        char ch = GetNextChar();
        do {
            sm.Process(ch, this);
            ProcessFlags(sm.GetFlags(), ch);
            ProcessCommands();
            ch = GetNextChar();
        } while(!sm.IsAccepting());
        assert(sm.IsAccepting());

        while (sm.Process(ch, this)) {
            ProcessFlags(sm.GetFlags(), ch);
            ProcessCommands();
            ch = GetNextChar();
        }
        assert(!sm.IsAccepting());
        DiscardCommands();

        PutBack(ch);
    }

    void Scanner::ProcessFlags(unsigned int flags, char ch) {
        if (flags & ADD_STRING_VALUE_CHAR_FLAG) {
            stringValue += ch;
        } else if (flags & MARK_TOKEN_START_FLAG) {
            tokenPos = position;
        }
    }

    bool Scanner::IsIgnoreToken(int type) const {
        return ignoreTokens.find(type) != ignoreTokens.end();
    }

    void Scanner::PutBack(char ch) {
        assert(position > 0);
        backlog.push(ch);
        --position;
    }

    char Scanner::GetNextChar() {
        char ch;

        if (backlog.empty()) {
            if (!source->fail()) {
                ch = static_cast<char>(source->get());
                ++position;
            } else {
                ch = END_OF_SOURCE;
            }
        } else {
            ch = backlog.front();
            ++position;
            backlog.pop();
        }

        if (ch == '\n') {
            if (newLines.empty() || newLines.back() != position)
                newLines.push_back(position);
        }

        return ch;
    }

    void Scanner::SetTokenType(int typeId, int priority) {
        pendingCommands.push_back(Command::CreateSetTokenType(priority, typeId));
    }

    void Scanner::AddStringValueChar(char ch, int priority) {
        pendingCommands.push_back(Command::CreateAddStringValueChar(priority, ch));
    }

    void Scanner::AddIntValue(int value, int factor, int priority) {
        pendingCommands.push_back(Command::CreateAddIntValue(priority, value, factor));
    }

    void Scanner::AddRealValue(int value, int factor, int priority) {
        pendingCommands.push_back(Command::CreateAddRealValue(priority, value, factor));
    }

    void Scanner::EmitError(size_t messageIdx, int priority) {
        pendingCommands.push_back(Command::CreateEmitError(priority, messageIdx));
    }

    void Scanner::ProcessCommands() {
        assert(uniqueCommands.empty());

        // Collect unique commands.
        for (auto &cmd : pendingCommands) {
            auto pos = find(uniqueCommands.begin(), uniqueCommands.end(), cmd);
            if (pos == uniqueCommands.end()) {
                uniqueCommands.push_back(cmd);
            } else {
                pos->Merge(cmd);
            }
        }
        pendingCommands.clear();

        // Process the combined commands.
        for (auto &cmd : uniqueCommands) {
            cmd.Execute(*this);
        }
        uniqueCommands.clear();
    }

    void Scanner::DiscardCommands() {
        pendingCommands.clear();
        assert(uniqueCommands.empty());
    }

    size_t Scanner::AddErrorMessage(const string &message) {
        errorMessages.push_back(message);
        return errorMessages.size() - 1;
    }

    size_t Scanner::GetTokenLine() const {
        size_t idx = 1;
        for (auto newlinePos : newLines) {
            if (newlinePos > tokenPos)
                break;
            ++idx;
        }
        return idx;
    }

    size_t Scanner::GetTokenColumn() const {
        size_t tokenLine = GetTokenLine();
        size_t lineStart = newLines.empty()  || tokenLine == 1 ? 0 : newLines[GetTokenLine() - 2];
        return tokenPos - lineStart;
    }

    /////////////////////////////////////////////////
    /////// Inner class Command
    /////////////////////////////////////////////////

    bool Scanner::Command::operator==(const Scanner::Command &rhs) const {
        return type == rhs.type;
    }

    void Scanner::Command::Execute(Scanner &parent) {
        switch (type) {
            case Type::SET_TOKEN:
                parent.tokenType = data1;
                break;

            case Type::ADD_STRING_VALUE_CHAR:
                parent.stringValue += static_cast<char>(data1);
                break;

            case Type::ADD_INT_VALUE:
                parent.intValue *= data2;
                parent.intValue += data1;
                break;

            case Type::ADD_REAL_VALUE:
                parent.realFactor *= data2;
                parent.realValue += data1 / parent.realFactor;
                break;

            case Type::EMIT_ERROR:
                throw ScannerError(parent.errorMessages.at(data1), parent.newLines.size() + 1);
                break;

            default:
                assert(false);
        }
    }

    void Scanner::Command::Merge(Scanner::Command &cmd) {
        assert(type == cmd.type);

        if (priority >= cmd.priority)
            return;

        switch (type) {
            case Type::ADD_INT_VALUE:
            case Type::ADD_REAL_VALUE:
                data2 = cmd.data2;
                // fall through

            case Type::SET_TOKEN:
            case Type::ADD_STRING_VALUE_CHAR:
            case Type::EMIT_ERROR:
                priority = cmd.priority;
                data1 = cmd.data1;
                break;
        }
    }

}
