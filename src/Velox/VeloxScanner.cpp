// ___________________________________________________________________________
//
//                   Code Synthesized by SWORD. Do not modify!
//                           SWORD Interpreter v1.8.15
// ___________________________________________________________________________
// 
// File: VeloxScanner.cpp

#include <cctype>
#include "VeloxScanner.h"

namespace astu {

	/////////////////////////////////////////////////
	/////// Class TokenPosition
	/////////////////////////////////////////////////
	
	void TokenPosition::setStart(size_t pos) {
		m_startPos = pos; 
	}
	
	void TokenPosition::setEnd(size_t pos) {
		m_endPos = pos; 
	}
	
	void TokenPosition::setLine(unsigned int line) {
		m_line = line; 
	}
	
	void TokenPosition::clear() {
		m_startPos = 0; 
		m_endPos = 0; 
		m_line = 0; 
	}
	
	/////////////////////////////////////////////////
	/////// Class VeloxCompilerException
	/////////////////////////////////////////////////
	
	/////////////////////////////////////////////////
	/////// Class VeloxScanner
	/////////////////////////////////////////////////
	
	const std::string VeloxScanner::KEYWORDS[] = {"if", "else", "while", "for", "print", "return", "true", "false", "function", "class", "new", "global", "import"};
	const VeloxScanner::Token VeloxScanner::KEYWORD_INDEX_TO_TOKEN[] = {Token::IF, Token::ELSE, Token::WHILE, Token::FOR, Token::PRINT, Token::RETURN, Token::TRUE, Token::FALSE, Token::FUNCTION, Token::CLASS, Token::NEW, Token::GLOBAL, Token::IMPORT};
	 
	VeloxScanner::VeloxScanner() {
		m_eatWhitespace = true; 
	}
	
	void VeloxScanner::setEatWhitespace(bool b) {
		m_eatWhitespace = b; 
	}
	
	int VeloxScanner::findKeyword(const std::string & keyword) {
		for (size_t i = 0; i < 13; ++i) { 
			if (KEYWORDS[i] == keyword) 
				return static_cast<int>(i); 
		} 
		return -1; 
	}
	
	void VeloxScanner::reset(std::shared_ptr<ICharStream> script) {
		m_script = script; 
		m_pos = 0; 
		m_tokenPosition.clear(); 
		m_prevTokenPosition.clear(); 
		 
		m_line = 1; 
		m_intValue = 0; 
		m_doubleValue = 0; 
		m_ident.clear(); 
		if (m_script) { 
			readChar(); 
		} 
	}
	
	VeloxScanner::Token VeloxScanner::nextToken() {
		doNextToken(); 
		 
		while (getToken() == LINE_COMMENT  || getToken() == BLOCK_COMMENT_START) { 
		 
			while (getToken() == BLOCK_COMMENT_START) { 
				doNextToken(); 
				while(getToken() != EOS && getToken() != BLOCK_COMMENT_END) { 
					doNextToken(); 
				} 
				if (getToken() == BLOCK_COMMENT_END) { 
					doNextToken(); 
				} 
			} 
		 
			while (getToken() == LINE_COMMENT) { 
				eatLine(); 
				doNextToken(); 
			} 
		 
		} 
		return m_token; 
	}
	
	void VeloxScanner::markTokenStart() {
		m_prevTokenPosition = m_tokenPosition; 
		m_tokenPosition.setStart(m_pos); 
	}
	
	void VeloxScanner::markTokenEnd() {
		m_tokenPosition.setEnd(m_pos); 
		m_tokenPosition.setLine(m_line); 
	}
	
	VeloxScanner::Token VeloxScanner::doNextToken() {
		eatWhitespace(); 
		markTokenStart(); 
		 
		switch(m_char) { 
		 
		// End of script 
		case -1: 
			m_token = EOS; 
			return m_token; 
		 
		// DIV || LINE_COMMENT || BLOCK_COMMENT_START || ASSIGN_DIV			 
		case '/': 
			readChar(); 
				 
			// LINE_COMMENT			 
			if (m_char == '/') { 
				readChar(); 
				// Recognized token LINE_COMMENT 
				m_token = LINE_COMMENT; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			else  
				 
			// BLOCK_COMMENT_START			 
			if (m_char == '*') { 
				readChar(); 
				// Recognized token BLOCK_COMMENT_START 
				m_token = BLOCK_COMMENT_START; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			else  
				 
			// ASSIGN_DIV			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token ASSIGN_DIV 
				m_token = ASSIGN_DIV; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token DIV 
			m_token = DIV; 
			markTokenEnd(); 
			return m_token; 
		 
		// MUL || BLOCK_COMMENT_END || ASSIGN_MUL			 
		case '*': 
			readChar(); 
				 
			// BLOCK_COMMENT_END			 
			if (m_char == '/') { 
				readChar(); 
				// Recognized token BLOCK_COMMENT_END 
				m_token = BLOCK_COMMENT_END; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			else  
				 
			// ASSIGN_MUL			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token ASSIGN_MUL 
				m_token = ASSIGN_MUL; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token MUL 
			m_token = MUL; 
			markTokenEnd(); 
			return m_token; 
		 
		// DOT			 
		case '.': 
			readChar(); 
			// Recognized token DOT 
			m_token = DOT; 
			markTokenEnd(); 
			return m_token; 
		 
		// COMMA			 
		case ',': 
			readChar(); 
			// Recognized token COMMA 
			m_token = COMMA; 
			markTokenEnd(); 
			return m_token; 
		 
		// SEMICOLON			 
		case ';': 
			readChar(); 
			// Recognized token SEMICOLON 
			m_token = SEMICOLON; 
			markTokenEnd(); 
			return m_token; 
		 
		// LEFT_BRACKET			 
		case '[': 
			readChar(); 
			// Recognized token LEFT_BRACKET 
			m_token = LEFT_BRACKET; 
			markTokenEnd(); 
			return m_token; 
		 
		// RIGHT_BRACKET			 
		case ']': 
			readChar(); 
			// Recognized token RIGHT_BRACKET 
			m_token = RIGHT_BRACKET; 
			markTokenEnd(); 
			return m_token; 
		 
		// LEFT_PARENTHESIS			 
		case '(': 
			readChar(); 
			// Recognized token LEFT_PARENTHESIS 
			m_token = LEFT_PARENTHESIS; 
			markTokenEnd(); 
			return m_token; 
		 
		// RIGHT_PARENTHESIS			 
		case ')': 
			readChar(); 
			// Recognized token RIGHT_PARENTHESIS 
			m_token = RIGHT_PARENTHESIS; 
			markTokenEnd(); 
			return m_token; 
		 
		// BLOCK_START			 
		case '{': 
			readChar(); 
			// Recognized token BLOCK_START 
			m_token = BLOCK_START; 
			markTokenEnd(); 
			return m_token; 
		 
		// BLOCK_END			 
		case '}': 
			readChar(); 
			// Recognized token BLOCK_END 
			m_token = BLOCK_END; 
			markTokenEnd(); 
			return m_token; 
		 
		// ADD || ASSIGN_ADD || INCREMENT			 
		case '+': 
			readChar(); 
				 
			// ASSIGN_ADD			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token ASSIGN_ADD 
				m_token = ASSIGN_ADD; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			else  
				 
			// INCREMENT			 
			if (m_char == '+') { 
				readChar(); 
				// Recognized token INCREMENT 
				m_token = INCREMENT; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token ADD 
			m_token = ADD; 
			markTokenEnd(); 
			return m_token; 
		 
		// SUB || ASSIGN_SUB || DECREMENT			 
		case '-': 
			readChar(); 
				 
			// ASSIGN_SUB			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token ASSIGN_SUB 
				m_token = ASSIGN_SUB; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			else  
				 
			// DECREMENT			 
			if (m_char == '-') { 
				readChar(); 
				// Recognized token DECREMENT 
				m_token = DECREMENT; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token SUB 
			m_token = SUB; 
			markTokenEnd(); 
			return m_token; 
		 
		// MOD || ASSIGN_MOD			 
		case '%': 
			readChar(); 
				 
			// ASSIGN_MOD			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token ASSIGN_MOD 
				m_token = ASSIGN_MOD; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token MOD 
			m_token = MOD; 
			markTokenEnd(); 
			return m_token; 
		 
		// ASSIGNMENT || EQUAL			 
		case '=': 
			readChar(); 
				 
			// EQUAL			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token EQUAL 
				m_token = EQUAL; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token ASSIGNMENT 
			m_token = ASSIGNMENT; 
			markTokenEnd(); 
			return m_token; 
		 
		// NOT || NOT_EQUAL			 
		case '!': 
			readChar(); 
				 
			// NOT_EQUAL			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token NOT_EQUAL 
				m_token = NOT_EQUAL; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token NOT 
			m_token = NOT; 
			markTokenEnd(); 
			return m_token; 
		 
		// COLON			 
		case ':': 
			readChar(); 
			// Recognized token COLON 
			m_token = COLON; 
			markTokenEnd(); 
			return m_token; 
		 
		// LESS_THAN || LESS_EQUAL			 
		case '<': 
			readChar(); 
				 
			// LESS_EQUAL			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token LESS_EQUAL 
				m_token = LESS_EQUAL; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token LESS_THAN 
			m_token = LESS_THAN; 
			markTokenEnd(); 
			return m_token; 
		 
		// GREATER_THAN || GREATER_EQUAL			 
		case '>': 
			readChar(); 
				 
			// GREATER_EQUAL			 
			if (m_char == '=') { 
				readChar(); 
				// Recognized token GREATER_EQUAL 
				m_token = GREATER_EQUAL; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// Recognized token GREATER_THAN 
			m_token = GREATER_THAN; 
			markTokenEnd(); 
			return m_token; 
		 
		// LOG_AND			 
		case '&': 
			readChar(); 
				 
			// LOG_AND			 
			if (m_char == '&') { 
				readChar(); 
				// Recognized token LOG_AND 
				m_token = LOG_AND; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// No token recognized so far. 
			unreadChar(); 
			break; 
		 
		// LOG_OR			 
		case '|': 
			readChar(); 
				 
			// LOG_OR			 
			if (m_char == '|') { 
				readChar(); 
				// Recognized token LOG_OR 
				m_token = LOG_OR; 
				markTokenEnd(); 
				return m_token; 
			} 
			 
			// No token recognized so far. 
			unreadChar(); 
			break; 
		 
		} // end of switch 
		 
		if (m_char == '"') { 
			readString(); 
			m_token = STRING; 
			markTokenEnd(); 
		}  
		else if (isdigit(m_char)) { 
			readInt(); 
			m_token = INT; 
		 
			if (m_char == '.') { 
				readReal(); 
				m_token = REAL; 
			} 
			markTokenEnd(); 
		} 
		else if(isIdentStart(m_char)) { 
			readIdent(); 
			auto idx = findKeyword(m_ident); 
			if (idx >= 0) { 
				m_token = KEYWORD_INDEX_TO_TOKEN[idx]; 
			} else { 
				m_token = IDENT; 
			} 
			markTokenEnd(); 
		} 
		else { 
			markTokenEnd(); 
			m_token = INVALID;		 
			readChar(); 
		}	 
			 
		return m_token;	 
	}
	
	void VeloxScanner::eatWhitespace() {
		if (!m_eatWhitespace) 
			return; 
		 
		while (m_char != -1 && isspace(m_char)) { 
			readChar(); 
		} 
	}
	
	bool VeloxScanner::isIdentStart(char ch) {
		return isalpha(ch) || ch == '_';			 
	}
	
	bool VeloxScanner::isIdent(char ch) {
		return isalnum(ch) || ch == '_';			 
	}
	
	void VeloxScanner::readIdent() {
		assert(isIdentStart(m_char)); 
		 
		m_ident = m_char; 
		readChar(); 
		while (isIdent(m_char) || m_char == '\\') { 
			if (m_char == '\\') { 
				readChar(); 
				if (m_char != ' ') { 
					throw VeloxCompilerException("illegal escape sequence '" + std::string("'"), m_line);				 
				} 
			} 
			m_ident += m_char; 
			readChar(); 
		} 
	}
	
	void VeloxScanner::readInt() {
		assert(isdigit(m_char)); 
		m_intValue = m_char - '0'; 
		readChar(); 
		 
		if (m_char == 'x' || m_char == 'X') { 
			readChar(); 
			readHexInt(); 
		} else { 
			while (m_char != -1 && isdigit(m_char)) { 
				m_intValue *= 10; 
				m_intValue += m_char - '0'; 
				readChar(); 
			} 
		} 
	}
	
	void VeloxScanner::readHexInt() {
		while (isxdigit(m_char)) { 
			m_intValue *= 16; 
			if (m_char >= '0' && m_char <= '9') { 
				m_intValue += m_char - '0'; 
			} else if (m_char >= 'A' && m_char <= 'F') { 
				m_intValue += 10 + m_char - 'A'; 
			}  else if (m_char >= 'a' && m_char <= 'f') { 
				m_intValue += 10 + m_char - 'a'; 
			} else { 
				assert(false); 
			} 
			readChar(); 
		} 
	}
	
	void VeloxScanner::readReal() {
		// TODO support scientific notation. 
		assert(m_char == '.'); 
		readChar();		 
		 
		double value = 0.0; 
		double i = 1.0; 
		 
		while (m_char != -1 && isdigit(m_char)) { 
			i *= 10; 
			value += (m_char - '0') / i; 
			readChar(); 
		} 
		m_doubleValue = m_intValue + value; 
	}
	
	const std::string & VeloxScanner::readWord() {
		m_string = ""; 
		eatWhitespace(); 
		while (m_char != -1 && !isspace(m_char)) { 
			m_string += m_char; 
			readChar(); 
		} 
		return m_string; 
	}
	
	const std::string & VeloxScanner::readLine() {
		m_string = ""; 
		eatWhitespace(); 
		while (m_char != -1 && m_char != '\n') { 
			m_string += m_char; 
			readChar(); 
		} 
		return m_string; 
	}
	
	void VeloxScanner::readString() {
		assert(m_char == '"'); 
		readChar();		 
		m_string.clear(); 
		while (m_char != -1 && (m_char != '"' || m_escape)) {  
			m_string += m_char;  
			if (m_escape)  
				m_escape = false; 
			readChar(); 
		}  
		 
		if (m_char != '"') { 
			throw VeloxCompilerException("string literal not closed", m_line); 
		} 
		readChar(); 
	}
	
	void VeloxScanner::eatLine() {
		while (m_char != -1 && m_char != '\n') { 
			readChar(); 
		} 
	}
	
	void VeloxScanner::readChar() {
		if (m_script->isEos()) { 
			m_char = -1;				 
		} else { 
			m_char = m_script->nextChar(); 
			if (m_char == '\n') { 
				++m_line; 
			} 
			else if (m_char == '\\' && !m_script->isEos()) { 
				m_char = m_script->nextChar(); 
				switch (m_char) { 
				case 'n': 
					m_char = '\n'; 
					break; 
				case 'r': 
					m_char = '\r'; 
					break; 
				case 't': 
					m_char = '\t'; 
					break; 
				case '"': 
					m_escape = true; 
					break; 
				case '\\': 
					m_char = '\\'; 
					break; 
				default: 
					throw VeloxCompilerException("unknown escape sequence \\" + m_char, m_line); 
				} 
		 
		} 
		 
			++m_pos; 
		} 
	}
	
	void VeloxScanner::unreadChar() {
			throw std::runtime_error("Putting back of characters from ICharStream not implemented"); 
	}
	
	std::string VeloxScanner::tokenToString(Token token) {
		switch (token) { 
		case EOS: 
			return "EOS"; 
		case INVALID: 
			return "INVALID"; 
		case LINE_COMMENT: 
			return "LINE_COMMENT"; 
		case BLOCK_COMMENT_START: 
			return "BLOCK_COMMENT_START"; 
		case BLOCK_COMMENT_END: 
			return "BLOCK_COMMENT_END"; 
		case DOT: 
			return "DOT"; 
		case COMMA: 
			return "COMMA"; 
		case SEMICOLON: 
			return "SEMICOLON"; 
		case INT: 
			return "INT"; 
		case REAL: 
			return "REAL"; 
		case STRING: 
			return "STRING"; 
		case IDENT: 
			return "IDENT"; 
		case LEFT_BRACKET: 
			return "LEFT_BRACKET"; 
		case RIGHT_BRACKET: 
			return "RIGHT_BRACKET"; 
		case LEFT_PARENTHESIS: 
			return "LEFT_PARENTHESIS"; 
		case RIGHT_PARENTHESIS: 
			return "RIGHT_PARENTHESIS"; 
		case BLOCK_START: 
			return "BLOCK_START"; 
		case BLOCK_END: 
			return "BLOCK_END"; 
		case ADD: 
			return "ADD"; 
		case SUB: 
			return "SUB"; 
		case MOD: 
			return "MOD"; 
		case MUL: 
			return "MUL"; 
		case DIV: 
			return "DIV"; 
		case ASSIGN_ADD: 
			return "ASSIGN_ADD"; 
		case ASSIGN_SUB: 
			return "ASSIGN_SUB"; 
		case ASSIGN_MUL: 
			return "ASSIGN_MUL"; 
		case ASSIGN_DIV: 
			return "ASSIGN_DIV"; 
		case ASSIGN_MOD: 
			return "ASSIGN_MOD"; 
		case ASSIGNMENT: 
			return "ASSIGNMENT"; 
		case INCREMENT: 
			return "INCREMENT"; 
		case DECREMENT: 
			return "DECREMENT"; 
		case NOT: 
			return "NOT"; 
		case EQUAL: 
			return "EQUAL"; 
		case NOT_EQUAL: 
			return "NOT_EQUAL"; 
		case COLON: 
			return "COLON"; 
		case LESS_THAN: 
			return "LESS_THAN"; 
		case LESS_EQUAL: 
			return "LESS_EQUAL"; 
		case GREATER_THAN: 
			return "GREATER_THAN"; 
		case GREATER_EQUAL: 
			return "GREATER_EQUAL"; 
		case LOG_AND: 
			return "LOG_AND"; 
		case LOG_OR: 
			return "LOG_OR"; 
		case IF: 
			return "IF"; 
		case ELSE: 
			return "ELSE"; 
		case WHILE: 
			return "WHILE"; 
		case FOR: 
			return "FOR"; 
		case PRINT: 
			return "PRINT"; 
		case RETURN: 
			return "RETURN"; 
		case TRUE: 
			return "TRUE"; 
		case FALSE: 
			return "FALSE"; 
		case FUNCTION: 
			return "FUNCTION"; 
		case CLASS: 
			return "CLASS"; 
		case NEW: 
			return "NEW"; 
		case GLOBAL: 
			return "GLOBAL"; 
		case IMPORT: 
			return "IMPORT"; 
		default:  
			throw std::logic_error("Unknown token"); 
		} 
	}
	
} // end of namespace


