#pragma once

/**
 * Types of binary arithmetic operators.
 */
enum class ArithmeticOperator {
    ADD, SUB, MUL, DIV, MOD,
};

/**
 * Types of binary relational operators.
 */
enum class RelationalOperator {
    LESS_THAN, LESS_EQUAL, GREATER_THAN, GREATER_EQUAL, EQUAL, NOT_EQUAL
};

/**
 * Types of binary logical operators.
 */
enum class LogicalOperator {
    AND, OR
};

