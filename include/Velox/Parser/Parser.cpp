// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Parser.h"

using namespace std;

namespace velox {

    unique_ptr<SyntaxNode> Parser::Parse(Source &source) {
        return ParseStatementSequence(source);
    }

    unique_ptr<SyntaxNode> Parser::ParseStatementSequence(Source &source) {
        return nullptr;
    }

    unique_ptr<SyntaxNode> Parser::ParseStatement(Source &source) {
        //switch ( source.GetToken() ) {
        //
        //}
        return nullptr;
    }


}

