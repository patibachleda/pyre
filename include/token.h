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
          TOKEN_ARROW, //13

          // Arithmetic
          TOKEN_EQUALS, // 14
          TOKEN_GREATER, // 15
          TOKEN_LESS, // 16
          TOKEN_GREATER_EQUAL, // 17
          TOKEN_LESS_EQUAL, // 18
          TOKEN_EQUAL_EQUAL, // 19
          TOKEN_BANG, // 20
          TOKEN_BANG_EQUAL, // 21
          TOKEN_PLUS, // 22
          TOKEN_MINUS, // 23
          TOKEN_STAR, // 24
          TOKEN_DIVIDE, // 25

          // Types
          TOKEN_STRING, // 26
          TOKEN_INT, // 27
          TOKEN_DOUBLE, // 28
          TOKEN_BOOLEAN, // 29
          TOKEN_CHARACTER, // 30

          // Syntax
          TOKEN_SEMICOLON, // 31
          TOKEN_COLON, // 32
          TOKEN_COMMA, // 33
          TOKEN_DOT, // 34
          TOKEN_LPAREN, // 35
          TOKEN_RPAREN, // 36
          TOKEN_LCURLY, // 37
          TOKEN_RCURLY, // 38

          // Types
          TOKEN_INT_TYPE, // 39
          TOKEN_DOUBLE_TYPE, // 40
          TOKEN_STRING_TYPE, // 41
          TOKEN_CHAR_TYPE, // 42
          TOKEN_BOOLEAN_TYPE, // 43

          TOKEN_EXPRESSION_INT, // 44
          TOKEN_EXPRESSION_DOUBLE, // 45
          TOKEN_EXPRESSION_STRING, // 46
          TOKEN_EXPRESSION_CHAR, // 47
          TOKEN_EXPRESSION_BOOLEAN // 48
     } type;

     char* value;
     int line;
} token_T;

token_T* init_token(int type, char* value, int line);
#endif
