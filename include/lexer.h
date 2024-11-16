#ifndef LEXER_H
#define LEXER_H
#include "token.h"

static const struct {
	const char* value;
	int type;
} keywords[] = {
	{"and",     TOKEN_AND},
	{"or",    TOKEN_OR},
	{"if",       TOKEN_IS},
	{"elif",     TOKEN_ELIS},
	{"else",     TOKEN_ELSE},
	{"process",    TOKEN_PROCESS},
	{"func",      TOKEN_FUNC},
	{"main",       TOKEN_MAIN},
	{"helper",      TOKEN_HELPER},
	{"while",      TOKEN_WHILE},
	{"for",      TOKEN_FOR},
	{"emit",    TOKEN_EMIT},
	{"int",	  TOKEN_INT_TYPE},
	{"double",	  TOKEN_DOUBLE_TYPE},
	{"string",	  TOKEN_STRING_TYPE},
	{"char",	  TOKEN_CHAR_TYPE},
	{"boolean",	  TOKEN_BOOLEAN_TYPE},
	{"true",		TOKEN_BOOLEAN},
	{"false",		TOKEN_BOOLEAN}
};

typedef struct LEXER_STRUCT {
     char current_char;
     unsigned int index;
     char* contents;
} lexer_T;


lexer_T* init_lexer(char* contents); // creates lexer 

void lexer_move_forward(lexer_T* lexer); // move our pointer to next character in content

void lexer_skip_whitespace(lexer_T* lexer); // remove whitespace

token_T* lexer_get_next_token(lexer_T* lexer); // get to next token in contents

token_T* lexer_parse_string(lexer_T* lexer); // how should we parse stuff in "<text>"

token_T* lexer_parse_number(lexer_T* lexer); // how should we parse stuff in "<text>"

token_T* lexer_parse_identifier(lexer_T* lexer); // how should we parse stuff in "<text>"

token_T* lexer_read_forward_with_token(lexer_T* lexer, token_T* token); // how should we parse stuff in "<text>"

char lexer_look_forward(lexer_T* lexer);

char* lexer_get_current_char_as_string(lexer_T* lexer); // return char c as a string

char* lexer_get_next_char_as_string(lexer_T* lexer);
#endif
