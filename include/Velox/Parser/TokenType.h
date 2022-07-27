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

        /** Defines the start of a loopBody block. */
        BLOCK_START,

        /** Defines the end of a loopBody block. */
        BLOCK_END,

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

        /** Equals. */
        EQUAL,

        /** Equals. */
        NOT_EQUAL,

        /** Less-than. */
        LESS_THAN,

        /** Less or equal. */
        LESS_EQUAL,

        /** Greater-than. */
        GREATER_THAN,

        /** Greater or equal. */
        GREATER_EQUAL,

        /** Assignment. */
        ASSIGNMENT,

        /** Assignment Add. */
        ASSIGN_ADD,

        /** Assignment Sub. */
        ASSIGN_SUB,

        /** Assignment Mul. */
        ASSIGN_MUL,

        /** Assignment Div. */
        ASSIGN_DIV,

        /** Assignment Mod. */
        ASSIGN_MOD,

        /** Increment. */
        INCREMENT,

        /** Decrement. */
        DECREMENT,

        /** Logical not. */
        NOT,

        /** Logical and. */
        LOG_AND,

        /** Logical or. */
        LOG_OR,

        /** Binary and. */
        BIN_AND,

        /** Binary or. */
        BIN_OR,

        /** The 'true' token. */
        TRUE,

        /** The 'false' token. */
        FALSE,

        /** The 'undefined' token. */
        UNDEFINED,

        /** Keyword for function definitions. */
        FUNCTION,

        /** Keyword causing a function to exit and defining it's return value. */
        RETURN,

        /** The keyword for 'if' statements. */
        IF,

        /** The keyword for 'else' statements. */
        ELSE,

        /** The keyword for 'while' statements. */
        WHILE,

        /** The keyword for 'do' statements. */
        DO,

        /** The keyword for 'loop' statements. */
        LOOP,

        /** The keyword for 'exit' statements. */
        EXIT,

        /** The keyword for 'continue' statements. */
        CONTINUE,

        /** The keyword for 'break' statements. */
        BREAK,

        /** The keyword for 'for' statements. */
        FOR,

        /** The keyword for 'class' definitions. */
        CLASS,

        /** The keyword for 'new' statements. */
        NEW,

        /** The keyword for 'instant' definitions. */
        INSTANT,

        /** The keyword for 'global' modifier. */
        GLOBAL,

        /** The keyword for 'global' loopBody. */
        IMPORT,

        /** Identifier. */
        IDENT,

        /** End of source. */
        EOS,
    };

}