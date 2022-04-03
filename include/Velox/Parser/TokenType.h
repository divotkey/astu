#pragma once

namespace velox {

    enum class TokenType {

        /** Invalid token. */
        INVALID,

        /** String literal. */
        STRING,

        /** Integer value. */
        INTEGER,

        /** Floating point value. */
        REAL,

        /** Left Bracket. */
        LEFT_BRACKET,

        /** Right Bracket. */
        RIGHT_BRACKET,

        /** Left Parenthesis. */
        LEFT_PARENTHESIS,

        /** Right Parenthesis. */
        RIGHT_PARENTHESIS,

        /** Used so separate statements. */
        SEMICOLON,

        /** Comma sign ','. */
        COMMA,

        /** Allows to access members of a class, struct etc. */
        MEMBER_ACCESS,

        /** Addition. */
        ADD,

        /** Subtraction. */
        SUB,

        /** Modulo. */
        MOD,

        /** Multiplication. */
        MUL,

        /** Division. */
        DIV,

        /** Assignment. */
        ASSIGNMENT,

        /** the 'true' token. */
        TRUE,

        /** the 'false' token. */
        FALSE,

        /** Identifier. */
        IDENT,

        /** End of source. */
        EOS,
    };

}