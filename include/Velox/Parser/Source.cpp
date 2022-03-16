// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Source.h"

// C++ Standard Library includes
#include <cassert>
#include <iostream>
#include <cctype>

using namespace std;

#define IDENT_STATE     "Ident"
#define IDENT_START     "IdentStart"
#define IDENT_TERMINAL  "IdentTerminal"

#define POSSIBLE_IDENT      0x0001
#define LAST_CHAR_OF_TOKEN  0x0020

namespace velox {

    Source::Source() : curToken(TokenType::INVALID, 0) {

        // Ignore white space characters.
        //sm.AddState("EatWhitespace", [this](const char& ch) { PrintDebug(ch); });
        sm.AddState("EatWhitespace");
        sm.SetStartState("EatWhitespace");

        auto whiteSpace = OrTriggerBuilder<char>()
                .Add(EqualTriggerBuilder(' ').Build())
                .Add(EqualTriggerBuilder('\t').Build())
                .Add(EqualTriggerBuilder('\r').Build())
                .Add(EqualTriggerBuilder('\n').Build())
                .Build();

        sm.AddTransition("EatWhitespace", "EatWhitespace", whiteSpace);

        auto identStart = OrTriggerBuilder<char>()
                .Add(EqualTriggerBuilder('_').Build())
                .Add(RangeTriggerBuilder('a', 'z').Build())
                .Add(RangeTriggerBuilder('A', 'Z').Build())
                .Build();

        auto identChar = OrTriggerBuilder<char>()
                .Add(EqualTriggerBuilder('_').Build())
                .Add(RangeTriggerBuilder('a', 'z').Build())
                .Add(RangeTriggerBuilder('A', 'Z').Build())
                .Add(RangeTriggerBuilder('0', '9').Build())
                .Build();

        sm.AddState(IDENT_START);
        sm.AddState(IDENT_STATE);
        sm.AddState(IDENT_TERMINAL);

        sm.AddTransition(IDENT_START, IDENT_STATE, identChar);
        sm.SetEnterFunc(IDENT_START, [this](const char& ch){
            curToken.pos = GetCharPosition();
            curToken.sValue += ch;
        });

        sm.AddTransition(IDENT_STATE, IDENT_STATE, identChar);
        sm.SetEnterFunc(IDENT_STATE, [this](const char& ch){
            curToken.sValue += ch;
        });

        sm.AddTransition(IDENT_STATE, IDENT_TERMINAL, AllTriggerBuilder<char>().Build());
        sm.SetEnterFunc(IDENT_TERMINAL, [this](const char& ch){
            curToken.type = TokenType::IDENT;
        });




        AddToken("true", TokenType::TRUE);
        //AddToken("truex", TokenType::TRUEX);
        AddToken("false", TokenType::FALSE);
        //AddToken("+", TokenType::ADD);
        //AddToken("-", TokenType::SUB);
        //AddToken("*", TokenType::MUL);
        //AddToken("/", TokenType::DIV);
        //AddToken("(", TokenType::RIGHT_PARENTHESIS);
        //AddToken(")", TokenType::LEFT_PARENTHESIS);
        //AddToken("[", TokenType::LEFT_BRACKET);
        //AddToken("]", TokenType::RIGHT_BRACKET);
        sm.AddTransition(sm.GetStartState(), IDENT_STATE, identStart);


    }

    const Token &Source::GetFirstToken() {
        GetFirstChar();
        return GetNextToken();
    }

    const Token &Source::GetNextToken() {
        // Prepare to recognize token.
        sm.Restart();
        curToken.Reset();

        // Recognize token.
        sm.Process(GetCurrentChar());
        while (!sm.IsLeaf()) {
            sm.Process(GetNextChar());
        }

        return curToken;
    }

    const Token &Source::GetCurrentToken() const {
        return curToken;
    }

    void Source::AddToken(const string &sequence, TokenType type) {
        assert(!sequence.empty());
        string curState = sm.GetStartState();

        size_t idx = 0;
        while (idx < sequence.size() && sm.HasTransition(curState, sequence[idx]) &&
               !sm.IsSet(curState, LAST_CHAR_OF_TOKEN))
        {
            curState = sm.GetTarget(curState, sequence[idx++]);
        }

        if (idx < sequence.size()) {

        } else {
            // idx >= sequence.size();
            if (sm.IsSet(curState, LAST_CHAR_OF_TOKEN)) {
                throw std::logic_error("Ambiguous character sequence for token '" + sequence + "'");
            }

            curState = AddTokenEndState(curState, type, IsIdentStart(sequence[0]));
        }

        if (sm.HasTransition(curState, sequence[0])) {
            throw std::logic_error("not implemented");
        } else {
            curState = AddTokenStartState(curState, sequence[idx++]);
        }

        while(idx < sequence.size()) {
            curState = AddTokenState(curState, sequence[idx++]);
        }

        //curState = AddTokenEndState(curState, type);

        if (IsIdentStart(sequence[0])) {
            sm.SetFlags(curState, POSSIBLE_IDENT);
        } else {
        }

        for (size_t i = idx; i < sequence.size(); ++i) {
            const char ch = sequence[idx];
        }
        //while (idx < sequence.size())

        bool first = true;
        for (const char ch : sequence) {
            if (!sm.HasTransition(curState, ch)) {
                string newState = sm.GetUniqueStateName() + "_" + ch;

                if (first) {
                    sm.AddState(newState, [this](const char &ch){
                        curToken.sValue += ch;
                        curToken.pos = GetCharPosition();
                    });
                } else {
                    sm.AddState(newState, [this](const char &ch){
                        curToken.sValue += ch;
                    });
                }
                sm.AddTransition(curState, newState, EqualTriggerBuilder(ch).Build());
                first = false;
            }

            curState = sm.GetTarget(curState, ch);
        }

        if (sm.IsLeaf(curState)) {
            sm.SetEnterFunc(curState, [this, type](const char &ch) {
                curToken.type = type;
                curToken.sValue += ch;
            });
        }
    }

    void Source::PrintDebug(char ch) {
        if (std::isspace(ch)) {
            cout << "char: (" << static_cast<int>(ch) << ")" << endl;
        } else {
            cout << "char: " << ch << " (" << static_cast<int>(ch) << ")" << endl;
        }
    }

    void Source::AddIdentTermination(const string &state, TokenType type) {
        auto identChar = sm.GetTrigger(IDENT_STATE, 0);

        sm.AddTransition(state, IDENT_STATE, identChar);

        auto terminal = state + "_terminal";
        sm.AddState(terminal);
        sm.AddTransition(state, terminal, NotTriggerBuilder(identChar).Build());
        sm.SetEnterFunc(terminal, [this, type](const char &ch) {
            curToken.type = type;
        });
    }

    std::string Source::AddTokenStartState(const string &curState, char ch) {
        string newState = sm.GetUniqueStateName() + "_" + ch;
        sm.AddState(newState, [this](const char &ch){
            curToken.sValue += ch;
            curToken.pos = GetCharPosition();
        });
        sm.AddTransition(curState, newState, EqualTriggerBuilder(ch).Build());
        return newState;
    }

    std::string Source::AddTokenState(const string &curState, char ch) {
        string newState = sm.GetUniqueStateName() + "_" + ch;
        sm.AddState(newState, [this](const char &ch){
            curToken.sValue += ch;
        });
        sm.AddTransition(curState, newState, EqualTriggerBuilder(ch).Build());
        return newState;
    }

    std::string Source::AddTokenEndState(const string &curState, TokenType type, bool useIdent) {
        if (useIdent) {
            auto identChar = sm.GetTrigger(IDENT_STATE, 0);
            sm.AddTransition(curState, IDENT_STATE, identChar);
        }

        // We need to mark the state, that resembles the last character within the sequence to
        // recognize tokens, that have the same initial character sequence.
        sm.SetFlags(curState, LAST_CHAR_OF_TOKEN);

        auto terminal = curState + "_end";
        sm.AddState(terminal);
        sm.AddTransition(curState, terminal, AllTriggerBuilder<char>().Build());
        sm.SetEnterFunc(terminal, [this, type](const char &ch) {
            curToken.type = type;
        });

        return terminal;
    }

    bool Source::IsIdentStart(char ch) {
        return true;
        //return sm.GetTrigger(IDENT_STATE, 0).Evaluate(ch);
    }

}
