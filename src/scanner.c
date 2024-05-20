#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "scanner.h"

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;

void initScanner (const char* code) {
    scanner.current = code;
    scanner.start   = code;
    scanner.line    = 1;
}

//==============================================================================
static bool isAlpha (char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit (char c) {
    return c >= '0' && c <= '9';
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

//==============================================================================
static TokenType checkKeyword (int start, int length, const char* rest, TokenType type) {
    if (scanner.current - scanner.start == start + length && memcmp (scanner.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static Token makeToken (TokenType type) {
    Token token;
    token.type   = type;
    token.start  = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.line   = scanner.line;

    // printf("make token: %.*s, %d\n", token.length, token.start, type);

    return token;
}

static Token errorToken (const char* message) {
    Token token;
    token.type   = TOKEN_ERROR;
    token.start  = message;
    token.length = (int) strlen (message);
    token.line   = scanner.line;
    return token;
}

//==============================================================================
static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (isAtEnd())
        return '\0';
    return scanner.current[1];
}

static char advance() {
    ++scanner.current;
    return scanner.current[-1]; // valid?
}

static bool match (char c) {
    if (isAtEnd() || *scanner.current != c)
        return false;
    ++scanner.current;
    return true;
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                ++scanner.line;
                advance();
                break;
            case '/': {
                if (peekNext() == '/') {
                    while (peek() != '\n' && ! isAtEnd())
                        advance();
                } else {
                    return;
                }
                break;
            }
            default:
                return;
        }
    }
}

static Token string() {
    while (peek() != '"' && ! isAtEnd()) {
        if (peek() == '\n')
            ++scanner.line;
        advance();
    }
    if (isAtEnd())
        return errorToken ("Unterminated string.");
    advance(); // close quote;
    return makeToken (TOKEN_STRING);
}

static Token number() {
    while (isDigit (peek()))
        advance();
    if (peek() == '.' && isDigit (peekNext())) {
        advance(); // consume the .
        while (isDigit (peek()))
            advance();
    }
    return makeToken (TOKEN_NUMBER);
}

static TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'n':
            return checkKeyword (1, 2, "il", TOKEN_NIL);
            break;
        case 'i':
            return checkKeyword (1, 1, "f", TOKEN_IF);
            break;
        case 'e':
            return checkKeyword (1, 3, "lse", TOKEN_ELSE);
            break;
        case 'a':
            return checkKeyword (1, 2, "nd", TOKEN_AND);
            break;
        case 'o':
            return checkKeyword (1, 1, "r", TOKEN_OR);
            break;
        case 'v':
            return checkKeyword (1, 2, "ar", TOKEN_VAR);
            break;
        case 'r':
            return checkKeyword (1, 5, "eturn", TOKEN_RETURN);
            break;

        case 'f': {
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a':
                        return checkKeyword (2, 3, "lse", TOKEN_FALSE);
                        break;
                    case 'o':
                        return checkKeyword (2, 1, "r", TOKEN_FOR);
                        break;
                    case 'u':
                        return checkKeyword (2, 1, "n", TOKEN_FUN);
                        break;
                }
            }
            break;
        }

        case 't': {
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h':
                        return checkKeyword (2, 3, "is", TOKEN_THIS);
                        break;
                    case 'r':
                        return checkKeyword (2, 2, "ue", TOKEN_TRUE);
                        break;
                }
            }
            break;
        }
    }

    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isAlpha (peek()) || isDigit (peek()))
        advance();

    return makeToken (identifierType());
}

//==============================================================================
Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;
    if (isAtEnd())
        return makeToken (TOKEN_EOF);

    char c = advance();

    if (isDigit (c))
        return number();
    if (isAlpha (c))
        return identifier();

    switch (c) {
        case '(':
            return makeToken (TOKEN_LEFT_PAREN);
            break;
        case ')':
            return makeToken (TOKEN_RIGHT_PAREN);
            break;
        case '{':
            return makeToken (TOKEN_LEFT_BRACE);
            break;
        case '}':
            return makeToken (TOKEN_RIGHT_BRACE);
            break;
        case ';':
            return makeToken (TOKEN_RIGHT_BRACE);
            break;
        case ',':
            return makeToken (TOKEN_RIGHT_BRACE);
            break;

        case '.':
            return makeToken (TOKEN_DOT);
            break;
        case '-':
            return makeToken (TOKEN_MINUS);
            break;
        case '+':
            return makeToken (TOKEN_PLUS);
            break;
        case '/':
            return makeToken (TOKEN_SLASH);
            break;
        case '*':
            return makeToken (TOKEN_STAR);
            break;

        case '!':
            return makeToken (match ('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken (match ('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken (match ('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken (match ('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

        case '"':
            return string();
            break;
    }

    return errorToken ("Unexpected character.");
}
