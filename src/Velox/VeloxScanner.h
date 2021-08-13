// ___________________________________________________________________________
//
//                   Code Synthesized by SWORD. Do not modify!
//                           SWORD Interpreter v1.8.15
// ___________________________________________________________________________
// 
// File: VeloxScanner.h

#pragma once

#include <cassert>
#include <string>
#include <memory>
#include "ICharStream.h"

namespace astu {

	/**
	 * Represents the position of a token in the source code.
	 */
	class TokenPosition {
	public: 
	
		/**
		 * Sets the start position.
		 *
		 * @param pos	the start position
		 */
		void setStart(size_t pos);
	
		/**
		 * Sets the end position.
		 *
		 * @param pos	the end position
		 */
		void setEnd(size_t pos);
	
		/**
		 * Sets the line number.
		 *
		 * @param line	the line number
		 */
		void setLine(unsigned int line);
	
		/**
		 * Resets the position to zero.
		 */
		void clear();
	
		/**
		 * Returns the start position of the token.
		 *
		 * @return the start position of the token
		 */
		size_t getStartPos() const {
			return m_startPos; 
		}
	
		/**
		 * Returns the end position of the token.
		 *
		 * @return the end position of the token
		 */
		size_t getEndPos() const {
			return m_endPos; 
		}
	
		/**
		 * Returns the line number of the token.
		 *
		 * @return the line number of the token
		 */
		unsigned int getLine() const {
			return m_line; 
		}
	
	private: 
	
		/** The start position of the token. */
		size_t m_startPos;
	
		/** The end position of the token. */
		size_t m_endPos;
	
		/** The line number of the token. */
		unsigned int m_line;
	
	};
	
	/**
	 * Exception thrown in case of a VeloxScanner scanner error.
	 */
	class VeloxCompilerException {
	public: 
	
		/**
		 * Constructor.
		 *
		 * @param message	the error text
		 * @param lineNumber	the line number of the error
		 */
		VeloxCompilerException(const std::string & message, unsigned int lineNumber) {
			m_message = message; 
			m_lineNumber = lineNumber; 
		}
	
		/**
		 * Returns the error message of this exception.
		 *
		 * @return the error message of this exception
		 */
		const std::string & getMessage() const {
			return m_message; 
		}
	
		/**
		 * Returns the line number within the script.
		 *
		 * @return the line number within the script
		 */
		unsigned int getLineNumber() const {
			return m_lineNumber; 
		}
	
	private: 
	
		/** The error message of this exception. */
		std::string m_message;
	
		/** The line number within the script. */
		unsigned int m_lineNumber;
	
	};
	
	/**
	 * Token scanner for Velox script files.
	 */
	class VeloxScanner {
	public: 
	
		/** The tokens recognized by this scanner. */
		enum Token {
	
			/** End of script. */
			EOS, 
	
			/** Invalid token. */
			INVALID, 
	
			/** Line comment. */
			LINE_COMMENT, 
	
			/** Start of block comment. */
			BLOCK_COMMENT_START, 
	
			/** End of block comment. */
			BLOCK_COMMENT_END, 
	
			/** Period. */
			DOT, 
	
			/** Period. */
			COMMA, 
	
			/** Semicolon. */
			SEMICOLON, 
	
			/** Integer value. */
			INT, 
	
			/** Floating point value. */
			REAL, 
	
			/** String literal. */
			STRING, 
	
			/** Identifier. */
			IDENT, 
	
			/** Left Bracket. */
			LEFT_BRACKET, 
	
			/** Right Bracket. */
			RIGHT_BRACKET, 
	
			/** Left Parenthesis. */
			LEFT_PARENTHESIS, 
	
			/** Right Parenthesis. */
			RIGHT_PARENTHESIS, 
	
			/** Left Parenthesis. */
			BLOCK_START, 
	
			/** Right Parenthesis. */
			BLOCK_END, 
	
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
	
			/** Assignment. */
			ASSIGNMENT, 
	
			/** Increment. */
			INCREMENT, 
	
			/** Decrement. */
			DECREMENT, 
	
			/** Not. */
			NOT, 
	
			/** Equals. */
			EQUAL, 
	
			/** Equals. */
			NOT_EQUAL, 
	
			/** Colon. */
			COLON, 
	
			/** Less-than. */
			LESS_THAN, 
	
			/** Less or equal. */
			LESS_EQUAL, 
	
			/** Greater-than. */
			GREATER_THAN, 
	
			/** Greater or equal. */
			GREATER_EQUAL, 
	
			/** Logical and. */
			LOG_AND, 
	
			/** Logical or. */
			LOG_OR, 
	
			/** the 'if' token. */
			IF, 
	
			/** the 'else' token. */
			ELSE, 
	
			/** the 'while' token. */
			WHILE, 
	
			/** the 'for' token. */
			FOR, 
	
			/** the 'print' token. */
			PRINT, 
	
			/** the 'return' token. */
			RETURN, 
	
			/** the 'true' token. */
			TRUE, 
	
			/** the 'false' token. */
			FALSE, 
	
			/** the 'function' token. */
			FUNCTION, 
	
			/** the 'class' token. */
			CLASS, 
	
			/** the 'new' token. */
			NEW, 
	
			/** the 'global' token. */
			GLOBAL, 
	
			/** the 'import' token. */
			IMPORT
		};
	
		/**
		 * Constructor.
		 */
		VeloxScanner();
	
		/**
		 * Returns the current token.
		 *
		 * @return the current token
		 */
		const Token & getToken() const {
			return m_token; 
		}
	
		/**
		 * Returns the position of the current token found the script.
		 *
		 * @return the position of the current token found the script
		 */
		const TokenPosition & getTokenPosition() const {
			return m_tokenPosition; 
		}
	
		/**
		 * Returns the position of the previous token found the script.
		 *
		 * @return the position of the previous token found the script
		 */
		const TokenPosition & getPrevTokenPosition() const {
			return m_prevTokenPosition; 
		}
	
		/**
		 * Returns the current line within the script.
		 *
		 * @return the current line within the script
		 */
		unsigned int getLine() const {
			return m_line; 
		}
	
		/**
		 * Returns the last read integer value.
		 *
		 * @return the last read integer value
		 */
		int getIntValue() const {
			return m_intValue; 
		}
	
		/**
		 * Returns the last read real value.
		 *
		 * @return the last read real value
		 */
		double getDoubleValue() const {
			return m_doubleValue; 
		}
	
		/**
		 * Returns the last read identifier.
		 *
		 * @return the last read identifier
		 */
		const std::string & getIdent() const {
			return m_ident; 
		}
	
		/**
		 * Returns the last read string literal.
		 *
		 * @return the last read string literal
		 */
		const std::string & getString() const {
			return m_string; 
		}
	
		/**
		 * Returns whether to eat white spaces between tokens.
		 *
		 * @return whether to eat white spaces between tokens
		 */
		bool isEatWhitespace() const {
			return m_eatWhitespace; 
		}
	
		/**
		 * Specifies whether to eat white space characters between tokens.
		 *
		 * @param b	`true` to eat white space characters
		 */
		void setEatWhitespace(bool b);
	
		/**
		 * Resets this scanner to process the specified script.
		 *
		 * @param script	character stream representing the script to process
		 */
		void reset(std::shared_ptr<ICharStream> script);
	
		/**
		 * Scans and returns the next token.
		 *
		 * @return the next token
		 */
		VeloxScanner::Token nextToken();
	
		/**
		 * Reads and returns a single word.
		 *
		 * @return the read word
		 */
		const std::string & readWord();
	
		/**
		 * Reads and returns the remaining line.
		 *
		 * @return the line
		 */
		const std::string & readLine();
	
		/**
		 * Converts the specified token to a human readable string.
		 *
		 * @param token	the token to convert
		 * @return the string representation of the specified token
		 */
		std::string tokenToString(Token token);
	
	private: 
	
		/** The keywords recognized by this scanner. */
		static const std::string KEYWORDS[];
	
		/** Maps keyword indices to tokens. */
		static const VeloxScanner::Token KEYWORD_INDEX_TO_TOKEN[];
	
		/** The script to process. */
		std::shared_ptr<ICharStream> m_script;
	
		/** The current token. */
		Token m_token;
	
		/** The current character. */
		char m_char;
	
		/** The position within the script. */
		size_t m_pos;
	
		/** The position of the current token found the script. */
		TokenPosition m_tokenPosition;
	
		/** The position of the previous token found the script. */
		TokenPosition m_prevTokenPosition;
	
		/** The current line within the script. */
		unsigned int m_line;
	
		/** The last read integer value. */
		int m_intValue;
	
		/** The last read real value. */
		double m_doubleValue;
	
		/** The last read identifier. */
		std::string m_ident;
	
		/** The last read string literal. */
		std::string m_string;
	
		/** Used to mark escaped double quotes. */
		bool m_escape;
	
		/** Whether to eat white spaces between tokens. */
		bool m_eatWhitespace;
	
		/**
		 * Returns the index of the specified keyword.
		 *
		 * @param keyword	the keyword to search for
		 * @return the index of the keyword or -1
		 */
		int findKeyword(const std::string & keyword);
	
		/**
		 * Stores the current position as token start..
		 */
		void markTokenStart();
	
		/**
		 * Stores the current position as token start..
		 */
		void markTokenEnd();
	
		/**
		 * Scans and returns the next token.
		 *
		 * @return the next token
		 */
		VeloxScanner::Token doNextToken();
	
		/**
		 * Consumes white space characters.
		 */
		void eatWhitespace();
	
		/**
		 * Returns whether the specified character is a valid start for an identifier.
		 *
		 * @param ch	the character to test
		 * @return `true` if the character starts an identifier
		 */
		bool isIdentStart(char ch);
	
		/**
		 * Returns whether the specified character is a valid within an identifier.
		 *
		 * @param ch	the character to test
		 * @return `true` if the character valid for an identifier
		 */
		bool isIdent(char ch);
	
		/**
		 * Reads an identifier.
		 */
		void readIdent();
	
		/**
		 * Reads an integer value.
		 */
		void readInt();
	
		/**
		 * Reads an integer in hexadecimal notation.
		 */
		void readHexInt();
	
		/**
		 * Reads a floating point value.
		 */
		void readReal();
	
		/**
		 * Reads a string literal.
		 */
		void readString();
	
		/**
		 * Consumes all characters until the end of the current line.
		 */
		void eatLine();
	
		/**
		 * Reads the next character.
		 */
		void readChar();
	
		/**
		 * Undoes the last call to readChar..
		 */
		void unreadChar();
	
	};
	
} // end of namespace


