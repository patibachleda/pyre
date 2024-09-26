#ifndef TOKEN_H
#define TOKEN_H
typedef struct TOKEN_STRUCT {
     enum {
          // Literals
          TOKEN_ID, // 1

          // Keywords
          TOKEN_AND, // 2
          TOKEN_OR, // 3
          TOKEN_IS, // 4
          TOKEN_ELIS, // 5
          TOKEN_ELSE, // 6
          TOKEN_YES, // 7
          TOKEN_NO, // 8
          TOKEN_PROCESS, // 9
          TOKEN_FUNC, // 10
          TOKEN_MAIN, // 11
          TOKEN_HELPER, // 12
          TOKEN_WHILE, // 13
          TOKEN_FOR, // 14
          TOKEN_PRINT, // 15
          TOKEN_EMIT, // 16

          // Arithmetic
          TOKEN_EQUALS, // 17
          TOKEN_GREATER, // 18
          TOKEN_LESS, // 19
          TOKEN_GREATER_EQUAL, // 20
          TOKEN_LESS_EQUAL, // 21
          TOKEN_EQUAL_EQUAL, // 22
          TOKEN_BANG, // 23
          TOKEN_BANG_EQUAL, // 24
          TOKEN_PLUS, // 25
          TOKEN_MINUS, // 26
          TOKEN_STAR, // 27
          TOKEN_DIVIDE, // 28

          // Types
          TOKEN_STRING, // 29
          TOKEN_INT, // 30
          TOKEN_DOUBLE, // 31
          TOKEN_BOOLEAN, // 32
          TOKEN_CHARACTER, // 33

          // Syntax
          TOKEN_SEMICOLON, // 34
          TOKEN_COMMA, // 35
          TOKEN_DOT, // 36
          TOKEN_LPAREN, // 37
          TOKEN_RPAREN, // 38
          TOKEN_LCURLY, // 39
          TOKEN_RCURLY, // 40

     } type;

     char* value;
     int line;
} token_T;

token_T* init_token(int type, char* value, int line);
#endif