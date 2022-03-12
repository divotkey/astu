#pragma once

namespace velox {

    enum class TokenType {

        /** Invalid token. */
        INVALID,

        /** Left Bracket. */
        LEFT_BRACKET,

        /** Right Bracket. */
        RIGHT_BRACKET,

        /** Left Parenthesis. */
        LEFT_PARENTHESIS,

        /** Right Parenthesis. */
        RIGHT_PARENTHESIS,

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

        /** the 'true' token. */
        TRUEX,

        /** the 'false' token. */
        FALSE,

        /** Identifier. */
        IDENT,

        /** End of source. */
        EOS,
    };

}