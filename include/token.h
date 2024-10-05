#ifndef TOKEN_H
#define TOKEN_H
typedef struct TOKEN_STRUCT {
     enum {
          // Literals
          TOKEN_ID, // 0

          // Keywords
          TOKEN_AND, // 1
          TOKEN_OR, // 2
          TOKEN_IS, // 3
          TOKEN_ELIS, // 4
          TOKEN_ELSE, // 5
          TOKEN_PROCESS, // 6
          TOKEN_FUNC, // 7
          TOKEN_MAIN, // 8
          TOKEN_HELPER, // 9
          TOKEN_WHILE, // 10
          TOKEN_FOR, // 11
          TOKEN_EMIT, // 12

          // Arithmetic
          TOKEN_EQUALS, // 13
          TOKEN_GREATER, // 14
          TOKEN_LESS, // 15
          TOKEN_GREATER_EQUAL, // 16
          TOKEN_LESS_EQUAL, // 17
          TOKEN_EQUAL_EQUAL, // 18
          TOKEN_BANG, // 19
          TOKEN_BANG_EQUAL, // 20
          TOKEN_PLUS, // 21
          TOKEN_MINUS, // 22
          TOKEN_STAR, // 23
          TOKEN_DIVIDE, // 24

          // Types
          TOKEN_STRING, // 25
          TOKEN_INT, // 26
          TOKEN_DOUBLE, // 27
          TOKEN_BOOLEAN, // 28
          TOKEN_CHARACTER, // 29

          // Syntax
          TOKEN_SEMICOLON, // 30
          TOKEN_COMMA, // 31
          TOKEN_DOT, // 32
          TOKEN_LPAREN, // 33
          TOKEN_RPAREN, // 34
          TOKEN_LCURLY, // 35
          TOKEN_RCURLY, // 36

     } type;

     char* value;
     int line;
} token_T;

token_T* init_token(int type, char* value, int line);
#endif
