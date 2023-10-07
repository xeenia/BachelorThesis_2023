
#include "Scanner.h"

std::map<std::string, TokenType> Scanner::keywords;
Scanner::Scanner(std::string source, Errors* err) : source(source), err(err) {
    keywords.insert({ "class", TokenType::CLASS });
    keywords.insert({ "else", TokenType::ELSE });
    keywords.insert({ "false", TokenType::FALSE });
    keywords.insert({ "for", TokenType::FOR });
    keywords.insert({ "fun", TokenType::FUN });
    keywords.insert({ "if", TokenType::IF });
    keywords.insert({ "nil", TokenType::NIL });
    keywords.insert({ "print", TokenType::PRINT });
    keywords.insert({ "printnl", TokenType::PRINTNL });
    keywords.insert({ "read", TokenType::READ });
    keywords.insert({ "return", TokenType::RETURN });
    keywords.insert({ "super", TokenType::SUPER });
    keywords.insert({ "this", TokenType::THIS });
    keywords.insert({ "true", TokenType::TRUE });
    keywords.insert({ "var", TokenType::VAR });
    keywords.insert({ "while", TokenType::WHILE });
    keywords.insert({ "do", TokenType::DO });
    keywords.insert({ "break", TokenType::BREAK });
    keywords.insert({ "continue", TokenType::CONTINUE });
}


std::vector<Token*> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    Token* token = new Token(TokenType::END_OF_FILE, "", nullptr, ValueType::NONVALUE, line-1);
    tokens.push_back(token);
    return tokens;
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
    case '(': addToken(TokenType::LEFT_PAREN); break;
    case ')': addToken(TokenType::RIGHT_PAREN); break;
    case '{': addToken(TokenType::LEFT_BRACE); break;
    case '}': addToken(TokenType::RIGHT_BRACE); break;
    case ',': addToken(TokenType::COMMA); break;
    case '.': addToken(TokenType::DOT); break;
    case '-': if(match('-')) addToken(TokenType::MINUSMINUS);
              else if(match('=')) addToken(TokenType::MINUSASSIGN);
              else addToken(TokenType::MINUS);
              break;
    case '+': if (match('+')) addToken(TokenType::PLUSPLUS);
              else if (match('=')) addToken(TokenType::PLUSASSIGN);
              else addToken(TokenType::PLUS);
              break;
    case ';': addToken(TokenType::SEMICOLON); break;
    case '*': if (match('=')) addToken(TokenType::STARASSIGN);
              else addToken(TokenType::STAR); break;
    case '%': if (match('=')) addToken(TokenType::MODASSIGN);
            else addToken(TokenType::MOD); break;
    case '?': addToken(TokenType::QUESTION_MARK); break;
    case ':': addToken(TokenType::COLON); break;
    case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '&':
        if(match('&'))  addToken(TokenType::LOGICAL_AND);
        else err->error(line, "You used '&' instead of '&&' for the logical AND operator.");
        break;
    case '|':
        if (match('|'))  addToken(TokenType::LOGICAL_OR);
        else err->error(line, "You used '|' instead of '||' for the logical OR operator.");
        break;
    case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::ASSIGN);
        break;
    case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '/':
        if (match('/')) {
            while (peek() != '\n' && !isAtEnd()) advance();
        }
        else if (match('*')) {
            comment();

        }
        else if (match('=')) addToken(TokenType::DIVASSIGN);
        else {
            addToken(TokenType::SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        break;

    case '\n':
        line++;
        break;
    case '"': string(); break;
    default:
        if (isDigit(c)) {
            number();
        }
        else if (isAlpha(c)) {
            identifier();
        }
        else {
             err->error(line, "Unexpected character.");
        }
        break;
    }
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();
    int dif = current - start;
    std::string text = source.substr(start, dif);
    TokenType type;
    auto iterator = keywords.find(text);
    if (iterator == keywords.end()) {
        type = TokenType::IDENTIFIER;
    }
    else {
        type = iterator->second;
    }
    addToken(type);
}
bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}
bool Scanner::isAlpha(char c) {
    return(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

void Scanner::number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) advance();
    }
    int dif = current - start;

    if (isInt(source.substr(start, dif))) {
        LiteralU* literal = new LiteralU(std::stoi(source.substr(start, dif)));
        addToken(TokenType::NUMBER,literal, ValueType::INT);
    }
    else{
        LiteralU* literal = new LiteralU(std::stod(source.substr(start, dif)));
        addToken(TokenType::NUMBER, literal, ValueType::DOUBLE);
    }   
}
bool Scanner::isInt(const std::string& str) {
    if (str.find('.') != std::string::npos)
        return false;
    return true;
}

char Scanner::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

char Scanner::advance() {
    return source[current++];
}

void Scanner::addToken(TokenType type) {
    addToken(type, nullptr, ValueType::NONVALUE);
}
void Scanner::addToken(TokenType type, LiteralU* literal, ValueType active) {
    int dif = current - start;
    std::string text = source.substr(start, dif);
    Token* token = new Token(type, text, literal,active, line-1);
    tokens.push_back(token);
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) {
        return '\0';
    }
    return source[current];
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        err->error(line, "Unterminated string.");
    }
    advance();

    int dif = (current-1) - (start+1);

    std::string value = source.substr( start + 1, dif);
    LiteralU* literal = new LiteralU(value);
    addToken(TokenType::STRING, literal, ValueType::STRING);
}

void Scanner::comment() {
    while (peek() != '*' || peekNext() != '/') {
        if (isAtEnd()) {
            err->error(line, "Unexpected end of file found in comment.");
            return;
        }
        if (peek() == '\n') {
            line++;
        }
        advance();
    }
    advance();
    advance();
}





